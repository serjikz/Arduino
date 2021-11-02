#pragma once
#include <WString.h>

class Sensor {
	String _id;

public:
	Sensor(String id);
	virtual void Init() = 0;
	const String& GetID() const;
	String GetSensorNotInitializedMsg() const;
protected:

	bool _inited;

};