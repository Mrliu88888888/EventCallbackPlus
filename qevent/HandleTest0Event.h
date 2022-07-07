#pragma once
#include "HandleEvent.h"


class HandleTest0Event
	: public HandleEvent
{
	Q_OBJECT

public:
	HandleTest0Event();

Q_SIGNALS:
	void newTestMessage(const QString& msg);

protected:
	SEventResult _handle(const QString& data) override;
};
