#include "esp_common.h"
#include "freertos/task.h"
#include "gpio.h"
#include <stdlib.h>
#include "led.h"

#define GPIO_ON 1
#define GPIO_OFF 0
#define LED_COUNT 4

LedAction* led_actions [LED_COUNT];

// TODO: replace fixed size with constant.
const int actionIds[] = { 0, 1, 2, 3 };
const char* actionNames[] = { "blink #1", "blink #2", "blink #3", "blink #4" };
const int blink_on_off_time = 200; // Time between the individual blinks in an action.
const int blink_pause = 1000; // Time between two blink intervals.
const xTaskHandle handles[LED_COUNT];
const bool actionInUse[] = { false, false, false, false };

/*
    Does the blinking. Is called separately for each action.
*/
void blink_led(void* parameter)
{
    // TODO: bounds check for id, logging?

    int id = (int)&parameter;
    int blinks = led_actions[id]->blinks_per_trigger;
    int gpio = led_actions[id]->gpio_id;

    if(blinks = 0)
    {
        GPIO_OUTPUT_SET(gpio, LED_OFF);
    }
    else if(blinks = 255)
    {
        GPIO_OUTPUT_SET(gpio, LED_OFF);
    }
    else
    {
        int i;
        for(i = 0; i < blinks; i++)
        {
            GPIO_OUTPUT_SET(led_actions[id]->gpio_id, GPIO_ON);
            vTaskDelay(blink_on_off_time/portTICK_RATE_MS);
            GPIO_OUTPUT_SET(led_actions[id]->gpio_id, GPIO_OFF);
            vTaskDelay(blink_on_off_time/portTICK_RATE_MS);
        }    
        vTaskDelay(blink_pause/portTICK_RATE_MS);
    }
}

/*
    Searches for the first action that is not in use. If no free slot is found, -1 is returned.
*/
int get_first_free_slot(void)
{
    int i ;
    for(i = 0; i < LED_COUNT; i++)
    {
        if(actionInUse[i] == false)
            return i;
    }
    return -1;
}

/*
    Adds an led action. There is a maximum of actions and the function will return an id if the action was added.
    Otherwise -1 is returned.
*/
int add_led_action(LedAction* action)
{
    int slot = get_first_free_slot();
    if(slot == -1 || slot > LED_COUNT)
        return -1;

    led_actions[slot] = action;
    xTaskCreate(&blink_led, actionNames[slot], 128, NULL, actionIds[slot], handles[slot]);
    return slot;
}

/*
    Removes an led action and stops the corresponding task. Will return true if an action was stopped.
*/
bool remove_led_action(int id)
{
    if(actionInUse[id] == false)
        return false;
    vTaskDelete(handles[id]);
    return true;
}