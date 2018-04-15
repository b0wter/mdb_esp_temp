#ifndef LED_H
#define LED_H

#define LED_COUNT 4
#define LED_OFF 0
#define LEF_ON 255

typedef struct LedAction
{
    int gpio_id;
    int blinks_per_trigger;
} LedAction;

int add_led_action(LedAction* action);
bool remove_led_action(int id);

#endif