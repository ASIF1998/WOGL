//
//  TextureRenderer.inl
//  WOGL
//
//  Created by Асиф Мамедов on 05/04/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include <GL/glew.h>

namespace WOGL
{
    class ITextureRenderer
    {
    public:
        /**
         * Метод необходимый для генерации mipmap'а.
        */
        virtual inline void genMipmap() const noexcept = 0;

        /**
         * Метод делающий текстуру текущей.
         * 
         * @param slot текстурный слот
        */
        virtual inline void bind(int32_t slot) const noexcept = 0;

        /**
         * Метод возвращаюй формат текселя.
         * 
         * @return формат текселя
        */
        virtual inline TexelFormat texelFormat() const noexcept = 0;
    };

    class BaseTextureRenderer :
        public ITextureRenderer
    {
    public:
        inline BaseTextureRenderer()
        {
            glGenTextures(1, &_textureRendererHandle);

            if (!_textureRendererHandle) {
                throw runtime_error("Error create texture renderer handle");
            }
        }

        inline BaseTextureRenderer(BaseTextureRenderer&& texture) :
            _textureRendererHandle{0}
        {
            swap(_textureRendererHandle, texture._textureRendererHandle);
        }

        virtual ~BaseTextureRenderer() noexcept
        {
            if (_textureRendererHandle) {
                glDeleteTextures(1, &_textureRendererHandle);
            } 
        }

        BaseTextureRenderer(const BaseTextureRenderer&) = delete;
        BaseTextureRenderer& operator=(const BaseTextureRenderer&) = delete;
        BaseTextureRenderer& operator=(BaseTextureRenderer&&) = delete;

        /**
         * Метод возвращающий дескриптор текстуры.
         * Данный метод не сделан константным так как пользователь сможет повлиять на текстуру с помощью функций OpenGL.
         * 
         * @return дескриптор текстуры
        */
        uint32_t id() noexcept
        {
            return _textureRendererHandle;
        }

    protected:
        template<typename T>
        static GLenum _type() noexcept
        {
            GLenum type = GL_FLOAT;

            if constexpr (is_same_v<T, int8_t>) {
                type = GL_BYTE;
            } else if constexpr (is_same_v<T, int16_t>) {
                type = GL_SHORT;
            } else if constexpr (is_same_v<T, int32_t>) {
                type = GL_INT;
            } else if constexpr (is_same_v<T, uint8_t>) {
                type = GL_UNSIGNED_BYTE;
            } else if constexpr (is_same_v<T, uint16_t>) {
                type = GL_UNSIGNED_SHORT;
            } else if constexpr (is_same_v<T, uint32_t>) {
                type = GL_UNSIGNED_INT;
            } 

            return type;
        }

        uint32_t _textureRendererHandle;
    };
}
