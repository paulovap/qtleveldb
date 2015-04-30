import QtQuick 2.3
import QtTest 1.0
import QtLevelDB 1.0

TestCase {
    name: "LevelDB"
    property url source: Qt.resolvedUrl("test.db")
    function init() {
        db.source = source
    }

    function cleanupTestCase() {
        db.destroyDB(source)
    }

    function test_create() {
        compare(db.opened, true, db.statusText)
        db.source = Qt.resolvedUrl("")
        compare(db.opened, false, db.statusText)
    }

    function test_destroy() {
        db.source = Qt.resolvedUrl("test2.db")
        compare(db.opened, true, db.statusText)
        db.source = Qt.resolvedUrl("")
        compare(db.opened, false, db.statusText)
        compare(db.destroyDB(Qt.resolvedUrl("test2.db")), true)
    }

// TODO: add this test, but remove "lost" folder at the end
//    function test_repair() {
//        compare(db.repairDB(source), LevelDB.Ok)
//    }

    function test_get_default(){
        compare(db.opened, true, db.statusText)
        compare(db.get("invalidKey", [{"id": "a"}]), [{"id": "a"}])
    }

    function test_put_get(){
        compare(db.opened, true, db.statusText)
        compare(db.putSync("asdf", "asdf"), true, "should be ok")
        compare(db.get("asdf"), "asdf")
    }

    function test_delete(){
        compare(db.opened, true, db.statusText)
        compare(db.put("asdf", "asdf"), true)
        compare(db.del("asdf"), true)
        compare(db.get("asdf"), undefined)
    }

    function test_batch(){
        compare(db.opened, true, db.statusText)
        var status = db.batch()
                        .put("asdf","asdf")
                        .put("string", "asdf2")
                        .put("list", ["asdf"])
                        .put("object", {"a": "b"})
                        .del("asdf")
                        .write()
        compare(status, LevelDB.Ok, "Batch Operation Failed")
        compare(db.get("asdf"), undefined)
        compare(db.get("string"),"asdf2", "Wrong data")
        compare(db.get("list"),["asdf"], "Wrong data")
        compare(db.get("object"),{"a": "b"}, "Wrong data")
    }
    LevelDB{
        id:db
        options{
            compressionType: Options.SnappyCompression
            createIfMissing: true
            errorIfExists: false
            paranoidChecks: false
        }
    }
}

