/*
 * Audio_Output.h
 *
 *  Created on: Apr 1, 2016
 *      Author: Ben
 */

#ifndef AUDIO_OUTPUT_H_
#define AUDIO_OUTPUT_H_

/*
 * Given source angle as an integer representing tenths of degrees
 * i.e. if sourceAngle = 50 then the angle is 5 degrees
 * Given distance of sound source in mm.
 * If distance is sent as 0 then it was not calculated.
 */
void outputFound(int sourceAngle, int distance);

#endif /* AUDIO_OUTPUT_H_ */
