#pragma once

#define THREAD_SAFE

#include <qobject.h>
#include <qstring.h>
#include <qmap>
#ifdef THREAD_SAFE
#include <qreadwritelock.h>
#endif


struct SEventResult
{
	QString context;
	enum ECode
	{
		Fail = -1, Succ, NotRegEvent, NotFoundSeparator
	} code;

	SEventResult(const ECode& _code = ECode::Fail, const QString& _context = "")
		: code(_code)
		, context(_context)
	{ }
};


class HandleEvent
	: public QObject
{
public:
	HandleEvent() = default;
	HandleEvent(const QString& eventKey);
	~HandleEvent();

	void addHandle();
	void delHandle();
	bool isHasHandle();

	QString handle(const QString& data);

protected:
	virtual SEventResult _handle(const QString& data);

private:
	const QString m_eventHandleKey;

private:
	static QMap<QString, HandleEvent*> sm_mapHandle;
	static const QString sm_commentChar;
#ifdef THREAD_SAFE
	static QReadWriteLock sm_rwmutHandle;
#endif
};
