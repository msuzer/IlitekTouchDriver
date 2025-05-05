#ifndef TOUCH_ILITEK_H
#define TOUCH_ILITEK_H

#include <stdint.h>
#include <stdbool.h>

#define MAX_TOUCH_REGIONS 8
#define EVENT_QUEUE_SIZE 8

typedef struct {
    uint16_t x;
    uint16_t y;
    bool isTouchDown;
    uint32_t timestamp;
} TouchEvent;

typedef struct {
    uint16_t x0, y0;
    uint16_t x1, y1;
} TouchRegion;

typedef struct {
    int id;
    const char* label;
} TouchButtonInfo;

typedef void (*TouchCallback)(const TouchEvent* evt, const TouchButtonInfo* info);

typedef struct {
    TouchRegion region;
    TouchCallback onTouchDown;
    TouchCallback onTouchUp;
    TouchCallback onTouchHold;
    uint32_t holdTimeMs;
    bool active;
    uint32_t lastDownTime;
    bool isHolding;
    const TouchButtonInfo* userContext;
} TouchRegionHandler;

void ILITEK_Init(void);
void ILITEK_ProcessTouch(void);
void ILITEK_PollTouchEvents(void);
bool ILITEK_ReadTouch(TouchEvent* evt);
void ILITEK_RegisterRegionHandler(const TouchRegionHandler* handler);
bool Touch_IsWithinRegion(const TouchEvent* evt, const TouchRegion* region);

typedef struct {
    uint16_t startX, startY;
    uint16_t width, height;
    uint16_t hSpacing, vSpacing;
    uint8_t columns;
} GridLayout;

void ILITEK_CreateGridButtons(const GridLayout* layout,
							  const TouchButtonInfo btnInfo [],
                              uint8_t count,
                              TouchCallback onTouchDown,
                              TouchCallback onTouchUp,
                              TouchCallback onTouchHold,
                              uint32_t holdTimeMs);

#endif // TOUCH_ILITEK_H
