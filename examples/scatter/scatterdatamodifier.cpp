/****************************************************************************
**
** Copyright (C) 2013 Digia Plc
** All rights reserved.
** For any questions to Digia, please use contact form at http://qt.digia.com
**
** This file is part of the QtDataVisualization module.
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

#include "scatterdatamodifier.h"
#include <QtDataVisualization/qscatterdataproxy.h>
#include <QtDataVisualization/q3dvalueaxis.h>
#include <QtDataVisualization/q3dscene.h>
#include <QtDataVisualization/q3dcamera.h>
#include <qmath.h>
using namespace QtDataVisualization;

//#define RANDOM_SCATTER // Uncomment this to switch to random scatter

const int numberOfItems = 10000;

ScatterDataModifier::ScatterDataModifier(Q3DScatter *scatter)
    : m_graph(scatter),
      m_fontSize(40.0f),
      m_style(QDataVis::MeshStyleSpheres),
      m_smooth(true)
{
    //! [0]
    QFont font = m_graph->font();
    font.setPointSize(m_fontSize);
    m_graph->setFont(font);
    m_graph->setObjectType(QDataVis::MeshStyleSpheres, true);
    m_graph->setTheme(QDataVis::ThemeEbony);
    m_graph->setShadowQuality(QDataVis::ShadowQualityHigh);
    m_graph->scene()->activeCamera()->setCameraPreset(QDataVis::CameraPresetFront);
    //! [0]

    //! [1]
    m_graph->setAxisX(new Q3DValueAxis);
    m_graph->setAxisY(new Q3DValueAxis);
    m_graph->setAxisZ(new Q3DValueAxis);
    //! [1]

    //! [2]
    QScatterDataProxy *proxy = new QScatterDataProxy;
    proxy->setItemLabelFormat("@xTitle: @xLabel @yTitle: @yLabel @zTitle: @zLabel");
    m_graph->setActiveDataProxy(proxy);
    //! [2]

    changeLabelStyle();
}

ScatterDataModifier::~ScatterDataModifier()
{
    delete m_graph;
}

//! [3]
void ScatterDataModifier::start()
{
    addData();
}
//! [3]

void ScatterDataModifier::addData()
{
    //! [4]
    // Add labels
    m_graph->axisX()->setTitle("X");
    m_graph->axisY()->setTitle("Y");
    m_graph->axisZ()->setTitle("Z");
    m_graph->axisX()->setRange(-50.0, 50.0);
    m_graph->axisY()->setRange(-1.0, 1.0);
    m_graph->axisZ()->setRange(-50.0, 50.0);
    //! [4]

    //! [5]
    QScatterDataArray *dataArray = new QScatterDataArray;
    dataArray->resize(numberOfItems);
    QScatterDataItem *ptrToDataArray = &dataArray->first();
    //! [5]

#ifdef RANDOM_SCATTER
    for (int i = 0; i < numberOfItems; i++) {
        ptrToDataArray->setPosition(randVector());
        ptrToDataArray++;
    }
#else
    //! [6]
    float limit = qSqrt(numberOfItems) / 2.0f;
    for (float i = -limit; i < limit; i++) {
        for (float j = -limit; j < limit; j++) {
            ptrToDataArray->setPosition(QVector3D(i, qCos(qDegreesToRadians((i * j) / 7.5)), j));
            ptrToDataArray++;
        }
    }
    //! [6]
#endif

    //! [7]
    m_graph->activeDataProxy()->resetArray(dataArray);
    //! [7]
}

//! [8]
void ScatterDataModifier::changeStyle(int style)
{
    m_style = QDataVis::MeshStyle(style + 5); // skip unsupported mesh types
    m_graph->setObjectType(m_style, m_smooth);
}

void ScatterDataModifier::setSmoothDots(int smooth)
{
    m_smooth = bool(smooth);
    m_graph->setObjectType(m_style, m_smooth);
}

void ScatterDataModifier::changeTheme(int theme)
{
    m_graph->setTheme((QDataVis::Theme)theme);
}

void ScatterDataModifier::changePresetCamera()
{
    static int preset = QDataVis::CameraPresetFrontLow;

    m_graph->scene()->activeCamera()->setCameraPreset((QDataVis::CameraPreset)preset);

    if (++preset > QDataVis::CameraPresetDirectlyBelow)
        preset = QDataVis::CameraPresetFrontLow;
}

void ScatterDataModifier::changeLabelStyle()
{
    static int style = QDataVis::LabelStyleFromTheme;

    m_graph->setLabelStyle((QDataVis::LabelStyle)style);

    if (++style > QDataVis::LabelStyleTransparent)
        style = QDataVis::LabelStyleOpaque;
}

void ScatterDataModifier::changeFont(const QFont &font)
{
    QFont newFont = font;
    newFont.setPointSizeF(m_fontSize);
    m_graph->setFont(newFont);
}

void ScatterDataModifier::shadowQualityUpdatedByVisual(QDataVis::ShadowQuality sq)
{
    int quality = int(sq);
    emit shadowQualityChanged(quality); // connected to a checkbox in main.cpp
}

void ScatterDataModifier::changeShadowQuality(int quality)
{
    QDataVis::ShadowQuality sq = QDataVis::ShadowQuality(quality);
    m_graph->setShadowQuality(sq);
}

void ScatterDataModifier::setBackgroundEnabled(int enabled)
{
    m_graph->setBackgroundVisible((bool)enabled);
}

void ScatterDataModifier::setGridEnabled(int enabled)
{
    m_graph->setGridVisible((bool)enabled);
}
//! [8]

QVector3D ScatterDataModifier::randVector()
{
    return QVector3D(
                (float)(rand() % 100) / 2.0f - (float)(rand() % 100) / 2.0f,
                (float)(rand() % 100) / 100.0f - (float)(rand() % 100) / 100.0f,
                (float)(rand() % 100) / 2.0f - (float)(rand() % 100) / 2.0f);
}