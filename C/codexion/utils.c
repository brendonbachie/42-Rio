#include "codexion.h"

int is_valid_number(char *s)
{
    int i;

    i = 0;
    if (s[i] == '-' || s[i] == '+')
        i++;
    if (!s[i])
        return (0);
    while (s[i])
    {
        if (s[i] < '0' || s[i] > '9')
            return (0);
        i++;
    }
    return (1);
}

int parser_numbers(char **v)
{
    int idx;
    long value;

    idx = 1;
    while (idx < 8)
    {
        if (!is_valid_number(v[idx]))
            return (0);
        value = atoi(v[idx]);
        if (idx < 3 && value <= 0)
            return (0);
        if (idx >= 3 && idx < 8 && value < 0)
            return (0);
        idx++;
    }
    return (1);
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
    t_dongle    *dongles;
    long        i;

    dongles = malloc(sizeof(t_dongle) * data->number_of_coders);
    if (!dongles)
        return (NULL);
    i = 0;
    while (i < data->number_of_coders)
    {
        dongles[i].id = i;
        pthread_mutex_init(&dongles[i].mutex, NULL);
        pthread_cond_init(&dongles[i].cond, NULL);
        dongles[i].is_taken = 0;
        dongles[i].cooldown = data->dongle_cooldown;
        dongles[i].free_at = data->start_time;
        dongles[i].queue.requests = malloc(sizeof(t_request) * data->number_of_coders);
        dongles[i].queue.size = 0;
        dongles[i].queue.capacity = data->number_of_coders;
        i++;
    }
    return (dongles);
}

t_coder *init_coders(t_data *data)
{
    t_coder *coders;
    long    i;

    coders = malloc(sizeof(t_coder) * data->number_of_coders);
    if (!coders)
        return (NULL);
    i = 0;
    while (i < data->number_of_coders)
    {
        coders[i].id = i + 1;
        coders[i].left_dongle = &data->dongle[i];
        coders[i].right_dongle = &data->dongle[(i + 1) % data->number_of_coders];
        coders[i].compiles_done = 0;
        coders[i].last_compile_start = data->start_time;
        coders[i].data = data;
        i++;
    }
    return (coders);
}
