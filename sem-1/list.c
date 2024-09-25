#include <stdio.h>
#include <stdlib.h>

struct node {
    int value;
    struct node* next;
};

typedef struct node node_t;

node_t* new_node(int value, node_t* next) {
    node_t* result = calloc(1, sizeof(node_t));
    result->value = value;
    result->next = next;
    return result;
}

void print_list(node_t* head) {
    //if (head == NULL) {
    //    printf("[]\n");
    //    return;
    //}

    printf("[");
    printf("%d", head->value);
        
    while (head/*->next*/) {
        head = head->next;
        printf(", %d", head->value);
    }
    printf("]\n");
}

void free_list(node_t* head) {
    if (head == NULL) {
        return;
    }
    free_list(head->next);
    free(head);
}

int main() {
    print_list(NULL);
    
    node_t* list = new_node(1, new_node(2, new_node(3, NULL)));
    print_list(list);
    //free_list(list);
    
    node_t* list2 = new_node(10, NULL);
    print_list(list2);
    //free(list2);
    
    return 0;
}
