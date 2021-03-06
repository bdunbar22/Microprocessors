/**
 *@file audioPlayer.c
 *
 *@brief
 *  - core module for audio player
 *
 * Target:   TLL6527v1-0      
 * Compiler: VDSP++     Output format: VDSP++ "*.dxe"
 *
 * @author:  Gunar Schirner
 *           Rohan Kangralkar
 * @date	03/08/2010
 *
 * LastChange:
 * $Id: audioPlayer.c 846 2014-02-27 15:35:54Z fengshen $
 *
 *******************************************************************************/

#include "audioManager.h"
#include "adau1761.h"
#include "zedboard_freertos.h"
#include "stdio.h"

/* number of chunks to allocate */
//TODO: up to 30 once heap size is increased
#define CHUNK_NUM 30

/* size of each chunk in bytes */
#define CHUNK_SIZE 512

/**
 * @def VOLUME_CHANGE_STEP
 * @brief Magnitude of change in the volume when increasing or decreasing
 */
#define VOLUME_CHANGE_STEP (4)
/**
 * @def VOLUME_MAX
 * @brief MAX volume possible is +6db refer to ssm2603 manual
 */
#define VOLUME_MAX (0x7F)
/**
 * @def VOLUME_MIN
 * @brief MIN volume possible is -73db refer to ssm2603 manual
 */
#define VOLUME_MIN (0x2F)

/**
 * Provide constant speed of sound
 * 344 m/s
 */
#define SPEED_SOUND 344

/**
 * Provide constant distance between the two microphones 45mm
 * Use units of meters
 */
#define MIC_DISTANCE 0.045

/**
 * Define the sampling period being used by the microphones
 */
#define SAMPLING_FREQUENCY 1/48000

/** initialize audio player 
 *@param pThis  pointer to own object 
 *
 *@return 0 success, non-zero otherwise
 **/
int audioManager_init(audioPlayer_t *pThis) {
    int status = 0;
    
    printf("[AP]: Init start\r\n");
    
    pThis->volume 		= VOLUME_MIN; /*default volume */
    pThis->frequency 	= 48000; /* default frequency */
    

    /* init the codec */
	adau1761_init(&(pThis->codec));

	/* allocate buffer pool */
	status = bufferPool_d_init(&(pThis->bp), CHUNK_NUM, CHUNK_SIZE);
    if ( 1 != status ) {
        return FAIL;
    }
    
    /* Initialize the gpio ... (TBD) */
    /* TODO insert code for GPIO init here */

    /* Initialize the audio RX module*/
    status = audioRx_init(&pThis->rx, &pThis->bp) ;
    if ( 1 != status) {
        return FAIL;
    }

    /* Initialize the audio TX module */
    //status = audioTx_init(&pThis->tx, &pThis->bp);
    if ( 1 != status ) {
        return FAIL;
    }   
    
    printf("[AP]: Init complete\r\n");

    return PASS;
}




/** startup phase after initialization 
 *@param pThis  pointer to own object 
 *
 *@return 0 success, non-zero otherwise
 **/
int audioManager_start(audioPlayer_t *pThis)
{
    printf("[AP]: startup \r\n");
    
	/* create the audio task */
	xTaskCreate( audioManager_task, ( signed char * ) "HW", configMINIMAL_STACK_SIZE, pThis, tskIDLE_PRIORITY + 1 , pThis );

    
    return PASS;
}



/** main loop of audio player does not terminate
 *@param pThis  pointer to own object 
 *
 *@return 0 success, non-zero otherwise
 **/
void audioManager_task (void *pArg) {
    
	int                      status = FAIL;
	audioPlayer_t *pThis = (audioPlayer_t *)  pArg;
    

	/* startup sub components */
    /* Start the audio RX module */
    status = audioRx_start(&pThis->rx);
    if ( 1 != status) {
    	/* how to indicate startup failure ?*/
        return;
    }

	/* main loop */
	while(1) {
    	/** get audio chunk */
		status = audioRx_get(&pThis->rx, &pThis->chunk);

        /** If we have chunks that can be processed then do so */
        if ( 1 == status ) {
        	audioManager_process(&pThis->chunk);
        }
  }
}

/**
 * Determine the direction from which a sound is coming from.
 * Call the output function with the direction and the distance if found.
 * sourceAngle is in tenths of degrees aka source angle of 50 = 5 degrees.
 * distance is in mm.
 *
 *@param pThis  pointer to own object
 *
 *@return 0 success, non-zero otherwise
 **/
void audioManager_process (chunk_d_t **pChunk) {
	//Get chunks

	//Chunks must be large enough so that the phase difference can be found
	
    int samplesDelayed = 11;

	//Angle equation:
    //Units for speed are defined in m/s
    //Units for length are defined in m
    //samples delay is an integer
    //Sampling period is a frequency
    //source Angle is calculated in degrees
	int sourceAngle = sin(1(SPEED_SOUND(samplesDelayed*SAMPLING_FREQUENCY)/MIC_DISTANCE);
	//Give tenths of degrees
    sourceAngle = sourceAngle * 10;
    //Not calculated at this point in time.
    int distance = 0;
	outputFound(sourceAngle, distance);
}


