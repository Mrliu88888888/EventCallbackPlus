#pragma once
#include "HandleEvent.h"


class HandleTestEvent
	: public HandleEvent
{
	Q_OBJECT

public:
	HandleTestEvent();

Q_SIGNALS:
	void newTestMessage(const QString& msg);

protected:
	SEventResult _handle(const QString& data) override;
};
