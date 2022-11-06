#pragma once
#include <LiquidCrystal_I2C.h>
#include <EncButton.h>

LiquidCrystal_I2C LCDMain(0x27, 16, 2);
EncButton<EB_TICK, PIND2> btnMain;

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
            , _btn(&btnMain)
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
                _lcdMain->clear();
                _lcdMain->setCursor(charPos, rowNumber);
                _lcdMain->print(str);
            }
        }

        void Update() {
            _btn->tick();
            if (_btn->isClick()) {
                _firstLineData--;
                if (_firstLineData < 0) {
                    _firstLineData = SENSORS_COUNT - 1;
                }
            }
        }

        void UpdateValues(float vals[3]) {
            _vals[0] = vals[0];
            _vals[1] = vals[1];
            _vals[2] = vals[2];
        }

        String GetStringDataByNumberInArray(int i) {
            if (i == 0) {
                return String("T balc " + String(_vals[0]) + String(char(223)) + String("C   "));
            }
            else if (i == 1) {
                return String("T str  " + String(_vals[1]) + String(char(223)) + String("C"));
            }
            else {
                return String("H balc  " + String(_vals[2]) + String("%"));
            }
        }

        void ShowStoragedData() {
            Print(0, 0, GetStringDataByNumberInArray(_firstLineData));
            int secondLineData = _firstLineData + 1;
            if (secondLineData > SENSORS_COUNT - 1) {
                secondLineData = 0;
            }
            Print(0, 1, GetStringDataByNumberInArray(secondLineData));
        }

    private:
        LiquidCrystal_I2C* _lcdMain;
        EncButton<EB_TICK, PIND2>* _btn;
        String _id;
        bool _inited;
        const size_t LOAD_CURSOR_POS = 3;
        static const size_t SENSORS_COUNT = 3;
        float _vals[SENSORS_COUNT]; // [0] - t balc, [1] - t street, [2] - humidity balc
        int _firstLineData = 0;     // ��� �������� � ������ ������ t balc � �����

    };
}

namespace Display {
    LCD1602Dislay LCD1602("Display.LCD1602");
}
