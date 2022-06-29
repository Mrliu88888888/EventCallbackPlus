#include "HandleEvent.h"
#include <qjsondocument.h>
#include <qjsonobject.h>

QMap<QString, EventCallbackHandle*> EventCallbackHandle::sm_cbHandle;
const QString EventCallbackHandle::sm_commentChar = "=";
#ifdef THREAD_SAFE
QReadWriteLock EventCallbackHandle::sm_rwmutCbHandle;
#endif

EventCallbackHandle::EventCallbackHandle(const QString& eventKey)
	: m_eventHandleKey(eventKey)
{ }

EventCallbackHandle::~EventCallbackHandle()
{
	this->delHandle();
}

void EventCallbackHandle::addHandle()
{
#ifdef THREAD_SAFE
	sm_rwmutCbHandle.lockForWrite();
#endif
	sm_cbHandle.insert(m_eventHandleKey, this);
#ifdef THREAD_SAFE
	sm_rwmutCbHandle.unlock();
#endif
}

void EventCallbackHandle::delHandle()
{
#ifdef THREAD_SAFE
	sm_rwmutCbHandle.lockForWrite();
#endif
	auto iter = sm_cbHandle.find(m_eventHandleKey);
	if (iter != sm_cbHandle.end())
	{
		sm_cbHandle.erase(iter);
	}
#ifdef THREAD_SAFE
	sm_rwmutCbHandle.unlock();
#endif
}

SEventResult EventCallbackHandle::_handle(const QString& data)
{
	const auto nPos = data.indexOf(sm_commentChar);
	if (nPos == -1) { return SEventResult::ECode::NotFoundSeparator; }

	const auto key = data.mid(0, nPos);
#ifdef THREAD_SAFE
	sm_rwmutCbHandle.lockForRead();
#endif
	auto iter = sm_cbHandle.find(key);
	if (iter == sm_cbHandle.end())
	{
#ifdef THREAD_SAFE
		sm_rwmutCbHandle.unlock();
#endif
		return SEventResult::ECode::NotRegEvent;
	}
#ifdef THREAD_SAFE
	sm_rwmutCbHandle.unlock();
#endif

	const auto value = data.mid(nPos + 1);
	return iter.value()->_handle(value);
}

const QString EventCallbackHandle::handle(const QString& data)
{
#define MESSAGE_NAME "message"
#define JSON_OBJECT obj
#define CASE_CODE_MSG(code, msg) \
	case (code): \
	{ \
		JSON_OBJECT.insert(MESSAGE_NAME, (msg)); \
		break; \
	}

	const auto res = this->_handle(data);
	QJsonObject JSON_OBJECT;
	JSON_OBJECT.insert("code", res.code);
	JSON_OBJECT.insert("context", res.context);
	switch (res.code)
	{
		CASE_CODE_MSG(SEventResult::ECode::Fail, "失败")
		CASE_CODE_MSG(SEventResult::ECode::Succ, "成功")
		CASE_CODE_MSG(SEventResult::ECode::NotFoundSeparator, "没有找到 [" + sm_commentChar + "] 分隔符")
		CASE_CODE_MSG(SEventResult::ECode::NotRegEvent, "没有注册此事件的回调")
		CASE_CODE_MSG(SEventResult::ECode::JsonParseError, "解析Json失败")
		CASE_CODE_MSG(SEventResult::ECode::SqlExecError, "SQL语句执行失败")
		default: JSON_OBJECT.insert(MESSAGE_NAME, "意料之外的错误");
	}
	return QJsonDocument(JSON_OBJECT).toJson(QJsonDocument::Compact);
}
