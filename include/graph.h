
#ifndef GRAPH_H
#define GRAPH_H

#include <stdlib.h>

typedef struct graph graph_t;

typedef struct djikstra_results
{
    int weight;
    size_t node_count;
    char **pp_nodes;
} djikstra_results_t;

/**
 * @brief Creates and initializes an empty graph.
 *
 * @return Pointer to the newly created graph, or NULL on allocation failure.
 */
graph_t *graph_create_graph();

/**
 * @brief Frees all memory associated with a graph.
 *
 * @param[in,out] p_graph Pointer to the graph pointer. Set to NULL after destruction.
 */
void graph_destroy_graph(graph_t **p_graph);

/**
 * @brief Adds a new vertex to the graph.
 *
 * @param[in,out] p_graph Graph to modify.
 * @param[in] p_name Name of the new vertex.
 *
 * @return EXIT_SUCCESS on success, otherwise EXIT_FAILURE.
 */
int graph_add_vertex(graph_t *p_graph, const char *p_name);

/**
 * @brief Removes a vertex and all edges referencing it.
 *
 * @param[in,out] p_graph Graph to modify.
 * @param[in] p_name Name of the vertex to remove.
 *
 * @return EXIT_SUCCESS on success, otherwise EXIT_FAILURE.
 */
int graph_remove_vertex(graph_t *p_graph, const char *p_name);

/**
 * @brief Renames an existing vertex.
 *
 * @param[in,out] p_graph Graph to modify.
 * @param[in] p_current_name Current vertex name.
 * @param[in] p_new_name New vertex name.
 *
 * @return EXIT_SUCCESS on success, otherwise EXIT_FAILURE.
 */
int graph_rename_vertex(graph_t *p_graph, const char *p_current_name, const char *p_new_name);

/**
 * @brief Retrieves the names of all neighboring vertices.
 *
 * The returned array must be freed by the caller. The strings within the array
 * are owned by the graph and must not be freed.
 *
 * @param[in] p_graph Graph to search.
 * @param[in] p_name Name of the source vertex.
 * @param[out] p_count Number of neighbors returned.
 *
 * @return Dynamically allocated array of neighbor names, or NULL if none exist
 *         or an error occurs.
 */
const char **graph_get_neighbors(graph_t *p_graph, const char *p_name, size_t *p_count);

/**
 * @brief Adds a directed weighted edge between two vertices.
 *
 * @param[in,out] p_graph Graph to modify.
 * @param[in] p_from Source vertex.
 * @param[in] p_to Destination vertex.
 * @param[in] weight Weight assigned to the edge.
 *
 * @return EXIT_SUCCESS on success, otherwise EXIT_FAILURE.
 */
int graph_add_edge(graph_t *p_graph, const char *p_from, const char *p_to, int weight);

/**
 * @brief Removes a directed edge matching the destination and weight.
 *
 * @param[in,out] p_graph Graph to modify.
 * @param[in] p_from Source vertex.
 * @param[in] p_to Destination vertex.
 * @param[in] weight Weight of the edge to remove.
 *
 * @return EXIT_SUCCESS on success, otherwise EXIT_FAILURE.
 */
int graph_remove_edge(graph_t *p_graph, const char *p_from, const char *p_to, int weight);

/**
 * @brief Retrieves all edge weights between two vertices.
 *
 * The returned array must be freed by the caller.
 *
 * @param[in] p_graph Graph to search.
 * @param[in] p_from Source vertex.
 * @param[in] p_to Destination vertex.
 * @param[out] p_count Number of weights returned.
 *
 * @return Dynamically allocated array of weights, or NULL if no matching edges
 *         exist or an error occurs.
 */
int *graph_get_edge_weight(graph_t *p_graph, const char *p_from, const char *p_to, int *p_count);

/**
 * @brief Changes the weight of a matching edge.
 *
 * @param[in,out] p_graph Graph to modify.
 * @param[in] p_from Source vertex.
 * @param[in] p_to Destination vertex.
 * @param[in] old_weight Existing edge weight.
 * @param[in] new_weight New edge weight.
 *
 * @return EXIT_SUCCESS on success, otherwise EXIT_FAILURE.
 */
int graph_set_edge_value(graph_t *p_graph, const char *p_from, const char *p_to, int old_weight, int new_weight);

/**
 * @brief Prints the graph as an adjacency list.
 *
 * @param[in] p_graph Graph to print.
 */
void graph_print_graph(graph_t *p_graph);

djikstra_results_t *graph_djikstras_search(graph_t *p_graph, const char *p_start, const char *p_end);

void graph_djikstras_print_all(graph_t *p_graph, const char *p_start);

#endif // GRAPH_H

/* end of file graph.h*/
