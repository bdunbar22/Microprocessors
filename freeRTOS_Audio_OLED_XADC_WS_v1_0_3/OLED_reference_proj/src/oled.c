/* Credits to : http://embeddedcentric.com/customized-hardware-design-and-integration/ */

/* Also refer to "\freeRTOS_Audio_OLED_XADC_bsp\ps7_cortexa9_0\libsrc\ZedboardOLED_v1_0"
 * for more options on writing to the OLED - Line or a character
 */
#include "ZedboardOLED.h"
#include <stdio.h>
#include <stdlib.h>

int main(void){

print_message("Hello",0);
print_message("World",2);
print_message("How cool is this",3);

int currentPos = 100;
int currentDist = 4;

clear_OLED();
char oled_angle[30];
char oled_dist[30];
sprintf(oled_angle, "Angle: %d", currentPos);
sprintf(oled_dist, "Distance: %dm", currentDist);

print_message(oled_angle,0);
print_message(oled_dist,2);

return 0;
}
