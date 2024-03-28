#include "HHCli.h"

char *script_name;

struct option long_options[] = {
    {"help", no_argument, 0, 'h'},
    {"id", required_argument, 0, 'i'},
    {"maximize", no_argument, 0, 'm'},
    {"minimize", no_argument, 0, 'n'},
    {"restore", no_argument, 0, 'r'},
    {"width", required_argument, 0, 100},
    {"height", required_argument, 0, 101},
    {"toggle-fixed-size", optional_argument, 0, 'z'},
    {"toggle-above", optional_argument, 0, 'a'},
    {"toggle-below", optional_argument, 0, 'b'},
    {"toggle-shade", optional_argument, 0, 's'},
    {"toggle-sticky", optional_argument, 0, 'y'},
    {"toggle-taskbar", optional_argument, 0, 't'},
    {"toggle-pager", optional_argument, 0, 'p'},
    {"toggle-fullscreen", optional_argument, 0, 'f'},
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
        "    <enabled>: %s(Optional)%s Supply any value to Enable, leave empty to Disable.\n\n",

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
