#include "event/EventCallbackHandle.h"

std::map<std::string, EventCallbackHandle*> EventCallbackHandle::sm_cbHandle;
boost::shared_mutex EventCallbackHandle::sm_rwmut_cbHandle;
const std::string EventCallbackHandle::sm_commentChar = "=";

void EventCallbackHandle::addHandle()
{
    sm_rwmut_cbHandle.lock();
    sm_cbHandle.insert(std::pair<std::string, EventCallbackHandle*>(m_eventHandleKey, this));
    sm_rwmut_cbHandle.unlock();
}

void EventCallbackHandle::removeHandle()
{
    sm_rwmut_cbHandle.lock();
    auto iter = sm_cbHandle.find(m_eventHandleKey);
    if (iter != sm_cbHandle.end())
    {
        sm_cbHandle.erase(iter);
    }
    sm_rwmut_cbHandle.unlock();
}

EventCallbackHandle::eEventCode EventCallbackHandle::handle(const std::string& data)
{
    const std::size_t nPos = data.find(sm_commentChar);
    if (nPos == data.npos)
    {
        return EventCallbackHandle::eEventCode::NotFoundSeparator;
    }

    const std::string key = data.substr(0, nPos);
    sm_rwmut_cbHandle.lock_shared();
    auto iter = sm_cbHandle.find(key);
    if (iter == sm_cbHandle.end())
    {
        sm_rwmut_cbHandle.unlock_shared();
        return EventCallbackHandle::eEventCode::NotRegEvent;
    }
    sm_rwmut_cbHandle.unlock_shared();

    const std::string value = data.substr(nPos + 1);
    return iter->second->handle(value);
}

const std::string EventCallbackHandle::handleCode(const eEventCode& code)
{
    nlohmann::json j;

    j["code"] = code;
    switch (code)
    {
    case eEventCode::Succ:
    {
        j["msg"] = "Succ";
        break;
    }
    case eEventCode::NotFoundSeparator:
    {
        j["msg"] = "没有找到 [" + sm_commentChar + "] 分隔符";
        break;
    }
    case eEventCode::NotRegEvent:
    {
        j["msg"] = "没有注册此事件的回调";
        break;
    }
    default:
    {
        j["msg"] = "意料之外的错误";
        break;
    }
    }

    return j.dump();
}
