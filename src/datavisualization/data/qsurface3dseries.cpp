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

#include "qsurface3dseries_p.h"
#include "surface3dcontroller_p.h"

QT_DATAVISUALIZATION_BEGIN_NAMESPACE

/*!
 * \class QSurface3DSeries
 * \inmodule QtDataVisualization
 * \brief Base series class for Q3DSurface.
 * \since Qt Data Visualization 1.0
 *
 * QSurface3DSeries manages the series specific visual elements, as well as series data
 * (via data proxy).
 *
 * If no data proxy is set explicitly for the series, the series creates a default
 * proxy. Setting another proxy will destroy the existing proxy and all data added to it.
 *
 * The object mesh set via QAbstract3DSeries::mesh property defines the selection
 * pointer shape in surface series.
 *
 * QSurface3DSeries supports the following format tags for QAbstract3DSeries::setItemLabelFormat():
 * \table
 *   \row
 *     \li @xTitle    \li Title from X axis
 *   \row
 *     \li @yTitle    \li Title from Y axis
 *   \row
 *     \li @zTitle    \li Title from Z axis
 *   \row
 *     \li @xLabel    \li Item value formatted using the same format as the X axis attached to the graph uses,
 *                            see \l{Q3DValueAxis::setLabelFormat()} for more information.
 *   \row
 *     \li @yLabel    \li Item value formatted using the same format as the Y axis attached to the graph uses,
 *                            see \l{Q3DValueAxis::setLabelFormat()} for more information.
 *   \row
 *     \li @zLabel    \li Item value formatted using the same format as the Z axis attached to the graph uses,
 *                            see \l{Q3DValueAxis::setLabelFormat()} for more information.
 * \endtable
 *
 * For example:
 * \snippet doc_src_qtdatavisualization.cpp 1
 *
 * \sa {Qt Data Visualization Data Handling}
 */

/*!
 * \qmltype Surface3DSeries
 * \inqmlmodule QtDataVisualization
 * \since QtDataVisualization 1.0
 * \ingroup datavisualization_qml
 * \instantiates QSurface3DSeries
 * \inherits Abstract3DSeries
 * \brief Base series type for Surfaces3D.
 *
 * This type  manages the series specific visual elements, as well as series data
 * (via data proxy).
 *
 * For more complete description, see QSurface3DSeries.
 *
 * \sa {Qt Data Visualization Data Handling}
 */

/*!
 * \qmlproperty SurfaceDataProxy Surface3DSeries::dataProxy
 *
 * This property holds the active data \a proxy. The series assumes ownership of any proxy set to
 * it and deletes any previously set proxy when a new one is added. The \a proxy cannot be null or
 * set to another series.
 */

/*!
 * \qmlproperty point Surface3DSeries::selectedPoint
 *
 * Selects a surface grid point in a \a position. The position is the (row, column) position in
 * the data array of the series.
 * Only one point can be selected at a time.
 * To clear selection, set invalidSelectionPosition() as the \a position.
 * If this series is added to a graph, the graph can adjust the selection according to user
 * interaction or if it becomes invalid.
 */

/*!
 * \qmlmethod point Surface3DSeries::invalidSelectionPosition()
 * \return a point signifying an invalid selection position. Set this to selectedPoint property
 * to clear the selection.
 */

/*!
 * \qmlproperty bool Surface3DSeries::flatShadingEnabled
 *
 * Sets surface flat shading to \a enabled. It is preset to \c true by default.
 * When disabled, the normals on the surface are interpolated making edges looking round.
 * When enabled, the normals are kept same on a triangle making the color of the triangle solid.
 * This makes the data more readable from the model.
 * \note Flat shaded surfaces require at least GLSL version 1.2 with GL_EXT_gpu_shader4 extension.
 * The value of flatShadingSupported property indicates if flat shading is supported at runtime.
 */

/*!
 * \qmlproperty bool Surface3DSeries::surfaceGridEnabled
 *
 * Sets surface grid to \a enabled. It is preset to \c true by default.
 */

/*!
 * \qmlproperty bool Surface3DSeries::flatShadingSupported
 *
 * Flat shading for surfaces requires at least GLSL version 1.2 with GL_EXT_gpu_shader4 extension.
 * If true, flat shading for surfaces is supported by current system.
 * \note This read-only property is set to its correct value after first render pass.
 * Before then it is always true.
 */

/*!
 * Constructs QSurface3DSeries with the given \a parent.
 */
QSurface3DSeries::QSurface3DSeries(QObject *parent) :
    QAbstract3DSeries(new QSurface3DSeriesPrivate(this), parent)
{
    // Default proxy
    dptr()->setDataProxy(new QSurfaceDataProxy);
}

/*!
 * Constructs QSurface3DSeries with the given \a dataProxy and the \a parent.
 */
QSurface3DSeries::QSurface3DSeries(QSurfaceDataProxy *dataProxy, QObject *parent) :
    QAbstract3DSeries(new QSurface3DSeriesPrivate(this), parent)
{
    dptr()->setDataProxy(dataProxy);
}

/*!
 * \internal
 */
QSurface3DSeries::QSurface3DSeries(QSurface3DSeriesPrivate *d, QObject *parent) :
    QAbstract3DSeries(d, parent)
{
}

/*!
 * Destroys QSurface3DSeries.
 */
QSurface3DSeries::~QSurface3DSeries()
{
}

/*!
 * \property QSurface3DSeries::dataProxy
 *
 * This property holds the active data \a proxy. The series assumes ownership of any proxy set to
 * it and deletes any previously set proxy when a new one is added. The \a proxy cannot be null or
 * set to another series.
 */
void QSurface3DSeries::setDataProxy(QSurfaceDataProxy *proxy)
{
    d_ptr->setDataProxy(proxy);
}

QSurfaceDataProxy *QSurface3DSeries::dataProxy() const
{
    return static_cast<QSurfaceDataProxy *>(d_ptr->dataProxy());
}

/*!
 * \property QSurface3DSeries::selectedPoint
 *
 * Selects a surface grid point in a \a position. The position is the (row, column) position in
 * the data array of the series.
 * Only one point can be selected at a time.
 * To clear selection, set invalidSelectionPosition() as the \a position.
 * If this series is added to a graph, the graph can adjust the selection according to user
 * interaction or if it becomes invalid.
 */
void QSurface3DSeries::setSelectedPoint(const QPoint &position)
{
    // Don't do this in private to avoid loops, as that is used for callback from controller.
    if (d_ptr->m_controller)
        static_cast<Surface3DController *>(d_ptr->m_controller)->setSelectedPoint(position, this);
    else
        dptr()->setSelectedPoint(position);
}

QPoint QSurface3DSeries::selectedPoint() const
{
    return dptrc()->m_selectedPoint;
}

/*!
 * \return a QPoint signifying an invalid selection position. Set this to selectedPoint property
 * to clear the selection.
 */
QPoint QSurface3DSeries::invalidSelectionPosition() const
{
    return Surface3DController::invalidSelectionPosition();
}

/*!
 * \property QSurface3DSeries::flatShadingEnabled
 *
 * Sets surface flat shading to \a enabled. It is preset to \c true by default.
 * When disabled, the normals on the surface are interpolated making edges looking round.
 * When enabled, the normals are kept same on a triangle making the color of the triangle solid.
 * This makes the data more readable from the model.
 * \note Flat shaded surfaces require at least GLSL version 1.2 with GL_EXT_gpu_shader4 extension.
 * The value of flatShadingSupported property indicates if flat shading is supported at runtime.
 */
void QSurface3DSeries::setFlatShadingEnabled(bool enabled)
{
    if (dptr()->m_flatShadingEnabled != enabled) {
        dptr()->setFlatShadingEnabled(enabled);
        emit flatShadingEnabledChanged(enabled);
    }
}

bool QSurface3DSeries::isFlatShadingEnabled() const
{
    return dptrc()->m_flatShadingEnabled;
}

/*!
 * \property QSurface3DSeries::flatShadingSupported
 * Flat shading for surfaces requires at least GLSL version 1.2 with GL_EXT_gpu_shader4 extension.
 * If true, flat shading for surfaces is supported by current system.
 * \note This read-only property is set to its correct value after first render pass.
 * Before then it is always true.
 */
bool QSurface3DSeries::isFlatShadingSupported() const
{
    if (d_ptr->m_controller)
        return static_cast<Surface3DController *>(d_ptr->m_controller)->isFlatShadingSupported();
    else
        return true;
}

/*!
 * \property QSurface3DSeries::surfaceGridEnabled
 *
 * Sets surface grid to \a enabled. It is preset to \c true by default.
 */
void QSurface3DSeries::setSurfaceGridEnabled(bool enabled)
{
    if (dptr()->m_surfaceGridEnabled != enabled) {
        dptr()->setSurfaceGridEnabled(enabled);
        emit surfaceGridEnabledChanged(enabled);
    }
}

bool QSurface3DSeries::isSurfaceGridEnabled() const
{
    return dptrc()->m_surfaceGridEnabled;
}

/*!
 * \internal
 */
QSurface3DSeriesPrivate *QSurface3DSeries::dptr()
{
    return static_cast<QSurface3DSeriesPrivate *>(d_ptr.data());
}

/*!
 * \internal
 */
const QSurface3DSeriesPrivate *QSurface3DSeries::dptrc() const
{
    return static_cast<const QSurface3DSeriesPrivate *>(d_ptr.data());
}

// QSurface3DSeriesPrivate

QSurface3DSeriesPrivate::QSurface3DSeriesPrivate(QSurface3DSeries *q)
    : QAbstract3DSeriesPrivate(q, QAbstract3DSeries::SeriesTypeSurface),
      m_selectedPoint(Surface3DController::invalidSelectionPosition()),
      m_flatShadingEnabled(true),
      m_surfaceGridEnabled(true)
{
    m_itemLabelFormat = QStringLiteral("(@xLabel, @yLabel, @zLabel)");
    m_mesh = QAbstract3DSeries::MeshSphere;
}

QSurface3DSeriesPrivate::~QSurface3DSeriesPrivate()
{
}

QSurface3DSeries *QSurface3DSeriesPrivate::qptr()
{
    return static_cast<QSurface3DSeries *>(q_ptr);
}

void QSurface3DSeriesPrivate::setDataProxy(QAbstractDataProxy *proxy)
{
    Q_ASSERT(proxy->type() == QAbstractDataProxy::DataTypeSurface);

    QAbstract3DSeriesPrivate::setDataProxy(proxy);

    emit qptr()->dataProxyChanged(static_cast<QSurfaceDataProxy *>(proxy));
}

void QSurface3DSeriesPrivate::connectControllerAndProxy(Abstract3DController *newController)
{
    QSurfaceDataProxy *surfaceDataProxy = static_cast<QSurfaceDataProxy *>(m_dataProxy);

    if (m_controller && surfaceDataProxy) {
        //Disconnect old controller/old proxy
        QObject::disconnect(surfaceDataProxy, 0, m_controller, 0);
        QObject::disconnect(q_ptr, 0, m_controller, 0);
    }

    if (newController && surfaceDataProxy) {
        Surface3DController *controller = static_cast<Surface3DController *>(newController);

        QObject::connect(surfaceDataProxy, &QSurfaceDataProxy::arrayReset, controller,
                         &Surface3DController::handleArrayReset);
        QObject::connect(surfaceDataProxy, &QSurfaceDataProxy::rowsChanged, controller,
                         &Surface3DController::handleRowsChanged);
        QObject::connect(surfaceDataProxy, &QSurfaceDataProxy::itemChanged, controller,
                         &Surface3DController::handleItemChanged);
    }
}

void QSurface3DSeriesPrivate::setSelectedPoint(const QPoint &position)
{
    if (position != m_selectedPoint) {
        m_selectedPoint = position;
        emit qptr()->selectedPointChanged(m_selectedPoint);
    }
}

void QSurface3DSeriesPrivate::setFlatShadingEnabled(bool enabled)
{
    m_flatShadingEnabled = enabled;
    if (m_controller)
        m_controller->markSeriesVisualsDirty();
}

void QSurface3DSeriesPrivate::setSurfaceGridEnabled(bool enabled)
{
    m_surfaceGridEnabled = enabled;
    if (m_controller)
        m_controller->markSeriesVisualsDirty();
}

QT_DATAVISUALIZATION_END_NAMESPACE
