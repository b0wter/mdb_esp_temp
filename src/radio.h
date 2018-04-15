#ifndef RADIO_H
#define RADIO_H

#define RADIO_LED 4

#define RADIO_STATUS_NONE 0
#define RADIO_STATUS_INIT 1
#define RADIO_STATUS_ACTIVE 2

int get_radio_state(void);
void task_radio_init(void* ignore);

#endif