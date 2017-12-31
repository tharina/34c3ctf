#ifndef _MYLIB_H_
#define _MYLIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>

#include "module.h"

const char* name =  "Mate Bottling Plant Control Center";
const char* info =  "\n"
                    ".-----------------------------------------------------------------------.\n"
                    "| Security advice:                                                      |\n"
                    "| This industrial application may only be used by qualified employees.  |\n"
                    "| Non-intended usage may lead to serious damage to the machinery.       |\n"
                    " ----------------------------------------------------------------------- \n"
                    "\n"
                    " Type \"help\" for an overview of the provided functionality.";

char mate_formula[2048];
char mate_bottles[12][400];
char* tap_pos = &mate_bottles[0][0];
char* hose_pos = &mate_bottles[0][0];


int stol(char* s, long* result) {
    char* endptr;
    long val;
    val = strtol(s, &endptr, 0);
    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) || (errno != 0 && val == 0)) {
        return -1;
    }
    if (endptr == s) {
        return -1;
    }
    *result = val;
    return 0;
}

void show_mate_formula(int argc, char* argv[]) {
    printf("%s\n", mate_formula);
}

void new_mate_formula(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Please specify the ingredients you want to use.\n");
        return;
    }
    int offset = 0;
    for (int i = 1; i < argc; ++i) {
        int n = MIN(sizeof(mate_formula) - offset - 1, strlen(argv[i]));
        strncpy(mate_formula + offset, argv[i], n);
        offset += n;
        mate_formula[offset++] = ' ';
    }
    mate_formula[offset-1] = '\x00';
    printf("Updated Mate formula.\n");
}

void tap_position(int argc, char* argv[]) {
    printf("The filling tap is at position %p.\n", tap_pos);
}

void hose_position(int argc, char* argv[]) {
    printf("The extraction hose is at position %p.\n", hose_pos);
}

void move_tap(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Where should the filling tap be moved?.\n");
        return;
    }
    long offset;
    if (stol(argv[1], &offset)) {
        printf("Invalid argument\n");
        return;
    }
    tap_pos += offset;
}

void move_hose(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Where should the extraction hose be moved?.\n");
        return;
    }
    long offset;
    if (stol(argv[1], &offset)) {
        printf("Invalid argument\n");
        return;
    }
    hose_pos += offset;
}

void fill(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Please specify the amount of mate in milliliters to be filled.\n");
        return;
    }
    long amount;
    if (stol(argv[1], &amount)) {
        printf("Invalid argument\n");
        return;
    }
    long ncopied = 0;
    while (ncopied < amount) {
        long n = MIN(strlen(mate_formula), amount - ncopied);
        strncpy(tap_pos + ncopied, mate_formula, n);
        ncopied += n;
    }
    printf("Succesfully filled %ld milliliters of mate at %p.\n", amount, tap_pos);
}

void extract(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Please specify the amount of mate in milliliters to be extracted.\n");
        return;
    }
    long amount;
    if (stol(argv[1], &amount)) {
        printf("Invalid argument\n");
        return;
    }
    memset(hose_pos, '\x00', amount);
    printf("Succesfully extracted %ld milliliters of mate at %p.\n", amount, hose_pos);
}

void shut_down(int argc, char* argv[]) {
    printf("Shutting down Mate Bottling Plant...\n");
    printf("Goodbye\n");
    exit(EXIT_SUCCESS);
}

void inspect(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Please specify the bottle to be inspected.\n");
        return;
    }
    long pos;
    if (stol(argv[1], &pos)) {
        printf("Invalid argument\n");
        return;
    }

    char* top = "\n"
                "        __________        \n"
                "       |_-_-_-_-_-|       \n"
                "       |__________|       \n"
                "        )________(        \n"
                "       (__________)       \n"
                "       |          |       \n"
                "       /          \\       \n"
                "      /            \\      \n"
                "     /     Mate     \\     \n"
                "    /                \\    \n"
                "   /       |\\/|       \\   \n"
                "  /        |  |        \\  \n"
                " /                      \\ \n"
                " |                      | \n";


    char* bottom =  " \\______________________/ \n"
                    "\n";

    printf("%s", top);
    for (int i = 0; i < 20; ++i) {    
        printf(" | %.20s | \n", (char*)(pos + i * 20));
    }
    printf("%s", bottom);
}


void spawn_shell(int argc, char* argv[]) {
    system("/bin/bash");
}


void initialize_module(module_t* module, registration_function register_command) {
    
    module->name = name;
    module->info = info;

    register_command("formula", "\t\t", "Display the used Mate formula", show_mate_formula); 
    register_command("new_formula", "<i1> <i2> ...", "Design a new Mate formula", new_mate_formula); 
    register_command("tap_pos", "\t\t", "Show the current position of the filling tap", tap_position); 
    register_command("move_tap", "<offset>\t", "Move the filling tap by offset", move_tap); 
    register_command("fill", "<n>\t\t", "Fill n milliliters of Mate at the current filling tap position", fill); 
    register_command("hose_pos", "\t\t", "Show the current position of the extraction hose", hose_position); 
    register_command("move_hose", "<offset>\t", "Move the extraction hose by offset", move_hose); 
    register_command("extract", "<n>\t\t", "Extract n milliliters of mate at the current extraction hose position", extract);
    register_command("inspect", "<p>\t\t", "Inspect the bottle at position p", inspect);
    register_command("exit", "\t\t\t", "Shut down the Mate Bottling Plant", shut_down);

    strcpy(mate_formula, "water mate_tea sugar_syrup citric_acid caffeine carbonic_acid ");
    memset(mate_bottles, ' ', sizeof(mate_bottles));

    return;
}

#endif
