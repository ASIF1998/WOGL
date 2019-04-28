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
        
        virtual ~ShaderProgram()
        {
            if (_shaderProgramHandle) {
                glDeleteProgram(_shaderProgramHandle);
            }
        }

        /**
         * Метод необходимый для присоединения шейдера к шейдерной программе.
         *
         * @param shader шейдер
        */
        template<ShaderTypes ShaderType, template<ShaderTypes> typename T>
        inline void add(const T<ShaderType>& shader) const noexcept
        {
            glAttachShader(_shaderProgramHandle, shader._shaderHandle);
        }

        /**
         * Метод необходимый для присоединения шейдера к шейдерной программе.
         *
         * @param shaderPtr указатель на шейдер
        */
        template<typename T>
        inline void add(const T& shaderPtr) const noexcept
        {
            glAttachShader(_shaderProgramHandle, shaderPtr->_shaderHandle);
        }

        /**к
         * Метод необходимый для присоединения шейдера к шейдерной программе.
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
         * Метод определяющий индекс функции в подпрограмме.
         * 
         * @param name имя функции в подпрограмме
         * @return индекс функции в подпрограмме
        */
        template<ShaderTypes St>
        optional<uint32_t> subrotineIndex(string_view name) const noexcept
        {
            if (uint32_t location = glGetSubroutineIndex(_shaderProgramHandle, static_cast<GLenum>(St), name.data()); location != INT32_MAX) {
                return location;
            }

            return nullopt;
        }

        /**
         *  Методы для установки значений uniform-переменным в текущей шейдерной программе по локации.
        */
        
        inline void setUniform(int32_t location, int32_t v) const noexcept
        {
            glUniform1i(location, v);
        }

        inline void setUniform(int32_t location, float v) const noexcept
        {
            glUniform1f(location, v);
        }

        inline void setUniform(int32_t location, vec2 v) const noexcept
        {
            glUniform2fv(location, 1, &v[0]);
        }

        inline void setUniform(int32_t location, ivec2 v) const noexcept
        {
            glUniform2iv(location, 1, &v[0]);
        }

        inline void setUniform(int32_t location, vec3 v) const noexcept
        {
            glUniform3fv(location, 1, &v[0]);
        }

        inline void setUniform(int32_t location, ivec3 v) const noexcept
        {
            glUniform3iv(location, 1, &v[0]);
        }

        inline void setUniform(int32_t location, vec4 v) const noexcept
        {
            glUniform4fv(location, 1, &v[0]);
        }

        inline void setUniform(int32_t location, ivec4 v) const noexcept
        {
            glUniform4iv(location, 1, &v[0]);
        }

        inline void setUniform(int32_t location, mat2 v) const noexcept
        {
            glUniformMatrix2fv(location, 1, GL_FALSE, &v[0][0]);
        }

        inline void setUniform(int32_t location, mat3 v) const noexcept
        {
            glUniformMatrix3fv(location, 1, GL_FALSE, &v[0][0]);
        }

        inline void setUniform(int32_t location, mat4 v) const noexcept
        {
            glUniformMatrix4fv(location, 1, GL_FALSE, &v[0][0]);
        }

        template<template<typename> typename Conteiner>
        inline void setUniform(int32_t location, const Conteiner<int32_t>& v) const noexcept
        {
            glUniform1iv(location, static_cast<GLsizei>(v.size()), &v[0]);
        }

        template<template<typename> typename Conteiner>
        inline void setUniform(int32_t location, const Conteiner<float>& v) const noexcept
        {
            glUniform1fv(location, static_cast<GLsizei>(v.size()), &v[0]);
        }

        template<template<typename> typename Conteiner>
        inline void setUniform(int32_t location, const Conteiner<vec2>& v) const noexcept
        {
            glUniform2fv(location, static_cast<GLsizei>(v.size()), &v[0].x);
        }

        template<template<typename> typename Conteiner>
        inline void setUniform(int32_t location, const Conteiner<ivec2>& v) const noexcept
        {
            glUniform2iv(location, static_cast<GLsizei>(v.size()), &v[0].x);
        }

        template<template<typename> typename Conteiner>
        inline void setUniform(int32_t location, const Conteiner<vec3>& v) const noexcept
        {
            glUniform3fv(location, static_cast<GLsizei>(v.size()), &v[0].x);
        }

        template<template<typename> typename Conteiner>
        inline void setUniform(int32_t location, const Conteiner<ivec3>& v) const noexcept
        {
            glUniform3iv(location, static_cast<GLsizei>(v.size()), &v[0].x);
        }

        template<template<typename> typename Conteiner>
        inline void setUniform(int32_t location, const Conteiner<vec4>& v) const noexcept
        {
            glUniform4fv(location, static_cast<GLsizei>(v.size()), &v[0].x);
        }

        template<template<typename> typename Conteiner>
        inline void setUniform(int32_t location, const Conteiner<ivec4>& v) const noexcept
        {
            glUniform4iv(location, static_cast<GLsizei>(v.size()), &v[0].x);
        }

        template<template<typename> typename Conteiner>
        inline void setUniform(int32_t location, const Conteiner<mat2>& v) const noexcept
        {
            glUniformMatrix2fv(location, static_cast<GLsizei>(v.size()), GL_FALSE, &v[0][0][0]);
        }

        template<template<typename> typename Conteiner>
        inline void setUniform(int32_t location, const Conteiner<mat3>& v) const noexcept
        {
            glUniformMatrix3fv(location, static_cast<GLsizei>(v.size()), GL_FALSE, &v[0][0][0]);
        }

        template<template<typename> typename Conteiner>
        inline void setUniform(int32_t location, const Conteiner<mat4>& v) const noexcept
        {
            glUniformMatrix4fv(location, static_cast<GLsizei>(v.size()), GL_FALSE, &v[0][0][0]);
        }

        template<template<typename, typename> typename Conteiner, typename T>
        inline void setUniform(int32_t location, const Conteiner<int32_t, T>& v) const noexcept
        {
            glUniform1iv(location, static_cast<GLsizei>(v.size()), &v[0]);
        }

        template<template<typename, typename> typename Conteiner, typename T>
        inline void setUniform(int32_t location, const Conteiner<float, T>& v) const noexcept
        {
            glUniform1fv(location, static_cast<GLsizei>(v.size()), &v[0]);
        }

        template<template<typename, typename> typename Conteiner, typename T>
        inline void setUniform(int32_t location, const Conteiner<vec2, T>& v) const noexcept
        {
            glUniform2fv(location, static_cast<GLsizei>(v.size()), &v[0].x);
        }

        template<template<typename, typename> typename Conteiner, typename T>
        inline void setUniform(int32_t location, const Conteiner<ivec2, T>& v) const noexcept
        {
            glUniform2iv(location, static_cast<GLsizei>(v.size()), &v[0].x);
        }

        template<template<typename, typename> typename Conteiner, typename T>
        inline void setUniform(int32_t location, const Conteiner<vec3, T>& v) const noexcept
        {
            glUniform3fv(location, static_cast<GLsizei>(v.size()), &v[0].x);
        }

        template<template<typename, typename> typename Conteiner, typename T>
        inline void setUniform(int32_t location, const Conteiner<ivec3, T>& v) const noexcept
        {
            glUniform3iv(location, static_cast<GLsizei>(v.size()), &v[0].x);
        }

        template<template<typename, typename> typename Conteiner, typename T>
        inline void setUniform(int32_t location, const Conteiner<vec4, T>& v) const noexcept
        {
            glUniform4fv(location, static_cast<GLsizei>(v.size()), &v[0].x);
        }

        template<template<typename, typename> typename Conteiner, typename T>
        inline void setUniform(int32_t location, const Conteiner<ivec4, T>& v) const noexcept
        {
            glUniform4iv(location, static_cast<GLsizei>(v.size()), &v[0].x);
        }

        template<template<typename, typename> typename Conteiner, typename T>
        inline void setUniform(int32_t location, const Conteiner<mat2, T>& v) const noexcept
        {
            glUniformMatrix2fv(location, static_cast<GLsizei>(v.size()), GL_FALSE, &v[0][0][0]);
        }

        template<template<typename, typename> typename Conteiner, typename T>
        inline void setUniform(int32_t location, const Conteiner<mat3, T>& v) const noexcept
        {
            glUniformMatrix3fv(location, static_cast<GLsizei>(v.size()), GL_FALSE, &v[0][0][0]);
        }

        template<template<typename, typename> typename Conteiner, typename T>
        inline void setUniform(int32_t location, const Conteiner<mat4, T>& v) const noexcept
        {
            glUniformMatrix4fv(location, static_cast<GLsizei>(v.size()), GL_FALSE, &v[0][0][0]);
        }

        /**
         *  Метод для установки значений uniform-переменным в текущей шейдерной программе по имени.
         * 
         * @param name имя uniform-переменной
         * @param v то что нужно передать uniform-переменной
        */
        template<typename T>
        inline void setUniform(string_view name, const T& v) const noexcept
        {
            if(auto local = uniformLocation(name); local) {
                setUniform(*local, v);
            }
        }

        /**
         * Метод необходимый для загрузки активной uniform-подпрограммы.
         * 
         * @param location индекс требуемой подпрограммы
        */
        template<ShaderTypes St>
        inline void setUniformSubroutine(uint32_t location) const noexcept
        {
            glUniformSubroutinesuiv(static_cast<GLenum>(St), 1, &location);
        }

        /**
         * Метод необходимый для загрузки активной uniform-подпрограммы.
         * 
         * @param locations индексы требуемых подпрограмм
        */
        template<typename DelType, template<typename, typename> typename Conteiner, ShaderTypes St>
        inline void setUniformSubroutine(const Conteiner<uint32_t, DelType>& locations) const noexcept
        {
            glUniformSubroutinesuiv(static_cast<GLenum>(St), static_cast<int32_t>(locations.size()), &locations[0]);
        }

        /**
         * Метод необходимый для загрузки активной uniform-подпрограммы.
         * 
         * @param locations индексы требуемых подпрограмм
        */
        template<template<typename> typename Conteiner, ShaderTypes St>
        inline void setUniformSubroutine(const Conteiner<uint32_t>& locations) const noexcept
        {
            glUniformSubroutinesuiv(static_cast<GLenum>(St), static_cast<int32_t>(locations.size()), &locations[0]);
        }

        /**
         * Метод необходимый для загрузки активной uniform-подпрограммы.
         * 
         * @param name имя подпрограммы
        */
        template<ShaderTypes St>
        inline void setUniformSubroutine(string_view name) const noexcept
        {
            if (auto location = subrotineIndex<St>(); location) {
                glUniformSubroutinesuiv(static_cast<GLenum>(St), 1, &location);
            }
        }

    private:
        uint32_t _shaderProgramHandle;
    };
}

