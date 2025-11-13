/*
 * fsm_auto.c
 *
 *  Created on: Oct 5, 2025
 *      Author: ADMIN
 */

#include "fsm_auto.h"
#include "fsm_man.h"
int state = 0;
int set=0;
int init_numRed   = 9;
int init_numGreen = 8;
int init_numAmber = 2;

int numRed;
int numGreen;
int numAmber;

static inline int clamp_min1(int v) { return v < 1 ? 1 : v; }
static inline int clamp_amber(int v, int maxAmber) {
    if (v > maxAmber) v = maxAmber;
    return v < 1 ? 1 : v;
}

int checkValidTime(int red, int green, int amber){
	if(red== green+ amber) return 1;
	else return 0;
}

void fsm_auto_run() {
	if(checkValidTime(init_numRed, init_numGreen, init_numAmber)){
    extern int timer3_flag;
    extern void set_timer0(int ms);
    extern void set_timer3(int ms);

    static int entry_GR = 1, entry_AR = 1, entry_RG = 1, entry_RA = 1;

    switch (state) {
    case INIT:
        LED_monitor(OFF_STATE);
        LED_monitor2(OFF_STATE);
        numRed   = init_numRed;
        numGreen = init_numGreen;
        numAmber = init_numAmber;
        state = GREEN_RED;
        entry_GR = entry_AR = entry_RG = entry_RA = 1;
        break;

    case GREEN_RED:
        if (entry_GR) {
            timer0_flag = 0;
            timer3_flag = 0;
            set_timer0(init_numGreen * 1000);
            set_timer3(1000);
            numGreen = init_numGreen;
            numRed   = init_numGreen + init_numAmber;
            entry_GR = 0;
        }

        LED_monitor(GREEN);
        LED_monitor2(RED);

        if (timer3_flag) {
            timer3_flag = 0; set_timer3(1000);
            if (numGreen > 1) numGreen--;
            if (numRed   > 1) numRed--;
        }

        displayNumber0(clamp_min1(numGreen));
        displayNumber1(clamp_min1(numRed));

        if (timer0_flag) {
            state = AMBER_RED;
            entry_AR = 1;
        }
        break;

    case AMBER_RED:
        if (entry_AR) {
            timer0_flag = 0;
            timer3_flag = 0;
            set_timer0(init_numAmber * 1000);
            set_timer3(1000);
            numAmber = init_numAmber;
            numRed   = init_numAmber;
            entry_AR = 0;
        }

        LED_monitor(YELLOW);
        LED_monitor2(RED);

        if (timer3_flag) {
            timer3_flag = 0; set_timer3(1000);
            if (numAmber > 1) numAmber--;
            if (numRed   > 1) numRed--;
        }

        displayNumber0(clamp_amber(numAmber, init_numAmber));
        displayNumber1(clamp_min1(numRed));

        if (timer0_flag) {
            state = RED_GREEN;
            entry_RG = 1;
        }
        break;


    case RED_GREEN:
        if (entry_RG) {
            timer0_flag = 0;
            timer3_flag = 0;
            set_timer0(init_numGreen * 1000);
            set_timer3(1000);
            numGreen = init_numGreen;
            numRed   = init_numGreen + init_numAmber;
            entry_RG = 0;
        }

        LED_monitor(RED);
        LED_monitor2(GREEN);

        if (timer3_flag) {
            timer3_flag = 0; set_timer3(1000);
            if (numGreen > 1) numGreen--;
            if (numRed   > 1) numRed--;
        }

        displayNumber0(clamp_min1(numRed));
        displayNumber1(clamp_min1(numGreen));

        if (timer0_flag) {
            state = RED_AMBER;
            entry_RA = 1;
        }
        break;


    case RED_AMBER:
        if (entry_RA) {
            timer0_flag = 0;
            timer3_flag = 0;
            set_timer0(init_numAmber * 1000);
            set_timer3(1000);
            numAmber = init_numAmber;
            numRed   = init_numAmber;
            entry_RA = 0;
        }

        LED_monitor(RED);
        LED_monitor2(YELLOW);

        if (timer3_flag) {
            timer3_flag = 0; set_timer3(1000);
            if (numAmber > 1) numAmber--;
            if (numRed   > 1) numRed--;
        }

        displayNumber0(clamp_min1(numRed));
        displayNumber1(clamp_amber(numAmber, init_numAmber));

        if (timer0_flag) {
            state = GREEN_RED;
            entry_GR = 1;
        }
        break;

    case NOT_AUTO:
        HAL_GPIO_WritePin(LED_RED0_GPIO_Port,    LED_RED0_Pin,    GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_GREEN0_GPIO_Port,  LED_GREEN0_Pin,  GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_YELLOW0_GPIO_Port, LED_YELLOW0_Pin, GPIO_PIN_SET);

        HAL_GPIO_WritePin(LED_RED1_GPIO_Port,    LED_RED1_Pin,    GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port,  LED_GREEN1_Pin,  GPIO_PIN_SET);
        HAL_GPIO_WritePin(LED_YELLOW1_GPIO_Port, LED_YELLOW1_Pin, GPIO_PIN_SET);

        state = NOTHING;
        break;

    default:
        break;
    }}
	else {
		if(mode_choose){
			if(!set){
		        HAL_GPIO_WritePin(LED_RED0_GPIO_Port,    LED_RED0_Pin,    GPIO_PIN_SET);
		        HAL_GPIO_WritePin(LED_GREEN0_GPIO_Port,  LED_GREEN0_Pin,  GPIO_PIN_SET);
		        HAL_GPIO_WritePin(LED_YELLOW0_GPIO_Port, LED_YELLOW0_Pin, GPIO_PIN_SET);

		        HAL_GPIO_WritePin(LED_RED1_GPIO_Port,    LED_RED1_Pin,    GPIO_PIN_SET);
		        HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port,  LED_GREEN1_Pin,  GPIO_PIN_SET);
		        HAL_GPIO_WritePin(LED_YELLOW1_GPIO_Port, LED_YELLOW1_Pin, GPIO_PIN_SET);
		        set=1;
			}
		}
		else {
        HAL_GPIO_WritePin(LED_RED0_GPIO_Port,    LED_RED0_Pin,    GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_GREEN0_GPIO_Port,  LED_GREEN0_Pin,  GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_YELLOW0_GPIO_Port, LED_YELLOW0_Pin, GPIO_PIN_RESET);

        HAL_GPIO_WritePin(LED_RED1_GPIO_Port,    LED_RED1_Pin,    GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_GREEN1_GPIO_Port,  LED_GREEN1_Pin,  GPIO_PIN_RESET);
        HAL_GPIO_WritePin(LED_YELLOW1_GPIO_Port, LED_YELLOW1_Pin, GPIO_PIN_RESET);
        displayNumber0(88);
        displayNumber1(88);
		}

	}
}
