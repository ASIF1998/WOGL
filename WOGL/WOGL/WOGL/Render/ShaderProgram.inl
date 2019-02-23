//
//  ShaderProgram.inl
//  WOGL
//
//  Created by Асиф Мамедов on 04/02/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "Shader.hpp"

#include <optional>

#include <glm/glm.hpp>

#include <string>

using namespace glm;

namespace WOGL
{
    class ShaderProgram
    {
    public:
        /**
         * Конструктор.
         *
         * @throw runtime_error в случае если не удалось создать дескриптор шейдерной программы
        */
        ShaderProgram() :
            _shaderProgramHandle(glCreateProgram())
        {
            if (!_shaderProgramHandle) {
                throw runtime_error("Error create shader program");
            }
        }

        ShaderProgram(ShaderProgram&& shaderProgram) :
            _shaderProgramHandle(0)
        {
            swap(_shaderProgramHandle, shaderProgram._shaderProgramHandle);
        }

        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;
        ShaderProgram& operator=(ShaderProgram&& shaderProgram) = delete;
        
        ~ShaderProgram()
        {
            if (_shaderProgramHandle) {
                glDeleteProgram(_shaderProgramHandle);
            }
        }

        /**
         * Метод необходимый для присоеденения шейлера к шейдерной программе.
         *
         * @param shader шейдер
        */
        template<ShaderTypes ShaderType, template<ShaderTypes> typename T>
        inline void add(const T<ShaderType>& shader) const noexcept
        {
            glAttachShader(_shaderProgramHandle, shader._shaderHandle);
        }

        /**
         * Метод необходимый для присоеденения шейлера к шейдерной программе.
         *
         * @param shaderPtr указатель на шейдер
        */
        template<typename T>
        inline void add(const T& shaderPtr) const noexcept
        {
            glAttachShader(_shaderProgramHandle, shaderPtr->_shaderHandle);
        }

        /**к
         * Метод необходимый для оинковки шейдерной программы.
         *
         * @throw runtime_error в случае если во время линковки произошла ошибка
        */
        inline void link() const
        {
            glLinkProgram(_shaderProgramHandle);

            int32_t lr = 0;

            glGetProgramiv(_shaderProgramHandle, GL_LINK_STATUS, &lr);

            if (!lr) {
                glGetProgramiv(_shaderProgramHandle, GL_INFO_LOG_LENGTH, &lr);

                if (lr > 0) {
                    string msg;
                    int32_t written;

                    msg.reserve(lr);

                    glGetProgramInfoLog(_shaderProgramHandle, lr, &written, &msg[0]);

                    throw runtime_error(msg);
                }
            }
        }

        /**
         * Метод, устанавливающий шейдерную программу как текущую.
        */
        inline void use() const noexcept
        {
            glUseProgram(_shaderProgramHandle);
        }

        /**
         * Метод необходимый для выявления локации у uniform-переменной.
         *
         * @param name имя uniform-переменной
         * @return локацию uniform-переменной или nullopt если такой переменной нет
        */
        optional<int32_t> uniformLocation(const string_view name) const noexcept
        {
            if (int32_t location = glGetUniformLocation(_shaderProgramHandle, name.data()); location != INT32_MAX) {
                return location;
            }
            
            return nullopt;
        }

        /**
         *  Методы для установки значений uniform-переменным в текущей шейдерной программе по локации.
        */
        
        void set_uniform(int32_t location, int32 v) const noexcept
        {
            glUniform1i(location, v);
        }

        void set_uniform(int32_t location, float v) const noexcept
        {
            glUniform1f(location, v);
        }

        void set_uniform(int32_t location, vec2 v) const noexcept
        {
            glUniform2fv(location, 1, &v[0]);
        }

        void set_uniform(int32_t location, ivec2 v) const noexcept
        {
            glUniform2iv(location, 1, &v[0]);
        }

        void set_uniform(int32_t location, vec3 v) const noexcept
        {
            glUniform3fv(location, 1, &v[0]);
        }

        void set_uniform(int32_t location, ivec3 v) const noexcept
        {
            glUniform3iv(location, 1, &v[0]);
        }

        void set_uniform(int32_t location, vec4 v) const noexcept
        {
            glUniform4fv(location, 1, &v[0]);
        }

        void set_uniform(int32_t location, ivec4 v) const noexcept
        {
            glUniform4iv(location, 1, &v[0]);
        }

        void set_uniform(int32_t location, mat2 v) const noexcept
        {
            glUniformMatrix2fv(location, 1, GL_FALSE, &v[0][0]);
        }

        void set_uniform(int32_t location, mat3 v) const noexcept
        {
            glUniformMatrix3fv(location, 1, GL_FALSE, &v[0][0]);
        }
        
        void set_uniform(int32_t location, mat4 v) const noexcept
        {
            glUniformMatrix4fv(location, 1, GL_FALSE, &v[0][0]);
        }

        /**
         *  Метод для установки значений uniform-переменным в текущей шейдерной программе по имени.
         * 
         * @param name имя uniform-переменной
         * @param v то что нужно передать uniform-переменной
        */
        template<typename T>
        void set_uniform(string_view name, T v) const noexcept
        {
            if(auto local = uniformLocation(name); local) {
                set_uniform(*local, v);
            }
        }

    private:
        uint32_t _shaderProgramHandle;
    };
}

