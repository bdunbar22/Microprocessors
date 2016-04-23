/*
 * convolveQueue.c
 *
 *  Created on: Apr 23, 2016
 *      Author: Ben
 */

#include "convolveQueue.h"
#include "chunk_d.h"
#include "zedboard_freertos.h"

int convolve(unsigned int left[], unsigned int right[], unsigned int size);

/**
 * This file realizes the functions of the convolve queue.
 */

/***************************************************
            Methods
***************************************************/

/** Initialize the queue

 * Parameters:
 * @param pThis  pointer to own object
 * @param pChunk the chunk to start off the array with.
 *
 * @return Zero on success.
 * Negative value on failure.
 */
int convolveQueue_init(convolveQueue *pThis) {
	pThis->position = 0;
	return 0;
}

/**
 * Push the left and right audio samples from the chunk onto the
 * left and right arrays accordingly.
 *
 * @return Zero on success.
 * Negative value on failure.
 */
int convolveQueue_push(convolveQueue *pThis, chunk_d_t *pChunk) {
	unsigned int sampleNr = 0;
	if ( NULL == pThis || NULL == pChunk ) {
		printf("[TX]: Chunk/Convolve Queue objects not initialized \r\n");
		return -1;
	}

	unsigned int samplesInChunk = pChunk->bytesUsed/sizeof(unsigned int);

	for(sampleNr=0;sampleNr < samplesInChunk;sampleNr++) {
		 pThis->left[pThis->position] = ((unsigned int)pChunk->u16_buff[sampleNr]) << 24;
		 pThis->right[pThis->position] = 0x00FF & (((unsigned int)pChunk->u16_buff[sampleNr]) << 16);
		 pThis->position += 1;
	}

	return 0;
}

/**
 * Return 1 if the convolve queue has reached the usable size.
 */
int convolveQueue_ready(convolveQueue *pThis) {
	if(pThis->position > CONVOLVE_QUEUE_DEPTH) {
		return 1;
	}
	return -1;
}

/**
 * Pop the queue down to the window size.
 *
 * @return None
 */
void convolveQueue_pop(convolveQueue *pThis) {
	int shift = pThis->position - CONVOLVE_QUEUE_DEPTH;
	if(shift < 1) {
		return;
	}
	int i = 0;
	for(i = 0; i < CONVOLVE_QUEUE_DEPTH; i ++) {
		pThis->left[i] = pThis->left[i + shift];
		pThis->right[i] = pThis->right[i + shift];
	}
	pThis->position = CONVOLVE_QUEUE_DEPTH;
}

/**
 * Determines the number of sample readings that the left array has to be moved over for the
 * max value of the convolution of the arrays to appear.
 * Finds both the positive and negative number of times it has to be moved and then
 * returns the one with the least absolute value.
 *
 * @return the int phase diff.
 */
int convolveQueue_getSampleCountPhaseDiff(convolveQueue *pThis) {
	return convolve(pThis->left,pThis->right, CONVOLVE_QUEUE_DEPTH);
}


/**
 * Returns an integer -Shift to Shift depending on the phase difference as determined by the convolve
 * operation.
 */
int convolve(unsigned int left[], unsigned int right[], unsigned int size) {
  //Create a void Result
  unsigned int Result[size + size - 1];
  size_t itr;

  for (itr = 0; itr < size + size - 1; itr++)
  {
    size_t kmin, kmax, k;

    Result[itr] = 0;

    kmin = (itr >= size - 1) ? itr - (size - 1) : 0;
    kmax = (itr < size - 1) ? itr : size - 1;

    for (k = kmin; k <= kmax; k++)
    {
      Result[itr] += left[k] * right[itr - k];
    }
  }

  //////////////Solution is in result
	//Max points location
	unsigned int max = 0;
	unsigned int location = 0;
	int i = 0;
    for(i = 0; i < (size + size - 1); i++)
    {
        if(Result[i] > max){
			max = Result[i];
			location = i;
		}
    }

	//Determine Phase
	unsigned int ZeroPhase = (size + size)/2;
	int phase = 0;

	if(ZeroPhase > (location+1)){
		phase = (ZeroPhase - (location+1))*-1;
	}
	else if(ZeroPhase < (location+1)){
		phase = (location+1) - ZeroPhase;
	}

	return phase;
}