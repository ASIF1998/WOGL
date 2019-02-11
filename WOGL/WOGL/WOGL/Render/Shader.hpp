//
//  Shader.hpp
//  WOGL
//
//  Created by Асиф Мамедов on 04/02/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#ifndef Shader_hpp
#define Shader_hpp

namespace WOGL 
{
    /*
        * Определение типов шйдера.
    */
    enum class ShaderTypes: GLenum
    {
        VERTEX = GL_VERTEX_SHADER,
        FRAGMENT = GL_FRAGMENT_SHADER,
        COMPUTE = GL_COMPUTE_SHADER,
        TESS_CONTROL = GL_TESS_CONTROL_SHADER,
        TESS_EVALUATION = GL_TESS_EVALUATION_SHADER,
        GEOMETRY = GL_GEOMETRY_SHADER
    };
}

#include "Shader.inl"

#endif /* Shader_hpp */
