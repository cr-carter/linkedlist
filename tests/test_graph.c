#include "graph.h"
#include "unity.h"

#include <stdio.h>
#include <stdlib.h>

void test_graph_search(void)
{
    graph_t *p_graph = graph_create_graph();

    graph_add_vertex(p_graph, "a");
    graph_add_vertex(p_graph, "b");
    graph_add_vertex(p_graph, "d");
    graph_add_vertex(p_graph, "g");
    graph_add_vertex(p_graph, "i");
    graph_add_vertex(p_graph, "h");
    graph_add_vertex(p_graph, "f");
    graph_add_vertex(p_graph, "c");
    graph_add_vertex(p_graph, "e");
    graph_add_vertex(p_graph, "j");

    graph_add_edge(p_graph, "a", "b", 4);
    graph_add_edge(p_graph, "b", "a", 4);

    graph_add_edge(p_graph, "a", "c", 6);
    graph_add_edge(p_graph, "c", "a", 6);

    graph_add_edge(p_graph, "b", "c", 11);
    graph_add_edge(p_graph, "c", "b", 11);

    graph_add_edge(p_graph, "b", "d", 10);
    graph_add_edge(p_graph, "d", "b", 10);

    graph_add_edge(p_graph, "c", "e", 7);
    graph_add_edge(p_graph, "e", "c", 7);

    graph_add_edge(p_graph, "c", "f", 1);
    graph_add_edge(p_graph, "f", "c", 1);

    graph_add_edge(p_graph, "d", "e", 2);
    graph_add_edge(p_graph, "e", "d", 2);

    graph_add_edge(p_graph, "f", "e", 6);
    graph_add_edge(p_graph, "e", "f", 6);

    graph_add_edge(p_graph, "d", "g", 7);
    graph_add_edge(p_graph, "g", "d", 7);

    graph_add_edge(p_graph, "h", "d", 4);
    graph_add_edge(p_graph, "d", "h", 4);

    graph_add_edge(p_graph, "h", "f", 2);
    graph_add_edge(p_graph, "f", "h", 2);

    graph_add_edge(p_graph, "h", "g", 14);
    graph_add_edge(p_graph, "g", "h", 14);

    graph_add_edge(p_graph, "g", "i", 9);
    graph_add_edge(p_graph, "i", "g", 9);

    graph_add_edge(p_graph, "h", "i", 100);
    graph_add_edge(p_graph, "i", "h", 100);

    printf("    b-10-d - 7 - g\n"
           "   /|    |\\      |\\\n"
           "  4 |    2 \\     | 9\n"
           " /  |    |  \\    |  \\\n"
           "a   11   e   4   14   i\n"
           " \\  |   /|    \\  |  /\n"
           "  6 | 7  6     \\ | 100\n"
           "   \\|/   |      \\|/\n"
           "    c- 1-f - 2 - h\n");

    printf("\n");

    // Should not run
    graph_add_edge(p_graph, "j", "k", 9);
    graph_add_edge(p_graph, "m", "l", 9);

    // graph_remove_vertex(p_graph, "j");

    djikstra_results_t *p_results = graph_djikstras_search(p_graph, "a", "j");

    if (NULL != p_results)
    {

        printf("weight: %i, node_count: %zu\n", p_results->weight, p_results->node_count);
        printf("path: ");

        for (size_t index = 0; index < p_results->node_count; index++)
        {
            printf("%s -> ", p_results->pp_nodes[index]);
        }
        printf("\n\n\n");

        for (size_t index = 0; index < p_results->node_count; index++)
        {
            free((void *)p_results->pp_nodes[index]);
        }

        free(p_results->pp_nodes);
        free(p_results);
    }

    // This works
    // graph_djikstras_print_all(p_graph, "a");

    graph_destroy_graph(&p_graph);
}
