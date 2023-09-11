#include <Active-Object.h>

void active_object_default_dispatcher(Event *e, Active *object)
{
    switch (e->event_type)
    {
    case DEFAULT:
    {
        Serial.println("Default action");

        break;
    }
    }
}

void active_object_task(void *args)
{
    Active *object = (Active *)args;
    Event ev;

    while (true)
    {
        if (xQueueReceive(object->queue, &ev, (TickType_t)10) != pdPASS)
            continue;

        if (!object->dispatcher(&ev, (void *)object->params))
            active_object_default_dispatcher(&ev, object);
    }
}

void active_object_start(Active *object)
{
    if (object->dispatcher == nullptr)
        return;

    TaskHandle_t handle;

    xTaskCreatePinnedToCore(active_object_task, object->task_name, 2000, (void *)object, configMAX_PRIORITIES - 1, &handle, 0);
    configASSERT(handle);
}

bool active_object_post(Active *object, Event event)
{
    return xQueueSend(object->queue, &event, (TickType_t)10) == pdPASS;
}