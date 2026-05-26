#include "codexion.h"

void    print_status(t_coder *coder, char *status)
{
    t_time current_time;

    pthread_mutex_lock(&coder->data->print_mutex);
    current_time = get_time() - coder->data->start_time;
    printf("%ld %ld %s", current_time, coder->id, status);
    pthread_mutex_unlock(&coder->data->print_mutex);
}

void    *coder_routine(void *arg)
{
    t_coder *coder = (t_coder *)arg;
    static bool dongles[2] = {false, false};

    while (!(coder->data->burnout_flag))
    {
        dongles[0] = take_
            if (dongles[0] && dongles[1])
            {
                coder->last_compile_time = get_time();
                coder->compiles_done++;
                print_status(coder, " is compiling\n");
                usleep(coder->data->time_to_compile * 1000);
                release_dongles(coder);
                print_status(coder, " is debugging\n");
                usleep(coder->data->time_to_debug * 1000);
                print_status(coder, " is refactoring\n");
                usleep(coder->data->time_to_refactor * 1000);
            }
    }
    return (NULL);
}
t_request comp_scheduler(t_request a, t_request b, t_scheduler sched)
{
    if (sched == FIFO)
    {
        if (a.arrival_time < b.arrival_time)
            return (1);
        else
            return (0);
    }
    else if (sched == EDF)
    {
        if (a.deadline < b.deadline)
            return (1);
        else
            return (0);
    }
    return (0);
}

t_request *get_next_request(t_data *data)
{
    t_request *next_request = NULL;
    t_heap *heap = data->heap;
    int i;

    while (heap->size > 0)
    {
        if (comp_scheduler(heap->requests[0], next_request, data->scheduler))
            next_request = heap->requests[0];
        i++;
    }
    return (next_request);
}

    
