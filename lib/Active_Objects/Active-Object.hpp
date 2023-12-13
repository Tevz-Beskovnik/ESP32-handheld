#pragma once
#include <stdio.h>
#include <Arduino.h>

#define DEFAULT_EVENT 0xFFFFFFFF

struct Event
{
    uint32_t event_type;
    int32_t params[10];
    void *misc = nullptr;
};

struct Active
{
    const char *task_name;
    QueueHandle_t queue;
    void *params = nullptr;

    bool (*dispatcher)(Event *, void *); // void * to pass a miscelanious parameter to it
};

/**
 * @brief put the active object on its thread and begin listening to events
 *
 * @param object pointer to Active type containing dispatcher, queue, miscelanious parameters and task name
 */
void active_object_start(Active *object);

/**
 * @brief post event to queue of active object to be dispatched
 *
 * @param object pointer to Active type
 *
 * @param event event to post
 */
bool active_object_post(Active *object, Event event);