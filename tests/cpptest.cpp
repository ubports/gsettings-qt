#include <QtTest/QtTest>

#include <QGSettings>

class TestDeferredDelete: public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void test_deferredDelete();
private:
    QGSettings * settings;
    QPointer<QObject> dummy;
};

void TestDeferredDelete::initTestCase()
{
    settings = new QGSettings("com.canonical.gsettings.Test", QByteArray(), this);
    dummy = new QObject;
    connect(settings, &QGSettings::changed, dummy.data(), &QObject::deleteLater); // delete the dummy object upon any gsettings change
}

void TestDeferredDelete::test_deferredDelete()
{
    QSignalSpy spy(dummy.data(), &QObject::destroyed); // watch the dummy object get destroyed
    settings->set("testString", "bar");

    QVERIFY(spy.wait(1));
    QVERIFY(dummy.isNull()); // verify dummy got destroyed for real
    QCOMPARE(settings->get("testString").toString(), QStringLiteral("bar")); // also verify the setting got written by reading it back
}

QTEST_MAIN(TestDeferredDelete)

#include "cpptest.moc"
