/**
 * @file graph.c
 *
 * @author Chase Carter
 *
 * @brief Functions to implement a graph and search with breadth first search
 */

// NOLINTNEXTLINE (bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp,-warnings-as-errors)
#define _POSIX_C_SOURCE 200809L

#include "graph.h"

#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_VERTICES_COUNT 10

typedef struct vertex vertex_t;
typedef struct edge edge_t;
typedef struct path path_t;

struct vertex
{
    size_t num_edges;
    size_t index;
    size_t capacity;
    char *p_name;
    void *p_data;
    edge_t **p_edges;
};

struct edge
{
    vertex_t *p_destination;
    size_t weight;
};

struct graph
{
    size_t num_vertices;
    size_t capacity;
    vertex_t **pp_vertices;
    free_data_fn free_data;
};

struct path
{
    size_t weight;
    int visited;
    vertex_t *p_current;
    path_t *previous;
};

/* STATIC FUNCTIONS */

/* Helpers for vertices management */
static vertex_t *static_create_vertex(const char *p_name);
static int static_resize_graph(graph_t *p_graph);
static void static_destroy_vertex(vertex_t **pp_vertex);
static vertex_t *static_find_vertex(graph_t *p_graph, const char *p_name, size_t *p_index);

/* Helpers for edge management */
static edge_t *static_create_edge(vertex_t *p_destination, size_t weight);
static int static_resize_vertex(vertex_t *p_vertex);
static void static_destroy_edges_containing(graph_t *p_graph, const char *p_name);

/* Helpers for dijkstra and a* search */
path_t *static_get_paths(graph_t *p_graph, size_t start_index);
search_results_t *static_get_dijkstras_results(path_t *p_answer);

/* PUBLIC FUNCTIONS */

graph_t *graph_create_graph(free_data_fn free_data)
{
    graph_t *p_graph = calloc(1, sizeof(*p_graph));

    if (NULL == p_graph)
    {
        return NULL;
    }

    p_graph->pp_vertices = calloc(INIT_VERTICES_COUNT, sizeof(*p_graph->pp_vertices));

    if (NULL == p_graph->pp_vertices)
    {
        free(p_graph);
        return NULL;
    }

    p_graph->capacity = INIT_VERTICES_COUNT;
    p_graph->free_data = free_data;

    return p_graph;
}

void graph_update_free_data(graph_t *p_graph, free_data_fn free_data)
{
    if (NULL == p_graph)
    {
        return;
    }

    p_graph->free_data = free_data;
}

void graph_destroy_graph(graph_t **p_graph)
{
    if ((NULL == p_graph) || (NULL == *p_graph))
    {
        return;
    }

    for (; (*p_graph)->num_vertices > 0;)
    {
        graph_remove_vertex(*p_graph, (*p_graph)->pp_vertices[0]->p_name);
    }

    free((*p_graph)->pp_vertices);
    free(*p_graph);
    *p_graph = NULL;
}

int graph_add_vertex(graph_t *p_graph, const char *p_name, const void *p_data, size_t data_size)
{
    if ((NULL == p_graph) || (NULL == p_name))
    {
        return EXIT_FAILURE;
    }

    if (NULL != static_find_vertex(p_graph, p_name, NULL))
    {
        return EXIT_FAILURE;
    }

    if (p_graph->num_vertices >= p_graph->capacity)
    {
        int check = static_resize_graph(p_graph);

        if (EXIT_FAILURE == check)
        {
            return EXIT_FAILURE;
        }
    }

    vertex_t *p_new_vertex = static_create_vertex(p_name);

    if (NULL == p_new_vertex)
    {
        return EXIT_FAILURE;
    }

    if ((NULL != p_data) && (data_size > 0))
    {
        p_new_vertex->p_data = calloc(1, data_size);

        if (NULL == p_new_vertex->p_data)
        {
            static_destroy_vertex(&p_new_vertex);
            return EXIT_FAILURE;
        }

        memcpy(p_new_vertex->p_data, p_data, data_size);
    }

    p_new_vertex->index = p_graph->num_vertices;
    p_graph->pp_vertices[p_graph->num_vertices] = p_new_vertex;
    p_graph->num_vertices += 1;

    return EXIT_SUCCESS;
}

int graph_remove_vertex(graph_t *p_graph, const char *p_name)
{
    if ((NULL == p_graph) || (NULL == p_name))
    {
        return EXIT_FAILURE;
    }
    size_t index = 0;

    vertex_t *p_vertex = static_find_vertex(p_graph, p_name, &index);

    if (NULL == p_vertex)
    {
        return EXIT_FAILURE;
    }

    static_destroy_edges_containing(p_graph, p_name);

    p_vertex = p_graph->pp_vertices[index];

    size_t last = p_graph->num_vertices - 1;

    if (index != last)
    {
        p_graph->pp_vertices[index] = p_graph->pp_vertices[last];
        p_graph->pp_vertices[index]->index = index;
    }

    p_graph->pp_vertices[last] = NULL;
    p_graph->num_vertices -= 1;

    if ((NULL != p_graph->free_data) && (NULL != p_vertex->p_data))
    {
        p_graph->free_data(p_vertex->p_data);
    }

    static_destroy_vertex(&p_vertex);

    for (size_t idx = 0; idx < p_graph->num_vertices; idx++)
    {
        p_graph->pp_vertices[idx]->index = idx;
    }

    return EXIT_SUCCESS;
}

int graph_rename_vertex(graph_t *p_graph, const char *p_current_name, const char *p_new_name)
{
    if ((NULL == p_graph) || (NULL == p_current_name) || (NULL == p_new_name))
    {
        return EXIT_FAILURE;
    }

    vertex_t *p_vertex = static_find_vertex(p_graph, p_current_name, NULL);

    if (NULL == p_vertex)
    {
        return EXIT_FAILURE;
    }

    if (NULL != static_find_vertex(p_graph, p_new_name, NULL))
    {
        return EXIT_FAILURE;
    }

    char *p_temp = strdup(p_new_name);

    if (NULL == p_temp)
    {
        return EXIT_FAILURE;
    }

    free(p_vertex->p_name);
    p_vertex->p_name = p_temp;

    return EXIT_SUCCESS;
}

const char **graph_get_neighbors(graph_t *p_graph, const char *p_name, size_t *p_count)
{
    if ((NULL == p_graph) || (NULL == p_name) || (NULL == p_count))
    {
        return NULL;
    }

    vertex_t *p_vertex = static_find_vertex(p_graph, p_name, NULL);

    if (NULL == p_vertex)
    {
        return NULL;
    }

    *p_count = p_vertex->num_edges;

    if (0 == *p_count)
    {
        return NULL;
    }

    const char **pp_neighbors = calloc(p_vertex->num_edges, sizeof(*pp_neighbors));

    if (NULL == pp_neighbors)
    {
        return NULL;
    }

    for (size_t index = 0; index < p_vertex->num_edges; index++)
    {
        pp_neighbors[index] = p_vertex->p_edges[index]->p_destination->p_name;
    }

    return pp_neighbors;
}

int graph_add_edge(graph_t *p_graph, const char *p_from, const char *p_to, size_t weight)
{
    if ((NULL == p_graph) || (NULL == p_from) || (NULL == p_to))
    {
        return EXIT_FAILURE;
    }

    vertex_t *p_location = static_find_vertex(p_graph, p_from, NULL);

    if (NULL == p_location)
    {
        return EXIT_FAILURE;
    }

    vertex_t *p_destination = static_find_vertex(p_graph, p_to, NULL);

    if (NULL == p_destination)
    {
        return EXIT_FAILURE;
    }

    if (p_location->num_edges >= p_location->capacity)
    {
        int check = static_resize_vertex(p_location);

        if (EXIT_FAILURE == check)
        {
            return EXIT_FAILURE;
        }
    }

    edge_t *p_new_edge = static_create_edge(p_destination, weight);

    if (NULL == p_new_edge)
    {
        return EXIT_FAILURE;
    }

    p_location->p_edges[p_location->num_edges] = p_new_edge;
    p_location->num_edges += 1;

    return EXIT_SUCCESS;
}

int graph_remove_edge(graph_t *p_graph, const char *p_from, const char *p_to, size_t weight)
{

    if ((NULL == p_graph) || (NULL == p_from) || (NULL == p_to))
    {
        return EXIT_FAILURE;
    }

    vertex_t *p_vertex = static_find_vertex(p_graph, p_from, NULL);

    if (NULL == p_vertex)
    {
        return EXIT_FAILURE;
    }

    for (size_t index = 0; index < p_vertex->num_edges;)
    {
        if ((0 == strcmp(p_to, p_vertex->p_edges[index]->p_destination->p_name)) &&
            (p_vertex->p_edges[index]->weight == weight))
        {
            free(p_vertex->p_edges[index]);
            p_vertex->num_edges -= 1;
            p_vertex->p_edges[index] = p_vertex->p_edges[p_vertex->num_edges];

            p_vertex->p_edges[p_vertex->num_edges] = NULL;

            return EXIT_SUCCESS;
        }
        else
        {
            index++;
        }
    }

    return EXIT_FAILURE;
}

size_t *graph_get_edge_weight(graph_t *p_graph, const char *p_from, const char *p_to, int *p_count)
{
    if ((NULL == p_graph) || (NULL == p_from) || (NULL == p_to) || (NULL == p_count))
    {
        return NULL;
    }

    vertex_t *p_vertex = static_find_vertex(p_graph, p_from, NULL);

    if (NULL == p_vertex)
    {
        return NULL;
    }

    edge_t **p_edges = p_vertex->p_edges;

    size_t *p_weights = calloc(p_vertex->num_edges, sizeof(*p_weights));

    if (NULL == p_weights)
    {
        return NULL;
    }

    *p_count = 0;

    for (size_t index = 0; index < p_vertex->num_edges; index++)
    {
        if (0 == strcmp(p_to, p_edges[index]->p_destination->p_name))
        {
            p_weights[*p_count] = p_edges[index]->weight;
            *p_count += 1;
        }
    }

    if (0 == *p_count)
    {
        free(p_weights);
        return NULL;
    }

    return p_weights;
}

int graph_set_edge_value(graph_t *p_graph, const char *p_from, const char *p_to, size_t old_weight, size_t new_weight)
{
    if ((NULL == p_graph) || (NULL == p_from) || (NULL == p_to))
    {
        return EXIT_FAILURE;
    }

    vertex_t *p_location = static_find_vertex(p_graph, p_from, NULL);

    if (NULL == p_location)
    {
        return EXIT_FAILURE;
    }

    vertex_t *p_destination = static_find_vertex(p_graph, p_to, NULL);

    if (NULL == p_destination)
    {
        return EXIT_FAILURE;
    }

    edge_t **p_edge = p_location->p_edges;

    for (size_t index = 0; index < p_location->num_edges; index++)
    {
        if ((0 == strcmp(p_to, p_edge[index]->p_destination->p_name)) && (old_weight == p_edge[index]->weight))
        {
            p_edge[index]->weight = new_weight;
            return EXIT_SUCCESS;
        }
    }

    return EXIT_FAILURE;
}

void graph_print_graph(graph_t *p_graph)
{
    if (NULL == p_graph)
    {
        return;
    }

    for (size_t index = 0; index < p_graph->num_vertices; index++)
    {
        vertex_t *p_vertex = p_graph->pp_vertices[index];

        printf("%s: ", p_vertex->p_name);

        for (size_t jindex = 0; jindex < p_vertex->num_edges; jindex++)
        {
            edge_t *p_edge = p_vertex->p_edges[jindex];

            printf("-> %s, %zu ", p_edge->p_destination->p_name, p_edge->weight);
        }
        printf("\n");
    }
}

search_results_t *graph_dijkstras_search(graph_t *p_graph, const char *p_start, const char *p_end)
{
    if ((NULL == p_graph) || (NULL == p_start) || (NULL == p_end))
    {
        return NULL;
    }

    size_t idx = 0;
    vertex_t *p_start_vertex = static_find_vertex(p_graph, p_start, &idx);

    if (NULL == p_start_vertex)
    {
        return NULL;
    }

    size_t end_index = 0;
    vertex_t *p_end_vertex = static_find_vertex(p_graph, p_end, &end_index);

    if (NULL == p_end_vertex)
    {
        return NULL;
    }

    path_t *p_paths = static_get_paths(p_graph, idx);

    if (NULL == p_paths)
    {
        return NULL;
    }

    if (ULONG_MAX == p_paths[end_index].weight)
    {
        free(p_paths);
        return NULL;
    }

    search_results_t *p_results = static_get_dijkstras_results(&p_paths[end_index]);

    if (NULL == p_results)
    {
        free(p_paths);
        return NULL;
    }

    free(p_paths);

    return p_results;
}

search_results_t *graph_a_star_search(graph_t *p_graph, const char *p_start, const char *p_end, heuristic_fn h_func)
{
    if ((NULL == p_graph) || (NULL == p_start) || (NULL == p_end) || (NULL == h_func))
    {
        return NULL;
    }

    size_t idx = 0;
    vertex_t *p_start_vertex = static_find_vertex(p_graph, p_start, &idx);
    if (NULL == p_start_vertex)
    {
        return NULL;
    }

    size_t end_index = 0;
    vertex_t *p_end_vertex = static_find_vertex(p_graph, p_end, &end_index);
    if (NULL == p_end_vertex)
    {
        return NULL;
    }

    path_t *p_paths = calloc(p_graph->num_vertices, sizeof(*p_paths));
    if (NULL == p_paths)
    {
        return NULL;
    }

    for (size_t index = 0; index < p_graph->num_vertices; index++)
    {
        p_paths[index].p_current = p_graph->pp_vertices[index];
        p_paths[index].weight = ULONG_MAX;
    }

    p_paths[idx].weight = 0;

    for (;;)
    {
        path_t *p_current = NULL;
        size_t best_priority = ULONG_MAX;

        // find unvisited node with lowest f(n) = g(n) + h(n)
        for (size_t index = 0; index < p_graph->num_vertices; index++)
        {
            if ((1 == p_paths[index].visited) || (ULONG_MAX == p_paths[index].weight))
            {
                continue;
            }

            size_t current_priority = 0;
            if ((NULL != p_paths[index].p_current->p_data) && (NULL != p_end_vertex->p_data))
            {
                current_priority =
                    p_paths[index].weight + h_func(p_paths[index].p_current->p_data, p_end_vertex->p_data);
            }

            if ((NULL == p_current) || (current_priority < best_priority))
            {
                p_current = &p_paths[index];
                best_priority = current_priority;
            }
        }

        // no nodes left to check
        if (NULL == p_current)
        {
            break;
        }

        p_current->visited = 1;

        // goal found
        if (0 == strcmp(p_current->p_current->p_name, p_end))
        {
            break;
        }

        // check outgoing edges
        for (size_t index = 0; index < p_current->p_current->num_edges; index++)
        {
            edge_t *p_edge = p_current->p_current->p_edges[index];

            assert(NULL != p_edge);
            assert(NULL != p_edge->p_destination);
            assert(p_edge->p_destination->index < p_graph->num_vertices);
            path_t *p_neighbor = &p_paths[p_edge->p_destination->index];

            if (1 == p_neighbor->visited)
            {
                continue;
            }

            size_t new_weight = p_current->weight + p_edge->weight;

            if (new_weight < p_neighbor->weight)
            {
                p_neighbor->weight = new_weight;
                p_neighbor->previous = p_current;
            }
        }
    }

    if (ULONG_MAX == p_paths[end_index].weight)
    {
        free(p_paths);
        return NULL;
    }

    search_results_t *p_results = static_get_dijkstras_results(&p_paths[end_index]);

    free(p_paths);

    return p_results;
}

void graph_dijkstras_print_all(graph_t *p_graph, const char *p_start)
{
    if ((NULL == p_graph) || (NULL == p_start))
    {
        return;
    }

    size_t idx = 0;
    vertex_t *p_temp = static_find_vertex(p_graph, p_start, &idx);
    if (NULL == p_temp)
    {

        return;
    }

    path_t *p_paths = static_get_paths(p_graph, idx);

    if (NULL == p_paths)
    {
        return;
    }

    for (size_t index = 0; index < p_graph->num_vertices; index++)
    {
        if (p_paths[index].weight == ULONG_MAX)
        {
            printf("%s: unreachable\n", p_paths[index].p_current->p_name);
            continue;
        }

        printf("path weight: %zu, %s", p_paths[index].weight, p_paths[index].p_current->p_name);

        path_t *p_parent = p_paths[index].previous;

        while (NULL != p_parent)
        {
            printf(" <- %s", p_parent->p_current->p_name);
            p_parent = p_parent->previous;
        }

        printf("\n");
    }

    free(p_paths);
}

void graph_free_results(search_results_t **pp_results)
{
    if ((NULL == pp_results) || (NULL == *pp_results))
    {
        return;
    }

    for (size_t index = 0; index < (*pp_results)->node_count; index++)
    {
        if (NULL == (*pp_results)->pp_nodes[index])
        {
            continue;
        }

        free((*pp_results)->pp_nodes[index]);
    }

    if (NULL != (*pp_results)->pp_nodes)
    {
        free((*pp_results)->pp_nodes);
    }

    free(*pp_results);
    *pp_results = NULL;
}

/* STATIC FUNCTIONS*/

/* Helpers for vertices management*/

/**
 * @brief Creates and initializes a vertex.
 *
 * @param[in] p_name Name assigned to the vertex.
 *
 * @return Pointer to the new vertex, or NULL on allocation failure.
 */
static vertex_t *static_create_vertex(const char *p_name)
{
    vertex_t *p_vertex = calloc(1, sizeof(*p_vertex));

    if (NULL == p_vertex)
    {
        return NULL;
    }

    p_vertex->p_edges = calloc(INIT_VERTICES_COUNT, sizeof(*p_vertex->p_edges));

    if (NULL == p_vertex->p_edges)
    {
        free(p_vertex);
        return NULL;
    }

    p_vertex->p_name = strdup(p_name);

    if (NULL == p_vertex->p_name)
    {
        free(p_vertex->p_edges);
        free(p_vertex);
        return NULL;
    }

    p_vertex->capacity = INIT_VERTICES_COUNT;

    return p_vertex;
}

/**
 * @brief Expands the graph's vertex storage capacity.
 *
 * @param[in,out] p_graph Graph to resize.
 *
 * @return EXIT_SUCCESS on success, otherwise EXIT_FAILURE.
 */
static int static_resize_graph(graph_t *p_graph)
{
    if (NULL == p_graph)
    {
        return EXIT_FAILURE;
    }

    size_t new_capacity = p_graph->capacity * 2;
    vertex_t **pp_temp = realloc(p_graph->pp_vertices, new_capacity * sizeof(*pp_temp));

    if (NULL == pp_temp)
    {
        return EXIT_FAILURE;
    }

    for (size_t index = p_graph->capacity; index < new_capacity; index++)
    {
        pp_temp[index] = NULL;
    }

    p_graph->pp_vertices = pp_temp;
    p_graph->capacity = new_capacity;

    return EXIT_SUCCESS;
}

/**
 * @brief Frees all memory associated with a vertex.
 *
 * @param[in,out] pp_vertex Pointer to the vertex pointer. Set to NULL on return.
 */
static void static_destroy_vertex(vertex_t **pp_vertex)
{
    if ((NULL == pp_vertex) || (NULL == *pp_vertex))
    {
        return;
    }

    vertex_t *p_vertex = *pp_vertex;

    for (size_t index = 0; index < p_vertex->num_edges; index++)
    {
        if (NULL != p_vertex->p_edges[index])
        {
            free(p_vertex->p_edges[index]);
        }
    }

    if (NULL != p_vertex->p_edges)
    {
        free(p_vertex->p_edges);
    }

    if (NULL != p_vertex->p_name)
    {
        free(p_vertex->p_name);
    }

    if (NULL != p_vertex)
    {
        free(p_vertex);
    }

    *pp_vertex = NULL;
}

/**
 * @brief Finds a vertex by name.
 *
 * Optionally returns the vertex's index in the graph.
 *
 * @param[in] p_graph Graph to search.
 * @param[in] p_name Vertex name.
 * @param[out] p_index Receives the vertex index, or NULL if not needed.
 *
 * @return Pointer to the matching vertex, or NULL if not found.
 */
static vertex_t *static_find_vertex(graph_t *p_graph, const char *p_name, size_t *p_index)
{
    if ((NULL == p_graph) || (NULL == p_name))
    {
        return NULL;
    }

    if (NULL != p_index)
    {
        *p_index = 0;
    }

    for (size_t position = 0; position < p_graph->num_vertices; position++)
    {
        if (0 == strcmp(p_name, p_graph->pp_vertices[position]->p_name))
        {
            if (NULL != p_index)
            {
                *p_index = position;
            }

            return p_graph->pp_vertices[position];
        }
    }

    return NULL;
}

/* Helpers for edge management */

/**
 * @brief Creates a directed edge.
 *
 * @param[in] p_destination Destination vertex.
 * @param[in] weight Edge weight.
 *
 * @return Pointer to the new edge, or NULL on allocation failure.
 */
static edge_t *static_create_edge(vertex_t *p_destination, size_t weight)
{
    if (NULL == p_destination)
    {
        return NULL;
    }

    edge_t *p_edge = calloc(1, sizeof(*p_edge));

    if (NULL == p_edge)
    {
        return NULL;
    }

    p_edge->p_destination = p_destination;
    p_edge->weight = weight;

    return p_edge;
}

/**
 * @brief Expands a vertex's edge storage capacity.
 *
 * @param[in,out] p_vertex Vertex to resize.
 *
 * @return EXIT_SUCCESS on success, otherwise EXIT_FAILURE.
 */
static int static_resize_vertex(vertex_t *p_vertex)
{
    if (NULL == p_vertex)
    {
        return EXIT_FAILURE;
    }

    size_t new_capacity = p_vertex->capacity * 2;
    edge_t **pp_temp = realloc(p_vertex->p_edges, new_capacity * sizeof(*pp_temp));

    if (NULL == pp_temp)
    {
        return EXIT_FAILURE;
    }

    for (size_t index = p_vertex->capacity; index < new_capacity; index++)
    {
        pp_temp[index] = NULL;
    }

    p_vertex->p_edges = pp_temp;
    p_vertex->capacity = new_capacity;

    return EXIT_SUCCESS;
}

/**
 * @brief Removes all edges that point to the specified vertex.
 *
 * @param[in,out] p_graph Graph to modify.
 * @param[in] p_name Name of the destination vertex.
 */
static void static_destroy_edges_containing(graph_t *p_graph, const char *p_name)
{
    if ((NULL == p_graph) || (NULL == p_name))
    {
        return;
    }

    for (size_t index = 0; index < p_graph->num_vertices; index++)
    {
        vertex_t *p_vertex = p_graph->pp_vertices[index];

        if (NULL == p_vertex)
        {
            continue;
        }

        for (size_t jindex = 0; jindex < p_vertex->num_edges;)
        {
            if ((NULL != p_vertex->p_edges[jindex]) &&
                (0 == strcmp(p_name, p_vertex->p_edges[jindex]->p_destination->p_name)))
            {
                free(p_vertex->p_edges[jindex]);
                p_vertex->num_edges -= 1;

                p_vertex->p_edges[jindex] = p_vertex->p_edges[p_vertex->num_edges];
                p_vertex->p_edges[p_vertex->num_edges] = NULL;
            }
            else
            {
                jindex++;
            }
        }
    }
}

/**
 * @brief Computes shortest paths from a start vertex index to all other reachable vertices.
 *
 * @param[in] p_graph Graph instance.
 * @param[in] start_index Index of the starting vertex.
 *
 * @return Dynamically allocated path array representing short paths, or NULL on failure.
 */
path_t *static_get_paths(graph_t *p_graph, size_t start_index)
{
    if (NULL == p_graph)
    {
        return NULL;
    }

    path_t *p_paths = calloc(p_graph->num_vertices, sizeof(*p_paths));

    if (NULL == p_paths)
    {
        return NULL;
    }

    for (size_t index = 0; index < p_graph->num_vertices; index++)
    {
        p_paths[index].p_current = p_graph->pp_vertices[index];
        p_paths[index].weight = ULONG_MAX;
    }

    p_paths[start_index].weight = 0;

    for (;;)
    {
        path_t *p_current = NULL;

        for (size_t index = 0; index < p_graph->num_vertices; index++)
        {
            if ((1 == p_paths[index].visited) || (ULONG_MAX == p_paths[index].weight))
            {
                continue;
            }

            if ((NULL == p_current) || (p_paths[index].weight < p_current->weight))
            {
                p_current = &p_paths[index];
            }
        }

        if (NULL == p_current)
        {
            break;
        }

        p_current->visited = 1;

        for (size_t index = 0; index < p_current->p_current->num_edges; index++)
        {
            edge_t *p_edge = p_current->p_current->p_edges[index];

            assert(NULL != p_edge);
            assert(NULL != p_edge->p_destination);
            assert(p_edge->p_destination->index < p_graph->num_vertices);

            path_t *p_neighbor = &p_paths[p_edge->p_destination->index];

            if (1 == p_neighbor->visited)
            {
                continue;
            }

            size_t new_weight = p_current->weight + p_edge->weight;

            if (new_weight < p_neighbor->weight)
            {
                p_neighbor->weight = new_weight;
                p_neighbor->previous = p_current;
            }
        }
    }

    return p_paths;
}

/**
 * @brief Constructs a Dijkstra result structure from a terminal path node node chain.
 *
 * @param[in] p_answer Pointer to the target destination path node.
 *
 * @return Dynamically allocated results structure, or NULL on failure.
 */
search_results_t *static_get_dijkstras_results(path_t *p_answer)
{
    search_results_t *p_results = calloc(1, sizeof(*p_results));

    if (NULL == p_results)
    {
        return NULL;
    }

    p_results->weight = p_answer->weight;

    size_t node_count = 0;
    path_t *p_current = p_answer;

    for (path_t *p_temp = p_current; NULL != p_temp; p_temp = p_temp->previous)
    {
        node_count++;
    }

    if (0 == node_count)
    {
        free(p_results);
        return NULL;
    }

    p_results->pp_nodes = calloc(node_count, sizeof(*p_results->pp_nodes));

    if (NULL == p_results->pp_nodes)
    {
        free(p_results);
        return NULL;
    }

    p_results->node_count = node_count;

    p_current = p_answer;

    for (size_t index = node_count; index > 0; index--)
    {
        p_results->pp_nodes[index - 1] = strdup(p_current->p_current->p_name);
        p_current = p_current->previous;
    }

    return p_results;
}

/* end of file graph.c*/
