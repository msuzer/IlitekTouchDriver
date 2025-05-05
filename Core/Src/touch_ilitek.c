
#include "touch_ilitek.h"
#include "stm32l0xx_hal.h"
#include <string.h>
#include <stdio.h>

#define ILITEK_I2C_ADDR     (0x41 << 1)
#define ILITEK_DATA_REG     0x00
#define I2C_TIMEOUT         100

extern I2C_HandleTypeDef hi2c1;
extern GPIO_TypeDef* TOUCH_INT_GPIO_Port;
extern uint16_t TOUCH_INT_Pin;
extern GPIO_TypeDef* TOUCH_RST_GPIO_Port;
extern uint16_t TOUCH_RST_Pin;

static TouchRegionHandler regionHandlers[MAX_TOUCH_REGIONS];
static TouchButtonInfo buttonInfos[MAX_TOUCH_REGIONS];
static uint8_t numRegionHandlers = 0;

static TouchEvent eventQueue[EVENT_QUEUE_SIZE];
static uint8_t eventHead = 0;
static uint8_t eventTail = 0;

static void enqueueEvent(const TouchEvent* evt) {
    uint8_t next = (eventHead + 1) % EVENT_QUEUE_SIZE;
    if (next != eventTail) {
        eventQueue[eventHead] = *evt;
        eventHead = next;
    }
}

static bool dequeueEvent(TouchEvent* evt) {
    if (eventHead == eventTail) return false;
    *evt = eventQueue[eventTail];
    eventTail = (eventTail + 1) % EVENT_QUEUE_SIZE;
    return true;
}

void ILITEK_Init(void) {
    HAL_GPIO_WritePin(TOUCH_RST_GPIO_Port, TOUCH_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(TOUCH_RST_GPIO_Port, TOUCH_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(100);
}

bool ILITEK_ReadTouch(TouchEvent* evt) {
    uint8_t data[6];
    if (HAL_I2C_Mem_Read(&hi2c1, ILITEK_I2C_ADDR, ILITEK_DATA_REG, I2C_MEMADD_SIZE_8BIT, data, 6, I2C_TIMEOUT) != HAL_OK)
        return false;

    uint8_t touchCount = data[0];
    uint8_t eventType = data[1];

    if (touchCount == 0)
        return false;

    evt->x = ((uint16_t)data[2] << 8) | data[3];
    evt->y = ((uint16_t)data[4] << 8) | data[5];
    evt->isTouchDown = (eventType == 0x01 || eventType == 0x02);
    evt->timestamp = HAL_GetTick();

    return true;
}

bool Touch_IsWithinRegion(const TouchEvent* evt, const TouchRegion* region) {
    return evt->x >= region->x0 && evt->x <= region->x1 &&
           evt->y >= region->y0 && evt->y <= region->y1;
}

void ILITEK_RegisterRegionHandler(const TouchRegionHandler* handler) {
    if (numRegionHandlers < MAX_TOUCH_REGIONS) {
        regionHandlers[numRegionHandlers++] = *handler;
    }
}

void ILITEK_ProcessTouch(void) {
    TouchEvent evt;
    if (ILITEK_ReadTouch(&evt)) {
        enqueueEvent(&evt);
    }
}

void ILITEK_PollTouchEvents(void) {
    TouchEvent evt;
    while (dequeueEvent(&evt)) {
        for (uint8_t i = 0; i < numRegionHandlers; ++i) {
            TouchRegionHandler* h = &regionHandlers[i];
            if (!h->active) continue;

            if (evt.isTouchDown) {
                if (Touch_IsWithinRegion(&evt, &h->region)) {
                    if (!h->lastDownTime) {
                        h->lastDownTime = evt.timestamp;
                        if (h->onTouchDown) h->onTouchDown(&evt, h->userContext);
                    } else if (!h->isHolding && (evt.timestamp - h->lastDownTime >= h->holdTimeMs)) {
                        h->isHolding = true;
                        if (h->onTouchHold) h->onTouchHold(&evt, h->userContext);
                    }
                } else {
                    h->lastDownTime = 0;
                    h->isHolding = false;
                }
            } else {
                if (h->lastDownTime) {
                    if (Touch_IsWithinRegion(&evt, &h->region) && h->onTouchUp) {
                        h->onTouchUp(&evt, h->userContext);
                    }
                    h->lastDownTime = 0;
                    h->isHolding = false;
                }
            }
        }
    }
}

void ILITEK_CreateGridButtons(const GridLayout* layout,
                              const TouchButtonInfo btnInfo[],
                              uint8_t count,
                              TouchCallback onTouchDown,
                              TouchCallback onTouchUp,
                              TouchCallback onTouchHold,
                              uint32_t holdTimeMs) {
    for (uint8_t i = 0; i < count && numRegionHandlers < MAX_TOUCH_REGIONS; ++i) {
        uint8_t row = i / layout->columns;
        uint8_t col = i % layout->columns;

        uint16_t x0 = layout->startX + col * (layout->width + layout->hSpacing);
        uint16_t y0 = layout->startY + row * (layout->height + layout->vSpacing);
        uint16_t x1 = x0 + layout->width;
        uint16_t y1 = y0 + layout->height;

        buttonInfos[numRegionHandlers] = btnInfo[i];

        printf("Registering button %u: '%s' at [%u,%u]-[%u,%u]\n", btnInfo[i].id, btnInfo[i].label, x0, y0, x1, y1);

        TouchRegionHandler handler = {
            .region = { x0, y0, x1, y1 },
            .onTouchDown = onTouchDown,
            .onTouchUp = onTouchUp,
            .onTouchHold = onTouchHold,
            .holdTimeMs = holdTimeMs,
            .active = true,
            .lastDownTime = 0,
            .isHolding = false,
            .userContext = &buttonInfos[numRegionHandlers]
        };

        ILITEK_RegisterRegionHandler(&handler);
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == TOUCH_INT_Pin) {
        ILITEK_ProcessTouch();
    }
}
