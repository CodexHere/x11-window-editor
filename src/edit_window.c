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
           "  maximize - Maximizes a Window\n"
           "  minimize - Minimize a Window\n"
           "  restore - Restore typical values for a Window\n"
           "  toggle-fixed-size [<enabled> <width> <height>] - Sets a fixed size for a Window\n"
           "    <enabled>: Enabled Status - If unsupplied, removes fixed-size and ignores other options.\n"
           "    <width>: If Enabled, Fixed Width of the Window\n"
           "    <height>: If Enabled, Fixed Height of the Window\n"
           "  toggle-above <enabled> - Toggles Always Above for a Window\n"
           "    <enabled>: Enabled Status\n"
           "  toggle-below <enabled> - Toggles Always Below for a Window\n"
           "    <enabled>: Enabled Status\n"
           "  toggle-fullscreen <enabled> - Toggles Full Screen for a Window\n"
           "    <enabled>: Enabled Status\n"
           "  toggle-pager <enabled> - Toggles Allow Pager for a Window (not working?)\n"
           "    <enabled>: Enabled Status\n"
           "  toggle-shade <enabled> - Toggles RollUp Shade for a Window\n"
           "    <enabled>: Enabled Status\n"
           "  toggle-sticky <enabled> - Toggles Sticky Mode for a Window\n"
           "    <enabled>: Enabled Status\n"
           "  toggle-taskbar <enabled> - Toggles Taskbar Entry for a Window\n"
           "    <enabled>: Enabled Status\n"
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

    Window window_id = strtol(argv[1], NULL, 0);
    char *sub_command = argv[2];
    long mode = (4 > argc) ? HH_EVENT_MODE_REMOVE : HH_EVENT_MODE_ADD;

    if (0 == strcmp(sub_command, "minimize"))
    {
        HHWindow.minimize(window_id);
    }
    else if (0 == strcmp(sub_command, "maximize"))
    {
        HHWindow.maximize(window_id);
    }
    else if (0 == strcmp(sub_command, "restore"))
    {
        HHWindow.restore(window_id);
    }
    else if (0 == strcmp(sub_command, "toggle-sticky"))
    {
        HHWindow.toggleSticky(window_id, mode);
    }
    else if (0 == strcmp(sub_command, "toggle-shade"))
    {
        HHWindow.toggleShade(window_id, mode);
    }
    else if (0 == strcmp(sub_command, "toggle-taskbar"))
    {
        long mode = (4 > argc) ? HH_EVENT_MODE_ADD : HH_EVENT_MODE_REMOVE;
        HHWindow.toggleTaskbar(window_id, mode);
    }
    else if (0 == strcmp(sub_command, "toggle-pager"))
    {
        long mode = (4 > argc) ? HH_EVENT_MODE_ADD : HH_EVENT_MODE_REMOVE;
        HHWindow.togglePager(window_id, mode);
    }
    else if (0 == strcmp(sub_command, "toggle-fullscreen"))
    {
        HHWindow.toggleFullscreen(window_id, mode);
    }
    else if (0 == strcmp(sub_command, "toggle-above"))
    {
        HHWindow.toggleAbove(window_id, mode);
    }
    else if (0 == strcmp(sub_command, "toggle-below"))
    {
        HHWindow.toggleBelow(window_id, mode);
    }
    else if (0 == strcmp(sub_command, "toggle-fixed-size"))
    {
        // Args 3 & 4 are width and height

        if (5 > argc)
        {
            HHWindow.toggleFixedSize(window_id, False, NULL, NULL);
        }
        else
        {
            HHWindow.toggleFixedSize(window_id, True, argv[3], argv[4]);
        }
    }
    else if (0 == strcmp(sub_command, "send-event"))
    {
        // Arg 3 = Event Name
        // Arg 4 = Atoms as comma-separated string
        // Arg 5 = Which HH_EVENT_MODE* to use

        if (5 > argc)
        {
            fprintf(stderr, "Error: Missing Options.\n\n");
            help(argv[0]);
            exit(5);
        }

        int num_atoms = 0;
        char *name = argv[3];
        char **atoms = HHUtil.delimit(argv[4], ",", &num_atoms);
        long mode = (6 > argc) ? HH_EVENT_MODE_TOGGLE : strtol(argv[5], NULL, 0);

        HHWindow.send_event(window_id, name, atoms, num_atoms, mode);
    }
    else if (0 == strcmp(sub_command, "set-prop"))
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

        HHWindow.set_property(window_id, name, atoms, num_atoms);
    }
    else
    {
        help(argv[0]);
        exit(1);
    }

    return 0;
}
