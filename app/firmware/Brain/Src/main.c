#include "activities.h"

int main(void){

	BRAIN_Init();
	uint32_t ma = millis();
	SPIDER_StepBase();
    while(7){
    	Control_Update();
    	SPIDER_GaitUpdate();
    	if(millis() - ma > 30){
    		ma = millis();
    		GPIO_PinToggle(brain_led);
    	}
    }
}
