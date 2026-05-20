#include "codexion.h"

void    print_status(t_coder *coder, char *status)
{
    long current_time;

    pthread_mutex_lock(&coder->data->print_mutex);
    current_time = get_time() - coder->data->start_time;
    printf("%ld %ld %s", current_time, coder->id, status);
    pthread_mutex_unlock(&coder->data->print_mutex);
}

void    *coder_routine(void *arg)
{
    t_coder *coder = (t_coder *)arg;

    while (!(coder->data->burnout_flag))
    {
            take_dongles(coder);
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
    return (NULL);
}
