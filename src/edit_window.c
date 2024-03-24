#include "./lib/HHDisplay.h"
#include "./lib/HHWindow.h"
#include "./lib/HHUtil.h"

/**
 * Exit Codes
 * 1 - Missing SubCommand
 * 5 - Invalid number of arguments for SubCommand
 * 50 - Window ID is invalid, or doesn't exist
 * 99 - Out of memory / Couldn't allocate
 */

void help(char *script)
{
    printf(""
           "Usage: %s <window_id> <subcommand> [options]\n\n"
           "    <window_id>: The ID of the Window\n"
           "    <subcommand>: Any one of the Sub Commands detailed below\n"
           "\n\n"
           "Sub Commands (and their options):\n\n"
           "  !fixed-size - Unsets any Fixed Size constraint previously set.\n"
           "                NOTE: This will effectively set the minimum size to 0!\n"
           "  fixed-size <width> <height> - Sets a fixed size for a Window\n"
           "    <width>: Width of the Window\n"
           "    <height>: Height of the Window\n"
           "\n\n"
           "",
           script);
}

int main(int argc, char *argv[])
{
    if (3 > argc)
    {
        help(argv[0]);
        exit(1);
    }

    Window windowId = strtol(argv[1], NULL, 0);
    char *subCommand = argv[2];

    if (0 == strcmp(subCommand, "!fixed-size"))
    {
        HHWindow.undo_fixed_size(windowId);
    }
    else if (0 == strcmp(subCommand, "fixed-size"))
    {
        // Args 3 & 4 are width and height

        if (5 > argc)
        {
            fprintf(stderr, "Error: Missing Options.\n\n");
            help(argv[0]);
            exit(5);
        }

        HHWindow.set_fixed_size(windowId, argv[3], argv[4]);
    }
    else if (0 == strcmp(subCommand, "send-event"))
    {
        // Arg 3 = Event Name
        // Arg 4 = Atoms as comma-separated string
        // Arg 5 = Whether or not to Add or Remove (any value = add)

        if (6 > argc)
        {
            fprintf(stderr, "Error: Missing Options.\n\n");
            help(argv[0]);
            exit(5);
        }

        int num_atoms = 0;
        char *name = argv[3];
        char **atoms = HHUtil.delimit(argv[4], ",", &num_atoms);
        Bool add = 0 == strlen(argv[5]) ? True : False;

        HHWindow.send_event(windowId, name, atoms, num_atoms, add);
    }
    else if (0 == strcmp(subCommand, "set-prop"))
    {
        // Arg 3 = Prop Name
        // Arg 4 = Atoms as comma-separated string

        if (5 > argc)
        {
            fprintf(stderr, "Error: Missing Options.\n\n");
            help(argv[0]);
            exit(5);
        }

        int num_atoms = 0;
        char *name = argv[3];
        char **atoms = HHUtil.delimit(argv[4], ",", &num_atoms);

        HHWindow.set_property(windowId, name, atoms, num_atoms);
    }
    else
    {
        help(argv[0]);
        exit(1);
    }

    return 0;
}
