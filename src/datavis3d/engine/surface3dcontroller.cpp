/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtDataVis3D module.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "surface3dcontroller_p.h"
#include "surface3drenderer_p.h"
#include "camerahelper_p.h"

#include <QMatrix4x4>
#include <QMouseEvent>

#include <QDebug>

QT_DATAVIS3D_BEGIN_NAMESPACE

Surface3dController::Surface3dController(QRect rect)
    : Abstract3DController(rect),
      m_mouseState(MouseNone),
      m_mousePos(QPoint(0, 0)),
      m_renderer(0),
      m_isInitialized(false),
      m_smoothSurface(false),
      m_surfaceGrid(true)
{
}

Surface3dController::~Surface3dController()
{
}

void Surface3dController::initializeOpenGL()
{
    // Initialization is called multiple times when Qt Quick components are used
    if (m_isInitialized)
        return;

    m_renderer = new Surface3dRenderer(this);
    m_isInitialized = true;
}

void Surface3dController::render(const GLuint defaultFboHandle)
{
    if (!m_isInitialized)
        return;

    m_renderer->render(m_cameraHelper, defaultFboHandle);
}

QMatrix4x4 Surface3dController::calculateViewMatrix(int zoom, int viewPortWidth, int viewPortHeight, bool showUnder)
{
    return m_cameraHelper->calculateViewMatrix(m_mousePos,
                                               zoom,
                                               viewPortWidth,
                                               viewPortHeight,
                                               showUnder);
}

void Surface3dController::setWidth(const int width)
{
    qDebug() << "Surface3dController::setWidth";
    m_renderer->setWidth(width);
}

void Surface3dController::setHeight(const int height)
{
    qDebug() << "Surface3dController::setHeight";
    m_renderer->setHeight(height);
}

void Surface3dController::setSmoothSurface(bool enable)
{
    m_smoothSurface = enable;
    emit smoothStatusChanged(m_smoothSurface);
}

bool Surface3dController::smoothSurface()
{
    return m_smoothSurface;
}

void Surface3dController::setSurfaceGrid(bool enable)
{
    m_surfaceGrid = enable;
    emit surfaceGridChanged(m_surfaceGrid);
}

bool Surface3dController::surfaceGrid()
{
    return m_surfaceGrid;
}


#if defined(Q_OS_ANDROID)
void Surface3dController::mouseDoubleClickEvent(QMouseEvent *event)
{
}
void touchEvent(QTouchEvent *event)
{
}
#endif

void Surface3dController::mousePressEvent(QMouseEvent *event, const QPoint &mousePos)
{
    if (Qt::RightButton == event->button()) {
    #if !defined(Q_OS_ANDROID)
        m_mouseState = Abstract3DController::MouseRotating;
    #else
        m_mouseState = Abstract3DController::MouseOnScene;
    #endif
        // update mouse positions to prevent jumping when releasing or repressing a button
        m_mousePos = mousePos; //event->pos();
    }
    m_cameraHelper->updateMousePos(m_mousePos);
}

void Surface3dController::mouseReleaseEvent(QMouseEvent *event, const QPoint &mousePos)
{
    Q_UNUSED(event)
    if (Abstract3DController::MouseRotating == m_mouseState) {
        // update mouse positions to prevent jumping when releasing or repressing a button
        m_mousePos = mousePos; //event->pos();
        m_cameraHelper->updateMousePos(mousePos); //event->pos());
    }
    m_mouseState = Abstract3DController::MouseNone;
}

void Surface3dController::mouseMoveEvent(QMouseEvent *event, const QPoint &mousePos)
{
    Q_UNUSED(event)
    if (Abstract3DController::MouseRotating == m_mouseState)
        m_mousePos = mousePos; //event->pos();
}

void Surface3dController::wheelEvent(QWheelEvent *event)
{
    Q_UNUSED(event)
}

// TODO: abstract renderer should have accessor for Drawer instead
Drawer *Surface3dController::drawer()
{
    if (m_renderer)
        return m_renderer->drawer();
    else
        return 0;
}

void Surface3dController::setTickCount(GLint tickCount, GLfloat step, GLfloat minimum)
{
    m_tickCount   = tickCount;
    m_tickStep    = step;
    m_tickMinimum = minimum;

    emit tickCountChanged(m_tickCount, m_tickStep, m_tickMinimum);
}

void Surface3dController::setGradientColorAt(qreal pos, const QColor &color)
{
    Theme t = theme();
    t.m_surfaceGradient.setColorAt(pos, color);
    emit themeChanged(t);
}

// TODO: Temp
void Surface3dController::setData(QList<qreal> series, int width, int depth)
{
    m_series = series;
    m_dataWidth = width;
    m_dataDepth = depth;

    m_renderer->setXZStuff(width, depth);
    m_renderer->setSeries(series);
}

QT_DATAVIS3D_END_NAMESPACE