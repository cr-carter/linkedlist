# DSA II – Graph Theory (Day 1)

## Topic: Introduction to Graphs

### 📌 Key Terminology

| Term               | Definition |
|--------------------|------------|
| **Vertex (Node)**  | A fundamental unit or point in a graph representing an entity (e.g., user, location, system). |
| **Edge**           | A connection between two vertices, representing a relationship or pathway. |
| **Neighbor**       | A vertex directly connected to another via an edge. |
| **Graph**          | A data structure consisting of a set of vertices and a set of edges. |
| **Directed Graph** | A graph where edges have direction (A ➝ B is not the same as B ➝ A). |
| **Undirected Graph** | A graph where all edges are bidirectional (A—B means A is a neighbor of B and vice versa). In programming, this is often modeled with *two* directed edges. |
| **Degree (Undirected)** | The total number of edges connected to a vertex. |
| **In-Degree / Out-Degree (Directed)** | The number of incoming/outgoing edges for a vertex. |
| **Path**           | A sequence of edges connecting a source vertex to a destination vertex. |
| **Connected Graph**| A graph is connected **if and only if** every vertex has a path to every other vertex. |
| **Complete Graph** | A graph in which every vertex is a neighbor to every other vertex. |
| **Tree**           | A special kind of graph that is **connected** and **acyclic** (no cycles). |
| **Weighted Edge**  | An edge that carries a cost or weight (e.g., distance, time, bandwidth). |

> **Key Principle:** All trees are graphs, but not all graphs are trees.

---

### 📍 Applications of Graphs

Graphs are a foundational structure in Computer Science and real-world systems:

- **Social Media / Friendships**: Who is connected to whom?
- **Navigation / GPS Routing**: What's the optimal path?
- **Network Topologies**: How are systems and resources interlinked?
- **Dependency Graphs**: What tasks must precede others?
- **Recommendation Engines**: Who/what is "nearby" or similar?

---

### 🧠 Common Graph API Design

In many implementations, graphs are backed by dictionaries, where each key maps to its list of neighbors:

```c
add_vertex(D, key);
del_vertex(D, key);

add_edge(D, key_x, key_y);
del_edge(D, key_x, key_y);

is_connected(D, key_x, key_y);
is_neighbor(D, key_x, key_y);
```

> `D` is typically a dictionary-like structure, such as a hashtable or hashmap, mapping keys (vertex names) to adjacency lists.

---

### 📦 Graph Representations

| Method             | Description |
|--------------------|-------------|
| **Adjacency List** | A mapping of each vertex to a list of neighboring vertices. Efficient for sparse graphs. Often implemented using a dictionary of linked lists. |
| **Adjacency Matrix** | A 2D array `NxN` where `matrix[i][j]` is `1` if there's an edge from vertex `i` to `j`. Efficient for dense graphs, but memory-heavy. |

---

### 🧪 Practical Exercise – Neighbor Lookups

#### 📄 Input: Adjacency List Style Format

```
Jack Adam
Joe Frank
Adam Jill
Frank Joe Jack
Jill Adam
```

📌 Interpretation:  
Each line represents a vertex followed by its outbound directed edges (i.e., neighbors).

#### ✅ Expected Behavior of `is_neighbor(X, Y)`

The function returns `true` **iff** there is a directed edge from `X ➝ Y`.

#### 📋 Exercise Outputs

```c
is_neighbor("Jack", "Jack");  // false
is_neighbor("Jack", "Joe");   // false
is_neighbor("Jack", "Adam");  // true
is_neighbor("Jack", "Frank"); // false
is_neighbor("Jack", "Jill");  // false

is_neighbor("Joe", "Jack");   // false
is_neighbor("Joe", "Joe");    // false
is_neighbor("Joe", "Adam");   // false
is_neighbor("Joe", "Frank");  // true
is_neighbor("Joe", "Jill");   // false

is_neighbor("Adam", "Jack");  // false
is_neighbor("Adam", "Joe");   // false
is_neighbor("Adam", "Adam");  // false
is_neighbor("Adam", "Frank"); // false
is_neighbor("Adam", "Jill");  // true

is_neighbor("Frank", "Jack"); // true
is_neighbor("Frank", "Joe");  // true
is_neighbor("Frank", "Adam"); // false
is_neighbor("Frank", "Frank");// false
is_neighbor("Frank", "Jill"); // false

is_neighbor("Jill", "Jack");  // false
is_neighbor("Jill", "Joe");   // false
is_neighbor("Jill", "Adam");  // true
is_neighbor("Jill", "Frank"); // false
is_neighbor("Jill", "Jill");  // false

is_neighbor("Jack", "Tom");   // false
is_neighbor("Tom", "Jack");   // false
is_neighbor("Tom", "Bill");   // false
```

---

### 📘 Wrap-Up and Day 1 Homework

1. **Build your own adjacency list parser** (from text input like above).  
   Try implementing it as a:
   - Dictionary of lists
   - Or adjacency matrix, if you're feeling bold

2. **Implement `is_neighbor(D, A, B)`** function in C (or Python) using the above format.

3. Bonus: Try extending the API to include:
   - `get_neighbors(D, A)`
   - `print_graph(D)` for debugging and visualization