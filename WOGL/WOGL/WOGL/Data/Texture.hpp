//
//  Texture.cpp
//  WOGL
//
//  Created by Асиф Мамедов on 29/03/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#ifndef Texture_hpp
#define Texture_hpp

#include <GL/glew.h>

namespace WOGL
{
    enum class TexelType
    {
        RED = GL_RED,
        RG = GL_RG,
        RGB = GL_RGB,
        RGBA = GL_RGBA
    };
    
    enum class Canal
    {
        RED,
        GREEN,
        BLUE,
        ALPHA
    };
}

#endif /* Texture_hpp */
