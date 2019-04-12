//
//  TextureRenderer.inl
//  WOGL
//
//  Created by Асиф Мамедов on 09/02/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include <GL/glew.h>

#include "../../Data/Texture2D.hpp"
#include "TextureMappingSetting.hpp"

#include "TextureRenderer.hpp"

namespace WOGL
{
    template<TexelFormat>
    class BaseFramebuffer;
    
    class BaseTextureRenderer2D :
        public BaseTextureRenderer
    {
        friend class InitializeCubeMapTextureRenderer;
        
    protected:
        /**
         * Конструктор.
         *
         * Выделеннуя в GPU память нельзя будет изменить !!!
         *
         * @param texture текстура
         * @param tf формат текселя
        */
        template<typename TextureType>
        explicit BaseTextureRenderer2D(const TextureType& texture, TexelFormat tf) :
            BaseTextureRenderer(),
            _height{static_cast<int32_t>(texture._height)},
            _width{static_cast<int32_t>(texture._height)}
        {
            glBindTexture(GL_TEXTURE_2D, _textureRendererHandle);
            glTexStorage2D(GL_TEXTURE_2D, 1, static_cast<GLenum>(tf), _height, _width);
            update(texture);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        /**
         * Конструктор.
         *
         * Выделеннуя в GPU память нельзя будет изменить !!!
         *
         * @param texture указатель на текстуру
         * @param tf формат текселя
        */
        template<typename TextureType, typename DelType, template<typename, typename> typename Ptr>
        explicit BaseTextureRenderer2D(const Ptr<TextureType, DelType>& texture, TexelFormat tf) :
            BaseTextureRenderer(),
            _height{static_cast<int32_t>(texture->_height)},
            _width{static_cast<int32_t>(texture->_height)}
        {
            glBindTexture(GL_TEXTURE_2D, _textureRendererHandle);
            glTexStorage2D(GL_TEXTURE_2D, 1, static_cast<GLenum>(tf), _height, _width);
            update(texture);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        /**
         * Конструктор.
         *
         * Выделеннуя в GPU память нельзя будет изменить !!!
         *
         * @param texture указатель на текстуру
         * @param tf формат текселя
        */
        template<typename TextureType, template<typename> typename Ptr>
        explicit BaseTextureRenderer2D(const Ptr<TextureType>& texture, TexelFormat tf) :
            BaseTextureRenderer(),
            _height{static_cast<int32_t>(texture->_height)},
            _width{static_cast<int32_t>(texture->_height)}
        {
            glBindTexture(GL_TEXTURE_2D, _textureRendererHandle);
            glTexStorage2D(GL_TEXTURE_2D, 1, static_cast<GLenum>(tf), _height, _width);
            update(texture);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        /**
         * Конструктор.
         * 
         * @param width ширина 
         * @param height высота
         * @param tf формат текскля
         * @throw в случае если width или height равны нулю
        */
        explicit BaseTextureRenderer2D(int32_t width, int32_t height, TexelFormat tf) :
            BaseTextureRenderer(),
            _height{height},
            _width{width}
        {
            assert(!(width == 0 || _height == 0));

            glBindTexture(GL_TEXTURE_2D, _textureRendererHandle);
            glTexStorage2D(GL_TEXTURE_2D, 1, static_cast<GLenum>(tf), _height, _width);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        BaseTextureRenderer2D(BaseTextureRenderer2D&& texture) :
            BaseTextureRenderer{move(texture)},
            _height{texture._height},
            _width{texture._width}
        {
        }

        BaseTextureRenderer2D(const BaseTextureRenderer2D&) = delete;
        BaseTextureRenderer2D& operator=(const BaseTextureRenderer2D&) = delete;
        BaseTextureRenderer2D& operator=(BaseTextureRenderer2D&&) = delete;

        virtual ~BaseTextureRenderer2D()
        {
            if (_textureRendererHandle) {
                glDeleteTextures(1, &_textureRendererHandle);
            }
        }

    public:
        /**
         * Метод предназначенный для обновления данных объекта.
         *
         * @param texture текстура котороя будет помещена в памя GPU
        */
        template<typename DataType, TexelType Tx>
        void update(const Texture2D<DataType, Tx>& texture)
        {
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _height, _width, static_cast<GLenum>(Tx), _type<DataType>(), &texture._data[0]);
        }

        /**
         * Метод предназначенный для обновления данных объекта.
         *
         * @param texture указатель на текстуру котороя будет помещена в памя GPU
        */
        template<typename DataType, TexelType Tx, typename DelType, template<typename, typename> typename Ptr>
        void update(const Ptr<Texture2D<DataType, Tx>, DelType>& texture)
        {
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _height, _width, static_cast<GLenum>(Tx), _type<DataType>(), &texture->_data[0]);
        }

        /**
         * Метод предназначенный для обновления данных объекта.
         *
         * @param texture указатель на текстуру котороя будет помещена в памя GPU
        */
        template<typename DataType, TexelType Tx, template<typename> typename Ptr>
        void update(const Ptr<Texture2D<DataType, Tx>>& texture)
        {
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _height, _width, static_cast<GLenum>(Tx), _type<DataType>(), &texture->_data[0]);
        }
        /**
         * Метод необходимый для определения способа увелечения текстуры.
         *
         * @param mf способа минимизации текстуры
        */
        static inline void magFilter(const TextureFilter mf) noexcept
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(mf));
        }
        
        /**
         * Метод необходимый для определения способа минимизации текстуры.
         *
         * @param mf способа увелечения текстуры
        */
        static inline void minFilter(const TextureFilter mf) noexcept
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(mf));
        }
        
        /**
         * Метод отвечающий за определение способа оптекания текстуры по оси S(т. е. X).
         *
         * @param ws определяет способ оптекания по оси S(определение TextureWrapping находится выше)
        */
        static inline void textureWrappingS(const TextureWrapping ws) noexcept
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLenum>(ws));
        }
        
        /**
         * Метод отвечающий за определение способа оптекания текстуры по оси T(т. е. Y).
         *
         * @param wt определяет способ оптекания по оси T(определение TextureWrapping находится выше)
        */
        static inline void textureWrappingT(const TextureWrapping wt) noexcept
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLenum>(wt));
        }
        
        /**
         * Устанавливает режим сравнения текстур для текущих привязанных текстур глубины.
        */
        static inline void textureCompareMode(const TextureCompareMode cm) noexcept
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, static_cast<GLenum>(cm));
        }
        
        /**
         * Устанавливает оператор сравнения, используемый, когда для TextureCompareMode установлено значение COMPARE_REF_TO_TEXTURE.
        */
        static inline void textureCompareFunc(const TextureCompareFunc cf) noexcept
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, static_cast<GLenum>(cf));
        }

        /**
         * Метод делающий текстуру не текущей.
         * По сути он деалет текущую цель привязки нулевой.
        */
        inline static void unbind() noexcept
        {
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        inline int32_t width() const noexcept
        {
            return _width;
        }

        inline int32_t height() const noexcept
        {
            return _height;
        }

    protected:
        int32_t _height;
        int32_t _width;
    };

    template<TexelFormat Tf>
    class TextureRenderer2D :
        public BaseTextureRenderer2D
    {
        friend class BaseFramebuffer<Tf>;

    public:
        /**
         * Конструктор.
         *
         * Выделеннуя в GPU память нельзя будет изменить !!!
         *
         * @param texture текстура
        */
        template<typename TextureType>
        explicit TextureRenderer2D(const TextureType& texture) :
            BaseTextureRenderer2D(texture, Tf)
        {
        }

        /**
         * Конструктор.
         * 
         * @param width ширина 
         * @param height высота
        */
        explicit TextureRenderer2D(int32_t width, int32_t height) :
            BaseTextureRenderer2D(width, height, Tf)
        {
        }

        TextureRenderer2D(TextureRenderer2D&& tr) :
            BaseTextureRenderer2D(forward<BaseTextureRenderer2D>(tr))
        {

        }
        TextureRenderer2D(const TextureRenderer2D&) = delete;
        TextureRenderer2D& operator=(const TextureRenderer2D&) = delete;
        TextureRenderer2D& operator=(TextureRenderer2D&&) = delete;

        /**
         * Метод необходимый для генерации mipmap'а.
        */
         virtual inline void genMipmap() const noexcept override
         {
            glGenerateMipmap(GL_TEXTURE_2D);
         }

        /**
         * Метод делающий текстуру текущей.
         * 
         * @param slot текстурный слот
        */
         virtual inline void bind(int32_t slot) const noexcept override
         {
            if (slot >= 0) {
                glActiveTexture(GL_TEXTURE0 + slot);
                glBindTexture(GL_TEXTURE_2D, _textureRendererHandle);
            }
         }

        /**
         * Метод возвращающий формат текселя.
         * 
         * @return формат текселя
        */
         virtual inline TexelFormat texelFormat() const noexcept override
         {
            return Tf;
         }

        /**
         * Метод возвращающий формат текселя.
         * 
         * @return формат текселя
        */
         inline constexpr TexelFormat cTexelFormat() const noexcept
         {
            return Tf;
         }
    };
}

