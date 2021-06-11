#ifndef TEST1_EVENT_H_
#define TEST1_EVENT_H_

#include "event/EventCallbackHandle.h"

class Test1Event : public EventCallbackHandle
{
public:
    Test1Event();
    eEventCode handle(const std::string& data) override;
};

#endif // !TEST1_EVENT_H_
