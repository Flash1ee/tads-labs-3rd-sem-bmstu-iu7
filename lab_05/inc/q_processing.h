#ifndef __Q_PROCESSING_H__
#define __Q_PROCESSING_H__

#include <inttypes.h>
#include "structures.h"

int queue_processing(queue_t *fst_queue, queue_t *snd_queue,
    int *const fst_out_counter, double *const downtime, double *const avg_in_queue,
    int64_t *const arr_time, int64_t *const list_time, int *const chance);

#endif
