#include <iostream>

#include "WOGL/WOGL.hpp"

#include "WOGL/Render/Shader.hpp"
#include "WOGL/Render/ShaderProgram.hpp"

#include "WOGL/Render/Buffers/Buffers.hpp"
#include "WOGL/Render/VertexArray.hpp"

#include "WOGL/Data/Texture.hpp"
#include "WOGL/Render/TextureRenderer.hpp"

#include "WOGL/Data/Model.hpp"
#include "WOGL/Render/ModelRenderer.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

using namespace std;
using namespace WOGL;
using namespace glm;

/**
 * Общие данные.
*/
vec4 LightPosition  (1.2f, 1.0f, 2.0f, 1.0f);
vec3 LightColor (1.0f, 1.0f, 1.0f);
float LightIntensive = 1.0f;

vec3 Ka (0.3f, 0.3f, 0.3f);
vec3 Kd (0.7f, 0.7f, 0.7f);
vec3 Ks (0.7f, 0.7f, 0.7f);

float F = 5.0f;

mat4 ProjectionMatrix = perspective(radians(90.0f), 800.0f /  600.0f, 0.1f, 100.0f);
mat4 ViewMatrix = lookAt(vec3(35.0f, 35.0f, 55.0f), vec3(-10.0f, -15.0f, 0.0f), vec3(0.0f, 1.0f , 0.0f));

/**
 * Топор викинга.
*/
mat4 VikingAxeModelMatrix(1.0f);
mat3 VikingAxeNormalMatrix(1.0f);

/**
 * Крупная галька.
*/
mat4 PebbelsModelMatrix(1.0f);
mat3 PebbelsNormalMatrix(1.0f);

float ScaleVikingAxeX = 0.5f;
float ScaleVikingAxeY = 0.5f;
float ScaleVikingAxeZ = 0.5f;

int main()
{
    try {
        init();

        Window window("Demo", 800, 600);
        Context context(window);

        auto vertexShader {
            make_unique<Shader<ShaderTypes::VERTEX>>("/Users/asifmamedov/Desktop/WOGL/WOGL/Example/Test/testing.vs.glsl")
        };
        
        auto fragmentShader {
            make_unique<Shader<ShaderTypes::FRAGMENT>>("/Users/asifmamedov/Desktop/WOGL/WOGL/Example/Test/testing.fs.glsl")
        };
        
        ShaderProgram shaderProgram;
        
        shaderProgram.add(vertexShader);
        shaderProgram.add(fragmentShader);
        shaderProgram.link();
        shaderProgram.use();
        
        VikingAxeModelMatrix = rotate(VikingAxeModelMatrix, 280.7f, vec3(1.0f, 0.0f, 0.0f));
        VikingAxeModelMatrix = translate(VikingAxeModelMatrix, vec3(-15.0f, 15.0f, 0.0f));

        PebbelsModelMatrix = rotate(PebbelsModelMatrix, 1.28f, vec3(1.0f, 0.0f, 0.0f));
        PebbelsModelMatrix = rotate(PebbelsModelMatrix, 0.80f, vec3(0.0f, 0.0f, 1.0f));

        shaderProgram.setUniform("LightColor", LightColor);
        
        shaderProgram.setUniform("Ka", Ka);
        shaderProgram.setUniform("Kd", Kd);
        shaderProgram.setUniform("Ks", Ks);
        
        shaderProgram.setUniform("F", F);
        shaderProgram.setUniform("LightIntensive", LightIntensive);

        VertexBuffer<3> pebblesPositions {
            -110.0f, -110.0f, 0.0f,
            -110.0f, 110.0f, 0.0,
            110.0f, 110.0f, 0.0f,
            110.0f, -110.0f, 0.0f
        };

        VertexBuffer<3> pebblesNormals {
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f,
            0.0f, -1.0f, 0.0f
        };

        VertexBuffer<2> pebblesPositionsTextureCoords {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f
        };

        IndexBuffer pebbelsIndexBuffer {
            0, 2, 1, 0, 2, 3
        };

        VertexArray pebblesVAO;

        pebblesVAO.add(pebblesPositions, 0);
        pebblesVAO.add(pebblesNormals, 1);
        pebblesVAO.add(pebblesPositionsTextureCoords, 2);

        auto pebbelsTexure {
            Texture<float, TexelType::RGB>::loadTexture("/Users/asifmamedov/Desktop/WOGL/WOGL/Example/Demo Scene/Source/Pebbles/Pebbles_006_COLOR.jpg")
        };

        TextureRenderer<TexelFormat::RGB16_F> pebblesTextureRenderer {
            pebbelsTexure
        };

        Model<float, TexelType::RGB> model {
            "/Users/asifmamedov/Desktop/WOGL/WOGL/Example/Demo Scene/Source/hie_viking_axe_d180212/scene.gltf"
        };

        model.setBaseColorTexture("/Users/asifmamedov/Desktop/WOGL/WOGL/Example/Demo Scene/Source/hie_viking_axe_d180212/textures/VikingAxe_D180212_baseColor.png");

        ModelRenderer<TexelFormat::RGB16_F> modelRenderer {
            model
        };

        context.enable(Enable::DEPTH_TEST);

        context.clearColor(1, 1, 1, 1);

        context.chechError();

        int32_t t1 = SDL_GetTicks(), t2 = 0;
        SDL_Event event;
        bool stay = true;
        float speed = 0.02f;
        float globalScale = 1.0f;

        context.enable(Enable::DEPTH_TEST);
        context.depth(DethFunc::NOTEQUAL);

        pebblesVAO.bind();
        pebbelsIndexBuffer.bind();
        pebblesTextureRenderer.bind();

        while(stay) {
            while(SDL_PollEvent(&event)) {
                switch(event.type) {
                    case SDL_QUIT: {
                        stay = false;
                        break;
                    }
                        
                    case SDL_KEYUP: {
                        if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                            stay = false;
                        } else if (event.key.keysym.scancode == SDL_SCANCODE_1) {
                            globalScale += 0.1f;
                        } else if (event.key.keysym.scancode == SDL_SCANCODE_2) {
                            globalScale -= 0.1f;
                        }
                        
                        break;
                    }
                }
            }

            auto size = window.size();

            t2 = SDL_GetTicks();
            
            if (t2 - t1 > 50) {
                t1 = t2;

                context.clearColorBuffer();
                context.clearDepthBuffer();
                context.clearStencilBuffer();

                ProjectionMatrix = perspective(radians(65.0f), (float)get<0>(size) / (float)get<1>(size), 0.01f, 500.0f);

                // Рендерим крупные гальки
                pebblesVAO.bind();
                pebbelsIndexBuffer.bind();
                pebblesTextureRenderer.bind();

                shaderProgram.setUniform("MV", ViewMatrix * PebbelsModelMatrix);
                shaderProgram.setUniform("MVP", ProjectionMatrix * ViewMatrix * PebbelsModelMatrix);
                
                shaderProgram.setUniform("ScaleX", 2.0f * globalScale);
                shaderProgram.setUniform("ScaleY", 2.0f * globalScale);
                shaderProgram.setUniform("ScaleZ", 2.0f * globalScale);
                
                Context::draw(DrawPrimitive::TRIANGLES, pebbelsIndexBuffer.size());

                // Рендерим топор.
                VikingAxeModelMatrix = rotate(VikingAxeModelMatrix, speed, vec3(0.0f, 0.0f, 1.0f));
                VikingAxeNormalMatrix = transpose(ViewMatrix * VikingAxeModelMatrix);

                shaderProgram.setUniform("MV", ViewMatrix * VikingAxeModelMatrix);
                shaderProgram.setUniform("MVP", ProjectionMatrix * ViewMatrix * VikingAxeModelMatrix);

                shaderProgram.setUniform("ScaleX", ScaleVikingAxeX * globalScale);
                shaderProgram.setUniform("ScaleY", ScaleVikingAxeY * globalScale);
                shaderProgram.setUniform("ScaleZ", ScaleVikingAxeZ * globalScale);

                Context::draw(modelRenderer, 0);

                context.chechError();
                window.present();
            }
        } 

        quit();
    } catch (const exception& ex) {
        cerr << ex.what() << endl;
    }

    return 0;
}
