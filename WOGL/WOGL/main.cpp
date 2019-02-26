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
mat4 ViewMatrix = lookAt(vec3(35, 35, 50), vec3(0, -10.0, 0), vec3(0,1,0));

/**
 * Топор викинга.
*/
mat4 VikingAxeModelMatrix(1.0f);
mat3 VikingAxeNormalMatrix(1.0f);

float ScaleVikingAxeX = 0.25f;
float ScaleVikingAxeY = 0.25f;
float ScaleVikingAxeZ = 0.25f;

/**
 * Крупный уголь.
*/
mat4 CobblestonesModelMatrix(1.0f);
mat3 CobblestonesNormalMatrix(1.0f);

float ScaleCobblestonesX = 4.0f;
float ScaleCobblestonesY = 1.0f;
float ScaleCobblestonesZ = 4.0f;

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
//        CobblestonesModelMatrix = rotate(CobblestonesModelMatrix, 0.5f, vec3(0.0f, 1.0f, 0.0f));
//        CobblestonesModelMatrix = translate(CobblestonesModelMatrix, vec3(69.0f, 0.0, 25.0));
        
        shaderProgram.setUniform("ViewMatrix", ViewMatrix);
        
        shaderProgram.setUniform("LightColor", LightColor);
        
        shaderProgram.setUniform("Ka", Ka);
        shaderProgram.setUniform("Kd", Kd);
        shaderProgram.setUniform("Ks", Ks);
        
        shaderProgram.setUniform("F", F);
        shaderProgram.setUniform("LightIntensive", LightIntensive);
        
        shaderProgram.setUniform("scaleX", ScaleVikingAxeX);
        shaderProgram.setUniform("scaleY", ScaleVikingAxeY);
        shaderProgram.setUniform("scaleZ", ScaleVikingAxeZ);
        
//        shaderProgram.setUniform("scaleX", ScaleCobblestonesX);
//        shaderProgram.setUniform("scaleY", ScaleCobblestonesY);
//        shaderProgram.setUniform("scaleZ", ScaleCobblestonesZ);
        

        Model<float, TexelType::RGB> model {
            "/Users/asifmamedov/Desktop/WOGL/WOGL/Example/Demo Scene/Source/hie_viking_axe_d180212/scene.gltf"
        };

//        Model<float, TexelType::RGB> model {
//            "/Users/asifmamedov/Desktop/WOGL/WOGL/Example/Demo Scene/Source/cobblestones/scene.gltf"
//        };
        
//        model.setBaseColorTexture("/Users/asifmamedov/Desktop/WOGL/WOGL/Example/Demo Scene/Source/cobblestones/textures/blinn1_baseColor.png");

        model.setBaseColorTexture("/Users/asifmamedov/Desktop/WOGL/WOGL/Example/Demo Scene/Source/hie_viking_axe_d180212/textures/VikingAxe_D180212_baseColor.png");

        ModelRenderer<TexelFormat::RGB16_F> modelRenderer {
            model
        };

        context.enable(Enable::DEPTH_TEST);
        context.enable(Enable::BLEAND);
        context.enable(Enable::CULL_FACE);
        
        context.blend(BlendFunc::SRC_ALPHA, BlendFunc::ONE_MINUS_DST_ALPHA);
        context.depth(DethFunc::LEQUAL);

        context.faceTraversal(FaceTraversal::CLOCKWISE);
        context.cullFace(Face::FRONT);

        context.clearColor(1, 1, 1, 1);

        context.chechError();

        int32_t t1 = SDL_GetTicks(), t2 = 0;
        SDL_Event event;
        bool stay = true;
        float speed = 0.02f;

        while(stay) {
            while(SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT || event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    stay = false;
                }
            }

            auto size = window.size();

            t2 = SDL_GetTicks();
            
            if (t2 - t1 > 50) {
                t1 = t2;

                ProjectionMatrix = perspective(radians(65.0f), (float)get<0>(size) /  (float)get<1>(size), 10.0f, 100.0f);
                VikingAxeModelMatrix = rotate(VikingAxeModelMatrix, speed, vec3(0.0f, 0.0f, 1.0f));
                VikingAxeNormalMatrix = transpose(ViewMatrix * VikingAxeModelMatrix);

                shaderProgram.setUniform("ModelMatrix", VikingAxeModelMatrix);
                shaderProgram.setUniform("ProjectionMatrix", ProjectionMatrix);
                shaderProgram.setUniform("NormalMatrix", VikingAxeNormalMatrix);
                
//                ProjectionMatrix = perspective(radians(65.0f), (float)get<0>(size) /  (float)get<1>(size), 10.0f, 100.0f);
//                CobblestonesNormalMatrix = transpose(CobblestonesModelMatrix * VikingAxeModelMatrix);
//
//                shaderProgram.setUniform("ModelMatrix", CobblestonesModelMatrix);
//                shaderProgram.setUniform("ProjectionMatrix", ProjectionMatrix);
//                shaderProgram.setUniform("NormalMatrix", CobblestonesNormalMatrix);
                
                context.clearColorBuffer();
                context.clearDepthBuffer();
                context.clearStencilBuffer();
                
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
