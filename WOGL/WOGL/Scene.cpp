//
//  Scene.cpp
//  WOGL
//
//  Created by Асиф Мамедов on 01/05/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "Scene.hpp"

#define loadTex2D(path, texelType) Texture2D<float, texelType>::loadTexture(path)

vector<ITextureRenderer*> loadTexturesRenderer()
{
    return vector<ITextureRenderer*> {
        /// Albedo
        new TextureRenderer2D<TexelFormat::RGB16_F>{loadTex2D(WAY_TO_THE_PROJECT "/Source/textures/lantern_Base_Color.jpg", TexelType::RGB)},
        /// Metalic
        new TextureRenderer2D<TexelFormat::RED16_F>{loadTex2D(WAY_TO_THE_PROJECT "/Source/textures/lantern_Metallic.jpg", TexelType::RED)},
        /// AO
        new TextureRenderer2D<TexelFormat::RED16_F>{loadTex2D(WAY_TO_THE_PROJECT "/Source/textures/lantern_Mixed_AO.jpg", TexelType::RED)},
        /// Normal Map
        new TextureRenderer2D<TexelFormat::RGB16_F>{loadTex2D(WAY_TO_THE_PROJECT "/Source/textures/lantern_Normal_OpenGL.jpg", TexelType::RGB)},
        /// Roughness
        new TextureRenderer2D<TexelFormat::RED16_F>{loadTex2D(WAY_TO_THE_PROJECT "/Source/textures/lantern_Roughness.jpg", TexelType::RED)}
    };
}

tuple<VertexArray, VertexBuffer<3>, VertexBuffer<2>, IndexBuffer> createVBO_VAO_EBO()
{
    VertexBuffer<3> tpVBO {
        -1.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 0.0f
    };
    
    VertexBuffer<2> tcVBO {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    };
    
    IndexBuffer tIB {
        0, 2, 1, 0, 3, 2
    };
    
    VertexArray tVAO;
    
    tVAO.add(tpVBO, 0);
    tVAO.add(tcVBO, 1);
    
    return make_tuple(move(tVAO),move(tpVBO), move(tcVBO), move(tIB));
}


float getDeltaTime()
{
    static uint32_t time1 = SDL_GetTicks(), time2 = 0;
    uint32_t deltaTime;
    
    time2 = SDL_GetTicks();
    deltaTime = time2 - time1;
    time1 = time2;
    
    return (static_cast<float>(deltaTime) / numeric_limits<uint32_t>::max()) * 10000000.0f;
}
