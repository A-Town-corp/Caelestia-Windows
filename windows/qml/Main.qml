pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls
import QtQuick.Effects
import Caelestia.Config
import Caelestia.Blobs
import M3Shapes

Item {
    id: root
    property bool targetOpen: false
    property bool exclusionTest: false
    property bool exclusionsEnabled: true
    readonly property real borderThickness: Config.border.thickness
    readonly property real barHeight: 60
    readonly property real drawerWidth: 854
    readonly property real drawerHeight: 340
    readonly property real progress: 1 - dashboard.offsetScale
    readonly property real drawerX: (width - drawerWidth) / 2

    // Diagnostic palette only. Reference scheme/fonts remain a separate visual gate.
    readonly property color surface: "#181820"
    readonly property color ink: "#e6e0e9"
    readonly property color secondaryInk: "#cac4d0"
    readonly property color accent: "#d0bcff"

    Item {
        anchors.fill: parent
        layer.enabled: true
        layer.effect: MultiEffect {
            shadowEnabled: true
            blurMax: 15
            shadowColor: Qt.rgba(0, 0, 0, 0.7)
        }
        BlobGroup {
            id: blobGroup
            color: root.surface
            smoothing: root.Config.border.smoothing
        }
        BlobInvertedRect {
            anchors.fill: parent
            anchors.margins: -50
            group: blobGroup
            radius: root.Config.border.rounding
            borderLeft: root.borderThickness + 50
            borderRight: root.borderThickness + 50
            borderTop: root.borderThickness + 50
            borderBottom: root.barHeight + 50
        }
        BlobRect {
            id: dashboardBackground
            group: blobGroup
            x: dashboard.x
            y: dashboard.y
            width: dashboard.width
            height: dashboard.height
            radius: root.Tokens.rounding.extraLarge
            deformScale: (0.1 * root.Config.appearance.deformScale) / 10000
        }
        BlobRect {
            id: exclusionLeft
            group: blobGroup
            visible: root.exclusionTest
            x: 200; y: 500; width: root.exclusionTest ? 100 : 0; height: 100; radius: 28
            exclude: root.exclusionsEnabled ? [exclusionRight] : []
        }
        BlobRect {
            id: exclusionRight
            group: blobGroup
            visible: root.exclusionTest
            x: 310; y: 500; width: root.exclusionTest ? 100 : 0; height: 100; radius: 28
            exclude: root.exclusionsEnabled ? [exclusionLeft] : []
        }
    }

    Item {
        id: dashboard
        property real offsetScale: root.targetOpen ? 0 : 1
        // Geometry, opacity and easing from modules/dashboard/Wrapper.qml.
        x: root.drawerX
        y: root.borderThickness + (-height - 5) * offsetScale
        width: root.drawerWidth
        height: root.drawerHeight
        visible: offsetScale < 1
        opacity: 1 - offsetScale
        transform: Matrix4x4 { matrix: dashboardBackground.deformMatrix }
        Behavior on offsetScale { Anim {} }

        Column {
            x: 28; y: 28; spacing: 16
            Text {
                text: "Caelestia · renderer / input test"
                color: root.ink
                font.family: "Segoe UI"; font.pixelSize: 24
            }
            Text {
                text: "Prototype content — full dashboard, volume and power are not implemented."
                color: root.secondaryInk
                font.family: "Segoe UI"; font.pixelSize: 16
            }
            Text {
                text: "Move away to retract. Hover the top centre to reopen.\nEscape dismisses. Ctrl+Alt+Q exits the desktop overlay."
                color: root.ink
                font.family: "Segoe UI"; font.pixelSize: 16
                lineHeight: 1.4
            }
            Text {
                text: testMode
                color: root.accent
                font.family: "Segoe UI"; font.pixelSize: 14
            }
            Button {
                text: "Close test"
                onClicked: Qt.quit()
            }
        }
        MaterialShape {
            anchors.right: parent.right; anchors.bottom: parent.bottom
            anchors.margins: 28
            width: 72; height: 72
            color: root.accent
            shape: MaterialShape.Flower
        }
    }
    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        y: root.height - root.barHeight / 2 - height / 2
        text: "TEST"
        color: root.accent
        font.family: "Segoe UI"; font.pixelSize: 10
    }
}
