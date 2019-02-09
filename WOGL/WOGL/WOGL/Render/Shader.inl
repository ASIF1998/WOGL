//
//  Shader.inl
//  WOGL
//
//  Created by Асиф Мамедов on 04/02/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include <GL/glew.h>

#include <cstdint>

#include <stdexcept>

#include <string_view>

#include <fstream>
#include <sstream>

using namespace std;

namespace WOGL
{
    inline namespace Render
    {
        /**
         * @template shaderType тип шейдера 
        */
        template<ShaderTypes ShaderType>
        class Shader
        {
            friend class ShaderProgram;
            
        public:
            /**
             * Конструктор.
             *
             * @param path путь до файла с шейдерным кодом
             * @throw runtime_error в случае ошибки создания или компиляции шейдера
            */
            explicit Shader(const string_view path) :
                _shaderHandle{glCreateShader(static_cast<GLenum>(ShaderType))}
            {
                if (!_shaderHandle) {
                    throw runtime_error("Error create shader");
                }

                ostringstream sstream;
                ifstream file(path.data());
                sstream << file.rdbuf();
                string shaderCode(sstream.str());
                const char* ptrShaderCode = shaderCode.c_str();

                glShaderSource(_shaderHandle, 1, &ptrShaderCode, nullptr);
                glCompileShader(_shaderHandle);

                int32_t cr = 0;

                glGetShaderiv(_shaderHandle, GL_COMPILE_STATUS, &cr);

                if (!cr) {
                    glGetShaderiv(_shaderHandle, GL_INFO_LOG_LENGTH, &cr);
                    
                    if (cr > 0) {
                        string msg;
                        int32_t written;
                        
                        msg.reserve(cr);
                        
                        glGetShaderInfoLog(_shaderHandle, cr, &written, &msg[0]);
                        
                        throw runtime_error(msg);
                    }
                }
            }

            Shader(Shader&& shader) :
                _shaderHandle{0}
            {
                swap(_shaderHandle, shader._shaderHandle);
            }

            Shader(const Shader&) = delete;
            Shader& operator=(const Shader&) = delete;

            ~Shader()
            {
                if (_shaderHandle) {
                    glDeleteShader(_shaderHandle);
                }
            }

            constexpr uint32_t handle() const noexcept
            {
                return _shaderHandle;
            }

            constexpr ShaderTypes type() const noexcept
            {
                return ShaderType;
            }

        private:
            uint32_t _shaderHandle;
        };
    }
}
