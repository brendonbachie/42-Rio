#include "codexion.h"

void print_status(t_coder *coder, char *status)
{
    t_time current_time;

    pthread_mutex_lock(&coder->data->print_mutex);
    current_time = get_time() - coder->data->start_time;
    printf("%lld %ld%s", current_time, coder->id, status);
    pthread_mutex_unlock(&coder->data->print_mutex);
}

int simulation_over(t_data *data)
{
    int over;

    pthread_mutex_lock(&data->state_mutex);
    over = data->burnout_flag || data->completion_flag;
    pthread_mutex_unlock(&data->state_mutex);
    return (over);
}

static void start_compiling(t_coder *coder)
{
    pthread_mutex_lock(&coder->data->state_mutex);
    coder->last_compile_start = get_time();
    pthread_mutex_unlock(&coder->data->state_mutex);
    print_status(coder, " is compiling\n");
}

static void finish_compiling(t_dongle *first, t_dongle *second, t_coder *coder)
{
    ft_usleep(coder->data->time_to_compile);
    release_dongle(second);
    release_dongle(first);
    pthread_mutex_lock(&coder->data->state_mutex);
    coder->compiles_done++;
    pthread_mutex_unlock(&coder->data->state_mutex);
}

void *coder_routine(void *arg)
{
    t_coder     *coder;
    t_data      *data;
    t_dongle    *first;
    t_dongle    *second;

    coder = (t_coder *)arg;
    data = coder->data;
    pick_order(coder, &first, &second);
    while (!simulation_over(data))
    {
        if (!acquire_dongle(first, coder))
            break ;
        if (!acquire_dongle(second, coder))
        {
            release_dongle(first);
            break ;
        }
        start_compiling(coder);
        finish_compiling(first, second, coder);
        if (simulation_over(data))
            break ;
        print_status(coder, " is debugging\n");
        ft_usleep(data->time_to_debug);
        if (simulation_over(data))
            break ;
        print_status(coder, " is refactoring\n");
        ft_usleep(data->time_to_refactor);
    }
    return (NULL);
}
