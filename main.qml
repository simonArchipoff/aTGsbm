import QtQuick
import CustomItems


Window {
    visible: true
    width: 640
    height: 480
    title: "Audio Buffer Viewer"

    /*ListView {
        anchors.fill: parent
        model: listModel
        delegate: Rectangle {
            width: parent ? parent.width : 0
            height: 20
            color: "white"
            Text {
                anchors.centerIn: parent
                text: model.value
            }
        }
    }*/

    Chronogram {
        id:chronograminstance
        anchors.fill: parent  // L'image remplit toute la fenêtre
        Connections {
            target: signalprocess
            function onDecimatedEnvelop(f){
             chronograminstance.addNewData(f);
            }
            function onDecimatedSampleRate(sr){
                chronograminstance.setSampleRate(sr);
            }
        }
    }


 }
