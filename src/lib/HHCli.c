#include "HHCli.h"

char *script_name;

struct option long_options[] = {
    {"help", no_argument, NULL, 'h'},
    {"id", required_argument, NULL, 'i'},
    {"maximize", no_argument, NULL, 'm'},
    {"minimize", no_argument, NULL, 'n'},
    {"restore", no_argument, NULL, 'r'},
    {"raise", no_argument, NULL, 500},
    {"set-title", required_argument, NULL, 'l'},
    {"set-role", required_argument, NULL, 'o'},
    {"set-window-type", required_argument, NULL, 'w'},
    {"width", required_argument, NULL, 100},
    {"height", required_argument, NULL, 101},
    {"toggle-fixed-size", optional_argument, NULL, 'z'},
    {"toggle-above", optional_argument, NULL, 'a'},
    {"toggle-below", optional_argument, NULL, 'b'},
    {"toggle-shade", optional_argument, NULL, 's'},
    {"toggle-sticky", optional_argument, NULL, 'y'},
    {"toggle-taskbar", optional_argument, NULL, 't'},
    {"toggle-pager", optional_argument, NULL, 'p'},
    {"toggle-fullscreen", optional_argument, NULL, 'f'},
    {"raw-set-prop", required_argument, NULL, 1000},
    {"raw-send-event", required_argument, NULL, 1010},
    {"atoms", required_argument, NULL, 1500},
    {"atom-format", required_argument, NULL, 1505},
    {"atom-type", required_argument, NULL, 1506},
    {"atom-value-raw", no_argument, NULL, 1507},
    {"event-mode", required_argument, NULL, 1510},
    {0, 0, 0, 0}};

void help(int exit_code, char *error_message)
{
    printf(
        "\nUsage: %s %s[--help]%s --id <window_id> <subcommand> %s[options]%s\n\n"
        " -h, --help: Displays this help message.\n\n"
        " -i <window_id>, --id <window_id>: The ID of the Window. This must *always* be supplied!\n\n"
        " <subcommand>: Any one of the Sub Commands detailed below, along with options:\n\n"
        " --width=<width>: Used to supply Width values where necessary.\n\n"
        " --height=<height>: Used to supply Height values where necessary.\n\n"
        " -m, --maximize: Maximizes a Window.\n\n"
        " -n, --minimize: Minimize a Window.\n\n"
        " -r, --restore: Restore typical values for a Window.\n\n"
        " -l=\"New Title\", --set-title=\"New Title\": Sets a Title for a Window.\n\n"
        " -o=\"New_Role\", --set-role=\"New_Role\": Sets a Role for a Window.\n\n"
        " -w=<window_type>, --set-window-type=<window_type>: Sets a Window Type for a Window.\n"
        "    See: https://specifications.freedesktop.org/wm-spec/1.5/ar01s05.html#:~:text=_NET_WM_WINDOW_TYPE%%2C%%20ATOM%%5B%%5D/32\n\n"
        " -z%s[=<enabled>]%s, --toggle-fixed-size%s[=<enabled>]%s: Sets a fixed size for a Window.\n"
        "    <enabled>: %s(Optional)%s Supply any value to Enable, leave empty to Disable.\n"
        "      - If Disabling, removes fixed-size and ignores other options.\n"
        "      - If Enabling, --width/--height *must* also be supplied!\n\n"
        " -a%s[=<enabled>]%s, --toggle-above=%s[<enabled>]%s: Toggles Always Above for a Window.\n"
        "    <enabled>: %s(Optional)%s Supply any value to Enable, leave empty to Disable.\n\n"
        " -b%s[=<enabled>]%s, --toggle-below=%s[<enabled>]%s: Toggles Always Below for a Window.\n"
        "    <enabled>: %s(Optional)%s Supply any value to Enable, leave empty to Disable.\n\n"
        " -s%s[=<enabled>]%s, --toggle-shade=%s[<enabled>]%s: Toggles RollUp Shade for a Window.\n"
        "    <enabled>: %s(Optional)%s Supply any value to Enable, leave empty to Disable.\n\n"
        " -y%s[=<enabled>]%s, --toggle-sticky=%s[<enabled>]%s: Toggles Sticky Mode for a Window.\n"
        "    <enabled>: %s(Optional)%s Supply any value to Enable, leave empty to Disable.\n\n"
        " -t%s[=<enabled>]%s, --toggle-taskbar=%s[<enabled>]%s: Toggles Taskbar Entry for a Window.\n"
        "    <enabled>: %s(Optional)%s Supply any value to Enable, leave empty to Disable.\n\n"
        " -p%s[=<enabled>]%s, --toggle-pager=%s[<enabled>]%s: Toggles Allow Pager for a Window (not working?).\n"
        "    <enabled>: %s(Optional)%s Supply any value to Enable, leave empty to Disable.\n\n"
        " -f%s[=<enabled>]%s, --toggle-fullscreen=%s[<enabled>]%s: Toggles Full Screen for a Window.\n"
        "    <enabled>: %s(Optional)%s Supply any value to Enable, leave empty to Disable.\n\n\n"
        "Special Sub Commands:\n\n"
        " --atoms=\"ATOM_1,ATOM_2\": Used to supply a list of ATOMs where necessary. Accepts a comma-separated and quoted string. If using a `--raw` Sub Command this *must* also be supplied!\n\n"
        " --atom-format=8|16|32: Used to specify the ATOM Format in Bits. Defaults to 32.\n\n"
        " --atom-type=<atom_type>: Used to specify the ATOM Type. Defaults to \"ATOM.\"\n"
        "   See: https://tronche.com/gui/x/xlib/window-information/properties-and-atoms.html#:~:text=The%%20built%%2Din%%20property%%20types%%20are\n\n"
        " --atom-value-raw: If set, values specified in `--atoms` will NOT be converted to XAtom types.\n\n"
        " --event-mode=0|1|2|3: Used to specify the Event Mode.\n"
        "   - 0 = Remove | 1 = Add | 2 = Toggle (doesn't always work) | 3 = IconicState\n\n"
        " --raw-set-prop=<prop_name>: Sets a series of Atoms for a given Property Name\n"
        "   See: https://tronche.com/gui/x/xlib/window-information/properties-and-atoms.html#:~:text=The%%20built%%2Din%%20property%%20names%%20are\n\n"
        " --raw-send-event=<event_name>: Sends a `ClientMessage` Event of Atoms to the Root Window.\n"
        "   See: https://tronche.com/gui/x/xlib/events/client-communication/client-message.html#\n"
        "   See: https://tronche.com/gui/x/xlib/event-handling/XSendEvent.html\n"
        "\n\n",

        script_name,

        COLOR_GREY, COLOR_RESET,
        COLOR_GREY, COLOR_RESET,
        COLOR_GREY, COLOR_RESET,
        COLOR_GREY, COLOR_RESET,
        COLOR_GREY, COLOR_RESET,
        COLOR_GREY, COLOR_RESET,
        COLOR_GREY, COLOR_RESET,
        COLOR_GREY, COLOR_RESET,
        COLOR_GREY, COLOR_RESET,
        COLOR_GREY, COLOR_RESET,
        COLOR_GREY, COLOR_RESET,
        COLOR_GREY, COLOR_RESET,
        COLOR_GREY, COLOR_RESET,
        COLOR_GREY, COLOR_RESET,
        COLOR_GREY, COLOR_RESET,
        COLOR_GREY, COLOR_RESET,
        COLOR_GREY, COLOR_RESET,
        COLOR_GREY, COLOR_RESET,
        COLOR_GREY, COLOR_RESET,
        COLOR_GREY, COLOR_RESET,
        COLOR_GREY, COLOR_RESET,
        COLOR_GREY, COLOR_RESET,
        COLOR_GREY, COLOR_RESET,
        COLOR_GREY, COLOR_RESET,
        COLOR_GREY, COLOR_RESET,
        COLOR_GREY, COLOR_RESET);

    if (NULL != error_message && 0 != strlen(error_message))
    {
        printf("%s%s%s\n\n",
               COLOR_RED,
               error_message,
               COLOR_RESET);
    }

    exit(exit_code);
}
