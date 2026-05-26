#include "codexion.h"

int parser_numbers(char **v)
{
    int i;
    int idx;

    idx = 1;

    while (idx < 8)
    {
        i = 0;
        i = atoi(v[idx]);
        if ((idx < 3) && i <= 0)
            return 0;
        if ((idx >= 3 && idx < 8) && i < 0)
            return 0;
        idx++;
    }
    return 1;
}

void parser(char **v)
{
    if (parser_numbers(v) == 0)
    {
        printf("%s", "Invalid input values.\n");
        exit(1);
    }
    if (strcmp(v[8], "edf") != 0 && strcmp(v[8], "fifo") != 0)
    {
        printf("%s", "Invalid scheduler. Use 'edf' or 'fifo'.\n");
        exit(1);
    }
}

t_dongle *init_dongles(t_data *data)
{
    t_dongle *dongles;
    long i;

    dongles = malloc(sizeof(t_dongle) * data->number_of_coders);
    if (!dongles)
        return (NULL);
    i = 0;
    while (i < data->number_of_coders)
    {
        dongles[i].id = i;
        pthread_mutex_init(&dongles[i].mutex, NULL);
        dongles[i].is_available = 1;
        dongles[i].cooldown = data->dongle_cooldown;
        dongles[i].lastpickup = 0;
        i++;
    }
    return (dongles);
}

t_coder *init_coders(t_data *data)
{
    t_coder *coders;
    long i;

    coders = malloc(sizeof(t_coder) * data->number_of_coders);
    if (!coders)
        return (NULL);
    i = 0;
    while (i < data->number_of_coders)
    {
        coders[i].id = i;
        coders[i].left_dongle = &data->dongle[i];
        coders[i].right_dongle = &data->dongle[(i + 1) % data->number_of_coders];
        coders[i].compiles_done = 0;
        coders[i].last_compile_time = 0;
        coders[i].data = data;
        i++;
    }
    return (coders);
}


t_data *init_data(char **v)
{
    t_data *data;

    data = malloc(sizeof(t_data));
    if (!data)
        return (NULL);
    data->number_of_coders = atoi(v[1]);
    data->time_to_burnout = atoi(v[2]);
    data->time_to_compile = atoi(v[3]);
    data->time_to_debug = atoi(v[4]);
    data->time_to_refactor = atoi(v[5]);
    data->number_of_compiles_required = atoi(v[6]);
    data->dongle_cooldown = atoi(v[7]);
    if (strcmp(v[8], "edf") == 0)
        data->scheduler = EDF;
    else
        data->scheduler = FIFO;
    data->burnout_flag = 0;
    pthread_mutex_init(&data->print_mutex, NULL);
    data->dongle = init_dongles(data);
    data->coders = init_coders(data);
    data->start_time = get_time();
    return (data);
}