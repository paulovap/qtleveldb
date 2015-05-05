# QtLevelDB
Qt/QML wrapper for Google's LevelDB

This plugin provides a way to use LevelDB on QML. LevelDB is a fast key-value storage library written at Google that provides an ordered mapping from string keys to string values.

There is two way to use it: Instancing a **LevelDB** object or **Settings** object.

**LevelDB** type provides an API to access the standard LevelDB API: **put**, **get**, **del**, **batch**.

**Settings** type provides a high level API, automatic storing property changes in the database.

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
