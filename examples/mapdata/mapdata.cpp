/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the documentation of QtDataVis3D module.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "mapdata.h"
#include <QImage>
#include <QFile>

using namespace QtDataVis3D;

MapsModifier::MapsModifier(Q3DMaps *maps)
    : m_chart(maps),
      m_imageRect(QRect()),
      m_fontSize(20.0f),
      m_barSpecs(QVector3D(30.0f, 30.0f, 30.0f))
{
    m_chart->setBarSpecs(m_barSpecs, Q3DMaps::AdjustHeight);
    QImage image = QImage(QStringLiteral(":/images/suomi"));
    m_imageRect = image.rect();
    m_chart->setAreaSpecs(m_imageRect, image);
    //m_chart->setBarType(Cones, false);
    m_chart->setMeshFileName(QStringLiteral(":/meshes/weirdthing"));
}

MapsModifier::~MapsModifier()
{
    delete m_chart;
}

void MapsModifier::start()
{
    addData();
}

void MapsModifier::addData()
{
    QDataItem *item;
    item = new QDataItem();
    item->setValue(191050);
    item->setLabel("Oulu", true);
    item->setPosition(QPoint(438, 510));
    m_chart->addDataItem(item);

    item = new QDataItem();
    item->setValue(22274);
    item->setLabel("Kemi", true);
    item->setPosition(QPoint(412, 445));
    m_chart->addDataItem(item);

    item = new QDataItem();
    item->setValue(60887);
    item->setLabel("Rovaniemi", true);
    item->setPosition(QPoint(451, 363));
    m_chart->addDataItem(item);

    item = new QDataItem();
    item->setValue(16176);
    item->setLabel("Kuusamo", true);
    item->setPosition(QPoint(587, 419));
    m_chart->addDataItem(item);

    item = new QDataItem();
    item->setValue(3998);
    item->setLabel("Ivalo", true);
    item->setPosition(QPoint(525, 156));
    m_chart->addDataItem(item);

    item = new QDataItem();
    item->setValue(37978);
    item->setLabel("Kajaani", true);
    item->setPosition(QPoint(577, 596));
    m_chart->addDataItem(item);

    item = new QDataItem();
    item->setValue(46809);
    item->setLabel("Kokkola", true);
    item->setPosition(QPoint(333, 624));
    m_chart->addDataItem(item);

    item = new QDataItem();
    item->setValue(105236);
    item->setLabel("Kuopio", true);
    item->setPosition(QPoint(560, 706));
    m_chart->addDataItem(item);

    item = new QDataItem();
    item->setValue(133557);
    item->setLabel("Jyväskylä", true);
    item->setPosition(QPoint(472, 754));
    m_chart->addDataItem(item);

    item = new QDataItem();
    item->setValue(65771);
    item->setLabel("Vaasa", true);
    item->setPosition(QPoint(259, 702));
    m_chart->addDataItem(item);

    item = new QDataItem();
    item->setValue(217603);
    item->setLabel("Tampere", true);
    item->setPosition(QPoint(365, 826));
    m_chart->addDataItem(item);

    item = new QDataItem();
    item->setValue(180350);
    item->setLabel("Turku", true);
    item->setPosition(QPoint(270, 951));
    m_chart->addDataItem(item);

    item = new QDataItem();
    item->setValue(605022);
    item->setLabel("Helsinki", true);
    item->setPosition(QPoint(399, 971));
    m_chart->addDataItem(item);

    item = new QDataItem();
    item->setValue(72400);
    item->setLabel("Lappeenranta", true);
    item->setPosition(QPoint(543, 897));
    m_chart->addDataItem(item);

    item = new QDataItem();
    item->setValue(14754);
    item->setLabel("Tammisaari", true);
    item->setPosition(QPoint(325, 1004));
    m_chart->addDataItem(item);

    item = new QDataItem();
    item->setValue(1879);
    item->setLabel("Enontekiö", true);
    item->setPosition(QPoint(380, 175));
    m_chart->addDataItem(item);

    item = new QDataItem();
    item->setValue(36624);
    item->setLabel("Savonlinna", true);
    item->setPosition(QPoint(587, 812));
    m_chart->addDataItem(item);

    item = new QDataItem();
    item->setValue(54887);
    item->setLabel("Kotka", true);
    item->setPosition(QPoint(497, 948));
    m_chart->addDataItem(item);
}

void MapsModifier::changeStyle()
{
    static int model = 0;
    switch (model) {
    case 0:
        m_chart->setBarType(Cylinders, false);
        break;
    case 1:
        m_chart->setBarType(Cylinders, true);
        break;
    case 2:
        m_chart->setBarType(Cones, false);
        break;
    case 3:
        m_chart->setBarType(Cones, true);
        break;
    case 4:
        m_chart->setBarType(Bars, false);
        break;
    case 5:
        m_chart->setBarType(Bars, true);
        break;
    case 6:
        m_chart->setBarType(Pyramids, false);
        break;
    case 7:
        m_chart->setBarType(Pyramids, true);
        break;
    case 8:
        m_chart->setBarType(Spheres, false);
        break;
    case 9:
        m_chart->setBarType(Spheres, true);
        break;
    }
    model++;
    if (model > 9)
        model = 0;
}

void MapsModifier::changePresetCamera()
{
    static int preset = PresetFrontLow;

    m_chart->setCameraPreset((CameraPreset)preset);

    if (++preset > PresetDirectlyAboveCCW45)
        preset = PresetFrontLow;
}

void MapsModifier::changeTheme()
{
    static int theme = ThemeSystem;

    m_chart->setTheme((ColorTheme)theme);

    if (++theme > ThemeLight)
        theme = ThemeSystem;
}

//void MapsModifier::changeSelectionMode()
//{
//    static int selectionMode = 0;

//    m_chart->setSelectionMode((SelectionMode)selectionMode);

//    if (++selectionMode > (int)ModeZoomColumn)
//        selectionMode = 0;
//}

void MapsModifier::changeTransparency()
{
    static int transparency = TransparencyFromTheme;

    m_chart->setLabelTransparency((LabelTransparency)transparency);

    if (++transparency > TransparencyNoBackground)
        transparency = TransparencyNone;
}

void MapsModifier::changeValueDimension(int dimension)
{
    m_chart->setBarSpecs(m_barSpecs, (Q3DMaps::AdjustmentDirection)dimension);
}

void MapsModifier::changeFont(const QFont &font)
{
    QFont newFont = font;
    newFont.setPointSizeF(m_fontSize);
    qDebug() << newFont << newFont.style();
    m_chart->setFont(newFont);
}

void MapsModifier::changeFontSize(int fontsize)
{
    m_fontSize = fontsize;
    m_chart->setFontSize((GLfloat)m_fontSize);
}

//void MapsModifier::setGridEnabled(int enabled)
//{
//    m_chart->setGridEnabled((bool)enabled);
//}

//void MapsModifier::rotateX(int rotation)
//{
//    m_xRotation = rotation;
//    m_chart->setCameraPosition(m_xRotation, m_yRotation);
//}

//void MapsModifier::rotateY(int rotation)
//{
//    m_yRotation = rotation;
//    m_chart->setCameraPosition(m_xRotation, m_yRotation);
//}

//void MapsModifier::setSpecsX(int barwidth)
//{
//    m_barWidth = (float)barwidth / 100.0f;
//    m_chart->setBarSpecs(QSizeF(m_barWidth, m_barDepth), QSizeF(m_barSpacingX, m_barSpacingZ));
//}

//void MapsModifier::setSpecsZ(int bardepth)
//{
//    m_barDepth = (float)bardepth / 100.0f;
//    m_chart->setBarSpecs(QSizeF(m_barWidth, m_barDepth), QSizeF(m_barSpacingX, m_barSpacingZ));
//}