/****************************************************************************
**
** Copyright (C) 2013 Digia Plc
** All rights reserved.
** For any questions to Digia, please use contact form at http://qt.digia.com
**
** This file is part of the QtDataVis3D module.
**
** $QT_BEGIN_LICENSE$
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** contact form at http://qt.digia.com
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.1
import QtQuick.Controls 1.0
import com.digia.QtDataVis3D 1.0

Item {
    id: mainview
    width: 800
    height: 500
    visible: true

    Item {
        id: dataView
        width: parent.width - shadowToggle.width
        height: parent.height
        anchors.right: parent.right;

        Image {
            id: testimage
            source: "qrc:/images/image.jpg"
        }

        DataItem {
            id: testitem1
            label: "Test"
            value: 200
            position: "100.0, 200.0"
        }
        DataItem {
            id: testitem2
            label: "Test2"
            value: 300
            position: "200.0, 200.0"
        }
        DataItem {
            id: testitem3
            label: "Test3"
            value: 400
            position: "100.0, 100.0"
        }
        DataItem {
            id: testitem4
            label: "Test4"
            value: 600
            position: "200.0, 100.0"
        }

        Maps3D {
            id: testmap
            width: dataView.width
            height: dataView.height

            Component.onCompleted: {
                console.log("testmap complete");
                console.log(testimage);
                console.log(testimage.sourceSize);
                setAreaSpecs(Qt.rect(0, 0, testimage.sourceSize.width, testimage.sourceSize.height),
                             testimage);
                //setImage(testimage);
                setImage(":/images/image.jpg");
                shadowQuality = Maps3D.ShadowNone
                selectionMode = Maps3D.ModeBar
                labelTransparency = Maps3D.TransparencyFromTheme
                addDataItem(testitem1);
                addDataItem(testitem2);
                addDataItem(testitem3);
                addDataItem(testitem4);
            }
        }
    }

    Component.onCompleted: {
        console.log("mainview complete");
    }

    Rectangle {
        id: shadowToggle
        color: "#FFFFFF"
        x: 0
        y: 0
        width: 160
        height: 80

        TextArea {
            id: buttonText
            text: "Toggle Shadows"
            anchors.fill: parent
            textColor: "#000000"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                if (testmap.shadowQuality === Maps3D.ShadowNone) {
                    testmap.shadowQuality = Maps3D.ShadowLow;
                    buttonText.textColor = "#FFFFFF";
                    shadowToggle.color = "#000000";
                } else {
                    testmap.shadowQuality = Maps3D.ShadowNone;
                    buttonText.textColor = "#000000";
                    shadowToggle.color = "#FFFFFF";
                }
            }
        }
    }
}