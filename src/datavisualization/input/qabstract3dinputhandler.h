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

#ifndef QABSTRACT3DINPUTHANDLER_H
#define QABSTRACT3DINPUTHANDLER_H

#include "qdatavisualizationenums.h"
#include "qabstract3dinputhandler_p.h"
#include "q3dscene.h"
#include <QObject>
#include <QMouseEvent>

QT_DATAVISUALIZATION_BEGIN_NAMESPACE

class QT_DATAVISUALIZATION_EXPORT QAbstract3DInputHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QtDataVisualization::QDataVis::InputState inputState READ inputState WRITE setInputState NOTIFY inputStateChanged)
    Q_PROPERTY(QPoint inputPosition READ inputPosition WRITE setInputPosition NOTIFY positionChanged)
    Q_PROPERTY(Q3DScene *scene READ scene WRITE setScene)

public:
    explicit QAbstract3DInputHandler(QObject *parent = 0);
    virtual ~QAbstract3DInputHandler();

    // Input event listeners
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void touchEvent(QTouchEvent *event);
    virtual void mousePressEvent(QMouseEvent *event, const QPoint &mousePos);
    virtual void mouseReleaseEvent(QMouseEvent *event, const QPoint &mousePos);
    virtual void mouseMoveEvent(QMouseEvent *event, const QPoint &mousePos);
    virtual void wheelEvent(QWheelEvent *event);

public:
    // Property get/set functions

    // TODO: Check if the inputState needs to be visible outside of subclasses in the final architecture
    QDataVis::InputState inputState();
    void setInputState(QDataVis::InputState inputState);
    void setInputPosition(const QPoint &position);
    QPoint inputPosition() const;

    Q3DScene *scene() const;
    void setScene(Q3DScene *scene);

protected:
    void setPrevDistance(int distance);
    int prevDistance() const;
    void setPreviousInputPos(const QPoint &position);
    QPoint previousInputPos() const;

signals:
    void positionChanged(const QPoint &position);
    void inputStateChanged(QDataVis::InputState state);
    void selectionAtPoint(const QPoint &point);

private:
    Q_DISABLE_COPY(QAbstract3DInputHandler)

    QScopedPointer<QAbstract3DInputHandlerPrivate> d_ptr;

    friend class Abstract3DController;
};

QT_DATAVISUALIZATION_END_NAMESPACE

#endif // QABSTRACT3DINPUTHANDLER_H