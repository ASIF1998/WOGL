//
//  Texture3D.inl
//  WOGL
//
//  Created by Асиф Мамедов on 29/03/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "Texture.hpp"

#include <vector>

#include <stdexcept>

#include "Conteiners/ArrayView.hpp"
#include "Conteiners/MatrixView.hpp"
#include "Conteiners//GearMatrixView.hpp"

using namespace std;

namespace WOGL
{
    template<typename DataType, TexelType Tx>
    class Texture3D
    {
        using Data = vector<DataType>;

        friend class BaseTextureRenderer3D;

    public:
        /**
         * Конструктор.
         * 
         * @param width ширина
         * @param height высота 
         * @param depth глубина
        */
        explicit Texture3D(size_t width, size_t height, size_t depth) :
            _width{width},
            _height{height},
            _depth{depth}
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

            _data.resize(_width * _height * _depth * _bpp);
        }

        /**
         * Конструктор.
         * 
         * @param data данные, которыми необходимо заполнить текстуру
         * @param width ширина
         * @param height высота 
         * @param depth глубина
        */
        template<typename T>
        explicit Texture3D(const T& data, size_t width, size_t height, size_t depth) :
            _width{width},
            _height{height},
            _depth{depth}
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
            
            set(data, width, height, depth);
        }

        Texture3D(const Texture3D& texture) :
            _data{texture._data},
            _width{texture._width},
            _height{texture._height},
            _depth{texture._depth},
            _bpp{texture._bpp}
        {
        }

        Texture3D(Texture3D&& texture) :
            _data{forward<Data>(texture._data)},
            _width{texture._width},
            _height{texture._height},
            _depth{texture._depth},
            _bpp{texture._bpp}
        {
        }

        /**
         * Метод предназначенный для обновления данных текстуры.
         * 
         * @param data данные, которыми необходимо заполнить текстуру
         * @param width ширина
         * @param height высота 
         * @param depth глубина
        */
        template<typename T>
        inline void set(const T& data, size_t width, size_t height, size_t depth)
        {
            size_t dataSize = width * height * depth * _bpp;

            if (_width * _height * _depth * _bpp != dataSize) {
                _data.resize(dataSize);
            }

            _width = width;
            _height = height;
            _depth = depth;

            copy(data.get(), data.get() + dataSize, begin(_data));
        }

        /**
         * Метод предназначенный для обновления данных текстуры.
         * 
         * @param data контейнер, в котором находятся данные, которыми необходимо заполнить текстуру
         * @param width ширина
         * @param height высота 
         * @param depth глубина
         * @throw invalid_argument в случае если переданные размеры(width, height, depth) не совпадают с размерами контейнера с учётом количества активных каналов
        */ 
        template<template<typename> typename Container>
        inline void set(const Container<DataType>& data, size_t width, size_t height, size_t depth)
        {
            if (width * height * depth * _bpp != data.size()) {
                throw invalid_argument("The 'data' size does not match the 'width', 'height' and 'depth', considering the number of active channels");
            }

            if (_width * _height * _depth * _bpp != data.size()) {
                _data.resize(data.size());
            }

            _width = width;
            _height = height;
            _depth = depth;

            copy(begin(data), end(data), begin(_data));
        }

        /**
         * Метод предназначенный для обновления данных текстуры.
         * 
         * @param data контейнер, в котором находятся данные, которыми необходимо заполнить текстуру
         * @param width ширина
         * @param height высота 
         * @param depth глубина
         * @throw invalid_argument в случае если переданные размеры(width, height, depth) не совпадают с размерами контейнера с учётом количества активных каналов
        */ 
        template<typename A, template<typename, typename> typename Container>
        inline void set(const Container<DataType, A>& data, size_t width, size_t height, size_t depth)
        {
            if (width * height * depth * _bpp != data.size()) {
                throw invalid_argument("The 'data' size does not match the 'width', 'height' and 'depth', considering the number of active channels");
            }

            if (_width * _height * _depth * _bpp != data.size()) {
                _data.resize(data.size());
            }

            _width = width;
            _height = height;
            _depth = depth;

            copy(begin(data), end(data), begin(_data));
        }

        /**
         * Метод предназначенный для обновления данных текстуры.
         * 
         * @param texture трёхмерная текстура
        */
        inline void set(const Texture3D& texture) 
        {
            if (_data.size() != texture._data.size()) {
                _data.resize(texture._data.size());
            }

            _width = texture._width;
            _height = texture._height;
            _depth = texture._depth;

            copy(begin(texture._data), end(texture._data), begin(_data));
        }

        /**
         * Метод предназначенный для обновления отдельных частей текстуры.
         * 
         * @param texture трёхмерная текстура
         * @param offZ сдвиг по глубине 
         * @param offY сдвиг по высоте
         * @param offX сдвиг по ширине
         * @throw invalid_argument в случае если во время копирования данных сможет произойти выход за границы 
        */
        inline void subSet(const Texture3D& texture, size_t offZ, size_t offY, size_t offX)
        {
            size_t subTexHeight = texture._height;
            size_t subTexWidth = texture._width;
            size_t subTexDepth = texture._depth;

            if (subTexWidth + offX > _width || subTexHeight + offY > _height || subTexDepth + offZ > _depth) {
                throw invalid_argument("The arguments passed do not fit in the original texture");
            }

            auto& subTex = texture._data;
            DataType* ptr{nullptr};
            const DataType* toCopy{nullptr};


            for (size_t z{0}; z < subTexDepth; z++) {
                for (size_t y{0}; y < subTexHeight; y++) {
                    ptr = &_data[(offZ + z) * _width * _height * _bpp + (offY + y) * _width * _bpp + offX * _bpp];
                    toCopy = &subTex[z * subTexWidth * subTexHeight * _bpp + y * subTexWidth * _bpp];
                    copy(toCopy, toCopy + (subTexWidth - 1) * _bpp, ptr);
                }
            }
        } 

        /**
         * Метод предназначенный для обновления отдельных частей текстуры.
         * 
         * @param data данные, которыми необходимо заполнить текстуру
         * @param width ширина
         * @param height высота 
         * @param depth глубина
         * @param offZ сдвиг по глубине 
         * @param offY сдвиг по высоте
         * @param offX сдвиг по ширине
         * @throw invalid_argument в случае если во время копирования данных сможет произойти выход за границы 
        */
        template<typename T>
        inline void subSet(const T& data, size_t width, size_t height, size_t depth, size_t offZ, size_t offY, size_t offX)
        {
            if (width + offX > _width || height + offY > _height || depth + offZ > _depth) {
                throw invalid_argument("The arguments passed do not fit in the original texture");
            }

            DataType* ptr{nullptr};
            const DataType* toCopy{nullptr};

            for (size_t z{0}; z < depth; z++) {
                for (size_t y{0}; y < height; y++) {
                    ptr = &_data[(offZ + z) * _width * _height * _bpp + (offY + y) * _width * _bpp + offX * _bpp];
                    toCopy = &data[z * width * height * _bpp + y * width * _bpp];
                    copy(toCopy, toCopy + (width - 1) * _bpp, ptr);
                }
            }
        }

        template<Canal canal>
        DataType at(size_t i, size_t j, size_t k) const 
        {
            auto* ptr = &_data.at(k * _width * _height * _bpp + j * _width * _bpp + i * _bpp);

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
        DataType& at(size_t i, size_t j, size_t k) 
        {
            auto* ptr = &_data.at(k * _width * _height * _bpp + j * _width * _bpp + i * _bpp);

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
        DataType atNoexcept(size_t i, size_t j, size_t k) const noexcept
        {
            auto* ptr = &_data[k * _width * _height * _bpp + j * _width * _bpp + i * _bpp];

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
        DataType& atNoexcept(size_t i, size_t j, size_t k) noexcept
        {
            auto* ptr = &_data[k * _width * _height * _bpp + j * _width * _bpp + i * _bpp];

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
         * Метод возвращающий строку у текстуры.
         * 
         * @param i индекс глубины
         * @param j индекс строки
         * @throw invalid_argument в случае если i больше количества строк 
        */
        inline auto line(size_t i, size_t j)
        {
            if (i >= _depth || j >= _width) {
                throw out_of_range("Out of range");
            }

            return ArrayView<DataType>{&_data[i * _width * _height * _bpp + j * _width * _bpp], _width * _bpp};
        }

        /**
         * Метод возвращающий строку у текстуры.
         * 
         * @param i индекс глубины
         * @param j индекс строки
         * @throw invalid_argument в случае если i больше количества строк 
        */
        inline const auto line(size_t i, size_t j) const
        {
            if (i >= _depth || j >= _width) {
                throw out_of_range("Out of range");
            }

            return ArrayView<DataType>{&_data[i * _width * _height * _bpp + j * _width * _bpp], _width * _bpp};
        }

        /**
         * Метод возвращающий значения двухмерной текстуры в обёрте MatrixView,
         * которая позволяет работать с текстурой как с 2-мерным массивом.
         * 
         * @param z глубина
         * @return  MatrixView<DataType> обёртка над массивом данных двухмерной текстуры
        */
        inline auto texture2D(size_t z)
        {
            return GearMatrixView<DataType>{&_data[z * _width * _height * _bpp], _width * _bpp, _height};
        }

        /**
         * Метод возвращающий значения двухмерной текстуры в обёрте MatrixView,
         * которая позволяет работать с текстурой как с 2-мерным массивом.
         * 
         * @param z глубина
         * @return  MatrixView<DataType> обёртка над массивом данных двухмерной текстуры
        */
        inline const auto texture2D(size_t z) const
        {
            return GearMatrixView<DataType>{&_data[z * _width * _height * _bpp], _width * _bpp, _height};
        }

        /**
         * Метод позволяющий получить кусок двухмерной текстуры от исходной текстуры.
         *
         * @param z глубина
         * @param offsetY смещение по строкам
         * @param offsetX смещение по столбцам
         * @param width ширина подтекстуры (так же ширина ещё зависит от Tx)
         * @param height высота подтекстуры 
         * @return GearMatrixView<DataType> обёртка над двухмерной подтекстурой
        */
        inline auto subTexture2D(size_t z, size_t offsetY, size_t offsetX, size_t width, size_t height)
        {
            return GearMatrixView<DataType>{&_data[z * _width * _height * _bpp], _width * _bpp, _height, offsetY, offsetX, width * _bpp, height};
        }

        /**
         * Метод позволяющий получить кусок двухмерной текстуры от исходной текстуры.
         *
         * @param z глубина
         * @param offsetY смещение по строкам
         * @param offsetX смещение по столбцам
         * @param width ширина подтекстуры (так же ширина ещё зависит от Tx)
         * @param height высота подтекстуры 
         * @return const GearMatrixView<DataType> обёртка над двухмерной подтекстурой
        */
        inline const auto subTexture2D(size_t z, size_t offsetY, size_t offsetX, size_t width, size_t height) const
        {
            return GearMatrixView<DataType>{&_data[z * _width * _height * _bpp], _width * _bpp, _height, offsetY, offsetX, width * _bpp, height};
        }

    private:
        Data _data;
        size_t _width;
        size_t _height;
        size_t _depth;
        size_t _bpp;
    };
}
