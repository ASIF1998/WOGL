#include <iostream>

#include "WOGL/Core/WOGL.hpp"

#include "WOGL/Render/Shader.hpp"
#include "WOGL/Render/ShaderProgram.hpp"

#include "WOGL/Render/Buffers/Buffers.hpp"
#include "WOGL/Render/VertexArray.hpp"

#include "WOGL/Data/Texture.hpp"
#include "WOGL/Render/Texture/TextureRenderer.hpp"
#include "WOGL/Render/Texture/CubeMapTextureRenderer.hpp"
#include "WOGL/Render/Texture/SkyBox.hpp"
#include "WOGL/Render/Texture/TextureMappingSetting.hpp"
#include "WOGL/Render/Texture/TextureMappingSetting.hpp"

#include "WOGL/Data/Model.hpp"
#include "WOGL/Render/ModelRenderer.hpp"

#include "WOGL/Data/Conteiners/ArrayView.hpp"
#include "WOGL/Data/Conteiners/MatrixView.hpp"

#include "WOGL/Render/Buffers/Framebuffer.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

using namespace std;
using namespace WOGL;
using namespace glm;

vec4 LightPosition (55.0f, 55.0f, 0.0f, 1.0f);
vec3 ModelPosition (35.0f, 35.0f, 10.0f);

vec3 LightColor (1.0f, 1.0f, 1.0f);
float LightIntensive = 1.0f;
float F = 50.0f;
vec3 Ka (0.5f);
vec3 Kd (0.7f);
vec3 Ks (0.8f);

mat4 PerspectiveProjectionMatrix = perspective(radians(90.0f), 800.0f /  600.0f, 0.01f, 100.0f);
mat4 OrhoProjectionMatrix = ortho(-150.0f, 130.0f, -130.0f, 130.0f, -100.0f, 100.0f);

mat4 ViewModelMatrix = glm::lookAt(glm::vec3(35, 25, 4), glm::vec3(0, 0, 0), glm::vec3(0,1,0));
mat4 ViewLightMatrix = glm::lookAt(glm::vec3(65, 15, -10), glm::vec3(0, 0, 0), glm::vec3(0,1,0));

mat4 ModelMatrix(1.0f);
mat3 NormalMatrix(1.0f);

int main()
{
    try {
        init();
        
        Window window("Demo", 1000, 1000);
        Context context(window);
        
        VertexBuffer<3> texturePosition {
            -1.0f, -1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, -1.0f, 0.0f
        };
        
        VertexBuffer<2> textureCoord {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f
        };
        
        IndexBuffer textureIndexBuffer {
            0, 2, 1, 0, 2, 3
        };
        
        VertexArray textureVAO;
        
        textureVAO.add(texturePosition, 0);
        textureVAO.add(textureCoord, 1);
        
        auto outputTextureVertexShader {
            make_unique<Shader<ShaderTypes::VERTEX>>("/Users/asifmamedov/Desktop/WOGL/Example/Test/outputTexture.vs.glsl")
        };
        
        auto outputTextureFragmentShader {
            make_unique<Shader<ShaderTypes::FRAGMENT>>("/Users/asifmamedov/Desktop/WOGL/Example/Test/outputTexture.fs.glsl")
        };
        
        auto createShadowMapVertexShader {
            make_unique<Shader<ShaderTypes::VERTEX>>("/Users/asifmamedov/Desktop/WOGL/Example/Test/createDepth.vs.glsl")
        };
        
        auto createShadowMapFragmentShader {
            make_unique<Shader<ShaderTypes::FRAGMENT>>("/Users/asifmamedov/Desktop/WOGL/Example/Test/createDepth.fs.glsl")
        };
        
        auto vertexShader {
            make_unique<Shader<ShaderTypes::VERTEX>>("/Users/asifmamedov/Desktop/WOGL/Example/Test/testing.vs.glsl")
        };
        
        auto fragmentShader {
            make_unique<Shader<ShaderTypes::FRAGMENT>>("/Users/asifmamedov/Desktop/WOGL/Example/Test/testing.fs.glsl")
        };
        
        ShaderProgram outputTextureShaderProgram;
        ShaderProgram createShadowMapShaderProgram;
        ShaderProgram shaderProgram;
        
        outputTextureShaderProgram.add(outputTextureVertexShader);
        outputTextureShaderProgram.add(outputTextureFragmentShader);
        outputTextureShaderProgram.link();
        outputTextureShaderProgram.use();
        
        outputTextureShaderProgram.setUniform("Texture", 3);
        
        createShadowMapShaderProgram.add(createShadowMapVertexShader);
        createShadowMapShaderProgram.add(createShadowMapFragmentShader);
        createShadowMapShaderProgram.link();
        
        shaderProgram.add(vertexShader);
        shaderProgram.add(fragmentShader);
        shaderProgram.link();
        shaderProgram.use();
        
        ModelMatrix = translate(ModelMatrix, vec3(0, 0, 3));
        ModelMatrix = glm::rotate(ModelMatrix, 280.7f, vec3(1.0f, 0.0f, 0.0f));
        
        shaderProgram.setUniform("LightColor", LightColor);
        
        shaderProgram.setUniform("Ka", Ka);
        shaderProgram.setUniform("Kd", Kd);
        shaderProgram.setUniform("Ks", Ks);
        
        shaderProgram.setUniform("F", F);
        shaderProgram.setUniform("LightIntensive", LightIntensive);
        shaderProgram.setUniform("LightPosition", LightPosition);
        
        shaderProgram.setUniform("ModelTexture", 0);
        shaderProgram.setUniform("NormalMap", 1);
        shaderProgram.setUniform("ShadowMap", 2);
        
        auto models {
            Model<float, TexelType::RGB>::makeModels("/Users/asifmamedov/Downloads/black-dragon-with-idle-animation/source/ef2da8ba53194e35a4be77969cff3949.fbx.fbx")
        };
        
        auto texture {
            Texture<float, TexelType::RGB>::loadTexture("/Users/asifmamedov/Downloads/hw_mystic/mystic_lf.tga")
        };
        
        models[0].setBaseColorTexture("/Users/asifmamedov/Downloads/black-dragon-with-idle-animation/textures/Floor_C.jpg");
        models[1].setBaseColorTexture("/Users/asifmamedov/Downloads/black-dragon-with-idle-animation/textures/Dragon_Bump_Col2.jpg");
        
        
        auto modelsRenderer {
            ModelRenderer<TexelFormat::RGB16_F>::makeModelsRenderer(models)
        };
        
        context.clearColor(1, 1, 1, 1);
        context.enable(Enable::MULTISAMPLE);
        
        int32_t t1 = SDL_GetTicks(), t2 = 0;
        SDL_Event event;
        bool stay = true, rotateFlag = true;
        float speed = 0.02f;
        float globalScale = 1.0f;
        
        glViewport(0, 0, 1000, 1000);
        Framebuffer<TexelFormat::RGB16_F,WritePixels::RenderBuffer, WritePixels::NoWrite> resultFrameBuffer(1000, 1000, 2);
        ShadowMapRenderer<32, 1000, 1000> depthFrameBuffer;
        
        depthFrameBuffer.bindDepthTexture(2);
        
        TextureRenderer<TexelFormat::RG16_F>::textureCompareMode(TextureCompareMode::COMPARE_REF_TO_TEXTURE);
        TextureRenderer<TexelFormat::RG16_F>::textureCompareFunc(TextureCompareFunc::LESS);
        
//        glEnable(GL_POINT_SIZE);

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
                        } else if (event.key.keysym.scancode == SDL_SCANCODE_0) {
                            cout << globalScale << endl;
                        } else if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
                            rotateFlag = !rotateFlag;
                        }
                        
                        break;
                    }
                }
            }
            
            if (rotateFlag) {
                auto size = window.size();
                
                t2 = SDL_GetTicks();
                
                if (t2 - t1 > 50) {
                    t1 = t2;
                    
                    // Включаем тест глубины
                    context.enable(Enable::DEPTH_TEST);
                    context.depth(DethFunc::LEQUAL);
                    
                    /**
                     * Создаем карту теней.
                     */
                    depthFrameBuffer.bind();
                    createShadowMapShaderProgram.use();
                    
                    context.clearDepthBuffer();
                    
                    PerspectiveProjectionMatrix = perspective(radians(65.0f), (float)get<WINDOW_WIDTH>(size) / (float)get<WINDOW_HEIGHT>(size), 0.01f, 500.0f);
                    
                    auto lightSpaceMatrix = OrhoProjectionMatrix * ViewLightMatrix * ModelMatrix;
                    
                    createShadowMapShaderProgram.setUniform("LightSpaceMatrix", lightSpaceMatrix);
                    
                    Context::draw(modelsRenderer, 0);
                    
                    /*
                     * Рисуем саму модель.
                     */
                    resultFrameBuffer.bind();
                    shaderProgram.use();
                    
                    depthFrameBuffer.bindDepthTexture(2);
                    
                    auto MV = ViewModelMatrix * ModelMatrix;
                    
                    context.clearColorBuffer();
                    context.clearDepthBuffer();
                    context.clearStencilBuffer();
                    
                    ModelMatrix = rotate(ModelMatrix, speed, vec3(0.0f, 0.0f, 1.0f));
                    
                    shaderProgram.setUniform("MV", MV);
                    shaderProgram.setUniform("MVP", PerspectiveProjectionMatrix * MV);
                    shaderProgram.setUniform("NormalMatrix", mat3(transpose(MV)));
                    shaderProgram.setUniform("LightSpaceMatrix", lightSpaceMatrix);
                    
                    shaderProgram.setUniform("ScaleX", globalScale);
                    shaderProgram.setUniform("ScaleY", globalScale);
                    shaderProgram.setUniform("ScaleZ", globalScale);
                    
                    Context::draw(modelsRenderer, 0);
                    
                    resultFrameBuffer.unbind();
                    
                    // Выключаем тест глубины
                    context.disable(Enable::DEPTH_TEST);
                    
                    context.clearColorBuffer();
                    
                    outputTextureShaderProgram.use();
                    textureVAO.bind();
                    textureIndexBuffer.bind();
                    resultFrameBuffer.colorBuffer(0).bind(3);
//                    depthFrameBuffer.bindDepthTexture(3);
                    
                    context.draw(DrawPrimitive::TRIANGLES, textureIndexBuffer);
                    
                    context.checkError();
                    window.present();
                }
            }
        }
        
        quit();
    } catch (const exception& ex) {
        cerr << ex.what() << endl;
    }
    
    return 0;
}
