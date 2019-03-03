//
//  ArrayView.inl
//  WOGL
//
//  Created by Асиф Мамедов on 02/03/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include <cstdint>

#include <array>

#include <stdexcept>

#include <memory>

using namespace std;

namespace WOGL
{
    /**
     * Данный класс является невладеющей обёрткой над сущностями
     * ведущими себя как одномерный массив.
     * 
     * @template DataType тип (например float)
    */
    template<typename DataType>
    class ArrayView
    {
    public:
        ArrayView() noexcept :
            _ptr{nullptr},
            _size{0}
        {
        }

        template<size_t N>
        explicit ArrayView(const array<DataType, N>& array) noexcept :
            _size{N},
            _ptr{begin(array)}
        {
        }

        template<size_t N>
        explicit ArrayView(array<DataType, N>& array) noexcept :
            _size{N},
            _ptr{begin(array)}
        {
        }

        template<size_t N>
        explicit ArrayView(const DataType(&array)[N]) noexcept : 
            _ptr{array},
            _size{N}
        {
        }

        template<size_t N>
        explicit ArrayView(DataType(&array)[N]) noexcept : 
            _ptr{array},
            _size{N}
        {
        }

        template<template<typename> typename Conteiner>
        explicit ArrayView(const Conteiner<DataType>& data) noexcept : 
            _ptr{begin(data)},
            _size{data.size()}
        {
        }

        template<template<typename> typename Conteiner>
        explicit ArrayView(Conteiner<DataType>& data) noexcept : 
            _ptr{begin(data)},
            _size{data.size()}
        {
        }

        template<template<typename> typename Ptr>
        explicit ArrayView(const Ptr<DataType>& ptr, size_t size) :
            _size{size}
        {
            if ((_size != 0 && !ptr) || (_size == 0 && ptr)) {
                throw invalid_argument("Passed parameters are not correct");
            }

            _ptr = ptr.get();
        }

        template<template<typename> typename Ptr>
        explicit ArrayView(Ptr<DataType>& ptr, size_t size) :
            _size{size}
        {
            if ((_size != 0 && !ptr) || (_size == 0 && ptr)) {
                throw invalid_argument("Passed parameters are not correct");
            }

            _ptr = ptr.get();
        }

        explicit ArrayView(const unique_ptr<DataType>& ptr, size_t size) :
            _size{size}
        {
            if ((_size != 0 && !ptr) || (_size == 0 && ptr)) {
                throw invalid_argument("Passed parameters are not correct");
            }

            _ptr = ptr.get();
        }

        explicit ArrayView(unique_ptr<DataType>& ptr, size_t size) :
            _size{size}
        {
            if ((_size != 0 && !ptr) || (_size == 0 && ptr)) {
                throw invalid_argument("Passed parameters are not correct");
            }

            _ptr = ptr.get();
        }

        explicit ArrayView(const DataType* ptr, size_t size) :
            _size{size}
        {
            if ((_size != 0 && !ptr) || (_size == 0 && ptr)) {
                throw invalid_argument("Passed parameters are not correct");
            }

            _ptr = ptr;
        }

        explicit ArrayView(DataType* ptr, size_t size) :
            _size{size}
        {
            if ((_size != 0 && !ptr) || (_size == 0 && ptr)) {
                throw invalid_argument("Passed parameters are not correct");
            }
            
            _ptr = ptr;
        }        

        ArrayView(const ArrayView& av) :
            _ptr{av._ptr},
            _size{av._size}
        {
        }

        ArrayView(ArrayView&& av) :
            _ptr{av._ptr},
            _size{av._size}
        {
        }

        ArrayView& operator=(const ArrayView&) = delete;
        ArrayView& operator=(ArrayView&&) = delete;

        /**
         * Методы для изменения указателя и размера.
        */

        template<size_t N>
        void reset(const array<DataType, N>& array) noexcept
        {
            _ptr = begin(array);
            _size = N;
        }

        template<size_t N>
        void reset(array<DataType, N>& array) noexcept
        {
            _ptr = begin(array);
            _size = N;
        }

        template<size_t N>
        void reset(const DataType(&array)[N]) noexcept
        {
            _ptr = array;
            _size = N;
        }

        template<size_t N>
        void reset(DataType(&array)[N]) noexcept
        {
            _ptr = array;
            _size = N;
        }

        template<template<typename> typename Conteiner>
        void reset(const Conteiner<DataType>& data) noexcept
        {
            _ptr = begin(data);
            _size = data.size();
        }

        template<template<typename> typename Conteiner>
        void reset(Conteiner<DataType>& data) noexcept
        {
            _ptr = begin(data);
            _size = data.size();
        }

        template<template<typename> typename Ptr>
        void reset(const Ptr<DataType>& ptr, size_t size)
        {
            if ((size != 0 && !ptr) || (size == 0 && ptr)) {
                throw invalid_argument("Passed parameters are not correct");
            }

            _ptr = ptr.get();
            _size = size;
        }

        template<template<typename> typename Ptr>
        void reset(Ptr<DataType>& ptr, size_t size)
        {
            if ((size != 0 && !ptr) || (size == 0 && ptr)) {
                throw invalid_argument("Passed parameters are not correct");
            }

            _ptr = ptr.get();
            _size = size;
        }

        void reset(const unique_ptr<DataType>& ptr, size_t size) 
        {
            if ((size != 0 && !ptr) || (size == 0 && ptr)) {
                throw invalid_argument("Passed parameters are not correct");
            }

            _ptr = ptr.get();
            _size = size;
        }

        void reset(unique_ptr<DataType>& ptr, size_t size) 
        {
            if ((size != 0 && !ptr) || (size == 0 && ptr)) {
                throw invalid_argument("Passed parameters are not correct");
            }

            _ptr = ptr.get();
            _size = size;
        }

        void reset(const DataType* ptr, size_t size)
        {
            if ((size != 0 && !ptr) || (size == 0 && ptr)) {
                throw invalid_argument("Passed parameters are not correct");
            }

            _ptr = ptr;
            _size = size;
        }

        void reset(DataType* ptr, size_t size)
        {
            if ((size != 0 && !ptr) || (size == 0 && ptr)) {
                throw invalid_argument("Passed parameters are not correct");
            }

            _ptr = ptr;
            _size = size;
        }

        /**
         * Метод с помощь которого можно узнать длинну строки.
         *
         * @return длинна строки
        */
        size_t size() const noexcept
        {
            return _size;
        }

        /**
         * Методы с помощью котроых можно итерироваться.
        */

        const DataType& at(size_t i) const 
        {
            if (i >= _size) {
                throw invalid_argument("Out of range");
            }

            return _ptr[i];
        }

        DataType& at(size_t i) 
        {
            if (i >= _size) {
                throw invalid_argument("Out of range");
            }

            return _ptr[i];
        }

        const DataType& operator[](size_t i) const noexcept
        {
            return _ptr[i];
        }

        DataType& operator[](size_t i) noexcept
        {
            return _ptr[i];
        }

        /**
         * Методы возвращающие указатели на начало и конец строки.
        */

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
            return _ptr + (_size - 1);
        }

        DataType* end() noexcept
        {
            return _ptr + (_size - 1);
        }

    private:
        DataType* _ptr;
        size_t _size;
    };
}
