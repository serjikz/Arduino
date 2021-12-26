#include "Sensor.h"

Sensor::Sensor(String id)
	:_id(id)
	, _inited(false)
{
}

const String& Sensor::GetID() const
{
	return _id;
}

String Sensor::GetSensorNotInitializedMsg() const
{
	return String("Sensor " + _id + " not initialized");
}

