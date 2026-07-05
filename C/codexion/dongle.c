#include "codexion.h"

void pick_order(t_coder *coder, t_dongle **first, t_dongle **second)
{
    if (coder->left_dongle->id < coder->right_dongle->id)
    {
        *first = coder->left_dongle;
        *second = coder->right_dongle;
    }
    else
    {
        *first = coder->right_dongle;
        *second = coder->left_dongle;
    }
}

static int dongle_ready(t_dongle *dongle, t_coder *coder)
{
    if (dongle->is_taken)
        return (0);
    if (get_time() < dongle->free_at)
        return (0);
    if (heap_peek_id(&dongle->queue) != coder->id)
        return (0);
    return (1);
}

static void build_timeout(t_dongle *dongle, struct timespec *ts)
{
    t_time wake_at;

    wake_at = get_time() + 1;
    if (dongle->free_at > wake_at)
        wake_at = dongle->free_at;
    ts->tv_sec = wake_at / 1000;
    ts->tv_nsec = (wake_at % 1000) * 1000000;
}

int acquire_dongle(t_dongle *dongle, t_coder *coder)
{
    t_data          *data;
    t_request       req;
    struct timespec ts;

    data = coder->data;
    req.id = coder->id;
    req.arrival_time = get_time();
    req.deadline = coder->last_compile_start + data->time_to_burnout;
    pthread_mutex_lock(&dongle->mutex);
    heap_push(&dongle->queue, req, data->scheduler);
    while (!dongle_ready(dongle, coder))
    {
        if (simulation_over(data))
        {
            heap_remove_id(&dongle->queue, coder->id, data->scheduler);
            pthread_mutex_unlock(&dongle->mutex);
            return (0);
        }
        build_timeout(dongle, &ts);
        pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
    }
    heap_pop(&dongle->queue, data->scheduler);
    dongle->is_taken = 1;
    pthread_mutex_unlock(&dongle->mutex);
    print_status(coder, " has taken a dongle\n");
    return (1);
}

void release_dongle(t_dongle *dongle)
{
    pthread_mutex_lock(&dongle->mutex);
    dongle->is_taken = 0;
    dongle->free_at = get_time() + dongle->cooldown;
    pthread_mutex_unlock(&dongle->mutex);
    pthread_cond_broadcast(&dongle->cond);
}
