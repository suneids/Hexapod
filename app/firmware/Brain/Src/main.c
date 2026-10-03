#include "activities.h"

int main(void){

	BRAIN_Init();
	uint32_t state_time = millis();
	SPIDER_StepBase();
    while(7){
    	Control_Update();
    	SPIDER_GaitUpdate();
    	if(millis() - state_time > 200){
    		BRAIN_SendCurrentState();
    		state_time = millis();
    	}
    }
}
