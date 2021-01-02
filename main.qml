import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3
import DirectoryWatcher 1.0

Window {
    id: mainWindow
    visible: true
    width: 390
    height: 555
    color: "#c7c0c0"
    title: qsTr("Directory Watcher")

    DirectoryWatcher {
        id: directoryWatcher
        onFilesListChanged: {
            fileView.model = directoryWatcher.filesList
//            console.log(listView.model)
        }
        onEventsListChanged: {
            eventView.model = directoryWatcher.eventsList
        }
    }

    FileDialog {
        id: fileDialog
        visible: false
        onAccepted: {
            textField.text = fileDialog.folder
        }
    }

    Button {
        id: button
        x: 269
        y: 25
        width: 100
        height: 30
        text: qsTr("Browse")
        onClicked: {
            fileDialog.selectFolder = true
            fileDialog.visible = true
        }
    }

    Rectangle {
        id: rectangle
        x: 20
        y: 24
        width: 220
        height: 31
        color: "#ffffff"
        border.width: 2

        TextField {
            id: textField
            x: 0
            y: 0
            width: 220
            height: 30
            text: qsTr("Select directory")
        }
    }

    Rectangle {
        id: rectangle1
        x: 20
        y: 68
        width: 349
        height: 200
        color: "#ffffff"

        ListView {
            id: fileView
            x: 0
            y: 0
            width: 350
            height: 200
            flickableDirection: Flickable.VerticalFlick
            boundsBehavior: Flickable.StopAtBounds
            clip: true
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: directoryWatcher.filesList
            delegate: Rectangle{
                width: 349
                height: 20
                Text {text: modelData }
            }
            ScrollBar.vertical: ScrollBar {}
        }
    }

    Button {
        id: button1
        x: 145
        y: 280
        text: qsTr("Start")
        checkable: true
        autoExclusive: false
        checked: false
        onClicked: {
            !checked? directoryWatcher.stopMonitoring() : directoryWatcher.setDirectoryPath(textField.text);
            !checked? text = qsTr("Start") : text = qsTr("Stop");
        }
    }

    Rectangle {
        id: rectangle2
        x: 20
        y: 332
        width: 349
        height: 200
        color: "#ffffff"
        ListView {
            id: eventView
            parent: rectangle2
            anchors.fill: parent
            x: 0
            y: 0
            width: 500
            height: 200
            flickableDirection: Flickable.VerticalFlick
            boundsBehavior: Flickable.StopAtBounds
            clip: true
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: directoryWatcher.eventsList
            delegate: Rectangle {
                width: 349
                height: 20
                Text {
                    text: modelData
                }
            }
            ScrollBar.vertical: ScrollBar {}
        }
    }

}

/*##^##
Designer {
    D{i:0;formeditorZoom:1.25}
}
##^##*/
