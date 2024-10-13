/**
 * F I N I T E  S T A T E  M A N A G E R  D E M O
 *
 * Based on D'Arcy Smith demonstrations.
 *
 * Compilation - I'm using LLVM:
 *
 *      clang -O3 -o fsm[.exe] main.c fsm.c
 *
 * Running:
 *
 *      ./fsm[.exe]
 *
 * Please not that the '[.exe]' is optional addition on WSL environent.
 * On UNIX or Linux that is not recommended.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>
#include "fsm.h"
static bool initialized = false;
static int initial_state;
typedef enum {INIT, START, A, B, END, ERROR} state;
static const char *State[] = {
    "INIT", "START", "A", "B", "END", "ERROR"
};
typedef struct {
    Environment common;
    int a_or_b_or_restart_or_end;
    int exitCode;
} TheEnvironment;
//
int start(Environment *env) {
    printf("%14s: from \033[34m%s\033[0m -> to \033[35m%s\033[0m ", "start running", State[env->from_state_id], State[env->current_state_id]);
    TheEnvironment *env2;
    env2 = (TheEnvironment *)env;
    int r, ret;
    if (!initialized) {
        initialized = true;
        if (env2->a_or_b_or_restart_or_end == 'A') {
            ret = A;
        } else {
            ret = B;
        }
    } else {
        r = (rand() % (100 - (0 + 1) + 0));
        ret = r > 50 ? A : B;
    }
    if (ret == A) printf("calling a\n");
    else printf("calling b\n");
    return ret;
}
int a(Environment *env) {
    printf("%14s: from \033[34m%s\033[0m -> to \033[35m%s\033[0m ", "a running", State[env->from_state_id], State[env->current_state_id]);
    TheEnvironment *env2;
    env2 = (TheEnvironment *)env;
    int r = (rand() % (100 - (0 + 1) + 0));
    env2->exitCode = r;
    int ret = r > 63 ? A : r <= 63 && r > 33 ? B : START;
    if (ret == A) printf("calling a\n");
    else if (ret == B) printf("calling b\n");
    else printf("calling start\n");
    return ret;
}
int b(Environment *env) {
    printf("%14s: from \033[34m%s\033[0m -> to \033[35m%s\033[0m ", "b running", State[env->from_state_id], State[env->current_state_id]);
    TheEnvironment *env2;
    env2 = (TheEnvironment *)env;
    int r = (rand() % (100 - (0 + 1) + 0));
    env2->exitCode = r;
    int ret = r > 63 ? A : r <= 63 && r > 33 ? B : START;
    if (ret == A) printf("calling a\n");
    else if (ret == B) printf("calling b\n");
    else printf("calling start\n");
    return ret;
}
int end(Environment *env) {
    printf("%14s: from \033[34m%s\033[0m -> to \033[35m%s\033[0m\n", "end running", State[env->from_state_id], State[env->current_state_id]);
    TheEnvironment *env2;
    env2 = (TheEnvironment *)env;
    exit(env2->exitCode);
}
int terminate(Environment *env) {
    printf("%14s: from \033[34m%s\033[0m -> to \033[35m%s\033[0m\n", "terminate running", State[env->from_state_id], State[env->current_state_id]);
    exit(EXIT_FAILURE);
}
//
int main(void) {

    srand(time(NULL) % (int)getpid());
    initial_state = (rand() % (100 - (0 + 1) + 0));
    TheEnvironment env = {
        // .a_or_b_or_restart_or_end =  state > 63 ? 'A' : (state <= 63 && state > 33) ? 'B' : 'R'
        .a_or_b_or_restart_or_end = initial_state > 50 ? 'A' : 'B'
    };
    printf("Start Code: \033[91m%c\033[0m, so \033[34mINIT\033[0m triggers \033[34mSTART\033[0m state which will turn into \033[35m%c\033[0m state\n", (char)env.a_or_b_or_restart_or_end, (char)env.a_or_b_or_restart_or_end);
    StateTransition transitions[] = {
        {INIT, START, &start},
        {START, A, &a},
        {START, B, &b},
        {A, A, &a},
        {A, B, &b},
        {B, A, &a},
        {B, B, &b},
        {A, START, &start},
        {A, END, &a},
        {B, START, &start},
        {B, END, &b},
        {START, ERROR, &terminate},
        {A, ERROR, &terminate},
        {B, ERROR, &terminate},
        {-1, -1, NULL}
    };
    fsm_run((Environment *)&env, INIT, START, (const StateTransition *)&transitions);
    return EXIT_SUCCESS;
}
