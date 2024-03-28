#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./lib/HHCli.h"
#include "./lib/HHDisplay.h"
#include "./lib/HHWindow.h"

/**
 * Exit Codes
 * 1 - Missing SubCommand
 * 5 - Invalid number of arguments for SubCommand
 * 19 - Invalid format/type of arguments for SubCommand
 * 50 - Window ID is invalid, or doesn't exist
 * 99 - Out of memory / Couldn't allocate
 */

Window window_id;
Action action = ACTION_NONE;
Bool shouldToggle = False;
int width = -1, height = -1;

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
                help(5, "Invalid Window ID Format.");
            }

            break;
        case 'm':
            action = ACTION_MAXIMIZE;
            break;
        case 'n':
            action = ACTION_MINIMIZE;
            break;
        case 'r':
            action = ACTION_RESTORE;
            break;

        /////////////////////////////////////////////////////////////////////////////
        // Toggle Params
        /////////////////////////////////////////////////////////////////////////////
        case 'z':
            action = ACTION_TOGGLE_FIXED_SIZE;
            shouldToggle = NULL != optarg;
            break;
        case 'a':
            action = ACTION_TOGGLE_ABOVE;
            shouldToggle = NULL != optarg;
            break;
        case 'b':
            action = ACTION_TOGGLE_BELOW;
            shouldToggle = NULL != optarg;
            break;
        case 's':
            action = ACTION_TOGGLE_SHADE;
            shouldToggle = NULL != optarg;
            break;
        case 'y':
            action = ACTION_TOGGLE_STICKY;
            shouldToggle = NULL != optarg;
            break;
        case 't':
            action = ACTION_TOGGLE_TASKBAR;
            shouldToggle = NULL != optarg;
            break;
        case 'p':
            action = ACTION_TOGGLE_PAGER;
            shouldToggle = NULL != optarg;
            break;
        case 'f':
            action = ACTION_TOGGLE_FULLSCREEN;
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
            sprintf(err_msg, "Unknown Argument: %c", opt);
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
    case ACTION_TOGGLE_FIXED_SIZE:
        if (shouldToggle && -1 >= width)
        {
            help(5, "Missing or invalid Width option.");
        }

        if (shouldToggle && -1 >= height)
        {
            help(5, "Missing or invalid Height option.");
        }

        printf("Toggling Fixed Size: %s : w=%i, h=%i", shouldToggle ? "Yes" : "No", width, height);

        HHWindow.toggleFixedSize(window_id, shouldToggle, width, height);
        break;
    case ACTION_TOGGLE_ABOVE:
        printf("Toggling Always Above: %s", shouldToggle ? "Yes" : "No");
        HHWindow.toggleAbove(window_id, shouldToggle);
        break;
    case ACTION_TOGGLE_BELOW:
        printf("Toggling Always Below: %s", shouldToggle ? "Yes" : "No");
        HHWindow.toggleBelow(window_id, shouldToggle);
        break;
    case ACTION_TOGGLE_SHADE:
        printf("Toggling Shade: %s", shouldToggle ? "Yes" : "No");
        HHWindow.toggleShade(window_id, shouldToggle);
        break;
    case ACTION_TOGGLE_STICKY:
        printf("Toggling Sticky: %s", shouldToggle ? "Yes" : "No");
        HHWindow.toggleSticky(window_id, shouldToggle);
        break;
    case ACTION_TOGGLE_TASKBAR:
        printf("Toggling Taskbar: %s", shouldToggle ? "Yes" : "No");
        HHWindow.toggleTaskbar(window_id, shouldToggle);
        break;
    case ACTION_TOGGLE_PAGER:
        printf("Toggling Pager: %s", shouldToggle ? "Yes" : "No");
        HHWindow.togglePager(window_id, shouldToggle);
        break;
    case ACTION_TOGGLE_FULLSCREEN:
        printf("Toggling Fullscreen: %s", shouldToggle ? "Yes" : "No");
        HHWindow.toggleFullscreen(window_id, shouldToggle);
        break;
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
