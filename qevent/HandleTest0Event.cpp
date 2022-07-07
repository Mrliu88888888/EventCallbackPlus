#include "HandleTest0Event.h"

HandleTest0Event::HandleTest0Event()
	: HandleEvent("Test0Event")
{
}

SEventResult HandleTest0Event::_handle(const QString& data)
{
	emit newTestMessage(data);
	return SEventResult(SEventResult::ECode::Succ, "HandleTestEvent_HandleTestEvent");
}
