#pragma once

// Class is needed to show device initialization
class RGYColorsIniter {
public:

	RGYColorsIniter(int redDiodPin, int yellowDiodPin, int greenDiodPin) 
	{
		if (redDiodPin > 0 && yellowDiodPin > 0 && greenDiodPin > 0) 
		{
			pinMode(redDiodPin, OUTPUT);
    		pinMode(yellowDiodPin, OUTPUT);
    		pinMode(greenDiodPin, OUTPUT);
			_inited = true;
		}
		else
		{
			_inited = false;
		}
	}

protected:
	bool _inited;

};
