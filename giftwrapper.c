#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>

#include "module.h"

const char* name = "Gift Wrapping Factory";
const char* info = "Welcome to the new gift wrapping service!\nType \"help\" for help :)";


void wrap(int argc, char* argv[]) {

    printf("What is the size of the gift you want to wrap?\n |> ");
    char buf[11];
    memset(buf, 0, sizeof(buf));
    if (read(1, buf, 10) <= 0)
        exit(EXIT_FAILURE);
    short size = strtol(buf, NULL, 0);
    if (size >= 100) {
        puts("Sorry! This gift is too large.");
        return;
    }
    
    printf("Please send me your gift.\n |> ");
    char input_buffer[100];
    memset(input_buffer, '\x00', sizeof(input_buffer));
    int nread = read(1, input_buffer, (unsigned short) size + 1);
    input_buffer[nread - 1] = '\x00';

    const char* gift_top =    "         _   _       \n"
                        "        ((\\o/))      \n"
                        " .-------//^\\\\------.\n"
                        " |      /`   `\\     |\n"
                        " |                  |\n";
    const char* gift_bottom = " |                  |\n"
                        "  ------------------ \n";
    
    printf("%s", gift_top);
    for (int i = 0; i < size; i += 16) {
        int n = printf(" | %.16s", input_buffer + i);
        printf("%*c |\n", 19 - n, ' ');
    }
    printf("%s\n", gift_bottom);

    printf("Wow! This looks so beautiful\n");
}

void spawn_shell(int argc, char* argv[]) {
    system("/bin/bash");
}


void initialize_module(module_t* module, registration_function register_command) {
    
    module->name = name;
    module->info = info;

    register_command("wrap", "\t\t\t", "Wrap a gift", wrap);
}
