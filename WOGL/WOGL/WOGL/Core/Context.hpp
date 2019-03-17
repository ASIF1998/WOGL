//
//  Context.hpp
//  WOGL
//
//  Created by Асиф Мамедов on 04/02/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#ifndef Context_hpp
#define Context_hpp

#include <GL/glew.h>

namespace WOGL
{
    enum class Enable: GLenum
    {
        ALPHA_TEST = GL_ALPHA_TEST,
        BLEAND = GL_BLEND,
        DEPTH_TEST = GL_DEPTH_TEST,
        STENCIL_TEST = GL_STENCIL_TEST,
        CULL_FACE = GL_CULL_FACE,
        GAMMA_CORECTION = GL_FRAMEBUFFER_SRGB,
        MULTISAMPLE = GL_MULTISAMPLE
    };

    enum class StensilFunc: GLenum
    {
        NEVER = GL_NEVER,
        LESS = GL_LESS,
        GREATER = GL_GREATER,
        GEQUAL = GL_GEQUAL,
        EQUAL = GL_EQUAL,
        NOTEQUAL = GL_NOTEQUAL,
        ALWAYS = GL_ALWAYS
    };

    enum class DethFunc: GLenum
    {
        NEVER = GL_NEVER,
        LESS = GL_LESS,
        GREATER = GL_GREATER,
        GEQUAL = GL_GEQUAL,
        EQUAL = GL_EQUAL,
        LEQUAL = GL_LEQUAL,
        NOTEQUAL = GL_NOTEQUAL,
        ALWAYS = GL_ALWAYS
    };

    enum class BlendFunc: GLenum
    {
        ZERO = GL_ZERO,
        ONE = GL_ONE,
        SRC_COLOR = GL_SRC_COLOR,
        ONE_MINUS_SRC_COLOR  =  GL_ONE_MINUS_SRC_COLOR,
        DST_COLOR = GL_DST_COLOR,
        ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
        SRC_ALPHA = GL_SRC_ALPHA,
        ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
        DST_ALPHA = GL_DST_ALPHA,
        ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
        CONSTANT_COLOR = GL_CONSTANT_COLOR,
        ONE_MINUS_CONSTANT_COLOR = GL_ONE_MINUS_CONSTANT_COLOR,
        CONSTANT_ALPHA = GL_CONSTANT_ALPHA,
        ONE_MINUS_CONSTANT_ALPHA = GL_ONE_MINUS_CONSTANT_ALPHA,
        SRC_ALPHA_SATURATE = GL_SRC_ALPHA_SATURATE,
        SRC1_COLOR = GL_SRC1_COLOR,
        ONE_MINUS_SRC1_COLOR = GL_ONE_MINUS_SRC1_COLOR,
        SRC1_ALPHA = GL_SRC1_ALPHA,
        ONE_MINUS_SRC1_ALPHA = GL_ONE_MINUS_SRC1_ALPHA
    };

    enum class Face: GLenum
    {
        FRONT = GL_FRONT,
        BACK = GL_BACK,
        FRONT_AND_BACK = GL_FRONT_AND_BACK
    };

    enum class FaceTraversal: GLenum
    {
        COUTERCLOCKWISE = GL_CCW,
        CLOCKWISE = GL_CW
    };

    enum class DrawPrimitive
    {
        POINTS = GL_POINTS,
        LINE_STRIP = GL_LINE_STRIP,
        LINE_LOOP = GL_LINE_LOOP,
        LINES = GL_LINES,
        LINE_STRIP_ADJACENCY = GL_LINE_STRIP_ADJACENCY,
        LINES_ADJACENCY = GL_LINES_ADJACENCY,
        TRIANGLES = GL_TRIANGLES,
        TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
        TRIANGLE_FAN = GL_TRIANGLE_FAN,
        TRIANGLE_STRIP_ADJACENCY = GL_TRIANGLE_STRIP_ADJACENCY,
        TRIANGLES_ADJACENCY = GL_TRIANGLES_ADJACENCY,
        PATCHES = GL_PATCHES
    };
    
    using ColorAttachment = GLenum;
}

#include "Context.inl"

#endif /* Context_hpp */
