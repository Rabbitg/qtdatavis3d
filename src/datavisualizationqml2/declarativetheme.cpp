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

#include "declarativetheme_p.h"

QT_DATAVISUALIZATION_BEGIN_NAMESPACE

DeclarativeTheme3D::DeclarativeTheme3D(QObject *parent)
    : Q3DTheme(parent),
      m_colors(QList<DeclarativeColor *>()),
      m_gradients(QList<ColorGradient *>()),
      m_singleHLGradient(0),
      m_multiHLGradient(0),
      m_dummyGradients(false),
      m_dummyColors(false)
{
}

DeclarativeTheme3D::~DeclarativeTheme3D()
{
}

QQmlListProperty<QObject> DeclarativeTheme3D::seriesChildren()
{
    return QQmlListProperty<QObject>(this, this, &DeclarativeTheme3D::appendSeriesChildren,
                                     0, 0, 0);
}

void DeclarativeTheme3D::appendSeriesChildren(QQmlListProperty<QObject> *list, QObject *element)
{
    Q_UNUSED(list)
    Q_UNUSED(element)
    // Nothing to do, seriesChildren is there only to enable scoping gradient items in Theme3D item.
}

void DeclarativeTheme3D::handleBaseColorUpdate()
{
    int colorCount = m_colors.size();
    int changed = 0;
    // Check which one changed
    DeclarativeColor *color = qobject_cast<DeclarativeColor *>(QObject::sender());
    for (int i = 0; i < colorCount; i++) {
        if (color == m_colors.at(i)) {
            changed = i;
            break;
        }
    }
    // Update the changed one from the list
    QList<QColor> list = Q3DTheme::baseColors();
    list[changed] = m_colors.at(changed)->color();
    // Set the changed list
    Q3DTheme::setBaseColors(list);
}

void DeclarativeTheme3D::handleBaseGradientUpdate()
{
    int gradientCount = m_gradients.size();
    int changed = 0;
    // Check which one changed
    ColorGradient *gradient = qobject_cast<ColorGradient *>(QObject::sender());
    for (int i = 0; i < gradientCount; i++) {
        if (gradient == m_gradients.at(i)) {
            changed = i;
            break;
        }
    }
    // Update the changed one from the list
    QList<QLinearGradient> list = Q3DTheme::baseGradients();
    list[changed] = convertGradient(gradient);
    // Set the changed list
    Q3DTheme::setBaseGradients(list);
}

void DeclarativeTheme3D::handleSingleHLGradientUpdate()
{
    if (m_singleHLGradient)
        setThemeGradient(m_singleHLGradient, GradientTypeSingleHL);
}

void DeclarativeTheme3D::handleMultiHLGradientUpdate()
{
    if (m_multiHLGradient)
        setThemeGradient(m_multiHLGradient, GradientTypeMultiHL);
}

void DeclarativeTheme3D::setSingleHighlightGradient(ColorGradient *gradient)
{
    // connect new / disconnect old
    if (gradient != m_singleHLGradient) {
        if (m_singleHLGradient)
            QObject::disconnect(m_singleHLGradient, 0, this, 0);

        m_singleHLGradient = gradient;

        if (m_singleHLGradient) {
            QObject::connect(m_singleHLGradient, &ColorGradient::updated, this,
                             &DeclarativeTheme3D::handleSingleHLGradientUpdate);
        }

        emit singleHighlightGradientChanged(m_singleHLGradient);
    }

    if (m_singleHLGradient)
        setThemeGradient(m_singleHLGradient, GradientTypeSingleHL);
}

ColorGradient *DeclarativeTheme3D::singleHighlightGradient() const
{
    return m_singleHLGradient;
}

void DeclarativeTheme3D::setMultiHighlightGradient(ColorGradient *gradient)
{
    // connect new / disconnect old
    if (gradient != m_multiHLGradient) {
        if (m_multiHLGradient)
            QObject::disconnect(m_multiHLGradient, 0, this, 0);

        m_multiHLGradient = gradient;

        if (m_multiHLGradient) {
            QObject::connect(m_multiHLGradient, &ColorGradient::updated, this,
                             &DeclarativeTheme3D::handleMultiHLGradientUpdate);
        }

        emit multiHighlightGradientChanged(m_multiHLGradient);
    }

    if (m_multiHLGradient)
        setThemeGradient(m_multiHLGradient, GradientTypeMultiHL);
}

ColorGradient *DeclarativeTheme3D::multiHighlightGradient() const
{
    return m_multiHLGradient;
}

void DeclarativeTheme3D::setThemeGradient(ColorGradient *gradient, GradientType type)
{
    QLinearGradient newGradient = convertGradient(gradient);

    switch (type) {
    case GradientTypeSingleHL:
        Q3DTheme::setSingleHighlightGradient(newGradient);
        break;
    case GradientTypeMultiHL:
        break;
        Q3DTheme::setMultiHighlightGradient(newGradient);
    default:
        qWarning("Incorrect usage. Type may be GradientTypeSingleHL or GradientTypeMultiHL.");
        break;
    }
}

QLinearGradient DeclarativeTheme3D::convertGradient(ColorGradient *gradient)
{
    QLinearGradient newGradient;
    QGradientStops stops;
    QList<ColorGradientStop *> qmlstops = gradient->m_stops;

    // Get sorted gradient stops
    for (int i = 0; i < qmlstops.size(); i++) {
        int j = 0;
        while (j < stops.size() && stops.at(j).first < qmlstops[i]->position())
            j++;
        stops.insert(j, QGradientStop(qmlstops.at(i)->position(), qmlstops.at(i)->color()));
    }

    newGradient.setStops(stops);

    return newGradient;
}

ColorGradient *DeclarativeTheme3D::convertGradient(const QLinearGradient &gradient)
{
    ColorGradient *newGradient = new ColorGradient(this);
    QGradientStops stops = gradient.stops();
    ColorGradientStop *qmlstop;

    // Convert stops
    for (int i = 0; i < stops.size(); i++) {
        qmlstop = new ColorGradientStop(newGradient);
        qmlstop->setColor(stops.at(i).second);
        qmlstop->setPosition(stops.at(i).first);
        newGradient->m_stops.append(qmlstop);
    }

    return newGradient;
}

void DeclarativeTheme3D::addColor(DeclarativeColor *color)
{
    clearDummyColors();
    m_colors.append(color);
    connect(color, &DeclarativeColor::colorChanged,
            this, &DeclarativeTheme3D::handleBaseColorUpdate);
    QList<QColor> list = Q3DTheme::baseColors();
    list.append(color->color());
    Q3DTheme::setBaseColors(list);
}

QList<DeclarativeColor *> DeclarativeTheme3D::colorList()
{
    if (m_colors.isEmpty()) {
        // Create dummy ThemeColors from theme's gradients
        m_dummyColors = true;
        QList<QColor> list = Q3DTheme::baseColors();
        foreach (QColor item, list) {
            DeclarativeColor *color = new DeclarativeColor(this);
            color->setColor(item);
            m_colors.append(color);
            connect(color, &DeclarativeColor::colorChanged,
                    this, &DeclarativeTheme3D::handleBaseColorUpdate);
        }
    }
    return m_colors;
}

void DeclarativeTheme3D::clearColors()
{
    clearDummyColors();
    foreach (DeclarativeColor *item, m_colors)
        disconnect(item, 0, this, 0);
    m_colors.clear();
    Q3DTheme::setBaseColors(QList<QColor>());
}

void DeclarativeTheme3D::clearDummyColors()
{
    if (m_dummyColors) {
        foreach (DeclarativeColor *item, m_colors)
            delete item;
        m_colors.clear();
        m_dummyColors = false;
    }
}

void DeclarativeTheme3D::addGradient(ColorGradient *gradient)
{
    clearDummyGradients();
    m_gradients.append(gradient);
    connect(gradient, &ColorGradient::updated,
            this, &DeclarativeTheme3D::handleBaseGradientUpdate);
    QList<QLinearGradient> list = Q3DTheme::baseGradients();
    list.append(convertGradient(gradient));
    Q3DTheme::setBaseGradients(list);
}

QList<ColorGradient *> DeclarativeTheme3D::gradientList()
{
    if (m_gradients.isEmpty()) {
        // Create dummy ColorGradients from theme's gradients
        m_dummyGradients = true;
        QList<QLinearGradient> list = Q3DTheme::baseGradients();
        foreach (QLinearGradient item, list) {
            ColorGradient *gradient = convertGradient(item);
            m_gradients.append(gradient);
            connect(gradient, &ColorGradient::updated,
                    this, &DeclarativeTheme3D::handleBaseGradientUpdate);
        }
    }

    return m_gradients;
}

void DeclarativeTheme3D::clearGradients()
{
    clearDummyGradients();
    foreach (ColorGradient *item, m_gradients)
        disconnect(item, 0, this, 0);
    m_gradients.clear();
    Q3DTheme::setBaseGradients(QList<QLinearGradient>());
}

void DeclarativeTheme3D::clearDummyGradients()
{
    if (m_dummyGradients) {
        foreach (ColorGradient *item, m_gradients)
            delete item;
        m_gradients.clear();
        m_dummyGradients = false;
    }
}

QQmlListProperty<DeclarativeColor> DeclarativeTheme3D::baseColors()
{
    return QQmlListProperty<DeclarativeColor>(this, this,
                                              &DeclarativeTheme3D::appendBaseColorsFunc,
                                              &DeclarativeTheme3D::countBaseColorsFunc,
                                              &DeclarativeTheme3D::atBaseColorsFunc,
                                              &DeclarativeTheme3D::clearBaseColorsFunc);
}

void DeclarativeTheme3D::appendBaseColorsFunc(QQmlListProperty<DeclarativeColor> *list,
                                              DeclarativeColor *color)
{
    reinterpret_cast<DeclarativeTheme3D *>(list->data)->addColor(color);
}

int DeclarativeTheme3D::countBaseColorsFunc(QQmlListProperty<DeclarativeColor> *list)
{
    return reinterpret_cast<DeclarativeTheme3D *>(list->data)->colorList().size();
}

DeclarativeColor *DeclarativeTheme3D::atBaseColorsFunc(QQmlListProperty<DeclarativeColor> *list,
                                                       int index)
{
    return reinterpret_cast<DeclarativeTheme3D *>(list->data)->colorList().at(index);
}

void DeclarativeTheme3D::clearBaseColorsFunc(QQmlListProperty<DeclarativeColor> *list)
{
    reinterpret_cast<DeclarativeTheme3D *>(list->data)->clearColors();
}

QQmlListProperty<ColorGradient> DeclarativeTheme3D::baseGradients()
{
    return QQmlListProperty<ColorGradient>(this, this,
                                           &DeclarativeTheme3D::appendBaseGradientsFunc,
                                           &DeclarativeTheme3D::countBaseGradientsFunc,
                                           &DeclarativeTheme3D::atBaseGradientsFunc,
                                           &DeclarativeTheme3D::clearBaseGradientsFunc);
}

void DeclarativeTheme3D::appendBaseGradientsFunc(QQmlListProperty<ColorGradient> *list,
                                                 ColorGradient *gradient)
{
    reinterpret_cast<DeclarativeTheme3D *>(list->data)->addGradient(gradient);
}

int DeclarativeTheme3D::countBaseGradientsFunc(QQmlListProperty<ColorGradient> *list)
{
    return reinterpret_cast<DeclarativeTheme3D *>(list->data)->gradientList().size();
}

ColorGradient *DeclarativeTheme3D::atBaseGradientsFunc(QQmlListProperty<ColorGradient> *list,
                                                       int index)
{
    return reinterpret_cast<DeclarativeTheme3D *>(list->data)->gradientList().at(index);
}

void DeclarativeTheme3D::clearBaseGradientsFunc(QQmlListProperty<ColorGradient> *list)
{
    reinterpret_cast<DeclarativeTheme3D *>(list->data)->clearGradients();
}

QT_DATAVISUALIZATION_END_NAMESPACE