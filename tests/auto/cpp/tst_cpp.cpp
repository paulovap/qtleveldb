#include <QtTest/QtTest>

#include <qleveldb.h>
#include <qleveldbbatch.h>

#include <QObject>
#include <QDir>
#include <QDebug>
#include <QVariant>
#include <QVariantList>
#include <QVariantMap>

class CppTest: public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    void test_create();
    void test_put_get();
    void test_batch();
    void test_readStream();
private:
    QLevelDB *m_leveldb;
    QVariantMap dataMap;
    QVariantList dataList;
};



void CppTest::initTestCase()
{
    dataList << "1" << 2 << 2.3;
    dataMap.insert("int", 1);
    dataMap.insert("list", dataList);
    dataMap.insert("string", QString("asdf"));
}

void CppTest::cleanupTestCase()
{
}

void CppTest::init()
{
    m_leveldb = new QLevelDB("test.db");
    m_leveldb->open();
}

void CppTest::cleanup()
{
    m_leveldb->close();
    m_leveldb->destroyDB("test.db");
}

void CppTest::test_create()
{
    QVERIFY2(m_leveldb->opened(), "unable to open database");
}

void CppTest::test_put_get()
{

    QVERIFY2(m_leveldb->opened(), "unable to open database");

    QVERIFY(m_leveldb->put("int", 1));
    QVERIFY(m_leveldb->put("float", 21.23));
    QVERIFY(m_leveldb->put("list", dataList));
    QVERIFY(m_leveldb->put("map", dataMap));

    QVERIFY(m_leveldb->get("int") == 1);
    QVERIFY(m_leveldb->get("float") == 21.23);
    QVERIFY(m_leveldb->get("list") == dataList);
    QVERIFY(m_leveldb->get("map") == dataMap);
}

void CppTest::test_batch()
{
    QVERIFY2(m_leveldb->opened(), "unable to open database");
    QLevelDBBatch *batch = m_leveldb->batch();
    bool result = batch
            ->put("map", dataMap)
            ->put("list", dataList)
            ->put("string", "asdf")
            ->del("map")->write();
    QVERIFY(result);
    QVERIFY(m_leveldb->get("list") == dataList);
    QVERIFY(m_leveldb->get("map") == QVariant());
    QVERIFY(m_leveldb->get("string") == "asdf");
    //delete batch;
}

void CppTest::test_readStream()
{
    QVERIFY2(m_leveldb->opened(), "unable to open database");

    bool result = m_leveldb->batch()
            ->put("d", dataMap)
            ->put("c", dataList)
            ->put("a", "asdf")
            ->write();
    QVERIFY(result);
    QLevelDBReadStream *stream = m_leveldb->readStream();
    QSignalSpy spy(stream, SIGNAL(nextKeyValue(QString,QVariant)));
    stream->start();
    QCOMPARE(spy.count(), 3);
}

QTEST_MAIN(CppTest)
#include "tst_cpp.moc"
