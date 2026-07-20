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
    edge_t **p_edges;
};

struct edge
{
    vertex_t *p_destination;
    int weight;
};

struct graph
{
    size_t num_vertices;
    size_t capacity;
    vertex_t **pp_vertices;
};

struct path
{
    int weight;
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
static edge_t *static_create_edge(vertex_t *p_destination, int weight);
static int static_resize_vertex(vertex_t *p_vertex);
static void static_destroy_edges_containing(graph_t *p_graph, const char *p_name);

/* PUBLIC FUNCTIONS */

graph_t *graph_create_graph()
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

    return p_graph;
}

void graph_destroy_graph(graph_t **p_graph)
{
    if ((NULL == p_graph) || (NULL == *p_graph))
    {
        return;
    }

    for (size_t index = 0; index < (*p_graph)->num_vertices; index++)
    {
        vertex_t *p_temp_vertex = (*p_graph)->pp_vertices[index];

        static_destroy_vertex(&p_temp_vertex);
    }

    free((*p_graph)->pp_vertices);
    free(*p_graph);
    *p_graph = NULL;
}

int graph_add_vertex(graph_t *p_graph, const char *p_name)
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

    size_t last = p_graph->num_vertices - 1;

    if (index != last)
    {
        p_graph->pp_vertices[index] = p_graph->pp_vertices[last];
        p_graph->pp_vertices[index]->index = index;
    }

    p_graph->pp_vertices[last] = NULL;
    p_graph->num_vertices -= 1;

    static_destroy_vertex(&p_vertex);

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

    for (size_t index = 0; index < p_vertex->num_edges; index++)
    {
        pp_neighbors[index] = p_vertex->p_edges[index]->p_destination->p_name;
    }

    return pp_neighbors;
}

int graph_add_edge(graph_t *p_graph, const char *p_from, const char *p_to, int weight)
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

int graph_remove_edge(graph_t *p_graph, const char *p_from, const char *p_to, int weight)
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
            p_vertex->p_edges[index] = p_vertex->p_edges[p_vertex->num_edges - 1];

            p_vertex->p_edges[p_vertex->num_edges - 1] = NULL;

            p_vertex->num_edges -= 1;
            return EXIT_SUCCESS;
        }
        else
        {
            index++;
        }
    }

    return EXIT_FAILURE;
}

int *graph_get_edge_weight(graph_t *p_graph, const char *p_from, const char *p_to, int *p_count)
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

    int *p_weights = calloc(p_vertex->num_edges, sizeof(*p_weights));

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

int graph_set_edge_value(graph_t *p_graph, const char *p_from, const char *p_to, int old_weight, int new_weight)
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

            printf("-> %s, %i ", p_edge->p_destination->p_name, p_edge->weight);
        }
        printf("\n");
    }
}

void graph_dijkstras_search(graph_t *p_graph, const char *p_start)
{
    if ((NULL == p_graph) || (NULL == p_start))
    {
        return;
    }

    path_t *p_paths = calloc(p_graph->num_vertices, sizeof(*p_paths));

    if (NULL == p_paths)
    {
        return;
    }

    for (size_t index = 0; index < p_graph->num_vertices; index++)
    {
        p_paths[index].p_current = p_graph->pp_vertices[index];
        p_paths[index].weight = INT_MAX;
    }

    size_t idx = 0;
    vertex_t *p_temp = static_find_vertex(p_graph, p_start, &idx);
    if (NULL == p_temp)
    {
        free(p_paths);
        return;
    }

    p_paths[idx].weight = 0;

    for (;;)
    {
        path_t *p_current = NULL;
        for (size_t index = 0; index < p_graph->num_vertices; index++)
        {
            if ((1 == p_paths[index].visited) || (INT_MAX == p_paths[index].weight))
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

            path_t *p_neighbor = &p_paths[p_edge->p_destination->index];

            if (1 == p_neighbor->visited)
            {
                continue;
            }

            int new_weight = p_current->weight + p_edge->weight;

            if (new_weight < p_neighbor->weight)
            {
                p_neighbor->weight = new_weight;
                p_neighbor->previous = p_current;
            }
        }
    }

    for (size_t index = 0; index < p_graph->num_vertices; index++)
    {
        if (p_paths[index].weight == INT_MAX)
        {
            printf("%s: unreachable\n", p_paths[index].p_current->p_name);
            continue;
        }

        printf("path weight: %i, %s", p_paths[index].weight, p_paths[index].p_current->p_name);

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
static edge_t *static_create_edge(vertex_t *p_destination, int weight)
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
            if (0 == strcmp(p_name, p_vertex->p_edges[jindex]->p_destination->p_name))
            {
                free(p_vertex->p_edges[jindex]);

                p_vertex->p_edges[jindex] = p_vertex->p_edges[p_vertex->num_edges - 1];
                p_vertex->num_edges -= 1;
            }
            else
            {
                jindex++;
            }
        }
    }
}

/* end of file graph.c*/
