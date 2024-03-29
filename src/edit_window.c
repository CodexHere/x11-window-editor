#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./lib/HHCli.h"
#include "./lib/HHDisplay.h"
#include "./lib/HHWindow.h"
#include "./lib/HHUtil.h"

/**
 * Exit Codes
 * 1 - Missing SubCommand
 * 5 - Invalid number of arguments for SubCommand
 * 10 - Invalid format/type of arguments for SubCommand
 * 50 - Window ID is invalid, or doesn't exist
 * 99 - Out of memory / Couldn't allocate
 */

Window window_id;
Action action = ACTION_NONE;
Bool shouldToggle = False;
int width = -1, height = -1;

char *raw_name;
int num_atoms = 0;
char **atoms;
HH_EVENT_MODE event_mode;
Bool is_atom_raw;
int atom_format = 32;
char *atom_property_type = "ATOMS";

void set_action(Action setting_action)
{
    if (ACTION_NONE != action)
    {
        help(1, "Please only perform ONE Sub Command per run.");
    }

    action = setting_action; // NOLINT
}

void process_arguments(int argc, char *argv[])
{
    int opt;
    int option_index = 0;

    char *opstring = ":hi:mnrz::a::b::s::y::t::p::f::";
    char err_msg[100];

    while ((opt = getopt_long(argc, argv, opstring, long_options, &option_index)) != -1)
    {
        switch (opt)
        {
        case 'h':
            help(0, NULL);
            break;
        case 'i':
            window_id = strtol(optarg, NULL, 0);

            if (0 == window_id)
            {
                help(10, "Invalid Window ID Format.");
            }

            break;
        case 'm':
            set_action(ACTION_MAXIMIZE);
            break;
        case 'n':
            set_action(ACTION_MINIMIZE);
            break;
        case 'r':
            set_action(ACTION_RESTORE);
            break;
        case 'l':
            set_action(ACTION_SET_TITLE);
            raw_name = optarg;
            break;
        case 'o':
            set_action(ACTION_SET_ROLE);
            raw_name = optarg;
            break;
        case 'w':
            set_action(ACTION_SET_WINDOW_TYPE);
            raw_name = optarg;
            break;

        /////////////////////////////////////////////////////////////////////////////
        // Toggle Params
        /////////////////////////////////////////////////////////////////////////////
        case 'z':
            set_action(ACTION_TOGGLE_FIXED_SIZE);
            shouldToggle = NULL != optarg;
            break;
        case 'a':
            set_action(ACTION_TOGGLE_ABOVE);
            shouldToggle = NULL != optarg;
            break;
        case 'b':
            set_action(ACTION_TOGGLE_BELOW);
            shouldToggle = NULL != optarg;
            break;
        case 's':
            set_action(ACTION_TOGGLE_SHADE);
            shouldToggle = NULL != optarg;
            break;
        case 'y':
            set_action(ACTION_TOGGLE_STICKY);
            shouldToggle = NULL != optarg;
            break;
        case 't':
            set_action(ACTION_TOGGLE_TASKBAR);
            shouldToggle = NULL != optarg;
            break;
        case 'p':
            set_action(ACTION_TOGGLE_PAGER);
            shouldToggle = NULL != optarg;
            break;
        case 'f':
            set_action(ACTION_TOGGLE_FULLSCREEN);
            shouldToggle = NULL != optarg;
            break;

        /////////////////////////////////////////////////////////////////////////////
        // Reusable Params
        /////////////////////////////////////////////////////////////////////////////
        case 100: // width
            width = atoi(optarg);
            if (0 >= width)
            {
                help(10, "Invalid Width.");
            }
            break;
        case 101: // height
            height = atoi(optarg);
            if (0 >= height)
            {
                help(10, "Invalid Height.");
            }
            break;

        /////////////////////////////////////////////////////////////////////////////
        // Actions w/o Short Codes
        /////////////////////////////////////////////////////////////////////////////
        case 500:
            set_action(ACTION_RAISE);
            break;

        /////////////////////////////////////////////////////////////////////////////
        // Raw Setters
        /////////////////////////////////////////////////////////////////////////////
        case 1000: // Set Raw Property
            set_action(ACTION_RAW_PROP);
            raw_name = optarg;
            break;
        case 1010: // Send Raw Event
            set_action(ACTION_RAW_EVENT);
            raw_name = optarg;
            break;
        case 1500: // Set ATOMs list
            atoms = HHUtil.delimit(optarg, ",", &num_atoms);
            break;
        case 1505: // Set ATOMs format
            atom_format = atoi(optarg);
            if (atom_format != 8 && atom_format != 16 && atom_format != 32)
            {
                sprintf(err_msg, "Invalid ATOM Format! %s", optarg);
                help(10, err_msg);
            }
            break;
        case 1506: // Set ATOMs Type
            atom_property_type = optarg;
            break;
        case 1507: // Set Atom Raw mode
            is_atom_raw = True;
            break;
        case 1510: // Set Event Mode (add/remove/toggle/etc)
            event_mode = atoi(optarg);
            break;

        /////////////////////////////////////////////////////////////////////////////
        // Error Cases / Handlers
        /////////////////////////////////////////////////////////////////////////////
        case '?': // Extra param?
            break;
        case ':': // Missing param?
            sprintf(err_msg, "Missing expected option argument for %s, be sure to check your supplied options.", argv[optind - 1]);
            help(10, err_msg);
            break;
        case -1: // End of parameters
            help(1, "An unknown error has occurred.");
            break;
        default:
            sprintf(err_msg, "Unknown Argument: %c (%i)", opt, opt);
            help(5, err_msg);
        }
    }
}

void perform_action()
{
    // Perform the action based on the selected option
    switch (action)
    {
    case ACTION_MAXIMIZE:
        printf("Window Maximize");
        HHWindow.maximize(window_id);
        break;
    case ACTION_MINIMIZE:
        printf("Window Minimize");
        HHWindow.minimize(window_id);
        break;
    case ACTION_RESTORE:
        printf("Window Restore");
        HHWindow.restore(window_id);
        break;
    case ACTION_RAISE:
        printf("Window Raise");
        HHWindow.raise(window_id);
        break;

    case ACTION_SET_TITLE:
        printf("Setting Title: %s", raw_name);
        HHWindow.set_title(window_id, raw_name);
        break;
    case ACTION_SET_ROLE:
        printf("Setting Role: %s", raw_name);
        HHWindow.set_role(window_id, raw_name);
        break;
    case ACTION_SET_WINDOW_TYPE:
        printf("Setting Window Type: %s", raw_name);
        HHWindow.set_window_type(window_id, raw_name);
        break;
    case ACTION_TOGGLE_FIXED_SIZE:
        if (shouldToggle && -1 >= width)
        {
            help(10, "Missing or invalid Width option.");
        }

        if (shouldToggle && -1 >= height)
        {
            help(10, "Missing or invalid Height option.");
        }

        printf("Toggling Fixed Size: %s : w=%i, h=%i", shouldToggle ? "Yes" : "No", width, height);

        HHWindow.toggle_fixed_size(window_id, shouldToggle, width, height);
        break;
    case ACTION_TOGGLE_ABOVE:
        printf("Toggling Always Above: %s", shouldToggle ? "Yes" : "No");
        HHWindow.toggle_above(window_id, shouldToggle);
        break;
    case ACTION_TOGGLE_BELOW:
        printf("Toggling Always Below: %s", shouldToggle ? "Yes" : "No");
        HHWindow.toggle_below(window_id, shouldToggle);
        break;
    case ACTION_TOGGLE_SHADE:
        printf("Toggling Shade: %s", shouldToggle ? "Yes" : "No");
        HHWindow.toggle_shade(window_id, shouldToggle);
        break;
    case ACTION_TOGGLE_STICKY:
        printf("Toggling Sticky: %s", shouldToggle ? "Yes" : "No");
        HHWindow.toggle_sticky(window_id, shouldToggle);
        break;
    case ACTION_TOGGLE_TASKBAR:
        printf("Toggling Taskbar: %s", shouldToggle ? "Yes" : "No");
        HHWindow.toggle_taskbar(window_id, shouldToggle);
        break;
    case ACTION_TOGGLE_PAGER:
        printf("Toggling Pager: %s", shouldToggle ? "Yes" : "No");
        HHWindow.toggle_pager(window_id, shouldToggle);
        break;
    case ACTION_TOGGLE_FULLSCREEN:
        printf("Toggling Fullscreen: %s", shouldToggle ? "Yes" : "No");
        HHWindow.toggle_fullscreen(window_id, shouldToggle);
        break;

    /////////////////////////////////////////////////////////////////////////////
    // Raw Setters
    /////////////////////////////////////////////////////////////////////////////
    case ACTION_RAW_PROP:
        if (0 == strlen(raw_name))
        {
            help(5, "Missing Property Name to set.");
        }
        if (0 == num_atoms)
        {
            help(5, "Missing ATOMs to set for the property.");
        }

        Atom temp_atom = XInternAtom(HHDisplay.attach(), atom_property_type, False);
        printf("Setting Property: %s -> %s\n", raw_name, *atoms);
        printf("ATOM Type: %s(%ld), ATOM Format (raw = %s): %i\n", atom_property_type, temp_atom, is_atom_raw ? "Yes" : "No", atom_format);

        HHWindow.set_property(window_id, raw_name, atoms, num_atoms, atom_property_type, atom_format, is_atom_raw);
        break;
    case ACTION_RAW_EVENT:
        if (0 == strlen(raw_name))
        {
            help(5, "Missing Event Name to send.");
        }
        if (0 == num_atoms)
        {
            help(5, "Missing ATOMs to send with the Event.");
        }

        printf("Sending Event: %s -> %s (mode = %i, raw = %s)", raw_name, *atoms, event_mode, is_atom_raw ? "Yes" : "No");
        HHWindow.send_event(window_id, raw_name, atoms, num_atoms, event_mode, is_atom_raw);
        break;

    /////////////////////////////////////////////////////////////////////////////
    // Error Cases / Handlers
    /////////////////////////////////////////////////////////////////////////////
    default:
        help(5, "Missing Sub Command to perform!");
        break;
    }

    printf("\n");
}

int main(int argc, char *argv[])
{
    script_name = argv[0];

    if (argc < 2)
    {
        help(5, "Not enough arguments.");
    }

    process_arguments(argc, argv);

    // Make sure we have a Window ID set! Literally nothing will work without this!
    if (0 == window_id) // NOLINT
    {
        help(5, "Missing Window ID (--id, -id).");
    }

    perform_action();

    return 0;
}
