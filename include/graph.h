/**
 * @file graph.h
 *
 * @author Chase Carter
 *
 * @brief Header file for functions to implement a graph and search with dijkstra and a* search
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <stdlib.h>

/**
 * @brief Opaque structure representing a graph data structure.
 */
typedef struct graph graph_t;

/**
 * @brief Structure to store pathfinding result data.
 */
typedef struct search_results
{
    size_t weight;     /**< Total cost/weight of the calculated path. */
    size_t node_count; /**< Total number of nodes in the calculated path. */
    char **pp_nodes;   /**< Array of string names of the nodes in traversal order. */
} search_results_t;

/**
 * @brief Function pointer type for calculating heuristic distance between two vertex payloads.
 *
 * @param[in] current_data Payload pointer of the current vertex.
 * @param[in] goal_data Payload pointer of the target destination vertex.
 *
 * @return Estimated remaining cost from the current vertex to the goal.
 */
typedef size_t (*heuristic_fn)(const void *current_data, const void *goal_data);

/**
 * @brief Function pointer type for freeing user-allocated vertex payload data.
 *
 * @param[in] p_data Pointer to the payload data to be freed.
 *
 * @return EXIT_SUCCESS on successful cleanup, or standard error code on failure.
 */
typedef void (*free_data_fn)(void *p_data);

/**
 * @brief Creates and initializes an empty graph.
 *
 * @param[in] free_data Function pointer to free custom vertex payloads, or NULL if unused.
 *
 * @return Pointer to the newly created graph, or NULL on allocation failure.
 */
graph_t *graph_create_graph(free_data_fn free_data);

/**
 * @brief Updates or sets the custom payload destructor function for the graph.
 *
 * @param[in,out] p_graph Graph instance to update.
 * @param[in] free_data New function pointer to free custom vertex payload data.
 */
void graph_update_free_data(graph_t *p_graph, free_data_fn free_data);

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
 * @param[in] p_name Unique identifier name of the new vertex.
 * @param[in] p_data Pointer to custom data associated with this vertex (optional).
 * @param[in] data_size Size in bytes of the custom data payload to copy.
 *
 * @return EXIT_SUCCESS on success, otherwise EXIT_FAILURE.
 */
int graph_add_vertex(graph_t *p_graph, const char *p_name, const void *p_data, size_t data_size);

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
 * @param[in] p_from Source vertex name.
 * @param[in] p_to Destination vertex name.
 * @param[in] weight Weight assigned to the edge.
 *
 * @return EXIT_SUCCESS on success, otherwise EXIT_FAILURE.
 */
int graph_add_edge(graph_t *p_graph, const char *p_from, const char *p_to, size_t weight);

/**
 * @brief Removes a directed edge matching the destination and weight.
 *
 * @param[in,out] p_graph Graph to modify.
 * @param[in] p_from Source vertex name.
 * @param[in] p_to Destination vertex name.
 * @param[in] weight Weight of the edge to remove.
 *
 * @return EXIT_SUCCESS on success, otherwise EXIT_FAILURE.
 */
int graph_remove_edge(graph_t *p_graph, const char *p_from, const char *p_to, size_t weight);

/**
 * @brief Retrieves all edge weights between two vertices.
 *
 * The returned array must be freed by the caller.
 *
 * @param[in] p_graph Graph to search.
 * @param[in] p_from Source vertex name.
 * @param[in] p_to Destination vertex name.
 * @param[out] p_count Number of weights returned.
 *
 * @return Dynamically allocated array of weights, or NULL if no matching edges
 *         exist or an error occurs.
 */
size_t *graph_get_edge_weight(graph_t *p_graph, const char *p_from, const char *p_to, int *p_count);

/**
 * @brief Changes the weight of a matching edge.
 *
 * @param[in,out] p_graph Graph to modify.
 * @param[in] p_from Source vertex name.
 * @param[in] p_to Destination vertex name.
 * @param[in] old_weight Existing edge weight.
 * @param[in] new_weight New edge weight.
 *
 * @return EXIT_SUCCESS on success, otherwise EXIT_FAILURE.
 */
int graph_set_edge_value(graph_t *p_graph, const char *p_from, const char *p_to, size_t old_weight, size_t new_weight);

/**
 * @brief Prints the graph as an adjacency list to standard output.
 *
 * @param[in] p_graph Graph to print.
 */
void graph_print_graph(graph_t *p_graph);

/**
 * @brief Performs Dijkstra's search algorithm to find the shortest path between two vertices.
 *
 * The returned results structure and its `pp_nodes` array must be freed by the caller.
 *
 * @param[in] p_graph Graph to search.
 * @param[in] p_start Name of the starting vertex.
 * @param[in] p_end Name of the target destination vertex.
 *
 * @return Dynamically allocated results structure containing total weight and path nodes,
 *         or NULL if no path exists or an error occurs.
 */
search_results_t *graph_dijkstras_search(graph_t *p_graph, const char *p_start, const char *p_end);

/**
 * @brief Calculates and prints Dijkstra's shortest path costs and routes from a starting
 *        vertex to all reachable vertices in the graph.
 *
 * @param[in] p_graph Graph to search.
 * @param[in] p_start Name of the origin vertex.
 */
void graph_dijkstras_print_all(graph_t *p_graph, const char *p_start);

/**
 * @brief Performs an A* (A-Star) search to find the shortest path using a custom heuristic callback.
 *
 * The returned results structure and its `pp_nodes` array must be freed by the caller.
 *
 * @param[in] p_graph Graph to search.
 * @param[in] p_start Name of the starting vertex.
 * @param[in] p_end Name of the target destination vertex.
 * @param[in] h_func Heuristic function estimating cost between node payloads.
 *
 * @return Dynamically allocated results structure containing total weight and path nodes,
 *         or NULL if no path exists or an error occurs.
 */
search_results_t *graph_a_star_search(graph_t *p_graph, const char *p_start, const char *p_end, heuristic_fn h_func);

/**
 * @brief Frees all memory associated with the search results.
 *
 * @param[in,out] p_graph Pointer to the search results pointer. Set to NULL after destruction.
 */
void graph_free_results(search_results_t **pp_results);

#endif // GRAPH_H

/* end of file graph.h */