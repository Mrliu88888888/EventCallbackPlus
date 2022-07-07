#include "HandleTestEvent.h"
#include "HandleTest0Event.h"
#include "HandleTest1Event.h"
#include <QtCore/QCoreApplication>
#include <qdebug.h>
#include <qthread.h>

int initAllEvent()
{


	return 0;
}

HandleEvent* getHandleEvent()
{
	static const auto tmp = initAllEvent();
	return Q_NULLPTR;
}

static HandleEvent e;

#include <qrandom.h>
class MyThread :public QThread
{
protected:
	void run()
	{
		while (true)
		{
			static QChar chs[4] = { '0', '1', '2', '3' };
			const auto num = QRandomGenerator::global()->bounded(5);

			const auto msg = QString("Test%1Event=%2").arg(chs[num]).arg(QString::number(num));
			qDebug() << e.handle(msg);

			QThread::sleep(0);
		}
	}
};

int main(int argc, char* argv[])
{
	QCoreApplication a(argc, argv);

	initAllEvent();

	static HandleTestEvent test2;
	QObject::connect(&test2, &HandleTestEvent::newTestMessage, [](const QString& msg) {
		qDebug() << "new test msg: " << msg;
		});
	static HandleTest0Event test0;
	static HandleTest1Event test1;

	for (int i = 0; i < 3; ++i)
	{
		auto th = new MyThread;
		th->start();
	}

	while (true)
	{
		const auto flag = QRandomGenerator::global()->bounded(2) == 0;
		const auto flag2 = QRandomGenerator::global()->bounded(2) == 0;
		const auto flag3 = QRandomGenerator::global()->bounded(2) == 0;

		flag ?
			test0.addHandle() :
			test0.delHandle();

		flag2 ?
			test1.addHandle() :
			test1.delHandle();

		flag3 ?
			test2.addHandle() :
			test2.delHandle();

		QThread::sleep(0);
	}

	a.exec();
}
