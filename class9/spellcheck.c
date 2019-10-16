#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define WORD_MAX_LEN 256

// ternary search tree node
typedef struct tst_node {
    char c;
    // just in this structure definition we have to call the type
    // "struct tst_node" because the tst_node_t definition doesn't exist yet
    struct tst_node *low;
    struct tst_node *equal;
    struct tst_node *high;
} tst_node_t;

// ternary search tree
typedef struct tst {
    tst_node_t *node;
} tst_t;

tst_node_t *tst_node_create(char c) {
    tst_node_t *node = calloc(1, sizeof(*node)); // calloc also zeros out the memory
    node->c = c;
    return node;
}

void tst_node_destroy(tst_node_t *node) {
    if (node->low) {
        tst_node_destroy(node->low);
    }
    if (node->equal) {
        tst_node_destroy(node->equal);
    }
    if (node->high) {
        tst_node_destroy(node->high);
    }
    if (node) {
        free(node);
    }
}

tst_t *tst_create(void) {
    return calloc(1, sizeof(tst_t)); // calloc also zeros out the memory
}

void tst_destroy(tst_t *tst) {
    //recursively free nodes in tst
    //base case: if !tst return;
    if (!tst) {
        return;
    }
    tst_node_destroy(tst->node);
    free(tst);
}

void tst_add(tst_t *tst, const char *word) {
    if (!tst->node) {
        tst->node = tst_node_create(word[0]);
    }
    tst_node_t *node = tst->node;

    while (1) {
        // printf("word[%d]: %c\n", i, word[0]);
        if (word[0] < node->c) {
            if (!node->low) {
                node->low = tst_node_create(word[0]);
            }
            node = node->low;
        } else if (word[0] > node->c) {
            if (!node->high) {
                node->high = tst_node_create(word[0]);
            }
            node = node->high;
        } else {
            word++;
            if (!node->equal) {
                node->equal = tst_node_create(word[0]);
            }
            node = node->equal;
            if (!word[0]) {
                break;
            }
        }
    }
}

void tst_node_search(tst_node_t *node, char *word, char *suggestion, char *sugg_start, int errs) {
    while (node) {
        if (errs > 0) {
            //insertion case
            tst_node_search(node, word + 1, suggestion, sugg_start, errs -1);
            //deletion case
            suggestion[0] = node->c;
            tst_node_search(node->equal, word, suggestion + 1, sugg_start, errs -1);
            //replacement case
            tst_node_search(node->equal, word + 1, suggestion + 1, sugg_start, errs -1);
            //transpose case
            char tmp = word[0];
            word[0] = word[1];
            word[1] = tmp;
            tst_node_search(node, word, suggestion, sugg_start, errs -1);
        }
        if (word[0] < node->c) {
            tst_node_search(node->high, word, suggestion, sugg_start, errs);
            node = node->low;
        } else if (word[0] > node->c) {
            tst_node_search(node->low, word, suggestion, sugg_start, errs);
            node = node->high;
        } else {
            tst_node_search(node->low, word, suggestion, sugg_start, errs);
            tst_node_search(node->high, word, suggestion, sugg_start, errs);
            node = node->equal;
            suggestion[0] = word[0];
            if (!word[0]) {
                printf("%s\n", sugg_start);
            }
            word++;
            suggestion++;

        }
    }
}

void tst_search(tst_t *tst, char *word) {
    if (!tst->node) {
        return;
    }
    char word_suggestion[WORD_MAX_LEN];
    tst_node_search(tst->node, word, &word_suggestion[0], word_suggestion, 1);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <word>+\n", argv[0]);
        return 1;
    }

    FILE *f = fopen("words.txt", "r");
    if (!f) {
        perror("Failed to open words.txt");
        return 1;
    }

    tst_t *tst = tst_create();

    // char str_buffer[WORD_MAX_LEN];
    // while (1) {
    //     int ret = fscanf(f, "%255s", str_buffer);
    //     if (ret != 1) {
    //         break;
    //     }
    //     tst_add(tst, str_buffer);
    // }
    // fclose(f);

    //tst_add(tst, "to");
    tst_add(tst, "the");
    tst_add(tst, "tea");
    tst_add(tst, "that");
    tst_add(tst, "thee");
    tst_add(tst, "hospital");

    for (int i = 1; i < argc; i++) {
        if (strlen(argv[i]) >= WORD_MAX_LEN) {
            fprintf(stderr, "word '%s' is too long!\n", argv[i]);
            continue;
        }

        tst_search(tst, argv[i]);
        printf("\n");
    }

    tst_destroy(tst);
    return 0;
}
