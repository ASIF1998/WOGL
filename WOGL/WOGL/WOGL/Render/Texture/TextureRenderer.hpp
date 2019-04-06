//
//  TextureRenderer.hpp
//  WOGL
//
//  Created by Асиф Мамедов on 05/04/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#ifndef TextureRenderer_hpp
#define TextureRenderer_hpp

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

    /**
     * Определение режимы сравнения текстур для текущих привязанных текстур глубины. Эти текстур используются в кадровых буферах
     * (напрмер в ShadowMapRenderer или в Framebuffer<TexelFormat::RGB16_F,WritePixels::Texture, WritePixels::NoWrite>).
     * 
     * @field COMPARE_REF_TO_TEXTURE при использовании этого алгоритма, при обращении к текстуре возвращается результат
     * хранения а не цвет.
     * 
     * @field NONE указывает, что красному каналу должено быть назначено соответствующее значение из текущей текстуры глубины.
    */
    enum class TextureCompareMode
    {
        COMPARE_REF_TO_TEXTURE = GL_COMPARE_REF_TO_TEXTURE,
        NONE = GL_NONE
    };

    /**
     * Определяет оператор сравнения, используемый, когда для TextureCompareMode установлено значение COMPARE_REF_TO_TEXTURE.
     * 
     * @field NEVER res[0.0, 0.0]
     * @field LESS res[1.0, 0.0] r < Dt r >= Dt
     * @field GREATER  res[1.0, 0.0] r > Dt r <= Dt
     * @field GEQUAL res[1.0, 0.0] r >= Dt r < Dt
     * @field EQUAL res[1.0, 0.0] r = Dt r ≠ Dt
     * @field NOTEQUAL res[1.0, 0.0] r ≠ Dt r = Dt
     * @field ALWAYS res[1.0, 1.0]
    */
    enum class TextureCompareFunc
    {
        NEVER = GL_NEVER,
        LESS = GL_LESS,
        GREATER = GL_GREATER,
        GEQUAL = GL_GEQUAL,
        EQUAL = GL_EQUAL,
        NOTEQUAL = GL_NOTEQUAL,
        ALWAYS = GL_ALWAYS
    };
}

#include "TextureRenderer.inl"

#endif /* TextureRenderer_hpp */
