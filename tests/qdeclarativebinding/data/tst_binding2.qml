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
import QtTest 1.0

Rectangle {
    id: screen
    width: 320; height: 240
    property string text
    property bool changeColor: false

    Text { id: s1; text: "Hello" }
    Rectangle { id: r1; width: 1; height: 1; color: "yellow" }
    Rectangle { id: r2; width: 1; height: 1; color: "red" }

    Binding { target: screen; property: "text"; value: s1.text }
    Binding { target: screen; property: "color"; value: r1.color }
    Binding { target: screen; property: "color"; value: r2.color; when: screen.changeColor == true }

    TestCase {
        name: "Binding2"

        function test_binding2() {
            compare(screen.color, "#ffff00")    // Yellow
            compare(screen.text, "Hello")

            screen.changeColor = true
            compare(screen.color, "#ff0000")    // Red
        }
    }
}
