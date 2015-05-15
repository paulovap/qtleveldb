import QtQuick 2.3
import QtTest 1.0
import QtLevelDB 1.0

//TODO: tests with multiple instances of LevelDB
TestCase {
    id:testcase
    name: "Settings"
    property url source: Qt.resolvedUrl("settings.db")

    Item{
        id:dummy
        property int a
        property string b
    }

    Settings{
        id:settings
        source:testcase.source
        property alias aa : dummy.a
        property alias bb : dummy.b
    }

    Settings{
        id:settings2
        source:testcase.source
        property variant aa
    }

    SignalSpy{
        id:spy
        target:settings2
        signalName:"aaChanged"
    }
    SignalSpy{
        id:spy2
        target:settings2
        signalName:"keyValueChanged"
    }

    function init(){
        settings.source = testcase.source
        settings2.source = testcase.source
    }

    function test_settings_alias(){

        compare(settings.opened, true, settings.statusText)
        dummy.a = 10
        settings.source = ""
        compare(settings.opened, false, "should not be open, invalid path")
        settings.aa = -10
        settings.source = testcase.source
        compare(settings.opened, true, settings.statusText)
        compare(settings.aa, dummy.a)
    }
    function test_emit_property_notification(){
        compare(settings.opened, true, settings.statusText)
        compare(settings2.opened, true, settings.statusText)
        spy.clear()
        spy2.clear()
        dummy.a = 1234
        spy.wait(400)
        compare(spy.count, 1)
        compare(spy2.signalArguments[0], {"0":"aa", "1":1234})
    }
}
