#ifndef UNIONFIND_H
#define UNIONFIND_H

typedef struct element
{
    struct element *p_parent;
    void *p_data;
    int depth;
} element_t;

element_t *make_set(void *p_data);
element_t *find(element_t *p_element);
element_t *merge(element_t *p_left, element_t *p_right);

#endif // UNIONFIND_H

/* End of file unionfind.h */
