#include "HandleTestEvent.h"

HandleTestEvent::HandleTestEvent()
	: HandleEvent("Test2Event")
{
}

SEventResult HandleTestEvent::_handle(const QString& data)
{
	emit newTestMessage(data);
	return SEventResult(SEventResult::ECode::Succ, "HandleTestEvent_HandleTestEvent");
}
