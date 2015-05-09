# QtLevelDB
Qt/QML wrapper for Google's LevelDB

This plugin provides a way to use LevelDB on QML. LevelDB is a fast key-value storage library written at Google that provides an ordered mapping from string keys to string values.

There is two way to use it: Instancing a **LevelDB** object or **Settings** object.

**LevelDB** type provides an API to access the standard LevelDB API: **put**, **get**, **del**, **batch** and **readStream**.

**Settings** type provides a high level API, automatic storing property changes in the database.

#### Settings impovements over Qt.labs.Settings's Settings:

Added a **source** property. So it is possible to create settings in different databases:

```qml
Settings{
    id:windowSettings
    source: "/home/test/.cache/Appname/window_settings.db"
}

Settings{
    id:userSettings
    source: "/home/test/.cache/Appname/user_settings.db"
}
```

Properties are synchronized between instances. So updating one property in a instance will trigger and update in all other instances that have the same source

```qml
Settings{
    id:windowSettings
    property int width
    source: "/home/test/.cache/Appname/window_settings.db"
    Component.onCompleted: width = 200
}

Settings{
    id:windowSettings2
    property int width
    source: "/home/test/.cache/Appname/window_settings.db"
    onWidthChanged: console.log("width changed to: " + width)
}
```
**OBS:** For a property to be updated it need to be declared.

API for receive property change signals when you don't want to declare all the properties:

```qml
Settings{
    source: "/home/test/.cache/Appname/window_settings.db"
    onPropertyChanged: console.log("property:" + key + changed to: " + value)
}
```

Read only properties are ignored. Good for using constants:

```qml
Settings{
    readonly property real pi = 3.14 //this will not be save on disk or shared between instances
}
```

Probably more efficient too, since leveldb has many cache layers and Qt's implentation writes on ini files.

## Usage Example:
Here is a couple of examples.
### Using LevelDB type:
```qml
import QtQuick 2.2
import QtLevelDB 1.0

ApplicationWindow {
    id: window
    width: 400
    height: 250

    LevelDB {
        id: db
        source: "local.db"
        Component.onCompleted: {
          window.width = db.get("width", 400)
          window.height = db.get("height", 250)
        }
    }
    onWidthChanged: db.put("width", width)
    onHeightChanged: db.put("height", height)
}
```

### Using Settings type
```qml
import QtQuick 2.2
import QtLevelDB 1.0

ApplicationWindow {
    id: window
    width: 400
    height: 250

    Settings {
        source: "local.db"
        property alias width: window.width
        property alias height: window.height
    }
}
```
## Instalation:
On Unix just call theses commands from the terminal:
```bash
qmake
make
make install
```

On Windows is very similar, just make sure you have Qt and QtCreator binaries in your path enviroment variable:
```bash
qmake
jom
jom install
```

Feel Free to Contribute
