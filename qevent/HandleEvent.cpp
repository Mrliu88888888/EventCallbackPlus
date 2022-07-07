#include "HandleEvent.h"
#include <qjsondocument.h>
#include <qjsonobject.h>
#pragma execution_character_set("utf-8")


QMap<QString, HandleEvent*> HandleEvent::sm_mapHandle;
const QString HandleEvent::sm_commentChar = "=";
#ifdef THREAD_SAFE
QReadWriteLock HandleEvent::sm_rwmutHandle;
#endif


HandleEvent::HandleEvent(const QString& eventKey)
	: m_eventHandleKey(eventKey)
{
	this->addHandle();
}

HandleEvent::~HandleEvent()
{
	this->delHandle();
}

void HandleEvent::addHandle()
{
	if (this->isHasHandle() == false)
	{
#ifdef THREAD_SAFE
		sm_rwmutHandle.lockForWrite();
#endif
		sm_mapHandle.insert(m_eventHandleKey, this);
#ifdef THREAD_SAFE
		sm_rwmutHandle.unlock();
#endif
	}
}

void HandleEvent::delHandle()
{
#ifdef THREAD_SAFE
	sm_rwmutHandle.lockForRead();
#endif
	auto iter = sm_mapHandle.find(m_eventHandleKey);
	if (iter == sm_mapHandle.end())
	{
#ifdef THREAD_SAFE
		sm_rwmutHandle.unlock();
#endif
		return;
	}
#ifdef THREAD_SAFE
	sm_rwmutHandle.unlock();
#endif

#ifdef THREAD_SAFE
	sm_rwmutHandle.lockForWrite();
#endif
	sm_mapHandle.erase(iter);
#ifdef THREAD_SAFE
	sm_rwmutHandle.unlock();
#endif
}

bool HandleEvent::isHasHandle()
{
#ifdef THREAD_SAFE
	sm_rwmutHandle.lockForRead();
#endif
	const auto res = (sm_mapHandle.find(m_eventHandleKey) != sm_mapHandle.end());
#ifdef THREAD_SAFE
	sm_rwmutHandle.unlock();
#endif
	return res;
}

SEventResult HandleEvent::_handle(const QString& data)
{
	const auto nPos = data.indexOf(sm_commentChar);
	if (nPos == -1) { return SEventResult::ECode::NotFoundSeparator; }

	const auto key = data.mid(0, nPos);
	const auto value = data.mid(nPos + 1);
#ifdef THREAD_SAFE
	sm_rwmutHandle.lockForRead();
#endif
	auto iter = sm_mapHandle.find(key);
	if (iter == sm_mapHandle.end())
	{
#ifdef THREAD_SAFE
		sm_rwmutHandle.unlock();
#endif
		return SEventResult::ECode::NotRegEvent;
	}
	const auto res = iter.value()->_handle(value);
#ifdef THREAD_SAFE
	sm_rwmutHandle.unlock();
#endif
	return res;
}

QString HandleEvent::handle(const QString& data)
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
	default: JSON_OBJECT.insert(MESSAGE_NAME, "意料之外的错误");
	}
	return QJsonDocument(JSON_OBJECT).toJson(QJsonDocument::Compact);
}
