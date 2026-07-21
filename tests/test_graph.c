#include "graph.h"
#include "unity.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct coord
{
    int x;
    int y;
} coord_t;

size_t heuristic_test(const void *current_data, const void *goal_data)
{
    if ((NULL == current_data) || (NULL == goal_data))
    {
        return 0;
    }
    const coord_t a = *(const coord_t *)current_data;
    const coord_t b = *(const coord_t *)goal_data;

    int dx = abs(a.x - b.x);
    int dy = abs(a.y - b.y);

    return (size_t)(dx + dy);
}

void test_graph_search(void)
{
    graph_t *p_graph = graph_create_graph(free);

    graph_add_vertex(p_graph, "A", &(coord_t){.x = 0, .y = 4}, sizeof(coord_t));
    graph_add_vertex(p_graph, "B", &(coord_t){.x = 4, .y = 8}, sizeof(coord_t));
    graph_add_vertex(p_graph, "D", &(coord_t){.x = 8, .y = 8}, sizeof(coord_t));
    graph_add_vertex(p_graph, "G", &(coord_t){.x = 12, .y = 8}, sizeof(coord_t));
    graph_add_vertex(p_graph, "I", &(coord_t){.x = 16, .y = 4}, sizeof(coord_t));
    graph_add_vertex(p_graph, "H", &(coord_t){.x = 12, .y = 0}, sizeof(coord_t));
    graph_add_vertex(p_graph, "F", &(coord_t){.x = 8, .y = 0}, sizeof(coord_t));
    graph_add_vertex(p_graph, "C", &(coord_t){.x = 4, .y = 0}, sizeof(coord_t));
    graph_add_vertex(p_graph, "E", &(coord_t){.x = 8, .y = 4}, sizeof(coord_t));

    graph_add_edge(p_graph, "A", "B", 4);
    graph_add_edge(p_graph, "B", "A", 4);

    graph_add_edge(p_graph, "A", "C", 6);
    graph_add_edge(p_graph, "C", "A", 6);

    graph_add_edge(p_graph, "B", "C", 11);
    graph_add_edge(p_graph, "C", "B", 11);

    graph_add_edge(p_graph, "B", "D", 10);
    graph_add_edge(p_graph, "D", "B", 10);

    graph_add_edge(p_graph, "C", "E", 7);
    graph_add_edge(p_graph, "E", "C", 7);

    graph_add_edge(p_graph, "C", "F", 1);
    graph_add_edge(p_graph, "F", "C", 1);

    graph_add_edge(p_graph, "D", "E", 2);
    graph_add_edge(p_graph, "E", "D", 2);

    graph_add_edge(p_graph, "F", "E", 6);
    graph_add_edge(p_graph, "E", "F", 6);

    graph_add_edge(p_graph, "D", "G", 7);
    graph_add_edge(p_graph, "G", "D", 7);

    graph_add_edge(p_graph, "H", "D", 4);
    graph_add_edge(p_graph, "D", "H", 4);

    graph_add_edge(p_graph, "H", "F", 2);
    graph_add_edge(p_graph, "F", "H", 2);

    graph_add_edge(p_graph, "H", "G", 14);
    graph_add_edge(p_graph, "G", "H", 14);

    graph_add_edge(p_graph, "G", "I", 9);
    graph_add_edge(p_graph, "I", "G", 9);

    graph_add_edge(p_graph, "H", "I", 100);
    graph_add_edge(p_graph, "I", "H", 100);

    printf("This is the graph used in the test:\n");
    printf("    B-10-D - 7 - G\n"
           "   /|    |\\      |\\\n"
           "  4 |    2 \\     | 9\n"
           " /  |    |  \\    |  \\\n"
           "A   11   E   4   14   I\n"
           " \\  |   /|    \\  |  /\n"
           "  6 | 7  6     \\ | 100\n"
           "   \\|/   |      \\|/\n"
           "    C- 1-F - 2 - H\n");

    printf("\n");

    graph_add_vertex(p_graph, "x", NULL, 0);
    graph_add_vertex(p_graph, "y", NULL, 0);
    graph_add_vertex(p_graph, "z", NULL, 0);

    // Should not run
    graph_add_edge(p_graph, "x", "k", 9);
    graph_add_edge(p_graph, "y", "l", 9);
    graph_add_edge(p_graph, "m", "z", 9);

    graph_remove_edge(p_graph, "A", "D", 1);
    graph_remove_edge(p_graph, "A", "B", 1);
    graph_remove_edge(p_graph, "A", "C", 4);

    // Will be deleted
    graph_add_edge(p_graph, "x", "z", 4);
    graph_add_edge(p_graph, "y", "z", 2);
    graph_add_edge(p_graph, "x", "y", 6);

    graph_add_edge(p_graph, "A", "x", 3);
    graph_add_edge(p_graph, "B", "y", 7);
    graph_add_edge(p_graph, "C", "z", 5);

    // junk
    graph_add_vertex(p_graph, "j", &(coord_t){.x = 4, .y = -4}, sizeof(coord_t));
    graph_add_vertex(p_graph, "k", &(coord_t){.x = 8, .y = -4}, sizeof(coord_t));
    graph_add_vertex(p_graph, "l", &(coord_t){.x = 12, .y = -4}, sizeof(coord_t));
    graph_add_vertex(p_graph, "m", &(coord_t){.x = 6, .y = -8}, sizeof(coord_t));
    graph_add_vertex(p_graph, "n", &(coord_t){.x = 10, .y = -8}, sizeof(coord_t));
    graph_add_vertex(p_graph, "o", &(coord_t){.x = 8, .y = -12}, sizeof(coord_t));

    graph_add_edge(p_graph, "C", "j", 5);
    graph_add_edge(p_graph, "j", "C", 5);

    graph_add_edge(p_graph, "F", "k", 7);
    graph_add_edge(p_graph, "k", "F", 7);

    graph_add_edge(p_graph, "H", "l", 4);
    graph_add_edge(p_graph, "l", "H", 4);

    graph_add_edge(p_graph, "j", "m", 2);
    graph_add_edge(p_graph, "m", "j", 2);

    graph_add_edge(p_graph, "k", "m", 14);
    graph_add_edge(p_graph, "m", "k", 14);

    graph_add_edge(p_graph, "k", "n", 9);
    graph_add_edge(p_graph, "n", "k", 9);

    graph_add_edge(p_graph, "l", "n", 1);
    graph_add_edge(p_graph, "n", "l", 1);

    graph_add_edge(p_graph, "o", "n", 3);
    graph_add_edge(p_graph, "n", "o", 3);

    graph_add_edge(p_graph, "o", "m", 3);
    graph_add_edge(p_graph, "m", "o", 3);

    /* Work */

    size_t count = 0;
    const char **p_neighbors = graph_get_neighbors(p_graph, "A", &count);

    if (3 != count)
    {
        printf("count: %zu\n", count);
        TEST_FAIL();
    }

    free(p_neighbors);

    // Delete them
    graph_remove_vertex(p_graph, "x");
    graph_remove_vertex(p_graph, "y");
    graph_remove_vertex(p_graph, "z");

    p_neighbors = graph_get_neighbors(p_graph, "A", &count);

    if (2 != count)
    {
        printf("count: %zu\n", count);
        TEST_FAIL();
    }

    free(p_neighbors);

    printf("\nTesting dijkstras search:\n");
    clock_t begin = clock();
    search_results_t *p_results = graph_dijkstras_search(p_graph, "A", "I");
    clock_t end = clock();
    double calc_time = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("Time to calculate: %f ms\n", calc_time * 1000.0);

    if (NULL != p_results)
    {
        printf("weight: %zu, node_count: %zu\n", p_results->weight, p_results->node_count);
        printf("path: ");
        for (size_t index = 0; index < p_results->node_count; index++)
        {
            printf("%s -> ", p_results->pp_nodes[index]);
        }
    }
    printf("\n");

    graph_free_results(&p_results);

    printf("\nTesting a* search:\n");
    begin = clock();
    p_results = graph_a_star_search(p_graph, "A", "I", heuristic_test);
    end = clock();
    calc_time = (double)(end - begin) / CLOCKS_PER_SEC;

    printf("Time to calculate: %f ms\n", calc_time * 1000.0);

    if (NULL != p_results)
    {
        printf("weight: %zu, node_count: %zu\n", p_results->weight, p_results->node_count);
        printf("path: ");
        for (size_t index = 0; index < p_results->node_count; index++)
        {
            printf("%s -> ", p_results->pp_nodes[index]);
        }
    }
    else
    {
        printf("a* failed\n");
    }

    graph_free_results(&p_results);

    printf("\n\n");

    // Delete them
    graph_remove_vertex(p_graph, "j");
    graph_remove_vertex(p_graph, "k");
    graph_remove_vertex(p_graph, "l");
    graph_remove_vertex(p_graph, "m");
    graph_remove_vertex(p_graph, "n");
    graph_remove_vertex(p_graph, "o");

    printf("\nPrint all paths from node a\n");
    graph_dijkstras_print_all(p_graph, "A");

    graph_destroy_graph(&p_graph);
}
