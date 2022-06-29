#pragma once

#include <qstring.h>
#include <qmap>
#include <qreadwritelock.h>

// #define THREAD_SAFE



struct SEventResult
{
	QString context;
	enum ECode : int
	{
		Fail = -1, Succ, NotRegEvent, NotFoundSeparator
	} code;

	SEventResult(const SEventResult::ECode& _code = SEventResult::ECode::Fail, const QString& _context = "")
		: code(_code)
		, context(_context)
	{ }
};

class EventCallbackHandle
{
public:
	EventCallbackHandle() = default;
	EventCallbackHandle(const QString& eventKey);
	~EventCallbackHandle();

	void addHandle();
	void delHandle();

	const QString handle(const QString& data);

protected:
	virtual SEventResult _handle(const QString& data);

private:
	const QString m_eventHandleKey;

private:
	static QMap<QString, EventCallbackHandle*> sm_cbHandle;
	static const QString sm_commentChar;
#ifdef THREAD_SAFE
	static QReadWriteLock sm_rwmutCbHandle;
#endif
};
