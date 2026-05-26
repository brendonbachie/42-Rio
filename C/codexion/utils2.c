#include "codexion.h"

t_time get_time()
{
    struct timeval  tv;
    t_time  time;

    gettimeofday(&tv, NULL);
    time = (tv.tv_sec * 1000) + (tv_usec / 1000);
    return (time)
}

void ft_usleep (t_time time)
{
    t_time  curent_time;
    t_time  start_time;

    start_time = get_time();
    current_time = start_time;

    while(current_time - start_time < time)
    {
        usleep(time / 10);
        current_time = get_time();
    }
}

int comp_schenduler(t_request a, t_request b, t_scheduler sched)
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