/**
 * @file graph.c
 *
 * @author Chase Carter
 *
 * @brief Functions to implement a graph and search with breadth first search
 */

/**
 * TO-DO
 * this currently allows multiple edges between same destinations. should this be changed?
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// NOLINTNEXTLINE (bugprone-reserved-identifier,cert-dcl37-c,cert-dcl51-cpp,-warnings-as-errors)
#define _POSIX_C_SOURCE 200809L

#define INIT_VERTICES_COUNT 10

typedef struct graph graph_t;
typedef struct vertex vertex_t;
typedef struct edge edge_t;

struct vertex
{
    char *p_name;
    int num_edges;
    int capacity;
    edge_t **p_edges;
};

struct edge
{
    vertex_t *p_destination;
    int weight;
};

struct graph
{
    int num_vertices;
    int capacity;
    vertex_t **pp_vertices;
};

/* STATIC FUNCTIONS */

/* Helpers for vertices management */
static vertex_t *static_create_vertex(const char *p_name);
static int static_resize_graph(graph_t *p_graph);
static void static_destroy_vertex(vertex_t **pp_vertex);
static vertex_t *static_find_vertex(graph_t *p_graph, const char *p_name, int *p_index);

/* Helpers for edge management */
static edge_t *static_create_edge(vertex_t *p_destination, int weight);
static int static_resize_vertex(vertex_t *p_vertex);
static void static_destroy_edges_containing(graph_t *p_graph, const char *p_name);

/* PUBLIC FUNCTIONS */

graph_t *create_graph()
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

void destroy_graph(graph_t **p_graph)
{
    if ((NULL == p_graph) || (NULL == *p_graph))
    {
        return;
    }

    for (int index = 0; index < (*p_graph)->num_vertices; index++)
    {
        vertex_t *p_temp_vertex = (*p_graph)->pp_vertices[index];

        static_destroy_vertex(&p_temp_vertex);
    }

    free((*p_graph)->pp_vertices);
    free(*p_graph);
    *p_graph = NULL;
}

int add_vertex(graph_t *p_graph, const char *p_name)
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

    p_graph->pp_vertices[p_graph->num_vertices] = p_new_vertex;
    p_graph->num_vertices += 1;

    return EXIT_SUCCESS;
}

int remove_vertex(graph_t *p_graph, const char *p_name)
{
    if ((NULL == p_graph) || (NULL == p_name))
    {
        return EXIT_FAILURE;
    }
    int index = -1;

    vertex_t *p_vertex = static_find_vertex(p_graph, p_name, &index);

    if (NULL == p_vertex)
    {
        return EXIT_FAILURE;
    }

    static_destroy_edges_containing(p_graph, p_name);

    static_destroy_vertex(&p_vertex);

    if (NULL != p_vertex)
    {
        return EXIT_FAILURE;
    }

    p_graph->pp_vertices[index] = p_graph->pp_vertices[p_graph->num_vertices - 1];
    p_graph->pp_vertices[p_graph->num_vertices - 1] = NULL;
    p_graph->num_vertices -= 1;

    return EXIT_SUCCESS;
}

int rename_vertex(graph_t *p_graph, const char *p_current_name, const char *p_new_name)
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

const char **get_neighbors(graph_t *p_graph, const char *p_name, int *p_count)
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

    for (int index = 0; index < p_vertex->num_edges; index++)
    {
        pp_neighbors[index] = p_vertex->p_edges[index]->p_destination->p_name;
    }

    return pp_neighbors;
}

int add_edge(graph_t *p_graph, const char *p_from, const char *p_to, int weight)
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

int remove_edge(graph_t *p_graph, const char *p_from, const char *p_to, int weight)
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

    for (int index = 0; index < p_vertex->num_edges;)
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

int *get_edge_weight(graph_t *p_graph, const char *p_from, const char *p_to, int *p_count)
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

    for (int index = 0; index < p_vertex->num_edges; index++)
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

int set_edge_value(graph_t *p_graph, const char *p_from, const char *p_to, int old_weight, int new_weight)
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

    for (int index = 0; index < p_location->num_edges; index++)
    {
        if ((0 == strcmp(p_to, p_edge[index]->p_destination->p_name)) && (old_weight == p_edge[index]->weight))
        {
            p_edge[index]->weight = new_weight;
            return EXIT_SUCCESS;
        }
    }

    return EXIT_FAILURE;
}

void print_graph(graph_t *p_graph)
{
    if (NULL == p_graph)
    {
        return;
    }

    for (int index = 0; index < p_graph->num_vertices; index++)
    {
        vertex_t *p_vertex = p_graph->pp_vertices[index];

        printf("%s: ", p_vertex->p_name);

        for (int jindex = 0; jindex < p_vertex->num_edges; jindex++)
        {
            edge_t *p_edge = p_vertex->p_edges[jindex];

            printf("-> %s, %i ", p_edge->p_destination->p_name, p_edge->weight);
        }
        printf("\n");
    }
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

    int new_capacity = p_graph->capacity * 2;
    vertex_t **pp_temp = realloc(p_graph->pp_vertices, new_capacity * sizeof(*pp_temp));

    if (NULL == pp_temp)
    {
        return EXIT_FAILURE;
    }

    for (int index = p_graph->capacity; index < new_capacity; index++)
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

    for (int index = 0; index < p_vertex->num_edges; index++)
    {
        free(p_vertex->p_edges[index]);
    }

    free(p_vertex->p_edges);
    free(p_vertex->p_name);
    free(p_vertex);
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
static vertex_t *static_find_vertex(graph_t *p_graph, const char *p_name, int *p_index)
{
    if ((NULL == p_graph) || (NULL == p_name))
    {
        return NULL;
    }

    if (NULL != p_index)
    {
        *p_index = -1;
    }

    for (int position = 0; position < p_graph->num_vertices; position++)
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

    int new_capacity = p_vertex->capacity * 2;
    edge_t **pp_temp = realloc(p_vertex->p_edges, new_capacity * sizeof(*pp_temp));

    if (NULL == pp_temp)
    {
        return EXIT_FAILURE;
    }

    for (int index = p_vertex->capacity; index < new_capacity; index++)
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

    for (int index = 0; index < p_graph->num_vertices; index++)
    {
        vertex_t *p_vertex = p_graph->pp_vertices[index];

        if (NULL == p_vertex)
        {
            continue;
        }

        for (int jindex = 0; jindex < p_vertex->num_edges;)
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
