#pragma once
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C LCDMain(0x27, 16, 2);

namespace Display {
    const String LOADING_WORD = "Loading...";
}
namespace Display {
    class LCD1602Dislay {
    public:

        LCD1602Dislay(String id)
            : _id(id)
            , _inited(false)
            , _lcdMain(&LCDMain)
        {}

        virtual void Init() {
            if (!_lcdMain) {
                return;
            }
            _lcdMain->init();
            _inited = true;
            _lcdMain->backlight();
            _lcdMain->setCursor(LOAD_CURSOR_POS, 0);
            _lcdMain->print(Display::LOADING_WORD);
        }

        void Print(size_t charPos, size_t rowNumber, const String str) {
            if (_inited) {
                _lcdMain->setCursor(charPos, rowNumber);
                _lcdMain->print(str);
            }
        }

    private:
        LiquidCrystal_I2C* _lcdMain;
        String _id;
        bool _inited;
        const size_t LOAD_CURSOR_POS = 3;
    };
}

namespace Display {
    LCD1602Dislay LCD1602("Display.LCD1602");
}
