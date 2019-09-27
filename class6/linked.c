#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    struct node *prev;
    struct node *next;
    int value;
} node_t;

typedef struct list {
    node_t *start;
    node_t *end;
} list_t;

list_t *list_create(void) {
    list_t *l = malloc(sizeof(list_t));
    l->start = NULL;
    l->end = NULL;
    return l;
}

void list_push_start(list_t *list, int value) {
    node_t *tmp;
    tmp = (node_t *)malloc(sizeof(node_t));
    tmp->value = value;
    tmp->next = list->start;
    tmp->prev = NULL;
    list->start = tmp;
    if (list->end == NULL) {
        list->end = tmp;
    }
}

void list_push_end(list_t *list, int value) {
    node_t *tmp;
    tmp = (node_t *)malloc(sizeof(node_t));
    tmp->value = value;
    tmp->next = NULL;
    tmp->prev = list->end;
    list->end = tmp;
    if (list->start == NULL) {
        list->start = tmp;
    }
}

int list_pop_start(list_t *list) {
    int tmp = list->start->value;
    node_t *newStart = list->start->next;
    list->start->next->prev = NULL;
    free(list->start);
    list->start = newStart;
    return tmp;
}

int list_pop_end(list_t *list) {
    //printf("starting list_pop_end\n");
    int tmp = list->end->value;
    printf("about to set null\n");
    //printf("list->end->prev->next = %ls\n", (int *)list->end->prev->next);
    //list->end->prev->next = NULL;
    node_t *newEnd = list->end->prev;
    free(list->end);
    list->end = newEnd;
    return tmp;
}

void list_destroy(list_t *list) {
    int tmp = 0;
    while (list->end != NULL) {
        printf("About to pop_end\n");
        tmp = list_pop_end(list);
        printf("popped end\n");
    }
    free(list);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <file>\n", argv[0]);
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    if (!f) {
        fprintf(stderr, "could not open %s: ", argv[1]);
        perror("");
        return 1;
    }

    list_t *list = list_create();

    while (!feof(f)) {
        char command[16];
        int res = fscanf(f, "%10s", command);
        if (res != 1) {
            if (!feof(f)) {
                fprintf(stderr, "trouble parsing command. stopping.\n");
            }
            break;
        }

        if (strcmp("push_start", command) == 0) {
            int value = 0;
            res = fscanf(f, "%d", &value);
            if (res != 1) {
                fprintf(stderr, "trouble parsing value. stopping.\n");
                break;
            }

            list_push_start(list, value);
        } else if (strcmp("push_end", command) == 0) {
            int value = 0;
            res = fscanf(f, "%d", &value);
            if (res != 1) {
                fprintf(stderr, "trouble parsing value. stopping.\n");
                break;
            }

            list_push_end(list, value);
        } else if (strcmp("pop_start", command) == 0) {
            int v = list_pop_start(list);
            printf("%d\n", v);
        } else if (strcmp("pop_end", command) == 0) {
            int v = list_pop_end(list);
            printf("%d\n", v);
        } else {
            fprintf(stderr, "did not recognize command '%s'. stopping.\n", command);
            break;
        }
    }

    list_destroy(list);

    fclose(f);

    return 0;
}
