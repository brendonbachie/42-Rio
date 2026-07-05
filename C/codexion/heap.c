#include "codexion.h"

static void heap_swap(t_request *a, t_request *b)
{
    t_request tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;
}

void heap_push(t_heap *heap, t_request req, t_scheduler sched)
{
    int i;
    int parent;

    heap->requests[heap->size] = req;
    i = heap->size;
    heap->size++;
    while (i > 0)
    {
        parent = (i - 1) / 2;
        if (!comp_higher_priority(heap->requests[i], heap->requests[parent], sched))
            break ;
        heap_swap(&heap->requests[i], &heap->requests[parent]);
        i = parent;
    }
}

void heap_sift_down(t_heap *heap, int i, t_scheduler sched)
{
    int left;
    int right;
    int top;

    while (1)
    {
        left = i * 2 + 1;
        right = i * 2 + 2;
        top = i;
        if (left < heap->size
            && comp_higher_priority(heap->requests[left], heap->requests[top], sched))
            top = left;
        if (right < heap->size
            && comp_higher_priority(heap->requests[right], heap->requests[top], sched))
            top = right;
        if (top == i)
            break ;
        heap_swap(&heap->requests[i], &heap->requests[top]);
        i = top;
    }
}

void heap_pop(t_heap *heap, t_scheduler sched)
{
    if (heap->size == 0)
        return ;
    heap->requests[0] = heap->requests[heap->size - 1];
    heap->size--;
    heap_sift_down(heap, 0, sched);
}
