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

#include "scatterobjectbufferhelper_p.h"
#include "objecthelper_p.h"
#include <QtGui/QVector2D>
#include <QtGui/QMatrix4x4>

QT_BEGIN_NAMESPACE_DATAVISUALIZATION

const GLfloat itemScaler = 3.0f;

ScatterObjectBufferHelper::ScatterObjectBufferHelper()
{
    m_indicesType = GL_UNSIGNED_INT;
}

ScatterObjectBufferHelper::~ScatterObjectBufferHelper()
{
}

void ScatterObjectBufferHelper::fullLoad(ScatterSeriesRenderCache *cache, qreal dotScale)
{
    initializeOpenGLFunctions();

    ObjectHelper *dotObj = cache->object();
    ScatterRenderItemArray &renderArray = cache->renderArray();
    const uint renderArraySize = renderArray.size();
    uint itemCount = renderArraySize;
    QQuaternion seriesRotation(cache->meshRotation());

    if (m_meshDataLoaded) {
        // Delete old data
        glDeleteBuffers(1, &m_vertexbuffer);
        glDeleteBuffers(1, &m_uvbuffer);
        glDeleteBuffers(1, &m_normalbuffer);
        glDeleteBuffers(1, &m_elementbuffer);
    }

    // Index vertices
    const QVector<unsigned short> indices = dotObj->indices();
    const QVector<QVector3D> indexed_vertices = dotObj->indexedvertices();
    const QVector<QVector2D> indexed_uvs = dotObj->indexedUVs();
    const QVector<QVector3D> indexed_normals = dotObj->indexedNormals();
    int indicesCount = indices.count();
    int verticeCount = indexed_vertices.count();
    int uvsCount = indexed_uvs.count();
    int normalsCount = indexed_normals.count();

    float itemSize = cache->itemSize() / itemScaler;
    if (itemSize == 0.0f)
        itemSize = dotScale;
    QVector3D modelScaler(itemSize, itemSize, itemSize);
    QMatrix4x4 modelMatrix;
    if (!seriesRotation.isIdentity()) {
        QMatrix4x4 matrix;
        matrix.rotate(seriesRotation);
        modelMatrix = matrix.transposed();
    }
    modelMatrix.scale(modelScaler);

    QVector<QVector3D> scaled_vertices;
    scaled_vertices.resize(verticeCount);
    for (int i = 0; i < verticeCount; i++)
        scaled_vertices[i] = indexed_vertices[i] * modelMatrix;

    QVector<GLuint> buffered_indices;
    QVector<QVector3D> buffered_vertices;
    QVector<QVector2D> buffered_uvs;
    QVector<QVector3D> buffered_normals;

    buffered_indices.resize(indicesCount * renderArraySize);
    buffered_vertices.resize(verticeCount * renderArraySize);
    buffered_uvs.resize(uvsCount * renderArraySize);
    buffered_normals.resize(normalsCount * renderArraySize);
    uint pos = 0;
    for (uint i = 0; i < renderArraySize; i++) {
        ScatterRenderItem &item = renderArray[i];
        if (!item.isVisible()) {
            itemCount--;
            continue;
        }

        int offset = pos * verticeCount;
        if (item.rotation().isIdentity()) {
            for (int j = 0; j < verticeCount; j++)
                buffered_vertices[j + offset] = scaled_vertices[j] + item.translation();
        } else {
            QMatrix4x4 matrix;
            matrix.rotate(seriesRotation * item.rotation());
            modelMatrix = matrix.transposed();
            modelMatrix.scale(modelScaler);

            for (int j = 0; j < verticeCount; j++)
                buffered_vertices[j + offset] = indexed_vertices[j] * modelMatrix
                        + item.translation();
        }

        offset = pos * normalsCount;
        for (int j = 0; j < normalsCount; j++)
            buffered_normals[j + offset] = indexed_normals[j];

        offset = pos * uvsCount;
        for (int j = 0; j < uvsCount; j++)
            buffered_uvs[j + offset] = indexed_uvs[j];

        int offsetVertice = i * verticeCount;
        offset = pos * indicesCount;
        for (int j = 0; j < indicesCount; j++) {
            buffered_indices[j + offset] = GLuint(indices[j] + offsetVertice);
        }

        pos++;
    }

    m_indexCount = indicesCount * itemCount;

    glGenBuffers(1, &m_vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, verticeCount * itemCount * sizeof(QVector3D),
                 &buffered_vertices.at(0),
                 GL_STATIC_DRAW);

    glGenBuffers(1, &m_normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, normalsCount * itemCount * sizeof(QVector3D),
                 &buffered_normals.at(0),
                 GL_STATIC_DRAW);

    glGenBuffers(1, &m_uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, uvsCount * itemCount * sizeof(QVector2D),
                 &buffered_uvs.at(0), GL_STATIC_DRAW);

    glGenBuffers(1, &m_elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesCount * itemCount * sizeof(GLint),
                 &buffered_indices.at(0), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    m_meshDataLoaded = true;
}

void ScatterObjectBufferHelper::update(ScatterSeriesRenderCache *cache, qreal dotScale)
{
    initializeOpenGLFunctions();

    ObjectHelper *dotObj = cache->object();
    ScatterRenderItemArray &renderArray = cache->renderArray();
    const int renderArraySize = renderArray.size();
    QQuaternion seriesRotation(cache->meshRotation());

    // Index vertices
    const QVector<QVector3D> indexed_vertices = dotObj->indexedvertices();
    int verticeCount = indexed_vertices.count();

    float itemSize = cache->itemSize() / itemScaler;
    if (itemSize == 0.0f)
        itemSize = dotScale;
    QVector3D modelScaler(itemSize, itemSize, itemSize);
    QMatrix4x4 modelMatrix;
    if (!seriesRotation.isIdentity()) {
        QMatrix4x4 matrix;
        matrix.rotate(seriesRotation);
        modelMatrix = matrix.transposed();
    }
    modelMatrix.scale(modelScaler);

    QVector<QVector3D> scaled_vertices;
    scaled_vertices.resize(verticeCount);
    for (int i = 0; i < verticeCount; i++)
        scaled_vertices[i] = indexed_vertices[i] * modelMatrix;

    QVector<QVector3D> buffered_vertices;

    buffered_vertices.resize(verticeCount * renderArraySize);
    for (int i = 0; i < renderArraySize; i++) {
        ScatterRenderItem &item = renderArray[i];
        if (!item.isVisible())
            continue;

        const int offset = i * verticeCount;
        if (item.rotation().isIdentity()) {
            for (int j = 0; j < verticeCount; j++)
                buffered_vertices[j + offset] = scaled_vertices[j] + item.translation();
        } else {
            QMatrix4x4 matrix;
            matrix.rotate(seriesRotation * item.rotation());
            modelMatrix = matrix.transposed();
            modelMatrix.scale(modelScaler);

            for (int j = 0; j < verticeCount; j++)
                buffered_vertices[j + offset] = indexed_vertices[j] * modelMatrix
                        + item.translation();
        }
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, buffered_vertices.size() * sizeof(QVector3D),
                 &buffered_vertices.at(0),
                 GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    m_meshDataLoaded = true;
}

QT_END_NAMESPACE_DATAVISUALIZATION
