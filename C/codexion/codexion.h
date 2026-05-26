#ifndef CODEXION_H
#define CODEXION_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>

typedef pthread_mutex_t t_mtx;
typedef long long t_time;

typedef enum   e_scheduler
{
    EDF,
    FIFO
}              t_scheduler;

typedef struct s_request
{
    t_time id;
    t_time arrival_time;
    t_time duration;
    t_time deadline;
}               t_request;

typedef struct s_heap
{
    int size;
    int capacity;
    t_request *requests;
}               t_heap;


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
    t_time last_pickup;
    t_mtx mutex;
    int is_available;
    t_time cooldown;
}               t_dongle;


typedef struct  s_data
{
    t_time start_time;
    int number_of_coders;
    t_time time_to_burnout;
    t_time time_to_compile;
    t_time time_to_debug;
    t_time time_to_refactor;
    int number_of_compiles_required;
    t_time dongle_cooldown;
    t_scheduler scheduler;
    t_coder *coders;
    t_dongle *dongle;
    long burnout_flag;
    pthread_mutex_t print_mutex;
    t_heap *heap;
}               t_data;

void    parser(char **v);
int     parser_numbers(char **v);
void    coders_threads(t_data *data);
t_data  *init_data(char **v);
void    start_simulation(t_data *data);
t_time    get_time(void);
void    print_status(t_coder *coder, char *status);
void    coders_join(t_data *data);
void    *coder_routine(void *arg);
void    *monitor_thread(void *arg);
void    ft_usleep(t_time time);

#endif