#include <iostream>

#include "WOGL/Core/WOGL.hpp"

#include "WOGL/Render/Shader.hpp"
#include "WOGL/Render/ShaderProgram.hpp"

#include "WOGL/Render/Buffers/Buffers.hpp"
#include "WOGL/Render/VertexArray.hpp"

#include "WOGL/Render/Texture/TextureRenderer2D.hpp"

using namespace std;
using namespace WOGL;

int main()
{
    try {
        init();
        
        Window window("Demo", 800, 600);
        Context context(window);
        
        auto vertexShader {
            make_unique<Shader<ShaderTypes::VERTEX>>("/Users/asifmamedov/Desktop/WOGL/WOGL/Example/texture mapping/Shaders/textureMapping.vs.glsl")
        };
        
        auto fragmentShader {
            make_unique<Shader<ShaderTypes::FRAGMENT>>("/Users/asifmamedov/Desktop/WOGL/WOGL/Example/texture mapping/Shaders/textureMapping.fs.glsl")
        };
        
        auto texture {
            Texture2D<float, TexelType::RGB>::loadTexture("/Users/asifmamedov/Desktop/WOGL/WOGL/Example/texture mapping/Data/foto.jpg")
        };
        
        TextureRenderer2D<TexelFormat::RGB16_F> textureRenderer {
            texture
        };
        
        ShaderProgram shaderProgram;
        
        shaderProgram.add(vertexShader);
        shaderProgram.add(fragmentShader);
        shaderProgram.link();
        shaderProgram.use();
        
        VertexBuffer<3> positionBuffer {
            -1.0f, -1.0f, 0.0f,
            -1.0f, 1.0f, 0.0,
            1.0f, 1.0f, 0.0f,
            1.0f, -1.0f, 0.0f
        };
        
        VertexBuffer<2> textureCoordsBuffer {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f
        };
        
        IndexBuffer indexBuffer {
            0, 2, 1, 0, 2, 3
        };
        
        VertexArray vao;
        
        vao.add(positionBuffer, 0);
        vao.add(textureCoordsBuffer, 1);
        
        shaderProgram.setUniform("Texture", 0);
        
        vao.bind();
        indexBuffer.bind();
        textureRenderer.bind(0);
        
        context.clearColor(1.0f, 1.0f, 0.0f, 1.0f);
        
        bool stay = true;
        SDL_Event event;
        
        while(stay) {
            while(SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT || event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    stay = false;
                }
            }
            
            context.clearColorBuffer();
            context.draw(DrawPrimitive::TRIANGLES, 6);
            window.present();
            Context::checkError();
        }
        
        quit();
        
    } catch (const exception& ex) {
        cerr << ex.what() << endl;
    }
    
    return 0;
}

