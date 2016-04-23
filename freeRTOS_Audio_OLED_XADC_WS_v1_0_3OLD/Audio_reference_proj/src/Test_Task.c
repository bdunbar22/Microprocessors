#include "Test_Task.h"
#include "Audio_Output.h"
#include "zedboard_freertos.h"
#include <stdio.h>


/*
 * This solution is set for MONO Audio O/P only.
 */

static void test_task( void ){

	printf("Test task running\n");
	int pos = 10;
	for(;;){

		if (pos > 1800)
		{
			pos = 0;
		}
		else
		{
			pos += 30;
		}

		printf("Test pos: %d\n", pos);
		outputFound(pos, 0);
		vTaskDelay(30);
	}
}

int test_start() {
	printf("Test_start\n");
	/* create the audio task */
	xTaskCreate( test_task, ( signed char * ) "HW", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2 , NULL );
	return 0;
}
