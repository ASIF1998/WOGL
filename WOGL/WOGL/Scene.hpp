//
//  Scene.hpp
//  WOGL
//
//  Created by Асиф Мамедов on 01/05/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "WOGL/Core/WOGL.hpp"
#include "WOGL/Core/Window.hpp"
#include "WOGL/Core/Context.hpp"

#include "WOGL/Render/ShaderProgram.hpp"

#include "WOGL/Data/Model.hpp"
#include "WOGL/Render/ModelRenderer.hpp"

#include "WOGL/Render/VertexArray.hpp"
#include "WOGL/Render/Buffers/Buffers.hpp"

#include "WOGL/Render/Texture/TextureRenderer2D.hpp"

using namespace WOGL;

#define WAY_TO_THE_PROJECT "/Users/asifmamedov/Desktop/WOGL/Example/PBR"

struct LightInfo 
{
    vec3 lightPosition;
    vec3 lightColor;
    float lightIntensive;
    float f;
};

vector<ITextureRenderer*> loadTexturesRenderer();
tuple<VertexArray, VertexBuffer<3>, VertexBuffer<2>, IndexBuffer> createVBO_VAO_EBO();
float getDeltaTime();

#endif /* Scene_hpp */
