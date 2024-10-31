#pragma once

#include <stdatomic.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct node {
  uintptr_t value;
  struct node* next;
} node;

typedef struct lfstack {
  volatile _Atomic (node*) head;
} lfstack_t;

int lfstack_init(lfstack_t *stack) {
  atomic_store(&stack->head, NULL);
  return 0; // success
}

int lfstack_push(lfstack_t *stack, uintptr_t value) {
  // TODO: push value into stack
  node* new_head = malloc(sizeof(node));
  new_head->value = value;
  new_head->next = atomic_load(&stack->head);
  
  // cas loop
  while (!atomic_compare_exchange_strong(&stack->head, &new_head->next, new_head)) {
      new_head->next = atomic_load(&stack->head);
  }
  return 0; // success
}

int lfstack_pop(lfstack_t *stack, uintptr_t *value) {
  // TODO: pop from stack and store result in *value
  return 0; // success
}

int lfstack_destroy(lfstack_t *stack) {
  // TODO: destroy empty stack
  return 0; // success
}
