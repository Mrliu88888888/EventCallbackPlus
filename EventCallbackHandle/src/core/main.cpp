#include "event/EventCallbackHandle.h"
#include "event/impl/Test1Event.h"
#include <iostream>
using std::cout;
using std::endl;

void test()
{
    static Test1Event test1; test1.addHandle();
}

int main()
{
    EventCallbackHandle eventCbHandle;

    test();

    cout << EventCallbackHandle::handleCode(eventCbHandle.handle("Event1b=asd")) << endl;

    getchar();
    return 0;
}
