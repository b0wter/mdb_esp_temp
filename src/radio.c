#include "esp_common.h"
#include "freertos/task.h"
#include "radio.h"

int Radio_State = 0;

int get_radio_state(void)
{
    return Radio_State;
}

void task_radio_init(void* ignore)
{
    Radio_State = 1;

    //TODO: not implemented

    vTaskDelay(NULL);
}