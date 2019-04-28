//
//  MatrixView.inl
//  WOGL
//
//  Created by Асиф Мамедов on 02/03/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include <cstdint>

#include <stdexcept>

#include <memory>

using namespace std;

namespace WOGL
{
    /**
     * Данный класс является не владеющей обёрткой над сущностями
     * ведущими себя как одномерный массив и позволяет работать с ними
     * как с 2-мерным массивом.
     * 
     * @template DataType тип (например float)
    */
    template<typename DataType>
    class MatrixView
    {
    public:
        template<typename T>
        explicit MatrixView(const T&  data, size_t width, size_t height) :
            _height{height},
            _width{width}
        {
            if constexpr (is_same_v<T, shared_ptr<const DataType>> || is_same_v<T, weak_ptr<const DataType>>) {
                if ((width * height != 0 && !data) || (width * height == 0 && data)) {
                    throw invalid_argument("Passed parameters are not correct");
                }

                _ptr = data.get();
            } else {
                if ((width * height != 0 && data.empty()) || (width * height == 0 && !data.empty())) {
                    throw invalid_argument("Passed parameters are not correct");
                }

                _ptr = &data[0];
            }
        }

        template<typename T>
        explicit MatrixView(T&&  data, size_t width, size_t height) :
            _height{height},
            _width{width}
        {
            if constexpr (is_same_v<T, shared_ptr<DataType>> || is_same_v<T, weak_ptr<DataType>>) {
                if ((width * height != 0 && !data) || (width * height == 0 && data)) {
                    throw invalid_argument("Passed parameters are not correct");
                }

                _ptr = data.get();
            } else {
                if ((width * height != 0 && data.empty()) || (width * height == 0 && !data.empty())) {
                    throw invalid_argument("Passed parameters are not correct");
                }

                _ptr = &data[0];
            }
        }

        explicit MatrixView(const unique_ptr<DataType>& data, size_t width, size_t height) :
            _height{height},
            _width{width}
        {
            if ((width * height != 0 && !data) || (width * height == 0 && data)) {
                throw invalid_argument("Passed parameters are not correct");
            }

            _ptr = data.get();
        }

        explicit MatrixView(unique_ptr<DataType>& data, size_t width, size_t height) :
            _height{height},
            _width{width}
        {
            if ((width * height != 0 && !data) || (width * height == 0 && data)) {
                throw invalid_argument("Passed parameters are not correct");
            }

            _ptr = data.get();
        }

        explicit MatrixView(const DataType* data, size_t width, size_t height) :
            _height{height},
            _width{width}
        {
            if ((width * height != 0 && !data) || (width * height == 0 && data)) {
                throw invalid_argument("Passed parameters are not correct");
            }

            _ptr = data;
        }

        explicit MatrixView(DataType* data, size_t width, size_t height) :
            _height{height},
            _width{width}
        {
            if ((width * height != 0 && !data) || (width * height == 0 && data)) {
                throw invalid_argument("Passed parameters are not correct");
            }

            _ptr = data;
        }

        MatrixView(const MatrixView& mv) :
            _ptr{mv._ptr},
            _height{mv._height},
            _width{mv._width}
        {
        }

        MatrixView(MatrixView&& mv) :
            _ptr{mv._ptr},
            _height{mv._height},
            _width{mv._width}
        {
        }

        size_t width() const noexcept
        {
            return _width;
        }

        size_t height() const noexcept
        {
            return _height;
        }

        MatrixView& operator=(const MatrixView&) = delete;
        MatrixView& operator=(MatrixView&&) = delete;

        const DataType& at(size_t i, size_t j) const 
        {
            if (i >= _height || j >= _width) {
                throw invalid_argument("Crossing the matrix");
            }

            return _ptr[i * _width + j];
        }

        DataType& at(size_t i, size_t j) 
        {
            if (i >= _height || j >= _width) {
                throw invalid_argument("Crossing the matrix");
            }

            return _ptr[i * _width + j];
        }

        const DataType* operator[](size_t i) const noexcept
        {
            return &_ptr[i * _width];
        }

        DataType* operator[](size_t i) noexcept
        {
            return _ptr + i * _width;
        }

        const DataType* cbegin() const noexcept
        {
            return _ptr;
        }

        DataType* begin() noexcept
        {
            return _ptr;
        }

        const DataType* cend() const noexcept
        {
            return _ptr + (_height * _width - 1);
        }

        DataType* end() noexcept
        {
            return _ptr + (_height * _width - 1);
        }

    private:
        DataType* _ptr;
        size_t _height;
        size_t _width;
    };
}
