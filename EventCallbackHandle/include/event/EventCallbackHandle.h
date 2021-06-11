#ifndef EVENT_CALLBACK_HANDLE_H_
#define EVENT_CALLBACK_HANDLE_H_

#include <string>
#include <map>
#include <boost/thread/shared_mutex.hpp>
#include <json.hpp>

class EventCallbackHandle
{
public:
    enum eEventCode
    {
        Succ, NotRegEvent, NotFoundSeparator
    };

public:
    void addHandle();
    void removeHandle();
    virtual eEventCode handle(const std::string& data);

public:
    static const std::string handleCode(const eEventCode& code);

protected:
    std::string m_eventHandleKey;

private:
    static std::map<std::string, EventCallbackHandle*> sm_cbHandle;
    static boost::shared_mutex sm_rwmut_cbHandle;
    static const std::string sm_commentChar;
};

#endif // !EVENT_CALLBACK_HANDLE_H_
