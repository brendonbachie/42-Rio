#ifndef CODEXION_H
#define CODEXION_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdbool.h>

typedef pthread_mutex_t t_mtx;
typedef long long t_time;
typedef struct s_data t_data;
typedef struct s_coder t_coder;
typedef struct s_dongle t_dongle;

typedef enum   e_scheduler
{
    EDF,
    FIFO
}              t_scheduler;

typedef struct s_request
{
    long    id;
    t_time  arrival_time;
    t_time  deadline;
}               t_request;

typedef struct s_heap
{
    int         size;
    int         capacity;
    t_request   *requests;
}               t_heap;

typedef struct  s_dongle
{
    long            id;
    t_mtx           mutex;
    pthread_cond_t  cond;
    int             is_taken;
    t_time          free_at;
    t_time          cooldown;
    t_heap          queue;
}               t_dongle;

typedef struct  s_coder
{
    long        id;
    t_dongle    *left_dongle;
    t_dongle    *right_dongle;
    pthread_t   thread;
    long        compiles_done;
    t_time      last_compile_start;
    t_data      *data;
}               t_coder;

typedef struct  s_data
{
    t_time          start_time;
    int             number_of_coders;
    t_time          time_to_burnout;
    t_time          time_to_compile;
    t_time          time_to_debug;
    t_time          time_to_refactor;
    int             number_of_compiles_required;
    t_time          dongle_cooldown;
    t_scheduler     scheduler;
    t_coder         *coders;
    t_dongle        *dongle;
    int             burnout_flag;
    int             completion_flag;
    t_mtx           print_mutex;
    t_mtx           state_mutex;
}               t_data;

/* utils.c */
int         is_valid_number(char *s);
int         parser_numbers(char **v);
void        parser(char **v);
t_dongle    *init_dongles(t_data *data);
t_coder     *init_coders(t_data *data);

/* utils2.c */
t_time      get_time(void);
void        ft_usleep(t_time time);
t_data      *init_data(char **v);
void        free_data(t_data *data);

/* heap.c */
void        heap_push(t_heap *heap, t_request req, t_scheduler sched);
void        heap_pop(t_heap *heap, t_scheduler sched);
void        heap_sift_down(t_heap *heap, int i, t_scheduler sched);

/* heap_utils.c */
long        heap_peek_id(t_heap *heap);
void        heap_remove_id(t_heap *heap, long id, t_scheduler sched);
int         comp_higher_priority(t_request a, t_request b, t_scheduler sched);

/* dongle.c */
void        pick_order(t_coder *coder, t_dongle **first, t_dongle **second);
int         acquire_dongle(t_dongle *dongle, t_coder *coder);
void        release_dongle(t_dongle *dongle);

/* simulation.c */
void        print_status(t_coder *coder, char *status);
int         simulation_over(t_data *data);
void        *coder_routine(void *arg);

/* monitor.c */
void        *monitor_thread(void *arg);

/* codexion.c */
void        coders_threads(t_data *data);
void        coders_join(t_data *data);
void        start_simulation(t_data *data);

#endif
