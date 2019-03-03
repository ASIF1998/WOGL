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

#include "WOGL/Data/Conteiners/ArrayView.hpp"
#include "WOGL/Data/Conteiners/MatrixView.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

using namespace std;
using namespace WOGL;
using namespace glm;

vec4 LightPosition  (1.2f, 1.0f, 2.0f, 1.0f);
vec3 LightColor (1.0f, 1.0f, 1.0f);
float LightIntensive = 1.0f;

vec3 Ka (0.3f, 0.3f, 0.3f);
vec3 Kd (0.7f, 0.7f, 0.7f);
vec3 Ks (0.7f, 0.7f, 0.7f);

float F = 5.0f;

mat4 ProjectionMatrix = perspective(radians(90.0f), 800.0f /  600.0f, 0.1f, 100.0f);
mat4 ViewMatrix = glm::lookAt(glm::vec3(35, 35, 4), glm::vec3(0, 0, 0), glm::vec3(0,1,0));
mat4 ModelMatrix(1.0f);
mat3 NormalMatrix(1.0f);

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

        ModelMatrix = glm::rotate(ModelMatrix, 280.7f, vec3(1.0f, 0.0f, 0.0f));

        shaderProgram.setUniform("LightColor", LightColor);

        shaderProgram.setUniform("Ka", Ka);
        shaderProgram.setUniform("Kd", Kd);
        shaderProgram.setUniform("Ks", Ks);
        
        shaderProgram.setUniform("F", F);
        shaderProgram.setUniform("LightIntensive", LightIntensive);

        auto models {
            Model<float, TexelType::RGB>::makeModels("/Users/asifmamedov/Downloads/black-dragon-with-idle-animation/source/ef2da8ba53194e35a4be77969cff3949.fbx.fbx")
        };
        
        auto texture {
            Texture<float, TexelType::RGB>::loadTexture("/Users/asifmamedov/Downloads/black-dragon-with-idle-animation/textures/Floor_S.jpg")
        };

        models[0].setBaseColorTexture(texture);
        models[1].setBaseColorTexture("/Users/asifmamedov/Downloads/black-dragon-with-idle-animation/textures/Dragon_Bump_Col2.jpg");
    

        auto modelsRenderer {
            ModelRenderer<TexelFormat::RGB16_F>::makeModelsRenderer(models)
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
        context.depth(DethFunc::LEQUAL);

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
                        }

                        break;
                    }
                        
                    case SDL_KEYDOWN: {
                        if (event.key.keysym.scancode == SDL_SCANCODE_EQUALS) {
                            globalScale += 0.015f;
                        } else if (event.key.keysym.scancode == SDL_SCANCODE_MINUS) {
                            globalScale -= 0.015f;
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
                
                ModelMatrix = rotate(ModelMatrix, speed, vec3(0.0f, 0.0f, 1.0f));

                shaderProgram.setUniform("MV", ViewMatrix * ModelMatrix);
                shaderProgram.setUniform("MVP", ProjectionMatrix * ViewMatrix * ModelMatrix);
                
                shaderProgram.setUniform("ScaleX", globalScale);
                shaderProgram.setUniform("ScaleY", globalScale);
                shaderProgram.setUniform("ScaleZ", globalScale);

                Context::draw(modelsRenderer, 0);

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
