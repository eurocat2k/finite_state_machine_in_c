#ifndef __FSM_H__
#define __FSM_H__

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int from_state_id;
    int current_state_id;
} Environment;

typedef int (*state_func)(Environment *env);
typedef struct {
    int from_id;
    int to_id;
    state_func perform;
} StateTransition;

void fsm_run(Environment *env, int from_id, int to_id, const StateTransition transitions[]);

#ifdef __cplusplus
}
#endif

#endif // __FSM_H__
