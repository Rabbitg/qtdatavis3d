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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the QtDataVisualization API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#ifndef TEXTUREHELPER_P_H
#define TEXTUREHELPER_P_H

#include "datavisualizationglobal_p.h"
#include <QOpenGLFunctions>
#include <QRgb>
#include <QLinearGradient>

QT_DATAVISUALIZATION_BEGIN_NAMESPACE

class TextureHelper : protected QOpenGLFunctions
{
    public:
    TextureHelper();
    ~TextureHelper();

    // Ownership of created texture is transferred to caller
    GLuint create2DTexture(const QImage &image, bool useTrilinearFiltering = false,
                           bool convert = true, bool smoothScale = true);
    GLuint createCubeMapTexture(const QImage &image, bool useTrilinearFiltering = false);
    // Returns selection texture and inserts generated framebuffers to framebuffer parameters
    GLuint createSelectionTexture(const QSize &size, GLuint &frameBuffer, GLuint &depthBuffer);
    GLuint createGradientTexture(const QLinearGradient &gradient);
#if !defined(QT_OPENGL_ES_2)
    // Returns depth texture and inserts generated framebuffer to parameter
    GLuint createDepthTexture(const QSize &size, GLuint &frameBuffer, GLuint textureSize = 1);
#endif
    void deleteTexture(const GLuint *texture);

    private:
    QImage convertToGLFormat(const QImage &srcImage);
    void convertToGLFormatHelper(QImage &dstImage, const QImage &srcImage, GLenum texture_format);
    QRgb qt_gl_convertToGLFormatHelper(QRgb src_pixel, GLenum texture_format);

    friend class Bars3DRenderer;
    friend class Surface3DRenderer;
    friend class Scatter3DRenderer;
};

QT_DATAVISUALIZATION_END_NAMESPACE

#endif
