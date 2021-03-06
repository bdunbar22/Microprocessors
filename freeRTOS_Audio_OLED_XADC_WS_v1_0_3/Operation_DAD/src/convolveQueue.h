/*
 * convolveQueue.h
 *
 *  Created on: Apr 23, 2016
 *      Author: Ben
 */
#ifndef SRC_CONVOLVEQUEUE_H_
#define SRC_CONVOLVEQUEUE_H_

#include "chunk_d.h"

/***************************************************
            DEFINES
***************************************************/

#define CONVOLVE_QUEUE_DEPTH_MAX 600
#define CONVOLVE_QUEUE_DEPTH 300

/***************************************************
            DATA TYPES
***************************************************/

/**
 * convolveQueue object
 * The convolve queue stores a left and right array of ints.
 * This queue is special because it allows for performing a
 * specialized convolve operation on the left and right arrays.
 */
typedef struct {
  unsigned int      left[CONVOLVE_QUEUE_DEPTH_MAX];  /* left array*/
  unsigned int   	right[CONVOLVE_QUEUE_DEPTH_MAX];  /* right array */
  unsigned int		position;
} convolveQueue;

/***************************************************
            Access Methods
***************************************************/

/** Initialize the queue

 * Parameters:
 * @param pThis  pointer to own object
 * @param pChunk the chunk to start off the array with.
 *
 * @return Zero on success.
 * Negative value on failure.
 */
int convolveQueue_init(convolveQueue *pThis);

/**
 * Push the left and right audio samples from the chunk onto the
 * left and right arrays accordingly.
 *
 * @return Zero on success.
 * Negative value on failure.
 */
int convolveQueue_push(convolveQueue *pThis, chunk_d_t *pChunk);

/**
 * Return 1 if the convolve queue has reached the usable size.
 */
int convolveQueue_ready(convolveQueue *pThis);

/**
 * Allow the queue to pop an item off of each list.
 *
 * @return None
 */
void convolveQueue_pop(convolveQueue *pThis);

/**
 * Determines the number of sample readings that the left array has to be moved over for the
 * max value of the convolution of the arrays to appear.
 * Finds both the positive and negative number of times it has to be moved and then
 * returns the one with the least absolute value.
 *
 * @return the int phase diff.
 */
int convolveQueue_getSampleCountPhaseDiff(convolveQueue *pThis);


#endif /* SRC_CONVOLVEQUEUE_H_ */
