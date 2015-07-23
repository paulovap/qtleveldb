#include <QtTest/QtTest>

#include <qleveldb.h>
#include <qleveldbbatch.h>

#include <QObject>
#include <QDir>
#include <QDebug>
#include <QVariant>
#include <QVariantList>
#include <QVariantMap>
#include <QtConcurrent>
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
    void test_readStreamConcurrent();
    void test_readStreamClosedDB();
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
    m_leveldb->close();
    m_leveldb->destroyDB("test.db");
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
            ->put("/comics/", dataMap)
            ->put("/comics/~", dataList)
            ->put("/collection/", "asdf")
            ->put("/comics/abc", "asdf")
            ->put("/collection/~", "asdf")
            ->put("/comics/a", "asdf")
            ->put("/comics/AA", "asdf")
            ->write();
    QVERIFY(result);
    int count = 0;

    std::function<bool(QString key, QVariant value)> func1 = [&count] (QString key, QVariant value) {
        Q_UNUSED(key);
        Q_UNUSED(value);
        count++;
        return true;
    };

    m_leveldb->readStream(func1);
    QCOMPARE(count, 7);

    QStringList sequenceKeys, toCompareKeys;
    sequenceKeys << "/comics/" << "/comics/AA" << "/comics/a" << "/comics/abc" << "/comics/~";
    m_leveldb->readStream(
                [&toCompareKeys](QString key, QVariant value){
        toCompareKeys << key;
        return true;
    }, "/comics/");
    QCOMPARE(sequenceKeys, toCompareKeys);
}

void CppTest::test_readStreamConcurrent()
{
    QVERIFY2(m_leveldb->opened(), "unable to open database");
    QList<int> list;
    list << 1 << 2 << 3 << 4 << 5 << 6 << 7 << 8;
    QString key("key");
    for(int i=0; i <1000; i++) {
        m_leveldb->putSync(key+i, key +i);
    }
    QFuture<void> future = QtConcurrent::map(list, [this](int i) {
        if (i%2 == 0) {
            m_leveldb->readStream([this, i] (QString key, QVariant value) -> bool {
                return true;
            });
        } else {
            for(int j=0;j<10000; j++){
                m_leveldb->put(QString("keke") + j, QString("keke") + j);
            }
        }
    });
    future.waitForFinished();
}

void CppTest::test_readStreamClosedDB() {
     QVERIFY2(m_leveldb->opened(), "unable to open database");
     m_leveldb->close();
     QVERIFY2(!m_leveldb->opened(), "unable to open database");
     m_leveldb->readStream([](QString key, QVariant value) -> bool {
         qDebug() << key;
         return true;
     });
}

QTEST_MAIN(CppTest)
#include "tst_cpp.moc"
