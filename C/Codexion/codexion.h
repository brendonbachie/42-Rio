#ifndef CODEXION_H
#define CODEXION_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef pthread_mutex_t t_mtx;

typedef enum   e_scheduler
{
    EDF,
    FIFO
}               t_scheduler;

typedef struct  s_coder
{
    long id;
    t_dongle *left_dongle;
    t_dongle *right_dongle;
    pthread_t thread;
    long compiles_done;
    long last_compile_time;
    t_data *data;
}               t_coder;

typedef struct  s_dongle
{
    long id;
    pthread_mutex_t mutex;
    int is_available;
    long cooldown;
}               t_dongle;


typedef struct  s_data
{
    long start_time;
    long number_of_coders;
    long time_to_burnout;
    long time_to_compile;
    long time_to_debug;
    long time_to_refactor;
    long number_of_compiles_required;
    long dongle_cooldown;
    t_scheduler scheduler;
    t_coder *coders;
    t_dongle *dongle;
    long burnout_flag;
    pthread_mutex_t print_mutex;
}               t_data;

void    parser(char **v);
int     parser_numbers(char **v);

#endif