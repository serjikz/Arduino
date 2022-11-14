#pragma once

// Class is needed to show a device initialization
class RGYColorsIniter {

	enum State {
		None = 0,
		RedLight = 1,
		YellowLight,
		GreenLight,
		GreenBlink
	};
public:

	RGYColorsIniter(int redDiodPin, int yellowDiodPin, int greenDiodPin) 
		:_redPin(redDiodPin),
		_yellowPin(yellowDiodPin),
		_greenPin(greenDiodPin)
	{};

	void Start() {
		if (_redPin > 0 && _yellowPin > 0 && _greenPin > 0) 
		{
			pinMode(_redPin, OUTPUT);
    		pinMode(_yellowPin, OUTPUT);
    		pinMode(_greenPin, OUTPUT);
			digitalWrite(_yellowPin, LOW);
			digitalWrite(_greenPin, LOW);
			_inited = true;
			_startTime = millis();
			// Enable a red diod
			digitalWrite(_redPin, HIGH);
			_state = State::RedLight;
		}
		else
		{
			_inited = false;
		}
	}

	void Update() 
	{
		if (_inited) 
		{
			switch (_state) {
				case State::RedLight:
					if (millis() > COLOR_SWITCH_DT) {
						digitalWrite(_redPin, LOW);
						digitalWrite(_yellowPin, HIGH);
						_state = State::YellowLight;
					}
				break;
				case State::YellowLight:
					if (millis() > 2 * COLOR_SWITCH_DT) {
						digitalWrite(_yellowPin, LOW);
						digitalWrite(_greenPin, HIGH);
						_state = State::GreenLight;
					}
				break;
				case State::GreenLight:
					if (millis() > 3 * COLOR_SWITCH_DT) {
						digitalWrite(_greenPin, LOW);
						_state = State::None;
						_isStartEffectCompleted = true;
					}
				break;
				case State::GreenBlink:
					if (millis() - _blinkStartTime >= BLINK_GREEN_DT)
					{
						digitalWrite(_greenPin, LOW);
						_state = State::None;
					}
				break;
				default:
				break;
			}
		}
	}

	bool IsStartEffectCompleted() const
	{
		return _isStartEffectCompleted;
	}

	void BlinkGreenLight() 
	{
		if (_isStartEffectCompleted) 
		{
			_state = State::GreenBlink;
			_blinkStartTime = millis();
			digitalWrite(_greenPin, HIGH);
		}
	}

protected:
	bool _inited = false;
	bool _isStartEffectCompleted= false;
	unsigned long _startTime = 0;
	unsigned long _blinkStartTime = 0;
	const unsigned int COLOR_SWITCH_DT = 3000;
	const unsigned int BLINK_GREEN_DT = 1000;
	int _redPin = 0;
	int _yellowPin = 0;
	int _greenPin = 0;
	State _state = State::None;
};
