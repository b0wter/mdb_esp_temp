#ifndef WIFI_H
#define WIFI_H

#define WIFI_CONNECTION_LED 5

#define WIFI_STATUS_NONE 0
#define WIFI_STATUS_SCANNING 1
#define WIFI_STATUS_FINISHED_SCAN 2
#define WIFI_STATUS_CONNECTING 3
#define WIFI_STATUS_CONNECTED 4

int get_wifi_state(void);
void task_wifi_init(void* ignore);

#endif