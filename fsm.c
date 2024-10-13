#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include "fsm.h"

static state_func fsm_transition(int from_id, int to_id, const StateTransition transitions[]);
void fsm_run(Environment *env, int from_id, int to_id, const StateTransition transitions[]) {
    int to_state_id;
    int from_state_id;

    to_state_id = to_id;
    from_state_id = from_id;

    do {
        state_func perform;
        perform = fsm_transition(from_state_id, to_state_id, transitions);
        env->from_state_id = from_state_id;
        env->current_state_id = to_state_id;
        from_state_id = to_state_id;
        to_state_id = perform(env);
        sleep(2);
    } while(true);
}

static state_func fsm_transition(int from_id, int to_id, const StateTransition transitions[]) {
    const StateTransition *transition;
    transition = &transitions[0];
    while(transition->perform) {
        if (transition->from_id == from_id && transition->to_id == to_id) {
            return transition->perform;
        }
        transition = transitions++;
    }
    return NULL;
}
