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

#ifndef Q3DSURFACE_H
#define Q3DSURFACE_H

#include <QtDataVisualization/qdatavisualizationenums.h>
#include <QtDataVisualization/q3dwindow.h>

QT_DATAVISUALIZATION_BEGIN_NAMESPACE

class Q3DSurfacePrivate;
class Q3DValueAxis;
class QSurfaceDataProxy;

class QT_DATAVISUALIZATION_EXPORT Q3DSurface : public Q3DWindow
{
    Q_OBJECT
    Q_PROPERTY(bool gridVisible READ isGridVisible WRITE setGridVisible)
    Q_PROPERTY(bool backgroundVisible READ isBackgroundVisible WRITE setBackgroundVisible)
    Q_PROPERTY(bool smoothSurface READ smoothSurface WRITE setSmoothSurface)
    Q_PROPERTY(bool surfaceGrid READ surfaceGrid WRITE setSurfaceGrid)

public:
    explicit Q3DSurface();
    ~Q3DSurface();

    // Enable or disable background grid
    void setGridVisible(bool visible);
    bool isGridVisible() const;

    // Enable or disable background mesh
    void setBackgroundVisible(bool visible);
    bool isBackgroundVisible() const;

    // Enable or disable the smoothes of the surface
    void setSmoothSurface(bool enable);
    bool smoothSurface() const;

    // Enable or disable the grid on the surface
    void setSurfaceGrid(bool enable);
    bool surfaceGrid() const;

    void setGradientColorAt(qreal pos, const QColor &color);

    // Axes
    void setAxisX(Q3DValueAxis *axis);
    Q3DValueAxis *axisX() const;
    void setAxisY(Q3DValueAxis *axis);
    Q3DValueAxis *axisY() const;
    void setAxisZ(Q3DValueAxis *axis);
    Q3DValueAxis *axisZ() const;
    void addAxis(Q3DValueAxis *axis);
    void releaseAxis(Q3DValueAxis *axis);
    QList<Q3DValueAxis *> axes() const;

    void setActiveDataProxy(QSurfaceDataProxy *proxy);
    QSurfaceDataProxy *activeDataProxy() const;
    void addDataProxy(QSurfaceDataProxy *proxy);
    void releaseDataProxy(QSurfaceDataProxy *proxy);
    QList<QSurfaceDataProxy *> dataProxies() const;

    // TODO: Remove when axes handling in use
    void setSegmentCount(int segmentCount, qreal step, qreal minimum = 0.0f);

    // TODO: Do these need to be public? Where are they called from?
    // Size
    void setWidth(const int width);
    void setHeight(const int height);

protected:
    void render();

#if defined(Q_OS_ANDROID)
    void mouseDoubleClickEvent(QMouseEvent *event);
    void touchEvent(QTouchEvent *event);
#endif
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    QScopedPointer<Q3DSurfacePrivate> d_ptr;
    Q_DISABLE_COPY(Q3DSurface)
};

QT_DATAVISUALIZATION_END_NAMESPACE

#endif // Q3DSURFACE_H