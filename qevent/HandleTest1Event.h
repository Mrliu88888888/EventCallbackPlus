#pragma once
#include "HandleEvent.h"


class HandleTest1Event
	: public HandleEvent
{
	Q_OBJECT

public:
	HandleTest1Event();

Q_SIGNALS:
	void newTestMessage(const QString& msg);

protected:
	SEventResult _handle(const QString& data) override;
};
