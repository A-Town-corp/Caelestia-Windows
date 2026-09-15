import QtQuick
import QtQuick.Controls

Rectangle {
    color: "#243343"
    property int clicks: 0
    Column {
        anchors.centerIn: parent
        spacing: 20
        Text {
            text: "Separate-process input test"
            color: "white"
            font.pixelSize: 22
        }
        Button {
            text: "Clicks received: " + parent.parent.clicks
            width: 260
            height: 60
            onClicked: {
                parent.parent.clicks++;
                console.log("INPUT_TARGET_CLICK", parent.parent.clicks);
            }
        }
    }
}
