#include "event/impl/Test1Event.h"
#include <iostream>
using std::cout;
using std::endl;

Test1Event::Test1Event()
{
    this->m_eventHandleKey = "Event1b";
}

EventCallbackHandle::eEventCode Test1Event::handle(const std::string& data)
{
    cout << "Test1Event data == " << data << endl;
    return EventCallbackHandle::eEventCode::Succ;
}
