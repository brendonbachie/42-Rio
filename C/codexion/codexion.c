#include "codexion.h"

void coders_threads(t_data *data)
{
    long i;

    i = 0;
    while (i < data->number_of_coders)
    {
        pthread_create(&data->coders[i].thread, NULL, coder_routine, &data->coders[i]);
        i++;
    }
}

void coders_join(t_data *data)
{
    long i;

    i = 0;
    while (i < data->number_of_coders)
    {
        pthread_join(data->coders[i].thread, NULL);
        i++;
    }
}

void start_simulation(t_data *data)
{
    pthread_t monitor;

    coders_threads(data);
    pthread_create(&monitor, NULL, monitor_thread, data);
    pthread_join(monitor, NULL);
    coders_join(data);
    free_data(data);
}
