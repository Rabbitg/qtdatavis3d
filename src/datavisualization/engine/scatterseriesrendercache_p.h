/****************************************************************************
**
** Copyright (C) 2014 Digia Plc
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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the QtDataVisualization API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#ifndef SCATTERSERIESRENDERCACHE_P_H
#define SCATTERSERIESRENDERCACHE_P_H

#include "datavisualizationglobal_p.h"
#include "seriesrendercache_p.h"
#include "qscatter3dseries_p.h"
#include "scatterrenderitem_p.h"

QT_BEGIN_NAMESPACE_DATAVISUALIZATION

class ScatterSeriesRenderCache : public SeriesRenderCache
{
public:
    ScatterSeriesRenderCache(QAbstract3DSeries *series, Abstract3DRenderer *renderer);
    virtual ~ScatterSeriesRenderCache();

    void cleanup(TextureHelper *texHelper);

    inline ScatterRenderItemArray &renderArray() { return m_renderArray; }
    inline QScatter3DSeries *series() const { return static_cast<QScatter3DSeries *>(m_series); }
    inline void setItemSize(float size) { m_itemSize = size; }
    inline float itemSize() const { return m_itemSize; }
    inline void setSelectionIndexOffset(int offset) { m_selectionIndexOffset = offset; }
    inline int selectionIndexOffset() const { return m_selectionIndexOffset; }

protected:
    ScatterRenderItemArray m_renderArray;
    float m_itemSize;
    int m_selectionIndexOffset; // Temporarily cached value for selection color calculations
};

QT_END_NAMESPACE_DATAVISUALIZATION

#endif