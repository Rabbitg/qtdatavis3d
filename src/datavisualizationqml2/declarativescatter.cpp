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

#include "declarativescatter_p.h"
#include "declarativescatterrenderer_p.h"
#include "qitemmodelscatterdataproxy.h"
#include "theme_p.h"

QT_DATAVISUALIZATION_BEGIN_NAMESPACE

const QString smoothString(QStringLiteral("Smooth"));

DeclarativeScatter::DeclarativeScatter(QQuickItem *parent)
    : AbstractDeclarative(parent),
      m_shared(0),
      m_initialisedSize(0, 0)
{
    setFlags(QQuickItem::ItemHasContents);
    setAcceptedMouseButtons(Qt::AllButtons);

    // TODO: These seem to have no effect; find a way to activate anti-aliasing
    setAntialiasing(true);
    setSmooth(true);

    // Create the shared component on the main GUI thread.
    m_shared = new Scatter3DController(boundingRect().toRect());
    setSharedController(m_shared);
    m_shared->setActiveDataProxy(new QItemModelScatterDataProxy);

    QObject::connect(m_shared, &Scatter3DController::selectedItemIndexChanged, this,
                     &DeclarativeScatter::selectedItemIndexChanged);
}

DeclarativeScatter::~DeclarativeScatter()
{
    delete m_shared;
}

QSGNode *DeclarativeScatter::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    // If old node exists and has right size, reuse it.
    if (oldNode && m_initialisedSize == boundingRect().size().toSize()) {
        // Update bounding rectangle (that has same size as before).
        static_cast<DeclarativeScatterRenderer *>( oldNode )->setRect(boundingRect());
        return oldNode;
    }

    // Create a new render node when size changes or if there is no node yet
    m_initialisedSize = boundingRect().size().toSize();

    // Delete old node
    if (oldNode)
        delete oldNode;

    // Create a new one and set it's bounding rectangle
    DeclarativeScatterRenderer *node = new DeclarativeScatterRenderer(window(), m_shared);
    node->setRect(boundingRect());
    m_shared->setBoundingRect(boundingRect().toRect());
    return node;
}

void DeclarativeScatter::setObjectColor(const QColor &baseColor, bool uniform)
{
    m_shared->setObjectColor(baseColor, uniform);
}

QScatterDataProxy *DeclarativeScatter::dataProxy() const
{
    return static_cast<QScatterDataProxy *>(m_shared->activeDataProxy());
}

void DeclarativeScatter::setDataProxy(QScatterDataProxy *dataProxy)
{
    m_shared->setActiveDataProxy(dataProxy);
}

Q3DValueAxis *DeclarativeScatter::axisX() const
{
    return static_cast<Q3DValueAxis *>(m_shared->axisX());
}

void DeclarativeScatter::setAxisX(Q3DValueAxis *axis)
{
    m_shared->setAxisX(axis);
}

Q3DValueAxis *DeclarativeScatter::axisY() const
{
    return static_cast<Q3DValueAxis *>(m_shared->axisY());
}

void DeclarativeScatter::setAxisY(Q3DValueAxis *axis)
{
    m_shared->setAxisY(axis);
}

Q3DValueAxis *DeclarativeScatter::axisZ() const
{
    return static_cast<Q3DValueAxis *>(m_shared->axisZ());
}

void DeclarativeScatter::setAxisZ(Q3DValueAxis *axis)
{
    m_shared->setAxisZ(axis);
}

void DeclarativeScatter::setObjectType(QDataVis::MeshStyle style)
{
    QString objFile = m_shared->meshFileName();
    bool smooth = objFile.endsWith(smoothString);
    m_shared->setObjectType(style, smooth);
}

QDataVis::MeshStyle DeclarativeScatter::objectType() const
{
    QString objFile = m_shared->meshFileName();
    if (objFile.contains("/sphere"))
        return QDataVis::MeshStyleSpheres;
    else
        return QDataVis::MeshStyleDots;
}

void DeclarativeScatter::setObjectSmoothingEnabled(bool enabled)
{
    QString objFile = m_shared->meshFileName();
    if (objFile.endsWith(smoothString)) {
        if (enabled)
            return; // Already smooth; do nothing
        else // Rip Smooth off the end
            objFile.resize(objFile.indexOf(smoothString));
    } else {
        if (!enabled) // Already flat; do nothing
            return;
        else // Append Smooth to the end
            objFile.append(smoothString);
    }
    m_shared->setMeshFileName(objFile);
}

bool DeclarativeScatter::isObjectSmoothingEnabled() const
{
    QString objFile = m_shared->meshFileName();
    return objFile.endsWith(smoothString);
}

void DeclarativeScatter::setMeshFileName(const QString &objFileName)
{
    m_shared->setMeshFileName(objFileName);
}

QString DeclarativeScatter::meshFileName() const
{
    return m_shared->meshFileName();
}

void DeclarativeScatter::setSelectedItemIndex(int index)
{
    m_shared->setSelectedItemIndex(index);
}

int DeclarativeScatter::selectedItemIndex() const
{
    return m_shared->selectedItemIndex();
}

QT_DATAVISUALIZATION_END_NAMESPACE
