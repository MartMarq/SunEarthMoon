#ifndef WATCHY_Uhr_H
#define WATCHY_Uhr_H

#include <Watchy.h>
#include "icons.h"
#include "FreeSans16pt7b.h"
#include "FreeSans40pt7b.h"

#define IS_DOUBLE_TAP       (wakeupBit & ACC_INT_MASK && guiState == WATCHFACE_STATE)
#define IS_BTN_RIGHT_UP     (wakeupBit & UP_BTN_MASK && guiState == WATCHFACE_STATE)
#define IS_BTN_LEFT_UP      (wakeupBit & BACK_BTN_MASK && guiState == WATCHFACE_STATE)
#define IS_BTN_RIGHT_DOWN   (wakeupBit & DOWN_BTN_MASK && guiState == WATCHFACE_STATE)
#define EXT_INT_MASK        MENU_BTN_MASK|BACK_BTN_MASK|UP_BTN_MASK|DOWN_BTN_MASK   

class Uhr : public Watchy{
    using Watchy::Watchy;
    public:
        Uhr();
		void drawWatchFace();
    virtual void handleButtonPress();
    void vibrate();
    esp_sleep_wakeup_cause_t wakeup_reason;
};

#endif
