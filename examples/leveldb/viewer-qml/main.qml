import QtQuick 2.3
import QtQuick.Controls 1.2
import QtLevelDB 1.0
import QtQuick.Dialogs 1.2

ApplicationWindow {
    width: 640
    height:480
    title: "LevelDB Viewer"
    visible:true

    menuBar: MenuBar{
        Menu{
            title:"File"
            MenuItem{
                text:"Open Leveldb..."
                onTriggered: fileDialog.open()
            }
            MenuItem{
                text: "Close"
                onTriggered: Qt.quit()
            }
        }
    }

    FileDialog{
        id:fileDialog
        title:"Please Choose LevelDB folder"
        selectFolder: true
        onAccepted: db.source = fileDialog.fileUrls[0]
    }

    LevelDB{
        id:db
        onOpenedChanged: {
            console.log("leveldb opened")
            model.clear()
            db.readStream(function (key, value){
                model.append({"key":key, "value": JSON.stringify(value)})
                return true;
            })
        }
    }

    ListModel {
        id: model
    }
    TableView {
        id:tableView
        anchors.fill: parent
        TableViewColumn {
            role: "key"
            title: "Key"
            width: 200
        }
        TableViewColumn {
            role: "value"
            title: "Value"
            width: 400
        }
        model: model
    }
}

