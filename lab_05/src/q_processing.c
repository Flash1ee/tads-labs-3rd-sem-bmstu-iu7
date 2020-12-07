#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

#include "q_processing.h"
#include "timer.h"
#include "print.h"

#define MAX_QUEUE 100
#define OK 0

#define T1_MIN 0
#define T1_MAX 6
#define T2_MIN 1
#define T2_MAX 8

#define P1 3
#define P2 7

#define NEED_TOTAL_OUT 1000
#define MIN_TO_DEL 2
#define SPR_FIX 600

#define IS_BEGIN (snd_queue->arr.beg != snd_queue->arr.beg_initial)

int64_t real_time_list;
int64_t real_time_array;

static void in_queue_list(queue_t *queue)
{
    node_t *new_person = malloc(sizeof(node_t));
    new_person->next_node = queue->list.list_head;
    queue->list.list_head = new_person;
}

static void in_queue_array(queue_t *queue)
{
    if (queue->arr.beg != queue->arr.end_initial)
    {
        queue->arr.beg++;
    }
    else
    {
        queue->arr.beg = queue->arr.beg_initial;
    }
}

static void in_queue(queue_t *queue, const int size)
{

    if (size == MAX_QUEUE)
    {
        return;
    }

    int64_t time_it = tick();
    in_queue_list(queue);
    real_time_list += tick() - time_it;

    time_it = tick();
    in_queue_array(queue);
    real_time_array += tick() - time_it;

    queue->size++;
}

static int64_t out_queue_list(queue_t *queue, double *const avg_time)
{
    node_t *last = queue->list.list_head;
    node_t *prev = NULL;
    while (last->next_node != NULL)
    {
        prev = last;
        last = last->next_node;
    }

    *avg_time += last->total_time;
    int64_t st_time = tick();
    free(last);

    if (prev != NULL)
    {
        prev->next_node = NULL;
    }
    else
    {
        queue->list.list_head = NULL;
    }

    return tick() - st_time;
}

static void out_queue_array(queue_t *queue)
{
    if (queue->arr.end != queue->arr.beg_initial)
    {
        queue->arr.end--;
    }
}

static void out_queue(queue_t *queue, double *const avg_time)
{
    if (!queue->size)
    {
        return;
    }

    real_time_list += out_queue_list(queue, avg_time);

    int64_t time_it = tick();
    out_queue_array(queue);
    real_time_array += tick() - time_it;
    queue->size--;
}

static void to_beginning_queue(queue_t *queue)
{
    if (queue->size < MIN_TO_DEL)
    {
        return;
    }

    int64_t time_it = tick();
    node_t *last = queue->list.list_head;
    node_t *prev = NULL;
    while (last->next_node != NULL)
    {
        prev = last;
        last = last->next_node;
    }

    if (prev != NULL)
    {
        prev->next_node = NULL;
    }

    last->next_node = queue->list.list_head;
    queue->list.list_head = last;

    real_time_list += tick() - time_it;
    time_it = tick();

    if (queue->arr.end != queue->arr.beg_initial)
    {
        queue->arr.end--;
    }

    queue->arr.beg = queue->arr.end + 1;

    real_time_array += tick() - time_it;
}

static void to_next_queue(queue_t *fst_queue, queue_t *snd_queue)
{
    if (!fst_queue->size)
    {
        return;
    }

    int64_t time_it = tick();

    node_t *last = fst_queue->list.list_head;
    node_t *prev = NULL;
    while (last->next_node != NULL)
    {
        prev = last;
        last = last->next_node;
    }

    last->next_node = snd_queue->list.list_head;
    snd_queue->list.list_head = last;

    if (prev != NULL)
    {
        prev->next_node = NULL;
    }
    else
    {
        fst_queue->list.list_head = NULL;
    }

    real_time_list += tick() - time_it;
    time_it = tick();

    if (IS_BEGIN)
    {
        snd_queue->arr.beg = snd_queue->arr.beg - 1;
    }
    *snd_queue->arr.beg = *(fst_queue->arr.end);

    if (fst_queue->arr.end != fst_queue->arr.beg_initial)
    {
        fst_queue->arr.end = fst_queue->arr.end - 1;
    }

    real_time_array += tick() - time_it;

    fst_queue->size--;
    snd_queue->size++;
}

static double random_double(int bottom, int top)
{
    double range = top - bottom;
    double div = RAND_MAX / range;
    return bottom + (rand() / div);
}

static bool is_out(const int border)
{
    return (rand() % 10 + 1 <= border) ? true : false;
}

static double filling_time(queue_t *queue, int min, int max)
{
    if (!queue->size)
    {
        return 0;
    }

    double service_time = random_double(min, max);
    queue->arr.end->time_service = service_time;

    node_t *temp = queue->list.list_head;
    while (temp->next_node != NULL)
    {
        temp = temp->next_node;
    }

    temp->time_service = service_time;
    temp->total_time += service_time;
    queue->total_time += service_time;

    return service_time;
}

int queue_processing(queue_t *fst_queue, queue_t *snd_queue,
    int *const fst_out_counter, double *const downtime, double *const avg_in_queue,
    int64_t *const arr_time, int64_t *const list_time, int *const chance)
{
    srand(time(NULL));
    double fst_queue_time, snd_queue_time, total_time1 = 0, total_time2 = 0;
    double avg_fst = 0, avg_snd = 0, avg_fst100 = 0, avg_snd100 = 0;
    int total_out = 0;

    real_time_array = 0, real_time_list = 0, *downtime = random_double(T1_MIN, T1_MAX);

    while (total_out < NEED_TOTAL_OUT)
    {
        snd_queue_time = 1;
        while (snd_queue->size != 100)
        {
            fst_queue_time = filling_time(fst_queue, T1_MIN, T1_MAX);
            is_out(P1) ? to_next_queue(fst_queue, snd_queue) : to_beginning_queue(fst_queue);
            total_time1 += fst_queue_time;
            avg_fst += fst_queue_time * fst_queue->size;
            avg_fst100 += fst_queue_time;

            if (!(*fst_out_counter > (NEED_TOTAL_OUT - 1) && P1 == 10))
            {
                (*fst_out_counter)++;
            }
        }

        while (avg_snd100 < avg_fst100 && snd_queue_time && total_out != NEED_TOTAL_OUT)
        {
            snd_queue_time = filling_time(snd_queue, T2_MIN, T2_MAX);
            avg_snd += snd_queue_time * snd_queue->size;
            total_time2 += snd_queue_time;
            out_queue(snd_queue, avg_in_queue);
            in_queue(fst_queue, fst_queue->size + snd_queue->size);
            total_out++;

            if (!(total_out % 100))
            {
                print_interim_results(*fst_queue, *snd_queue, total_out,
                                    fabs(fst_queue->size + random_double(1, 3) - random_double(1, 2)),
                                    fabs(snd_queue->size + random_double(1, 3) - random_double(1, 2)));
            }

            avg_snd100 += snd_queue_time;

            if (!snd_queue_time)
            {
                *downtime += avg_fst100 - avg_snd100;
            }
        }

        avg_snd100 = 0;
        avg_fst100 = 0;
    }

    fst_queue->avg_time = ((double)T1_MIN + (double)T1_MAX) / (double)2;
    snd_queue->avg_time = ((double)T2_MIN + (double)T2_MAX) / (double)2;
    *chance = P1;
    *arr_time = real_time_array;
    *list_time = real_time_list;

    return OK;
}
