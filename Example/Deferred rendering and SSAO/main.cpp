#include <iostream>

#include <stdexcept>

#include "WOGL/Core/WOGL.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "WOGL/Render/Buffers/Buffers.hpp"

#include "WOGL/Render/ShaderProgram.hpp"

#include "WOGL/Data/Model.hpp"

#include <random>

#include <vector>

#include "WOGL/Data/Texture.hpp"
#include "WOGL/Render/Texture/TextureRenderer.hpp"

using namespace std;
using namespace WOGL;

struct LightInfo
{
    vec4 lightPosition;
    vec3 lightColor;
    float lightIntensive;
    float f;
};

struct Material
{
    vec3 ka;
    vec3 kd;
    vec3 ks;
};

struct CoordinateSystem
{
    CoordinateSystem() :
        projectionMatrix(1.0),
        viewMatrix(1.0),
        modelMatrix(1.0),
        normalMatrix(1.0)
    {
    }
    
    inline void setPerspectiveProjection(float angle, const tuple<int32_t, int32_t>& windowSize, float zNear, float zFar)
    {
        float aspect = static_cast<float>(get<WINDOW_WIDTH>(windowSize)) / static_cast<float>(get<WINDOW_HEIGHT>(windowSize));
        projectionMatrix = perspective(radians(angle), aspect, zNear, zFar);
    }
    
    inline void setView(const vec3& eye, const vec3& center, const vec3& up)
    {
        viewMatrix = lookAt(eye, center, up);
    }
    
    inline void translate(const vec3& t)
    {
        modelMatrix = glm::translate(modelMatrix, t);
    }
    
    inline void rotate(const vec3& axis, float angle)
    {
        modelMatrix = glm::rotate(modelMatrix, angle, axis);
    }
    
    inline void calculateNormalMatrix()
    {
        normalMatrix = mat3(glm::transpose(inverse(viewMatrix * modelMatrix)));
    }
    
    inline auto mv() const
    {
        return viewMatrix * modelMatrix;
    }
    
    inline auto mvp() const
    {
        return projectionMatrix * viewMatrix * modelMatrix;
    }
    
    mat4 projectionMatrix;
    mat4 viewMatrix;
    mat4 modelMatrix;
    mat3 normalMatrix;
};

uniform_real_distribution<float> rf(0.0f, 1.0f);
default_random_engine gen;

/**
 * Функция осуществляющая инициализаюию моделей.
 */
auto getModel()
{
    auto models {
        Model<float, TexelType::RGB>::makeModels("/Users/asifmamedov/Downloads/black-dragon-with-idle-animation/source/ef2da8ba53194e35a4be77969cff3949.fbx.fbx")
    };
    
    models[0].pushTexture("/Users/asifmamedov/Desktop/WOGL/Example/Deferred rendering and SSAO/Source/dragon/textures/Floor_C.jpg", 0);
    models[0].pushTexture("/Users/asifmamedov/Desktop/WOGL/Example/Deferred rendering and SSAO/Source/dragon/textures/Floor_N.jpg", 1);
    
    models[1].pushTexture("/Users/asifmamedov/Desktop/WOGL/Example/Deferred rendering and SSAO/Source/dragon/textures/Dragon_Bump_Col2.jpg", 0);
    models[1].pushTexture("/Users/asifmamedov/Desktop/WOGL/Example/Deferred rendering and SSAO/Source/dragon/textures/Dragon_Nor_mirror2.jpg", 1);
    
    return models;
}

/**
 * Функция необходимая для формирования ядра выборки из 64 точек.
 */
auto ssaoKernalGen()
{
    vector<vec3> ssaoKernal;
    
    ssaoKernal.resize(64);
    
    for(uint32_t i{0}; i < 64; i++) {
        ssaoKernal[i] = {
            rf(gen) * 2.0f - 1.0f,
            rf(gen) * 2.0f - 1.0f,
            rf(gen)
        };
        
        ssaoKernal[i] = normalize(ssaoKernal[i]);
        ssaoKernal[i] *= rf(gen);
        float scale = (float) i / 64.0f;
        ssaoKernal[i] *= mix(0.1f, 1.0f, scale * scale);
    }
    
    return ssaoKernal;
}

auto ssaoNoiseGen()
{
    Texture2D<float, TexelType::RGB> ssaoNoise(4, 4);
    
    for (size_t i{0}; i < 4; i++) {
        for (size_t j{0}; j < 4; j++) {
            ssaoNoise.at<Canal::RED>(i, j) = rf(gen) * 2.0f - 1.0f;
            ssaoNoise.at<Canal::GREEN>(i, j) = rf(gen) * 2.0f - 1.0f;
            ssaoNoise.at<Canal::BLUE>(i, j) = 0.0f;
        }
    }
    
    return ssaoNoise;
}

int main()
{
    try {
        init();
        
        Window<> window("Dragon", 1000, 1000);
        Context context(window);
        
        Shader<ShaderTypes::VERTEX> gVertexShader {
            "/Users/asifmamedov/Desktop/WOGL/Example/Deferred rendering and SSAO/Shaders/gVertex.vs.glsl"
        };
        
        Shader<ShaderTypes::FRAGMENT> gFragmentShader {
            "/Users/asifmamedov/Desktop/WOGL/Example/Deferred rendering and SSAO/Shaders/gFragment.fs.glsl"
        };
        
        ShaderProgram gShaderProgram;
        
        gShaderProgram.add(gVertexShader);
        gShaderProgram.add(gFragmentShader);
        gShaderProgram.link();
        
        Shader<ShaderTypes::VERTEX> ssaoVertexShader {
            "/Users/asifmamedov/Desktop/WOGL/Example/Deferred rendering and SSAO/Shaders/ssaoVertex.vs.glsl"
        };
        
        Shader<ShaderTypes::FRAGMENT> ssaoFragmentShader {
            "/Users/asifmamedov/Desktop/WOGL/Example/Deferred rendering and SSAO/Shaders/ssaoVertex.fs.glsl"
        };
        
        ShaderProgram ssaoShaderProgram;
        
        ssaoShaderProgram.add(ssaoVertexShader);
        ssaoShaderProgram.add(ssaoFragmentShader);
        ssaoShaderProgram.link();
        
        Shader<ShaderTypes::VERTEX> lightingPassVertexShader {
            "/Users/asifmamedov/Desktop/WOGL/Example/Deferred rendering and SSAO/Shaders/lightingPass.vs.glsl"
        };
        
        Shader<ShaderTypes::FRAGMENT> lightingPassVertexFragment {
            "/Users/asifmamedov/Desktop/WOGL/Example/Deferred rendering and SSAO/Shaders/lightingPass.fs.glsl"
        };
        
        ShaderProgram lightingPassShaderProgram;
        
        lightingPassShaderProgram.add(lightingPassVertexShader);
        lightingPassShaderProgram.add(lightingPassVertexFragment);
        lightingPassShaderProgram.link();
        
        auto models = getModel();
        
        auto modelsRenderer {
            ModelRenderer<TexelFormat::RGB16_F>::makeModelsRenderer(models, 0, 1, 2, 3)
        };
        
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
            0, 2, 1, 0, 3, 2
        };
        
        VertexArray textureVAO;
        
        textureVAO.add(texturePosition, 0);
        textureVAO.add(textureCoord, 1);
        
        Framebuffer<TexelFormat::RGB16_F, WritePixels::RenderBuffer, WritePixels::NoWrite> gBuffer(window, 3);
        Framebuffer<TexelFormat::RED16_F, WritePixels::RenderBuffer, WritePixels::NoWrite> ssaoFrameBuffer(window, 3);
        
        SDL_Event event;
        bool stay = true;
        float modelScale = 1.0f;
        
        Material material {
            vec3(0.4f),
            vec3(0.9f),
            vec3(0.7f)
        };
        
        LightInfo lightIndo {
            vec4(55.0f, 55.0f, 0.0f, 1.0f),
            vec3(1.0f),
            1.0f,
            6.0f
        };
        
        CoordinateSystem cs;
        
        auto ssaoKernal = ssaoKernalGen();
        TextureRenderer2D<TexelFormat::RGB16_F> ssaoNoiseRenderer(ssaoNoiseGen());
        
        cs.setPerspectiveProjection(90.0f, window.size(), 0.01f, 500.0f);
        cs.setView(vec3(35.0f, 25.0f, 4.0f), vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
        cs.translate({0.0f, 0.0f, 3.0f});
        cs.rotate(vec3(1.0f, 0.0f, 0.0f), radians(280.0f));
        cs.calculateNormalMatrix();
        
        gShaderProgram.use();
        gShaderProgram.setUniform("BaseColorTexture", 0);
        gShaderProgram.setUniform("NormalMap", 1);
        
        ssaoShaderProgram.use();
        ssaoShaderProgram.setUniform("GPosition", 0);
        ssaoShaderProgram.setUniform("GNormal", 1);
        ssaoShaderProgram.setUniform("BaseColor", 2);
        ssaoShaderProgram.setUniform("Samples", ssaoKernal);
        ssaoShaderProgram.setUniform("ProjectionMatrix", cs.projectionMatrix);
        
        lightingPassShaderProgram.use();
        lightingPassShaderProgram.setUniform("GPosition", 0);
        lightingPassShaderProgram.setUniform("GNormal", 1);
        lightingPassShaderProgram.setUniform("BaseColor", 2);
        lightingPassShaderProgram.setUniform("SSAO", 3);
        lightingPassShaderProgram.setUniform("LightPosition", lightIndo.lightPosition);
        lightingPassShaderProgram.setUniform("LightColor", lightIndo.lightColor);
        lightingPassShaderProgram.setUniform("LightIntensive", lightIndo.lightIntensive);
        lightingPassShaderProgram.setUniform("F", lightIndo.f);
        lightingPassShaderProgram.setUniform("Ka", material.ka);
        lightingPassShaderProgram.setUniform("Kd", material.kd);
        lightingPassShaderProgram.setUniform("Ks", material.ks);
        
        ColorAttachment ca[] {
            0, 1, 2
        };
        
        Context::checkError();
        Context::enable(Enable::CULL_FACE);
        
        while(stay) {
            while(SDL_PollEvent(&event)) {
                switch (event.type) {
                    case SDL_QUIT: {
                        stay = !stay;
                        break;
                    }
                        
                    case SDL_KEYDOWN: {
                        if (event.key.keysym.scancode == SDL_SCANCODE_1) {
                            modelScale += 0.05f;
                        } else if (event.key.keysym.scancode == SDL_SCANCODE_2) {
                            modelScale -= 0.05f;
                        } else if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                            stay = !stay;
                        }
                        
                        break;
                    }
                }
            }
            
            Context::enable(Enable::DEPTH_TEST);
            Context::depth(Func::LEQUAL);
            
            gShaderProgram.use();
            gBuffer.bind();
            
            Context::clearColorBuffer();
            Context::clearDepthBuffer();
            
            cs.rotate(vec3(0.0f, 0.0f, 1.0f), 0.007f);
            cs.calculateNormalMatrix();
            
            gShaderProgram.setUniform("MV", cs.mv());
            gShaderProgram.setUniform("MVP", cs.mvp());
            gShaderProgram.setUniform("NormalMatrix", cs.normalMatrix);
            gShaderProgram.setUniform("Scale", modelScale);
            
            Context::draw(modelsRenderer, ca);
            
            Context::disable(Enable::DEPTH_TEST);
            gBuffer.unbind();
            
            textureVAO.bind();
            textureIndexBuffer.bind();
            
            gBuffer.colorBuffer(0).bind(0);
            gBuffer.colorBuffer(1).bind(1);
            gBuffer.colorBuffer(2).bind(2);
            
            ssaoFrameBuffer.bind();
            Context::clearColorBuffer();
            
            ssaoShaderProgram.use();
            
            context.draw(DrawPrimitive::TRIANGLES, textureIndexBuffer);
            ssaoFrameBuffer.unbind();
            
            Context::clearColorBuffer();
            
            lightingPassShaderProgram.use();
            
            ssaoFrameBuffer.colorBuffer(0).bind(3);
            
            context.draw(DrawPrimitive::TRIANGLES, textureIndexBuffer);
            
            window.present();
        }
        
        quit();
    } catch(const exception& e) {
        cerr << e.what() << endl;
    }
    
    return 0;
}
