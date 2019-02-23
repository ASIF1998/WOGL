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

mat4 projection = perspective(radians(90.0f), 800.0f /  600.0f, 500.0f, 100.0f);
mat4 view =  lookAt(vec3(35, 35, 4), vec3(0, 0, 0), vec3(0,1,0));
mat4 modelMatrix(1.0f);
mat3 normalMatrix(1.0f);

vec4 LightPosition  (1.2f, 1.0f, 2.0f, 1.0f);
vec3 LightColor (1.0f, 1.0f, 1.0f);
float LightIntensive = 1.0f;

vec3 ka (0.3f, 0.3f, 0.3f);
vec3 kd (0.7f, 0.7f, 0.7f);
vec3 ks (0.7f, 0.7f, 0.7f);

float f = 5.0f;

SDL_Event event;
bool stay = true;
float speed = 0.02f;

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
        
        modelMatrix = rotate(modelMatrix, 280.7f, vec3(1.0f, 0.0f, 0.0f));
        
        shaderProgram.set_uniform("ModelMatrix", modelMatrix);
        shaderProgram.set_uniform("ViewMatrix", view);
        shaderProgram.set_uniform("ProjectionMatrix", projection);
        
        shaderProgram.set_uniform("LightColor", LightColor);
        
        shaderProgram.set_uniform("Ka", ka);
        shaderProgram.set_uniform("Kd", kd);
        shaderProgram.set_uniform("Ks", ks);
        
        shaderProgram.set_uniform("F", f);
        shaderProgram.set_uniform("LightIntensive", LightIntensive);
        
        auto texture {
            Texture<float, TexelType::RGB>::loadTexture("/Users/asifmamedov/Downloads/black-dragon-with-idle-animation/textures/Dragon_Bump_Col2.jpg")
        };
        
        Model<float, TexelType::RGB> model {
            "/Users/asifmamedov/Downloads/black-dragon-with-idle-animation/source/ef2da8ba53194e35a4be77969cff3949.fbx.fbx"
        };
        
        model.texture(texture);

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
            
                projection = perspective(radians(65.0f), (float)get<0>(size) /  (float)get<1>(size), 10.0f, 100.0f);
                modelMatrix = rotate(modelMatrix, speed, vec3(1.0f, 0.0f, 0.0f));
                normalMatrix = transpose(view * modelMatrix);
                
                shaderProgram.set_uniform("ModelMatrix", modelMatrix);
                shaderProgram.set_uniform("ProjectionMatrix", projection);
                shaderProgram.set_uniform("NormalMatrix", normalMatrix);
                
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
