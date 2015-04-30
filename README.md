# QtLevelDB
Qt/QML wrapper for Google's LevelDB

## Usage Example:
Here is a coulple of examples.
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
        source: Qt.resolveUrl("local.db")
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
        source: Qt.resolveUrl("local.db")
        property alias width: window.width
        property alias height: window.height
    }
}
```
## Instalation:
```bash
qmake
make
make install
```

Feel Free to Contribute
