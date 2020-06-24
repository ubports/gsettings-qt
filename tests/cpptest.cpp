#include <QtTest/QtTest>

#include <QGSettings>

class TestGSettingQt: public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void test_deferredDelete();
    void test_get_set_reset();
    void test_keys();
    void test_changed();
private:
    QGSettings * settings;
    QPointer<QObject> dummy;
};

void TestGSettingQt::initTestCase()
{
    settings = new QGSettings("com.canonical.gsettings.Test", QByteArray(), this);
    dummy = new QObject;
    connect(settings, &QGSettings::changed, dummy.data(), &QObject::deleteLater); // delete the dummy object upon any gsettings change
}

void TestGSettingQt::test_deferredDelete()
{
    QSignalSpy spy(dummy.data(), &QObject::destroyed); // watch the dummy object get destroyed
    settings->set("testString", "bar");

    QVERIFY(spy.wait(1));
    QVERIFY(dummy.isNull()); // verify dummy got destroyed for real
    QCOMPARE(settings->get("testString").toString(), QStringLiteral("bar")); // also verify the setting got written by reading it back

    // reset to default
    settings->reset("testString");
}

void TestGSettingQt::test_get_set_reset()
{
    // Test default value
    QCOMPARE(settings->get("testInteger").toInt(), 42);

    // Set value
    settings->set("testInteger", 1);
    QCOMPARE(settings->get("testInteger").toInt(), 1);

    // Reset the value
    settings->reset("testInteger");
    QCOMPARE(settings->get("testInteger").toInt(), 42);
}

void TestGSettingQt::test_keys()
{
    auto keys = settings->keys();
    QStringList expectedKeys = {"testString", "testDouble", "testEnum",
                                "testBoolean", "testInteger",
                                "testMap", "testStringList"};
    keys.sort();
    expectedKeys.sort();

    QCOMPARE(keys, expectedKeys);
}

void TestGSettingQt::test_changed()
{
    QSignalSpy spy(settings, &QGSettings::changed);

    // Flush pending changed signals
    QVERIFY(spy.wait(1));
    spy.clear();

    settings->set("testBoolean", true);

    // Verify change
    QVERIFY(spy.wait(1));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toString(), QStringLiteral("testBoolean"));

    spy.clear();

    // Test signal on reset event
    settings->reset("testBoolean");
    QVERIFY(spy.wait(1));
    QCOMPARE(spy.count(), 1);
    QCOMPARE(spy.takeFirst().at(0).toString(), QStringLiteral("testBoolean"));
}

QTEST_MAIN(TestGSettingQt)

#include "cpptest.moc"
