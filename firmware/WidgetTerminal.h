/**
 * @file       WidgetTerminal.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief
 */

#ifndef WidgetTerminal_h
#define WidgetTerminal_h

#if !defined(LINUX) && !defined(MBED)
    #define BLYNK_USE_PRINT_CLASS
#endif

#include "BlynkApi.h"

#ifdef BLYNK_USE_PRINT_CLASS
    #if !(defined(SPARK) || defined(PARTICLE))
        // On Particle this is auto-included
        #include <Print.h> // TODO: RBL Duo
    #endif
#endif

class WidgetTerminal
#ifdef BLYNK_USE_PRINT_CLASS
    : public Print
#endif
{
public:
    WidgetTerminal(int vPin)
        : mPin(vPin), mOutQty(0)
    {}

    virtual size_t write(uint8_t byte) {
        mOutBuf[mOutQty++] = byte;
        if (mOutQty >= sizeof(mOutBuf)) {
            flush();
        }
        return 1;
    }

    void flush() {
        if (mOutQty) {
            Blynk.virtualWriteBinary(mPin, mOutBuf, mOutQty);
            mOutQty = 0;
        }
    }

#ifdef BLYNK_USE_PRINT_CLASS

    using Print::write;

    size_t write(const void* buff, size_t len) {
    	write((char*)buff, len);
    }

#else

    size_t write(const void* buff, size_t len) {
        uint8_t* buf = (uint8_t*)buff;
        while (len--) {
            write(*buf++);
        }
        return len;
    }

    size_t write(const char* str) {
        return write(str, strlen(str));
    }

#endif

private:
    uint8_t mPin;
    uint8_t mOutBuf[32];
    uint8_t mOutQty;
};

#endif
