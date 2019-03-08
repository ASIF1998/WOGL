//
//  Texture.inl
//  WOGL
//
//  Created by Асиф Мамедов on 07/02/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "../../../SOIL2/SOIL2.h"

#include "Conteiners/ArrayView.hpp"
#include "Conteiners/MatrixView.hpp"
#include "Conteiners//GearMatrixView.hpp"

using namespace std;

namespace WOGL
{
    /**
     * @template DataType тип каждого из каналов текстуры(например float)
     * @template Tx тип текселя
    */
    template<typename DataType, TexelType Tx>
    class Texture
    {
        using Data = vector<DataType>;

        friend class InitializeTextureRenderer;
        friend class InitializeCubeMapTextureRenderer;

    public:
        /**
         * Констуктор выделяющий память под текстуру размером width * height * bpp.
         * bpp зависит от того, кого чему равняется tx.
         *
         * @param width ширина текстуры
         * @param height высота текстуры
        */
        explicit Texture(size_t width, size_t height) :
            _height{height},
            _width{width}
        {
            if constexpr (Tx == TexelType::RED) {
                _bpp = 1;
            } else if constexpr (Tx == TexelType::RG) {
                _bpp = 2;
            } else if constexpr (Tx == TexelType::RGB) {
                _bpp = 3;
            } else if constexpr (Tx == TexelType::RGBA) {
                _bpp = 4;
            }

            _data.resize(_height * _width * _bpp);
        }

        /**
         * Конструктор выделяющий память под текстуру размером width * height * bpp и
         * заполняющий её данными из data.
         * 
         * @param data контейнер или указателя с данными (например типа std::vector<float>)
         * @param width ширина текстуры 
         * @param height высота текстуры
        */
        template<typename T>
        explicit Texture(const T& data, size_t width, size_t height)
        {
            if constexpr (Tx == TexelType::RED) {
                _bpp = 1;
            } else if constexpr (Tx == TexelType::RG) {
                _bpp = 2;
            } else if constexpr (Tx == TexelType::RGB) {
                _bpp = 3;
            } else if constexpr (Tx == TexelType::RGBA) {
                _bpp = 4;
            }

            set(data, width, height);
        }

        Texture(const Texture& texture) :
            _data{texture._data},
            _height{texture._height},
            _width{texture._width},
            _bpp{texture._bpp}
        {
        }

        Texture(Texture&& texture)  :
            _data{move(texture._data)},
            _height{texture._height},
            _width{texture._width},
            _bpp{texture._bpp}
        {
        }

        Texture& operator=(const Texture&) = delete;
        Texture& operator=(Texture&&) = delete;

        constexpr size_t width() const noexcept
        {
            return _width;
        }

        constexpr size_t height() const noexcept
        {
            return _height;
        }

        constexpr TexelType texelType() const noexcept
        {
            return Tx;
        }

        /**
         * Метод предназначенный для обновления данных текстуры.
         * 
         * @param data контейнер или указателя с данными (например типа std::vector<float>)
         * @param width ширина текстуры 
         * @param height высота текстуры
        */
        template<typename T>
        void set(const T& data, size_t width, size_t height)
        {
            using VecData = vector<DataType>;
            using UPtrData = unique_ptr<DataType[]>;
            using SPtrData = shared_ptr<DataType[]>;
            using WPtrData = weak_ptr<DataType[]>;

            if constexpr (is_same_v<T, VecData>) {
                if (data.size() != _data.size()) {
                    _data.resize(data.size());
                }

                copy(begin(data), end(data), begin(_data));
            } else if constexpr(is_same_v<T, UPtrData> || is_same_v<T, SPtrData> || is_same_v<T, WPtrData>) {
                if (width * height * _bpp != _data.size()) {
                    _data.reserve(width * height * _bpp);
                }

                copy(data.get(), data.get() + _data.size(), begin(_data));
            }

            _height = height;
            _width = width;
        }

        /**
         * Метод предназначенный для обновления отдельных частей текстуры.
         * 
         * param texture текстурв
         * @param x смещение по столбцу
         * @param y смещение по строке
         * @throw invalid_argument в случае если переданное смещение и размер текстуры не вписываеются в обновляемую текстуру
        */
        void subSet(const Texture& texture, size_t x, size_t y)
        {
            auto& subTex = texture._data;
            size_t subTexHeight = texture._height;
            size_t subTexWidth = texture._width;

            if ((y + subTexHeight) >= _height || (x + subTexWidth) >= _width) {
                throw invalid_argument("The arguments passed do not fit in the original texture");
            }

            for (DataType* ptr {&_data[(y * _width * _bpp) + (x * _bpp)]}; y < subTexHeight; y++, ptr = &_data[(y * _width * _bpp) + (x * _bpp)]) {
                copy(&subTex[y * subTexHeight * _bpp], &subTex[(y * subTexHeight * _bpp) + (x * _bpp)], ptr);
            }
        }

        template<Canal canal>
        DataType at(size_t i, size_t j) const
        {
            auto* ptr = &_data._data.at((i * _width * _bpp) + (j * _bpp));

            if (canal == Canal::GREEN && _bpp > 1) {
                ptr++;
            } else if (canal == Canal::BLUE && _bpp > 2) {
                ptr += 2;
            } else if (canal == Canal::ALPHA && _bpp > 3) {
                ptr += 3;
            }

            return *ptr;
        }

        template<Canal canal>
        DataType& at(size_t i, size_t j)
        {
            auto* ptr = &_data.at((i * _width * _bpp) + (j * _bpp));

            if (canal == Canal::GREEN && _bpp > 1) {
                ptr++;
            } else if (canal == Canal::BLUE && _bpp > 2) {
                ptr += 2;
            } else if (canal == Canal::ALPHA && _bpp > 3) {
                ptr += 3;
            }

            return *ptr;
        }

        template<Canal canal>
        DataType atNoexcept(size_t i, size_t j) const
        {
            auto* ptr = &_data[(i * _width * _bpp) + (j * _bpp)];

            if (canal == Canal::GREEN && _bpp > 1) {
                ptr++;
            } else if (canal == Canal::BLUE && _bpp > 2) {
                ptr += 2;
            } else if (canal == Canal::ALPHA && _bpp > 3) {
                ptr += 3;
            }

            return *ptr;
        }

        template<Canal canal>
        DataType& atNoexcept(size_t i, size_t j)
        {
            auto* ptr = &_data[(i * _width * _bpp) + (j * _bpp)];

            if (canal == Canal::GREEN && _bpp > 1) {
                ptr++;
            } else if (canal == Canal::BLUE && _bpp > 2) {
                ptr += 2;
            } else if (canal == Canal::ALPHA && _bpp > 3) {
                ptr += 3;
            }

            return *ptr;
        }

        /**
         * Функция предназначенная для загрузки изображения.
         *
         * @param path путь до изображения
         * @throw invalid_argument в случае если не удалось загрузить изображение
        */
        static auto loadTexture(const string_view path)
        {
            int32_t width, height;
            uint8_t* imageData;

            if constexpr(Tx == TexelType::RED) {
                imageData = SOIL_load_image(path.data(), &width, &height, nullptr, SOIL_LOAD_L);
            } else if constexpr(Tx == TexelType::RG) {
                imageData = SOIL_load_image(path.data(), &width, &height, nullptr, SOIL_LOAD_LA);
            } else if constexpr(Tx == TexelType::RGB) {
                imageData = SOIL_load_image(path.data(), &width, &height, nullptr, SOIL_LOAD_RGB);
            } else if constexpr(Tx == TexelType::RGBA) {
                imageData = SOIL_load_image(path.data(), &width, &height, nullptr, SOIL_LOAD_RGBA);
            }

            if (!imageData) {
                throw invalid_argument("Failed to load the image at the specified path");
            }

            Texture<DataType, Tx> tex (static_cast<size_t>(width), static_cast<size_t>(height));
            
            if constexpr (is_same_v<DataType, float> || is_same_v<DataType, double>) {
                for (size_t i{0}; i < tex._data.size(); i++) {
                    tex._data[i] = static_cast<DataType>(imageData[i]) / 255.0;
                }
            } else {
                copy(imageData, imageData + (width * height * tex._bpp), begin(tex._data));
            }

            delete [] imageData;
            
            return tex;
        }

        /**
         * Метод возвращающий строку у текстуры.
         * 
         * @param i индекс строки
         * @throw invalid_argument в случае если i больше количества строк 
        */
        auto line(size_t i)
        {
            if (i >= _height) {
                throw invalid_argument("Out of range");
            }

            return ArrayView<DataType>{&_data.at((i * _width * _bpp)), _width};
        }
        
        /**
         * Метод возвращающий строку у текстуры.
         * 
         * @param i индекс строки
         * @return ArrayView<DataType> обёртка над строкой
         * @throw invalid_argument в случае если i больше количества строк 
        */
        const auto line(size_t i) const
        {
            if (i >= _height) {
                throw invalid_argument("Out of range");
            }

            return ArrayView<const DataType>{&_data.at((i * _width * _bpp)), _width};
        }

        /**
         * Метод возвращающий значения текстуры в обёрте MatrixView,
         * которая позволяет работать с текстурой как с 2-мерным массивом.
         * 
         * @return  MatrixView<DataType> обёртка над массивом данных текстуры
        */
        auto textureMatrix() 
        {
            return MatrixView<DataType>{_data, _width * _bpp, _height};
        }

        /**
         * Метод возвращающий значения текстуры в обёрте MatrixView,
         * которая позволяет работать с текстурой как с 2-мерным массивом.
         * 
         * @return  MatrixView<DataType> обёртка над массивом данных текстуры
        */
        const auto textureMatrix() const 
        {
            return MatrixView<const DataType>{_data, _width * _bpp, _height};
        }

        /**
         * Метод позволяющий получить кусок текстуры от исходной текстуры.
         * 
         * @param offsetX смещение по столбцам
         * @param offsetY смещение по строкам
         * @param width ширина подтекстуры (так же ширина ещё зависит от Tx)
         * @param height высота подтекстуры 
         * @return GearMatrixView<DataType> обёртка над подтекстурой
        */
        auto subTexture(size_t offsetX, size_t offsetY, size_t width, size_t height)
        {
            return GearMatrixView<DataType>{_data, _width * _bpp, _height, offsetY, offsetX, width * _bpp, height};
        }

        /**
         * Метод позволяющий получить кусок текстуры от исходной текстуры.
         * 
         * @param offsetX смещение по столбцам
         * @param offsetY смещение по строкам
         * @param width ширина подтекстуры (так же ширина ещё зависит от Tx)
         * @param height высота подтекстуры 
         * @return GearMatrixView<DataType> обёртка над подтекстурой
        */
        const auto subTexture(size_t offsetX, size_t offsetY, size_t width, size_t height) const
        {
            return GearMatrixView<const DataType>{_data, _width * _bpp, _height, offsetY, offsetX, width * _bpp, height};
        }

    private:
        Data _data;
        size_t _height;
        size_t _width;
        size_t _bpp;
    };
}
