#ifndef _MYLIB_H_
#define _MYLIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <time.h>

#include "module.h"


const char* name =  "Mate Bottling Plant Control Center 2.0";
const char* info =  "\n"
                    " There were some minor problems with the previous version of the Mate Bottling Plant.\n"
                    " We hired the world's best Mate specialists to improve usability, functionality and security!\n"
                    "\n"
                    " Type \"help\" for an overview of the provided functionality.";



const char* truck = "\n"
                    "            ____________________                            \n"
                    "           |\\                   \\      l____              \n"
                    "           | \\___________________\\     |\\   \\           \n"
                    "           | |                    |    |\\l___\\___         \n"
                    "      [__]_[ |       |\\/|         |[\\\\]| |__|_\\__\\      \n"
                    "     /\\[__]\\ |       |  |         |\\[\\\\]\\|. | |===\\  \n"
                    "     \\ \\[__]\\[____________________] \\[__]|__|..___]     \n"
                    "      \\/.-.\\_______________________\\/.-.\\____\\/.-.\\   \n"
                    "       ( @ )                        ( @ )  =  ( @ )         \n"
                    "        `-'                          `-'       `-'          \n"
                    "\n";

const char* scooter =   "\n"
                        "                            (_\\       \n"
                        "                           / \\        \n"
                        "           ______     `== / /\\=,_     \n"
                        "          | |\\/| |_ _ _;--==\\\\  \\\\o   \n"
                        "          |_|__|_|     /____//__/__\\  \n"
                        "          (0)  (0)   @=`(0)     (0)   \n"
                        "\n";

const char* heli =  "\n"
                    "---------------+---------------          \n"
                    "          ___ /^^[___              _     \n"
                    "         /|^+----+   |#___________//     \n"
                    "       ( -+ |____|    ______-----+/      \n"
                    "        ==_________--'            \\     \n"
                    "          ~_|___|__                      \n"
                    "              |                          \n"
                    "              |                          \n"
                    "              |                          \n"
                    "           ___|___                       \n"
                    "          | |\\/| |                      \n"
                    "          |_|__|_|                       \n"
                    "                                         \n"
                    "\n";

const char* airplane =  "\n"
                        " __  _                               \n"
                        " \\ `/ |                             \n"
                        "  \\__`!                             \n"
                        "  / ,' `-.__________________         \n"
                        " '-'\\_____                 o`-.     \n"
                        "    <____()-=O=O=O=O=O=[]====--)     \n"
                        "      `.___ ,-----,_______...-'      \n"
                        "           /    .'                   \n"
                        "          /   .'                     \n"
                        "         /  .' |                     \n"
                        "         `-'   |                     \n"
                        "               |                     \n"
                        "            ___|___                  \n"
                        "           | |\\/| |                 \n"
                        "           |_|__|_|                  \n"
                        "                                     \n"
                        "\n";


void ship_airplane() {
    printf("%s", airplane);
}

void ship_heli() {
    printf("%s", heli);
}

void ship_scooter() {
    printf("%s", scooter);
}

void ship_truck() {
    printf("%s", truck);
}

char mate_formula[2048];


typedef struct bottle {
    size_t size;
    char content[];
} bottle_t;

typedef struct crate {
    size_t size;
    void (*shipping)();
    bottle_t* bottles[];
} crate_t;

crate_t* crate_stack[3];
int stack_height = 0;
bottle_t* tap_pos = NULL;

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

void new_crate(int argc, char* argv[]) {
    if (stack_height >= 3) {
        printf("You cannot stack more than 3 Mate crates. Please ship them first.\n");
    }
    if (argc < 3) {
        printf("Please specify the crate and bottle size. Crates of size 8, 12, or 16 are available.\n");
        return;
    }
    long n;
    if (stol(argv[1], &n) != 0 || ( n != 8 && n != 12 && n != 16)) {
        printf("Only crates of size 8, 12, or 16 are available.\n");
        return;
    }
    long m;
    if (stol(argv[2], &m) != 0 || m < 1 || m > 1024) {
        printf("Only bottles of size 1 to 1024 ml are available.\n");
        return;
    }
    crate_t* crate = malloc(sizeof(crate_t) + n * sizeof(bottle_t*));
    crate->size = n;
    for (int i = 0; i < n; ++i) {
        crate->bottles[i] = malloc(sizeof(bottle_t) + m * sizeof(char));
        crate->bottles[i]->size = m;
    }
    
    int r = rand() % 4;
    switch(r) {
        case 0:
            crate->shipping = ship_airplane;
            break;
        case 1:
            crate->shipping = ship_scooter;
            break;
        case 2:
            crate->shipping = ship_heli;
            break;
        case 3:
            crate->shipping = ship_truck;
            break;
    }

    crate_stack[stack_height++] = crate;
}

void show_crate(int argc, char* argv[]) {
    printf("There are currently %d Mate crates on the stack.\n", stack_height);
    if (stack_height < 1) {
        return;
    }
    printf("This is the crate on top:\n\n");

    crate_t* crate = crate_stack[stack_height - 1];
    printf("  ---------------- \n");
    for (int i = 0; i < crate->size; i += 4) {
        printf(" |");
        for (int j = 0; j < 4; ++j) {
            bottle_t* b = crate->bottles[i+j];
            if (strlen(b->content) > 0) {
                printf(" f |");
            } else {
                printf(" e |");
            }
        }
        printf("\n");
    }
    printf("  --- --- --- --- \n");
}

void tap_position(int argc, char* argv[]) {
    printf("The filling tap is at position %p.\n", tap_pos);
}

void move_tap(int argc, char* argv[]) {
    if (stack_height <= 0) {
        printf("There is no mate crate with bottles to be filled.\n");
        return;
    }
    if (argc < 2) {
        printf("Where should the filling tap be moved?.\n");
        return;
    }
    crate_t* crate = crate_stack[stack_height - 1];
    long index;
    if (stol(argv[1], &index)) {
        printf("Invalid argument\n");
        return;
    }
    if (index >= crate->size) {
        printf("There are only %ld bottles in the crate.\n", crate->size);
        return;
    }
    tap_pos = crate->bottles[index];
}

void fill_bottle(int argc, char* argv[]) {
    if (!tap_pos) {
        printf("Move the tap to a bottle first.\n");
        return;
    }
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
    if (amount > tap_pos->size) {
        printf("Bottle too small.");
        return;
    }
    while (ncopied < amount) {
        long n = MIN(strlen(mate_formula), amount - ncopied);
        strncpy(tap_pos->content + ncopied, mate_formula, n);
        ncopied += n;
    }
    //tap_pos->content[tap_pos->size - 1] = '\x00';
}

void shut_down(int argc, char* argv[]) {
    printf("Shutting down Mate Bottling Plant...\n");
    printf("Goodbye\n");
    exit(EXIT_SUCCESS);
}

void inspect(int argc, char* argv[]) {
    if (!tap_pos) {
        printf("No bottle at filling tap position found.\n");
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
    for (int i = 0; i < tap_pos->size; i += 20) {    
        int n = printf(" | %.20s", (tap_pos->content + i));
        printf("%*c| \n", 24 - n, ' ');

    }
    printf("%s", bottom);
}

void ship(int argc, char* argv[]) {
    for (int i = 0; i < stack_height; ++i) {
        crate_t* crate = crate_stack[i];
        crate->shipping();
        printf("\nShipped crate of size %ld\n\n", crate->size);
        for (int j = 0; j < crate->size; ++j) {
            free(crate->bottles[j]);
        }
        free(crate);
        crate_stack[i] = NULL;
    }
    stack_height = 0;
}

void spawn_shell(int argc, char* argv[]) {
    system("/bin/bash");
}

void initialize_module(module_t* module, registration_function register_command) {
    
    module->name = name;
    module->info = info;

    register_command("formula", "\t\t", "Display the used Mate formula", show_mate_formula); 
    register_command("new_formula", "<i1> <i2> ...", "Design a new Mate formula", new_mate_formula);
    register_command("new_crate", "<size> <bottle_size>", "Put a new crate of size 8, 12 or 16 on top of the crate stack", new_crate);
    register_command("show_crate", "\t\t", "Display the crate on top of the crate stack", show_crate);
    register_command("tap_pos", "\t\t", "Show the current position of the filling tap", tap_position); 
    register_command("move_tap", "<bottle_index>\t", "Move the filling tap to the bottle at a specific index in the top crate", move_tap); 
    register_command("fill", "<n>\t\t", "Fill the bottle at the current filling tap position", fill_bottle); 
    register_command("inspect", "\t\t", "Inspect the bottle at the current filling tap position.", inspect);
    register_command("ship", "\t\t\t", "Ship the current crate stack of Mate to ESPR", ship);
    register_command("exit", "\t\t\t", "Shut down the Mate Bottling Plant", shut_down);

    strcpy(mate_formula, "water mate_tea sugar_syrup citric_acid caffeine carbonic_acid ");

    srand(time(NULL));
    return;
}

#endif
