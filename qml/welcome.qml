import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0

Window {
    readonly property real shadowOffset: 5
    readonly property real shadowRadius: 4
    readonly property real shadowSamples: 16

    id: window1
    width: 800
    height: 600
    visible: true
    title: "DERPY's wardrobe"

    minimumHeight: height
    maximumHeight: height
    minimumWidth: width
    maximumWidth: width

    Component {
        id: shadow
        DropShadow {
            horizontalOffset: window1.shadowOffset
            verticalOffset: window1.shadowOffset
            radius: window1.shadowRadius
            samples: window1.shadowSamples
            color: "#80000000"
        }
    }

    Component {
        id: scrollingShader

        ShaderEffect {
            id: scrollingShaderPrivate
            property real offset: 0
            fragmentShader: "
                            uniform lowp sampler2D source; // this item
                            uniform lowp float qt_Opacity; // inherited opacity of this item
                            uniform highp float offset;
                            varying highp vec2 qt_TexCoord0;
                            void main() {
                                lowp vec4 p = texture2D(source, (qt_TexCoord0 + vec2(offset, 0.0)) * 6.0);
                                lowp float g = dot(p.xyz, vec3(0.344, 0.5, 0.156));
                                gl_FragColor = p * qt_Opacity;
                            }"

            NumberAnimation {
                target: scrollingShaderPrivate
                property: "offset"
                duration: 30000
                from: 0
                to: 1
                loops: Animation.Infinite
                running: true
            }
        }
    }

    Image {
        id: background
        visible: true
        //        sourceSize.height: 256
        //        sourceSize.width: 128
        //        fillMode: Image.Tile
        anchors.fill: parent
        source: "Mlp_resource_muffin_by_zutheskunk-d4co3wb.png"

        layer.enabled: true
        layer.effect: scrollingShader
        layer.wrapMode: ShaderEffectSource.Repeat
    }

    Text {
        id: text1
        y: 8
        text: "Choose the outfit for your DERPY"
        styleColor: "#8000e95a"
        anchors.horizontalCenter: outfitsList.horizontalCenter
        font.pointSize: 26
        layer.enabled: true
        layer.effect: shadow
    }

    Rectangle {
        id: rectangle1
        color: "#beaf33"
        //        z: -1
        anchors.fill: outfitsList
                    opacity: 0.5
        //            visible: false
        layer.enabled: true
        layer.effect: DropShadow {
            horizontalOffset: window1.shadowOffset
            verticalOffset: window1.shadowOffset
            radius: window1.shadowRadius
            samples: window1.shadowSamples
            color: "#80000000"
            transparentBorder: true
            source: rectangle1
        }
    }

    ListView {
        id: outfitsList
        clip: true
        snapMode: ListView.SnapToItem
        preferredHighlightBegin: 0
        preferredHighlightEnd: height
        highlightRangeMode: ListView.StrictlyEnforceRange
        //            highlight: Rectangle {color: "gold"; opacity: 0.5}

        anchors.top: text1.bottom
        anchors.bottom: image3.top
        anchors.bottomMargin: 8
        anchors.rightMargin: 24
        anchors.leftMargin: 24
        anchors.left: parent.left
        anchors.right: parent.right

        layer.enabled: true
        layer.effect: shadow

        //            contentY: scrollbar.y
        //            interactive: false
        //            rebound: Transition{NumberAnimation{duration: 2000}}
        //            maximumFlickVelocity: 10

        //            MouseArea {
        //                anchors.fill: parent

        //                onWheel: {
        //                    wheel.accepted = true
        //                    outfitsList.contentY += wheel.angleDelta.y
        //                    if(outfitsList.contentY < 0) outfitsList.contentY = 0
        //                }
        //            }

        onCurrentIndexChanged: {
            console.log("LOL")
        }

        model: ListModel {

//            ListElement {
//                name: "Test"
//                preview: "Derpy-Hooves-MLP-FiM-image-derpy-hooves-mlp-fim-36662239-900-506.jpg"
//                url: ""
//            }

//            ListElement {
//                name: "Test2"
//                preview: "derpy_hooves_background_by_ikillyou121-d4gm3qj.jpeg"
//                url: ""
//            }

//            ListElement {
//                name: "Test"
//                preview: "Derpy-Hooves-MLP-FiM-image-derpy-hooves-mlp-fim-36662239-900-506.jpg"
//                url: ""
//            }

//            ListElement {
//                name: "Test2"
//                preview: "derpy_hooves_background_by_ikillyou121-d4gm3qj.jpeg"
//                url: ""
//            }

//            ListElement {
//                name: "Test"
//                preview: "Derpy-Hooves-MLP-FiM-image-derpy-hooves-mlp-fim-36662239-900-506.jpg"
//                url: ""
//            }

//            ListElement {
//                name: "Test2"
//                preview: "derpy_hooves_background_by_ikillyou121-d4gm3qj.jpeg"
//                url: ""
//            }
        }

//        delegate: OutfitDelegate {
//            foldedWidth: outfitsList.width
//            foldedHeight: 100
//            text: name
//            image: preview

//            Connections {
//                target: outfitsList
//                onCurrentIndexChanged: {
//                    //                        console.log(outfitsList.currentIndex)
//                    if(!outfitsList.isCurrentItem) state = ""
//                }
//            }
//        }

        Text {
            anchors.centerIn: parent
            text: "Sorry, nothing to see here yet.\n There will be an online outfits browser soon."
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 26
        }
    }

//    Rectangle {
//        id: scrollbar
//        anchors.left: outfitsList.right
//        y: outfitsList.visibleArea.yPosition * outfitsList.height
//        width: 10
//        height: outfitsList.visibleArea.heightRatio * outfitsList.height
//        color: "black"

//        //                onYChanged: {
//        //                    console.log(outfitsList.visibleArea.yPosition)
//        //                }

//        //                MouseArea {
//        //                    anchors.fill: parent
//        //                    drag.target: scrollbar
//        //                    drag.axis: Drag.YAxis
//        //                    drag.minimumY: outfitsList.y
//        //                    drag.maximumY: outfitsList.height - scrollbar.height + outfitsList.y
//        //                }
//    }

    Item {
        id: textBaloon
        x: 287
        y: 531
        width: text3.width + 20
        height: text3.height + 20
        layer.enabled: true
        layer.effect: shadow

        opacity: 0.8

        Rectangle {
            id: rectangle2
            radius: 13
            anchors.rightMargin: 13
            anchors.fill: parent
            border.width: 2

            Text {
                id: text3
                text: "Click me to load your own outfit"
                wrapMode: Text.WordWrap
                visible: true
                font.pointSize: 20
                anchors.centerIn: parent
            }

            Item {
                id: item1
                anchors.verticalCenter: parent.verticalCenter
                width: 15
                height: 27
                clip: true
                anchors.rightMargin: -13
                anchors.right: parent.right

                Rectangle {
                    id: rectangle3
                    x: -10
                    y: 4
                    width: 20
                    height: 20
                    rotation: 45
                    border.width: 2
                }
            }
        }
    }

    Image {
        id: image3
        x: 670
        y: 476
        width: 122
        height: 116
        fillMode: Image.PreserveAspectFit
        source: "QcDm3.png"
        layer.enabled: true
        layer.effect: shadow

        MouseArea {
            anchors.fill: parent

            onClicked: {
                fileDialog.open()
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: "Please choose an outfit"
        nameFilters: [ "QML files (*.qml)" ]

        onAccepted: {
            console.log("You chose: " + fileDialog.fileUrls)
            outfitLoader.source = fileDialog.fileUrls[0]
        }
        onRejected: {
            console.log("Canceled")
        }
    }

    Loader {
        id: outfitLoader

        onLoaded: {
            item.show()
            window1.hide()
        }
    }


}

