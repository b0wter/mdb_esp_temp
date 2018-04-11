#include "esp_common.h"
#include "freertos/task.h"
#include "gpio.h"
#include "uart.h"
#include "wifi.h"
#include <string.h>

#define GPIO_ON 1
#define GPIO_OFF 0

/******************************************************************************
 * FunctionName : user_rf_cal_sector_set
 * Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
 *                We add this function to force users to set rf cal sector, since
 *                we don't know which sector is free in user's application.
 *                sector map for last several sectors : ABCCC
 *                A : rf cal
 *                B : rf init data
 *                C : sdk parameters
 * Parameters   : none
 * Returns      : rf cal sector
*******************************************************************************/
uint32 user_rf_cal_sector_set(void)
{
    flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;
    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}

void blink(uint8 count, uint32 pin)
{
    int i = 0;
    for(i = 0; i < count; ++i)
    {
        GPIO_OUTPUT_SET(pin, GPIO_ON);
        vTaskDelay(200/portTICK_RATE_MS);
        GPIO_OUTPUT_SET(pin, GPIO_OFF);
        vTaskDelay(200/portTICK_RATE_MS);
    }
}

void task_blink(void* ignore)
{
    gpio16_output_conf();

    while(true) {
        switch(get_wifi_state())
        {
            case WIFI_STATUS_NONE:
                GPIO_OUTPUT_SET(WIFI_CONNECTION_LED, GPIO_OFF);
                break;
            case WIFI_STATUS_SCANNING:
                blink(1, WIFI_CONNECTION_LED);
                vTaskDelay(1000/portTICK_RATE_MS);
                break;
            case WIFI_STATUS_FINISHED_SCAN:
                blink(2, WIFI_CONNECTION_LED);
                vTaskDelay(1000/portTICK_RATE_MS);
                break;
            case WIFI_STATUS_CONNECTING:
                blink(3, WIFI_CONNECTION_LED);
                vTaskDelay(1000/portTICK_RATE_MS);
                break;
            case WIFI_STATUS_CONNECTED:
                GPIO_OUTPUT_SET(WIFI_CONNECTION_LED, GPIO_ON);
                break;
        }
    }

    vTaskDelete(NULL);
}

void uart_config(void)
{
    UART_ConfigTypeDef	uart_config;
    uart_config.baud_rate				=	BIT_RATE_115200;
    uart_config.data_bits				=	UART_WordLength_8b;
    uart_config.parity					=	USART_Parity_None;
    uart_config.stop_bits				=	USART_StopBits_1;
    uart_config.flow_ctrl				=	USART_HardwareFlowControl_None;
    uart_config.UART_RxFlowThresh	    =	120;
    uart_config.UART_InverseMask		=	UART_None_Inverse;
    UART_ParamConfig(UART0,	&uart_config);
}

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void user_init(void)
{
    uart_config();
    xTaskCreate(&task_wifi_init, "wifi init", 2048, NULL, 1, NULL);
    xTaskCreate(&task_blink, "startup", 2048, NULL, 1, NULL);
}
