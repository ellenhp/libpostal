#include "trie.h"
#include "trie_search.h"

void dump(trie_t* trie, u_int32_t original_node_idx, FILE* str_out, FILE* data_out) {
    u_int32_t node_idx = original_node_idx;
    int iter = 0;
    char buf[1024];
    size_t cursor = 511;


    for (int i = cursor; i < 1024; i++) {
        buf[i] = '\0';
    }

    {
        trie_node_t node = trie_get_node(trie, node_idx);
        trie_data_node_t data_node = trie_get_data_node(trie, node);
        if (data_node.tail != 0 && data_node.tail < trie->tail->n) {
            strcpy(&buf[cursor+1], trie->tail->a + data_node.tail);
        }
    }
    do {
        if (node_idx == 0) {
            break;
        }
        if (node_idx == 1) {
            break;
        }
        if (iter > 256) {
            break;
        }
        trie_node_t node = trie_get_node(trie, node_idx);
        // printf("node: %d %d\n", node.base, node.check);
        u_int32_t old_node = node_idx;
        node_idx = node.check < 0 ? -node.check - 1 : node.check;
        trie_node_t parent_node = trie_get_node(trie, node_idx);
        if (iter == 0) {
            if (node.base < 0) {
                // printf("!!");
            }
        }

        bool found = false;
        for (int i = 0; i < 256; i++) {
            if (trie_get_transition_index(trie, parent_node, i) == old_node) {
                buf[cursor--] = i;
                found = true;
            }
        }
        if (!found) {
            // printf("not found: %d\n", (int)node_idx);
            // exit(1);
        } else {
            // printf("found\n");
        }
        ++iter;
    } while (node_idx != ROOT_NODE_ID);
    --iter;
    if (trie_get(trie, &buf[cursor + 1]) != original_node_idx) {
        // printf("%s\n", &buf[cursor + 1]);
        fprintf(str_out, "\n");
        fprintf(data_out, "\n");
    } else {
        fprintf(str_out, "%s\n", &buf[cursor + 1]);
        trie_node_t node = trie_get_node(trie, original_node_idx);
        trie_data_node_t data_node = trie_get_data_node(trie, node);
        fprintf(data_out, "%d\n", data_node.data);
    }
}

int main(int argc, char **argv) {
    trie_t *trie = trie_load(argv[1]);

    // printf("num keys: %d\n", trie_num_keys(trie));
    // exit(0);

    FILE *str_out = fopen(argv[2], "w");
    FILE *data_out = fopen(argv[3], "w");

    for (int node_to_dump = 0; node_to_dump < trie->nodes->n; node_to_dump++) {
        trie_prefix_result_t result = trie_get_prefix_from_index(trie, "", 0, node_to_dump, 0);
        trie_node_t node = trie_get_node(trie, node_to_dump);
        dump(trie, node_to_dump, str_out, data_out);
    }

    return 0;
}