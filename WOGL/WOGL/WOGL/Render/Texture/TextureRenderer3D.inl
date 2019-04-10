//
//  TextureRenderer3D.inl
//  WOGL
//
//  Created by Асиф Мамедов on 09/04/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "TextureRenderer.hpp"
#include "../../Data/Texture3D.hpp"

namespace WOGL
{
    class BaseTextureRenderer3D :
        public BaseTextureRenderer
    {
    protected:
        /**
         * Конструктор.
         * 
         * Выделеннуя в GPU память нельзя будет изменить !!!
         * 
         * @param texture трёхмерная текстура
         * @param tf формат текселя
        */
        template<typename TextureType>
        explicit BaseTextureRenderer3D(const TextureType& texture, TexelFormat tf) :
            BaseTextureRenderer(),
            _width{static_cast<int32_t>(texture._width)},
            _height{static_cast<int32_t>(texture._height)},
            _depth{static_cast<int32_t>(texture._depth)}
        {
            glBindTexture(GL_TEXTURE_3D, _textureRendererHandle);
            glTexStorage3D(GL_TEXTURE_3D, 1, static_cast<GLenum>(tf), _width, _height, _depth);
            update(texture);
            glBindTexture(GL_TEXTURE_3D, 0);
        }

        /**
         * Конструктор.
         * 
         * Выделеннуя в GPU память нельзя будет изменить !!!
         * 
         * @param texture указатель на трёхмерную текстуру
         * @param tf формат текселя
        */
        template<typename TextureType, typename DelType, template<typename, typename> typename Ptr>
        explicit BaseTextureRenderer3D(const Ptr<TextureType, DelType>& texture, TexelFormat tf) :
            BaseTextureRenderer(),
            _width{static_cast<int32_t>(texture->_width)},
            _height{static_cast<int32_t>(texture->_height)},
            _depth{static_cast<int32_t>(texture->_depth)}
        {
            glBindTexture(GL_TEXTURE_3D, _textureRendererHandle);
            glTexStorage3D(GL_TEXTURE_3D, 1, static_cast<GLenum>(tf), _width, _height, _depth);
            update(texture);
            glBindTexture(GL_TEXTURE_3D, 0);
        }

        /**
         * Конструктор.
         * 
         * Выделеннуя в GPU память нельзя будет изменить !!!
         * 
         * @param texture указатель на трёхмерную текстуру
         * @param tf формат текселя
        */
        template<typename TextureType, template<typename> typename Ptr>
        explicit BaseTextureRenderer3D(const Ptr<TextureType>& texture, TexelFormat tf) :
            BaseTextureRenderer(),
            _width{static_cast<int32_t>(texture->_width)},
            _height{static_cast<int32_t>(texture->_height)},
            _depth{static_cast<int32_t>(texture->_depth)}
        {
            glBindTexture(GL_TEXTURE_3D, _textureRendererHandle);
            glTexStorage3D(GL_TEXTURE_3D, 1, static_cast<GLenum>(tf), _width, _height, _depth);
            update(texture);
            glBindTexture(GL_TEXTURE_3D, 0);
        }

        /**
         * Конструктор.
         * 
         * Выделеннуя в GPU память нельзя будет изменить !!!
         * 
         * @param width ширина
         * @param height высота
         * @param depth глубина
         * @param tf формат текселя
        */
        explicit BaseTextureRenderer3D(int32_t width, int32_t height, int32_t depth, TexelFormat tf) :
            BaseTextureRenderer(),
            _width{width},
            _height{height},
            _depth{depth}
        {
            glBindTexture(GL_TEXTURE_3D, _textureRendererHandle);
            glTexStorage3D(GL_TEXTURE_3D, 1, static_cast<GLenum>(tf), _width, _height, _depth);
            glBindTexture(GL_TEXTURE_3D, 0);
        }

        BaseTextureRenderer3D(BaseTextureRenderer3D&& texture) :
            BaseTextureRenderer{move(texture)},
            _width{0},
            _height{0},
            _depth{0}
        {
            swap(_width, texture._width);
            swap(_height, texture._height);
            swap(_depth, texture._depth);
        }

        BaseTextureRenderer3D(const BaseTextureRenderer3D&) = delete;
        BaseTextureRenderer3D& operator=(const BaseTextureRenderer3D&) = delete;
        BaseTextureRenderer3D& operator=(BaseTextureRenderer3D&&) = delete;

    public:
        /**
         * Метод предназначенный для обновления данных объекта.
         * 
         * @param texture трёхмерная текстура
        */
        template<typename DataType, TexelType Tx>
        inline void update(const Texture3D<DataType, Tx>& texture) noexcept
        {
            glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, _width, _height, _depth, static_cast<GLenum>(Tx), _type<DataType>(), &texture._data[0]);
        }

        /**
         * Метод предназначенный для обновления данных объекта.
         * 
         * @param texture указатель на трёхмерную текстуру 
        */
        template<typename DataType, TexelType Tx, typename DelType, template<typename, typename> typename Ptr>
        inline void update(const Ptr<Texture3D<DataType, Tx>, DelType>& texture) noexcept
        {
            glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, _width, _height, _depth, static_cast<GLenum>(Tx), _type<DataType>(), &texture->_data[0]);
        }

        /**
         * Метод предназначенный для обновления данных объекта.
         * 
         * @param texture указатель на трёхмерную текстуру 
        */
        template<typename DataType, TexelType Tx, template<typename> typename Ptr>
        inline void update(const Ptr<Texture3D<DataType, Tx>>& texture) noexcept
        {
            glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, _width, _height, _depth, static_cast<GLenum>(Tx), _type<DataType>(), &texture->_data[0]);
        }

        /**
         * Метод необходимый для определения способа увелечения текстуры.
         *
         * @param mf способа минимизации текстуры
        */
        static inline void magFilter(const TextureFilter mf) noexcept
        {
            glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(mf));
        }
        
        /**
         * Метод необходимый для определения способа минимизации текстуры.
         *
         * @param mf способа увелечения текстуры
        */
        static inline void minFilter(const TextureFilter mf) noexcept
        {
            glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(mf));
        }
        
        /**
         * Метод отвечающий за определение способа оптекания текстуры по оси S(т. е. X).
         *
         * @param ws определяет способ оптекания по оси S(определение TextureWrapping находится выше)
        */
        static inline void textureWrappingS(const TextureWrapping ws) noexcept
        {
            glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, static_cast<GLenum>(ws));
        }

        /**
         * Метод отвечающий за определение способа оптекания текстуры по оси T(т. е. Y).
         *
         * @param wt определяет способ оптекания по оси T(определение TextureWrapping находится выше)
        */
        static inline void textureWrappingT(const TextureWrapping wt) noexcept
        {
            glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, static_cast<GLenum>(wt));
        }

        /**
         * Метод отвечающий за определение способа оптекания текстуры по оси R(т. е. Z).
         *
         * @param wr определяет способ оптекания по оси R(определение TextureWrapping находится выше)
        */
        static inline void textureWrappingR(const TextureWrapping wr) noexcept
        {
            glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, static_cast<GLenum>(wr));
        }
        
        /**
         * Устанавливает режим сравнения текстур для текущих привязанных текстур глубины.
        */
        static inline void textureCompareMode(const TextureCompareMode cm) noexcept
        {
            glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_COMPARE_MODE, static_cast<GLenum>(cm));
        }
        
        /**
         * Устанавливает оператор сравнения, используемый, когда для TextureCompareMode установлено значение COMPARE_REF_TO_TEXTURE.
        */
        static inline void textureCompareFunc(const TextureCompareFunc cf) noexcept
        {
            glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_COMPARE_FUNC, static_cast<GLenum>(cf));
        }

        /**
         * Метод делающий текстуру не текущей.
         * По сути он деалет текущую цель привязки нулевой.
        */
        inline static void unbind() noexcept
        {
            glBindTexture(GL_TEXTURE_3D, 0);
        }

        int32_t width() const noexcept
        {
            return _width;
        }

        int32_t height() const noexcept
        {
            return _height;
        }

        int32_t depth() const noexcept
        {
            return _depth;
        }

    protected:
        int32_t _width;
        int32_t _height;
        int32_t _depth;
    };

    template<TexelFormat Tf>
    class TextureRenderer3D :
        public BaseTextureRenderer3D
    {
    public:
        /**
         * Конструктор.
         * 
         * @param texture трёхмерная текстура
        */
        template<typename TextureType>
        explicit TextureRenderer3D(const TextureType& texture) :
            BaseTextureRenderer3D{texture, Tf}
        {
        }

        /**
         * Конструктор.
         * 
         * @param width ширина 
         * @param height высота
         * @param depth глубина
        */
        explicit TextureRenderer3D(int32_t width, int32_t height, int32_t depth) :
            BaseTextureRenderer3D{width, height, depth, Tf}
        {
        }

        TextureRenderer3D(TextureRenderer3D&& texture) :
            BaseTextureRenderer3D{forward<BaseTextureRenderer3D>(texture)}
        {
        }

        TextureRenderer3D(const TextureRenderer3D&) = delete;
        TextureRenderer3D& operator=(TextureRenderer3D&&) = delete;
        TextureRenderer3D& operator=(const TextureRenderer3D&) = delete; 

        /**
         * Метод необходимый для генерации mipmap'а.
        */
         virtual inline void genMipmap() const noexcept override
         {
            glGenerateMipmap(GL_TEXTURE_3D);
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
                glBindTexture(GL_TEXTURE_3D, _textureRendererHandle);
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
