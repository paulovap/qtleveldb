import QtQuick 2.3
import QtTest 1.0
import QtLevelDB 1.0

TestCase {
    id:testcase
    name: "Settings"
    property url source: Qt.resolvedUrl("test.db")

    function init() {
       // settings.source = source
    }

//    function test_data_conversion(){
//        compare(settings.jsonToVariant(settings.variantToJson(10)), 10)
//        compare(settings.jsonToVariant(settings.variantToJson("asdf")), "asdf")
//        compare(settings.jsonToVariant(settings.variantToJson(10.123123123123123)), 10.123123123123123)
//        compare(settings.jsonToVariant(settings.variantToJson(-10)), -10)
//        compare(settings.jsonToVariant(settings.variantToJson(Qt.point(1,2))), Qt.point(1,2))
//        compare(settings.jsonToVariant(settings.variantToJson([])), [])
//        compare(settings.jsonToVariant(settings.variantToJson([1,2,3, "asdf"])), [1,2,3, "asdf"])
//        compare(settings.jsonToVariant(settings.variantToJson({"asdf" : {}})), {"asdf" : {}})
//        var date = new Date()
//        compare(settings.jsonToVariant(settings.variantToJson(date)), date)
//    }

    function test_settings_alias(){
        settings.source = testcase.source
        dummy.a = 10
        settings.source = ""
        settings.aa = -10
        settings.source = testcase.source
        compare(settings.aa, dummy.a)
    }

    Item{
        id:dummy
        property int a
        property string b
    }

    Settings{
        id:settings
        property alias aa : dummy.a
        property alias bb : dummy.b
    }
}
