#include "HandleTest1Event.h"

HandleTest1Event::HandleTest1Event()
	: HandleEvent("Test1Event")
{
}

SEventResult HandleTest1Event::_handle(const QString& data)
{
	emit newTestMessage(data);
	return SEventResult(SEventResult::ECode::Succ, "HandleTestEvent_HandleTestEvent");
}
