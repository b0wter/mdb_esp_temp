#include "esp_common.h"
#include "esp_wifi.h"
#include "freertos/task.h"
#include "wifi.h"

int Wifi_State = WIFI_STATUS_NONE;

int get_wifi_state(void)
{
    return Wifi_State;
}

void on_wifi_scan_finished(void* arg, STATUS status)
{
    Wifi_State = WIFI_STATUS_SCANNING;
    uint8	ssid[33];
    char	temp[128];
    if	(status	==	OK)	{
        struct bss_info	*bss_link = (struct bss_info*)arg;
        while(bss_link!=NULL)
        {
            memset(ssid,0,33);
            if(strlen(bss_link->ssid)<=32)
                memcpy(ssid,bss_link->ssid,	strlen(bss_link->ssid));
            else
                memcpy(ssid,bss_link->ssid,32);
                printf("(%d,\"%s\",%d,\""MACSTR"\",%d)\r\n",
                    bss_link->authmode,	ssid,bss_link->rssi,
                    MAC2STR(bss_link->bssid),bss_link->channel);
                    bss_link=bss_link->next.stqe_next;
        }
    }
    else
    {
        printf("Wifi scan failed.\r\n");
    }

    Wifi_State = WIFI_STATUS_FINISHED_SCAN;
}

void task_wifi_init(void* ignore)
{
    Wifi_State = WIFI_STATUS_NONE;
    vTaskDelay(5000/portTICK_RATE_MS);
    wifi_set_opmode(STATION_MODE);
    wifi_station_scan(NULL, on_wifi_scan_finished);
    vTaskDelete(NULL);
}