//
//  TextureRenderer.hpp
//  WOGL
//
//  Created by Асиф Мамедов on 09/02/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#ifndef TextureRenderer_hpp
#define TextureRenderer_hpp

#include <GL/glew.h>

namespace WOGL
{
    /**
     * Определяет внутренний размер (в зависимости от количиства
     * инициализированных каналов и их размера) буфера в котором будт хранится текстура.
    */
    enum class TexelFormat: GLenum
    {
        RGBA32_F = GL_RGBA32F,
        RGBA16_F = GL_RGBA16F,
        RGBA32_U = GL_RGBA32UI,
        RGBA16_U = GL_RGBA16UI,
        RGBA8_U = GL_RGBA8UI,
        RGBA32_S = GL_RGBA32I,
        RGBA16_S = GL_RGBA16I,
        RGBA8_S = GL_RGBA8I,

        RGB32_F = GL_RGB32F,
        RGB16_F = GL_RGB16F,
        RGB32_S = GL_RGB32I,
        RGB16_S = GL_RGB16I,
        RGB8_S = GL_RGB8I,
        RGB32_U = GL_RGB32UI,
        RGB16_U = GL_RGB16UI,
        RGB8_U = GL_RGB8UI,

        RG32_F = GL_RG32F,
        RG16_F = GL_RG16F,
        RG32_U = GL_RG32UI,
        RG16_U = GL_RG16UI,
        RG8_U = GL_RG8UI,
        RG32_S = GL_RG32I,
        RG16_S = GL_RG16I,
        RG8_S = GL_RG8I,

        RED32_F = GL_R32F,
        RED16_F = GL_R16F,
        RED32_U = GL_R32UI,
        RED16_U = GL_R16UI,
        RED8_U = GL_R8UI,
        RED32_S = GL_R32I,
        RED16_S = GL_R16I,
        RED8_S = GL_R8I
    };
}

#include "TextureMappingSetting.hpp"
#include "TextureRenderer.inl"

#endif /* TextureRenderer_hpp */
