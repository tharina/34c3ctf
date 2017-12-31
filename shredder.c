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

const char* name =  "Gift Shredder Plant Control Center";
const char* info =  "\n"
                    "As a new service¹ for our customers who are not satisfied with this year's christmas presents,\n"
                    "we launched the modern, state of the art Gift Shredder Plant.\n"
                    "\n"
                    "(¹) Unfortunately we are not able to compensate for your financial loss.\n"
                    "\n"
                    "Type \"help\" for information on how to run the shredder.";


const char* shredder_pipe = "\n"
                            "             |  _       _   _        _  _  |\n"
                            "             | |_  |_| |_| |_ |\\ |\\ |_ |_| |\n"
                            "             |  _| | | |\\  |_ |/ |/ |_ |\\  |\n"
                            "             |_____________________________|\n";


void animate(const char* lines[], size_t speed) {
    int n = 0;
    while (n++ < (50 - 4 * speed)) {
        printf("%s", shredder_pipe);
        for(int i = 0; i < 10; i++) {
            printf("%s\n", lines[(9 - i+n) % 10]);
        }
        printf("\n");
        usleep(100000);
        printf("\r");
        printf("\x1b[16A");
    }
    printf("\x1b[16B");
}

void shred_o(size_t speed) {
    const char* lines[10];
    lines[0] = "                                       o";
    lines[1] = "                o         o             ";
    lines[2] = "                    o              o    ";
    lines[3] = "                                        ";
    lines[4] = "                       o                ";
    lines[5] = "                   o          o        o";
    lines[6] = "                                        ";
    lines[7] = "                      o          o      ";
    lines[8] = "                                        ";
    lines[9] = "                o              o        ";

    animate(lines, speed);
}

void shred_strip(size_t speed) {
    const char* lines[10];
    lines[0] = "                                       (";
    lines[1] = "                )         (             ";
    lines[2] = "                    (              )    ";
    lines[3] = "                                        ";
    lines[4] = "                       )                ";
    lines[5] = "                   (          (        )";
    lines[6] = "                                        ";
    lines[7] = "                      )          (      ";
    lines[8] = "                                        ";
    lines[9] = "                (              )        ";

    animate(lines, speed);
}

void shred_star(size_t speed) {
    const char* lines[10];
    lines[0] = "                                       *";
    lines[1] = "                *         *             ";
    lines[2] = "                    *              *    ";
    lines[3] = "                                        ";
    lines[4] = "                       *                ";
    lines[5] = "                   *          *        *";
    lines[6] = "                                        ";
    lines[7] = "                      *          *      ";
    lines[8] = "                                        ";
    lines[9] = "                *              *        ";

    animate(lines, speed);
}

void shred_dot(size_t speed) {
    const char* lines[10];
    lines[0] = "                                       .";
    lines[1] = "                .         .             ";
    lines[2] = "                    .              .    ";
    lines[3] = "                                        ";
    lines[4] = "                       .                ";
    lines[5] = "                   .          .        .";
    lines[6] = "                                        ";
    lines[7] = "                      .          .      ";
    lines[8] = "                                        ";
    lines[9] = "                .              .        ";

    animate(lines, speed);
}

typedef struct gift {
    char name[32];
} gift_t;

typedef struct pallet {
    size_t capacity;
    gift_t gifts[];
} pallet_t;

typedef struct shredder {
    size_t speed;
    void (*animation)();
} shredder_t;


shredder_t* shredder;
pallet_t* pallet;




int stol(char* s, size_t* result) {
    char* endptr;
    size_t val;
    val = strtoul(s, &endptr, 0);
    if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) || (errno != 0 && val == 0)) {
        return -1;
    }
    if (endptr == s) {
        return -1;
    }
    *result = val;
    return 0;
}



void request_shredder(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Please specify at which speed the shredder should operate.\n");
        return;
    }
    size_t speed;
    if (stol(argv[1], &speed)) {
        printf("Invalid argument\n");
        return;
    }
    if (speed < 1 || speed > 10) {
        printf("Our shredders are only able to shred 1-10 gifts per second.\n");
        return;
    }

    shredder_t* s = malloc(sizeof(shredder_t));
    s->speed = speed;

    int r = rand() % 4;
    switch(r) {
        case 0:
            s->animation = shred_o;
            break;
        case 1:
            s->animation = shred_star;
            break;
        case 2:
            s->animation = shred_strip;
            break;
        case 3:
            s->animation = shred_dot;
            break;
    }

    if (shredder) {
        free(shredder);
        printf("Returning the previous shredder.\n");
    }
    printf("Received new shredder.\n");
    shredder = s;
    
}

void request_pallet(int argc, char* argv[]) {
    if (pallet) {
        printf("You already have a pallet!");
        return;
    }

    if (argc < 2) {
        printf("Please specify the required gift capacity of the pallet.\n");
        return;
    }
    size_t capacity;
    if (stol(argv[1], &capacity)) {
        printf("Invalid argument\n");
        return;
    }
   
    size_t n = sizeof(pallet_t) + capacity * sizeof(gift_t);
    pallet_t* p = malloc(n);
    if (!p) {
        printf("Sorry. We do not have pallets that large.\n");
        return;
    }
    memset(p, '\x00',  n);
    p->capacity = capacity;
    
    printf("Received pallet\n");
    pallet = p;
}

void put_gift(int argc, char* argv[]) {
    if (!pallet) {
        printf("You have to request a pallet to store the gifts first.\n");
        return;
    }
    
    if (argc < 3) {
        printf("Specify the positon on the pallet where you want to put the gift and a description of it.\n");
        return;
    }
    size_t index;
    if (stol(argv[1], &index)) {
        printf("Invalid argument\n");
        return;
    }
   
    if (index >= pallet->capacity) {
        printf("The pallet is not large enough.\n");
        return;
    }

    strncpy(pallet->gifts[index].name, argv[2], sizeof(gift_t) - 1);
}

void shred(int argc, char* argv[]) {
    if (!shredder) {
        printf("Please request a shredder first.\n");
        return;
    }
    if (!pallet) {
        printf("There is no pallet with gifts to shred.\n");
        return;
    }

    shredder->animation(shredder->speed);

    free(pallet);
    printf("Returning pallet.\n");
    pallet = NULL;
}


void shut_down(int argc, char* argv[]) {
    printf("Shutting down Gift Shredder Plant...\n");
    printf("Goodbye\n");
    exit(EXIT_SUCCESS);
}



void spawn_shell(int argc, char* argv[]) {
    system("/bin/bash");
}


void initialize_module(module_t* module, registration_function register_command) {
   
    register_command("request_shredder", "<n>\t", "Request a shredder which can shred n gifts per second", request_shredder);
    register_command("request_pallet", "<n>\t", "Request a pallet on which n gifts can be stored", request_pallet);
    register_command("put", "<index>, <description>", "Put a gift at position <index> onto the pallet.", put_gift);
    register_command("shred", "\t\t\t", "Shred all the gifts", shred);

    module->name = name;
    module->info = info;
    
    srand(time(NULL));
}

#endif
