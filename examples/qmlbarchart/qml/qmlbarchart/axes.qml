/****************************************************************************
**
** Copyright (C) 2013 Digia Plc
** All rights reserved.
** For any questions to Digia, please use contact form at http://qt.digia.com
**
** This file is part of the QtDataVis3D module.
**
** Licensees holding valid Qt Enterprise licenses may use this file in
** accordance with the Qt Enterprise License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.
**
** If you have questions regarding the use of this file, please use
** contact form at http://qt.digia.com
**
****************************************************************************/

import QtQuick 2.1
import com.digia.QtDataVis3D 1.0

Item {
    property alias row: rowAxis
    property alias column: columnAxis
    property alias expenses: expensesAxis
    property alias income: incomeAxis

    CategoryAxis {
        id: rowAxis
        categoryLabels: ["2000", "2001", "2002", "2003"]
    }
    CategoryAxis {
        id: columnAxis
        categoryLabels: ["January", "February", "March", "April", "May", "June",
            "July", "August", "September", "October", "November", "December"]
    }
    ValueAxis {
        id: incomeAxis
        min: 0
        max: 35
        labelFormat: "%.2f M\u20AC"
        title: "Monthly income"
    }
    ValueAxis {
        id: expensesAxis
        min: 0
        max: 35
        labelFormat: "-%.2f M\u20AC"
        title: "Monthly expenses"
    }
}
