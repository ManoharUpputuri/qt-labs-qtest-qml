/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 1.0
import QtQuickTest 1.0

Rectangle {
    width: 50; height: 50
    id: top
    focus: true

    property bool leftKeyPressed: false
    property bool leftKeyReleased: false

    Keys.onLeftPressed: {
        leftKeyPressed = true
    }

    Keys.onReleased: {
        if (event.key == Qt.Key_Left)
            leftKeyReleased = true
    }

    property bool mouseHasBeenClicked: false

    MouseArea {
        anchors.fill: parent
        onClicked: {
            mouseHasBeenClicked = true
        }
    }

    TestCase {
        name: "Events"
        when: windowShown       // Must have this line for events to work.

        function test_key_click() {
            keyClick(Qt.Key_Left)
            tryCompare(top, "leftKeyPressed", true)
            tryCompare(top, "leftKeyReleased", true)
        }

        function test_mouse_click() {
            mouseClick(top, 25, 30)
            tryCompare(top, "mouseHasBeenClicked", true)
        }
    }
}