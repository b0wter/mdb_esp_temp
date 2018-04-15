#include "esp_common.h"
#include "freertos/task.h"

#include "output.h"

#define MAX_MESSAGES 255

char* messages[MAX_MESSAGES];
int number_of_messages;
xTaskHandle output_task;

/*
    Sets the content of each message to ((void *) 0) and resets 'number_of_messages'.
*/
void clear_messages(void)
{
    number_of_messages = 0;
    int i;
    for(int i = 0; i < MAX_MESSAGES; i++)
        messages[i] == NULL;
}

/*
    Writes all messages (only the ones set) to the standard output stream and calls 'clear_messages'.
*/
void write_all_output(void* ignore)
{
    int i;
    for(i = 0; i < number_of_messages; i++)
        printf(messages[i]);

    clear_messages();
}

/*
    Creates a periodic task that writes the contents of 'messages' to the standard output.
*/
void set_output_task()
{
    xTaskCreate(&write_all_output, "write output", 128, NULL, 1, &output_task);
}

/*
    Adds a message to the end of the message queue.
*/
void add_output(char* output)
{
    if(output_task == NULL)
    {
        clear_messages();
        set_output_task();
    }
}
