#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

// Inspired by https://gitlab.com/Lipovsky/tinyfiber

void* prepare_rsp_asm(void* stack, void* helper, void* helper_arg);

void switch_to_and_save_current(void **rsp_for_save, void* to_rsp);

struct scheduler;

typedef struct fiber {
    void(*fun)();
    void* context;
    bool finished;
    struct scheduler* scheduler;
} fiber_t;

typedef struct scheduler {
    void* main_context;
    fiber_t* current_fiber;
    // some queue...
} scheduler_t;

scheduler_t* global_scheduler;

void run_fiber(fiber_t* fiber) {
    (*fiber->fun)();
    fiber->finished = true;
    switch_to_and_save_current(&(fiber->context), fiber->scheduler->main_context);
}

void start_helper(void*, void*, void*, void*, void*, void*, void* arg7) {
    run_fiber((fiber_t*)arg7);
}

void prepare_rsp(void** rsp_for_prepare, void* fiber_stack, fiber_t* fiber) {
    *rsp_for_prepare = prepare_rsp_asm(fiber_stack, start_helper, fiber);
}

//////////////////////////////////

void func() {
    printf("[func] starting execution...\n");
    switch_to_and_save_current(&(global_scheduler->current_fiber->context), global_scheduler->main_context);
    printf("[func] resuming...\n");
    switch_to_and_save_current(&(global_scheduler->current_fiber->context), global_scheduler->main_context);
    printf("[func] exiting...\n");
}

int main() {
    // create scheduler
    scheduler_t scheduler;
    global_scheduler = &scheduler;

    // allocate stack
    const size_t ST_SZ = 1024*1024;
    void* stack_end = malloc(ST_SZ);
    void* stack = stack_end + ST_SZ - 1;
    // create fiber
    fiber_t fiber = {
        .fun = func,
        .context = NULL,
        .finished = false,
        .scheduler = &scheduler,
    };
    // set current_fiber
    global_scheduler->current_fiber = &fiber;
    // prepare fiber context
    prepare_rsp(&(fiber.context), stack, &fiber);

    // schedule fiber until finished
    printf("[main] program started\n");
    uint32_t steps = 0;
    while (!scheduler.current_fiber->finished) {
        switch_to_and_save_current(&(scheduler.main_context), scheduler.current_fiber->context);
        ++steps;
        printf("[main] execution suspended %d\n", steps);
    }  
    printf("[main] fiber finished - exiting...\n");
    
    // free stack
    free(stack_end);
}