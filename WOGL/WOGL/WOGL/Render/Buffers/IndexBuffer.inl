//
//  IndexBuffer.inl
//  WOGL
//
//  Created by Асиф Мамедов on 06/02/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include <GL/glew.h>

#include <stdexcept>

#include <array>
#include <vector>
#include <initializer_list>

#include <memory>

using namespace std;

namespace WOGL
{
    class IndexBuffer
    {
        friend class Context;

    public:
        /**
         * Конструктор который создаёт дескриптор индексного буффера и помещает туда данные.
         *
         * @param data массив данных
         * @template Size размер массива
        */
        template<size_t Size>
        explicit IndexBuffer(const uint32_t(&data)[Size]) :
            _size{static_cast<int32_t>(Size)}
        {
            _createHandle();

            bind();
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, Size * sizeof(uint32_t), data, GL_STATIC_DRAW);
            unbind();
        }

        /**
         * Конструктор который создаёт дескриптор индексного буффера и помещает туда данные.
         *
         * @param data объект типа std::array
         * @template Size размер массива внутри объекта std::array
        */
        template<size_t Size>
        explicit IndexBuffer(const array<uint32_t, Size>& data) :
            _size{static_cast<int32_t>(Size)}
        {
            _createHandle();

            bind();
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, Size * sizeof(uint32_t), &data[0], GL_STATIC_DRAW);
            unbind();
        }

        /**
         * Конструктор который создаёт дескриптор индексного буффера и помещает туда данные.
         *
         * @param data объект типа std::initializer_list
        */
        explicit IndexBuffer(const initializer_list<uint32_t>& data) :
            _size{static_cast<int32_t>(data.size())}
        {
            _createHandle();

            bind();
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, _size * sizeof(uint32_t), begin(data), GL_STATIC_DRAW);
            unbind();
        }

        /**
         * Конструктор который создаёт дескриптор индексного буффера и помещает туда данные.
         * Тип контейнера или указателя (T) должен содержать данные типа uint32_t или unsigned int !!!
         *
         * @param data контейнер или указателя с данными (например типа std::vector<uint32_t>)
        */
        template<typename T>
        explicit IndexBuffer(const T& data) :
            _size{static_cast<int32_t>(data.size())}
        {
            _createHandle();
            
            bind();
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, _size * sizeof(uint32_t), &data[0], GL_STATIC_DRAW);
            unbind();
        }

        /**
         * Конструктор который создаёт дескриптор индексного буффера и помещает туда данные.
         * Тип контейнера или указателя (T) должен содержать данные типа uint32_t или unsigned int !!!
         *
         * @param data контейнер или указателя с данными (например типа std::unique_ptr<uint32_t[]>)
        */
        template<typename T>
        explicit IndexBuffer(const T& data, size_t size) :
            _size{static_cast<int32_t>(size)}
        {
            _createHandle();

            if (data && size) {
                bind();
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, _size * sizeof(uint32_t), data.get(), GL_STATIC_DRAW);
                unbind();
            }
        }

        IndexBuffer(IndexBuffer&& buffer) :
            _indexBufferHandle{0},
            _size{buffer._size}
        {
            swap(_indexBufferHandle, buffer._indexBufferHandle);
        }

        IndexBuffer(const IndexBuffer&) = delete;
        IndexBuffer& operator=(const IndexBuffer&) = delete;
        IndexBuffer& operator=(IndexBuffer&&) = delete;

        virtual ~IndexBuffer()
        {
            if (_indexBufferHandle) {
                glDeleteBuffers(1, &_indexBufferHandle);
            }
        }

        /**
         * Метод делающий индексный буффер текущим.
        */
        inline void bind() const noexcept
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferHandle);
        }

        /**
         * Метод делающий индексный буффер не текущим. По сути он деалет текущую цель привязки нулевой.
        */
        static inline void unbind() noexcept
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

        int32_t size() const noexcept
        {
            return _size;
        }

        /**
         * Метод возвращающий дескриптор EBO.
         * Данный метод не сделан константным так как пользователь сможет повлиять на EBO с помощью функций OpenGL.
         *
         * @return дескриптор EBO
         */
        uint32_t id() noexcept
        {
            return _indexBufferHandle;
        }

    private:
        inline void _createHandle()
        {
            glGenBuffers(1, &_indexBufferHandle);

            if (!_indexBufferHandle) {
                throw runtime_error("Error create index buffer handle");
            }
        }

        uint32_t _indexBufferHandle;
        int32_t _size;
    };
}
