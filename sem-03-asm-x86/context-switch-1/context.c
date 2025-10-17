#include <stdio.h>
#include <stdlib.h>

// Inspired by https://gitlab.com/Lipovsky/sure

void* prepare_rsp_asm(void* stack, void* fun);

/////////////// public interface

// rsp == execution context

void switch_to_and_save_current(void **rsp_for_save, void* to_rsp);

void prepare_rsp(void** rsp_for_prepare, void* fun_stack, void(*fun)()) {
    *rsp_for_prepare = prepare_rsp_asm(fun_stack, fun);
}

//////////////////////////////////

void* main_rsp = NULL;
void* fun_rsp = NULL;

void func() {
    printf("[func] starting execution...\n");
    switch_to_and_save_current(&fun_rsp, main_rsp);
    printf("[func] resuming...\n");
    switch_to_and_save_current(&fun_rsp, main_rsp);
    printf("[func] exiting...\n");
    switch_to_and_save_current(&fun_rsp, main_rsp);
}

int main() {
    const size_t ST_SZ = 1024*1024;
    void* stack_end = malloc(ST_SZ);
    void* stack = stack_end + ST_SZ - 1;

    prepare_rsp(&fun_rsp, stack, func);

    printf("[main] program started\n");
    switch_to_and_save_current(&main_rsp, fun_rsp);
    printf("[main] execution suspended 1\n");
    switch_to_and_save_current(&main_rsp, fun_rsp);
    printf("[main] execution suspended 2\n");
    switch_to_and_save_current(&main_rsp, fun_rsp);
    printf("[main] execution suspended 3 - exiting\n");
    
    free(stack_end);
}