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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the QtDataVis3D API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#ifndef ABSTRACTITEMMODELHANDLER_P_H
#define ABSTRACTITEMMODELHANDLER_P_H

#include "datavis3dglobal_p.h"
#include <QAbstractItemModel>
#include <QPointer>
#include <QTimer>

QT_DATAVIS3D_BEGIN_NAMESPACE

class QAbstractDataMapping;

class AbstractItemModelHandler : public QObject
{
    Q_OBJECT
public:
    AbstractItemModelHandler(QObject *parent = 0);
    virtual ~AbstractItemModelHandler();

    virtual void setItemModel(const QAbstractItemModel *itemModel);
    virtual const QAbstractItemModel *itemModel() const;
    virtual void setActiveMapping(QAbstractDataMapping *mapping);
    virtual QAbstractDataMapping *activeMapping() const;
    virtual void addMapping(QAbstractDataMapping *mapping);
    virtual void releaseMapping(QAbstractDataMapping *mapping);
    virtual QList<QAbstractDataMapping *> mappings() const;

public slots:
    virtual void handleColumnsInserted(const QModelIndex &parent, int start, int end);
    virtual void handleColumnsMoved(const QModelIndex &sourceParent, int sourceStart,
                                    int sourceEnd, const QModelIndex &destinationParent,
                                    int destinationColumn);
    virtual void handleColumnsRemoved(const QModelIndex &parent, int start, int end);
    virtual void handleDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight,
                                   const QVector<int> &roles = QVector<int> ());
    virtual void handleLayoutChanged(const QList<QPersistentModelIndex> &parents = QList<QPersistentModelIndex>(),
                                     QAbstractItemModel::LayoutChangeHint hint = QAbstractItemModel::NoLayoutChangeHint);
    virtual void handleModelReset();
    virtual void handleRowsInserted(const QModelIndex &parent, int start, int end);
    virtual void handleRowsMoved(const QModelIndex &sourceParent, int sourceStart, int sourceEnd,
                                 const QModelIndex &destinationParent, int destinationRow);
    virtual void handleRowsRemoved(const QModelIndex &parent, int start, int end);

    virtual void handleMappingChanged();
    virtual void handlePendingResolve();

protected:
    virtual void resolveModel() = 0;

    QPointer<const QAbstractItemModel> m_itemModel;  // Not owned
    QAbstractDataMapping *m_activeMapping;
    bool resolvePending;
    QTimer m_resolveTimer;
    QList<QAbstractDataMapping *> m_mappings;

private:
    Q_DISABLE_COPY(AbstractItemModelHandler)
};

QT_DATAVIS3D_END_NAMESPACE

#endif