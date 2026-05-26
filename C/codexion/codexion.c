#include "codexion.h"

void    coders_threads(t_data *data)
{
    long i;

    i = 0;
    if data->scheduler == FIFO:
        while (i < data->number_of_coders)
        {
            pthread_create(&data->coders[i].thread, NULL, coder_routine, data);
            i++;
        }
    else:
        coders_edf(data)

}

void    coders_join(t_data *data)
{
    long i;

    i = 0;
    while (i < data->number_of_coders)
    {
        pthread_join(data->coders[i].thread, NULL);
        i++;
    }
}

void    *monitor_thread(void *arg)
{
    t_data *data = (t_data *)arg;
    while (1)
    {
        if (check_burnout(data) == 1)
            break ;
        if (check_completion(data) == 1)
            break ;
        usleep(1000);
    }
    return (NULL);
}

void    start_simulation(t_data *data)
{
    pthread_t monitor;

    coders_threads(data);
    pthread_create(&monitor, NULL, (void *)monitor_thread, data);
    pthread_join(monitor, NULL);
    coders_join(data);
}