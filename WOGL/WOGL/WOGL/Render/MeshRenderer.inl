//
//  MeshRenderer.inl
//  WOGL
//
//  Created by Асиф Мамедов on 15/02/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include <memory>

#include "Buffers/Buffers.hpp"
#include "VertexArray.hpp"

#include "../Data/Mesh.hpp"

#include <stdexcept>

#include <GL/glew.h>

#include <vector>

using namespace std;

namespace WOGL
{
    class MeshRenderer
    {
        using VertexBuffers = vector<unique_ptr<IVertexBuffer>>;

    public:
        /**
         * Конструктор.
         * 
         * @param mesh меш
         * @param posAttribIndex индекс атрибута позиции
         * @param normalAttribIndx индекс атрибута нормал
         * @param texCoordAttribIndx индекс атрибута текстурной координаты
         * @param tangAttribIndx индекс атрибута касательной
         * @throw runtime_error в случае если не удалось создать дескриптор основного вершинного буфера
        */
        explicit MeshRenderer(const Mesh& mesh, uint32_t posAttribIndex = 0, uint32_t normalAttribIndx = 1, uint32_t texCoordAttribIndx = 2, uint32_t tangAttribIndx = 3) :
            _ebo{mesh._indices},
            _buffers(0)
        {
            _createVBO();

            _vao.bind();
            glBindBuffer(GL_ARRAY_BUFFER, _vbo);
            _ebo.bind();

            glBufferData(GL_ARRAY_BUFFER, mesh._vertices.size() * sizeof(Vertex), &mesh._vertices[0], GL_STATIC_DRAW);

            glEnableVertexAttribArray(posAttribIndex);
            glEnableVertexAttribArray(normalAttribIndx);
            glEnableVertexAttribArray(texCoordAttribIndx);
            glEnableVertexAttribArray(tangAttribIndx);

            glVertexAttribPointer(posAttribIndex, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
            glVertexAttribPointer(normalAttribIndx, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
            glVertexAttribPointer(texCoordAttribIndx, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
            glVertexAttribPointer(tangAttribIndx, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tang));

            _ebo.unbind();
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            _vao.bind();
        }

        /**
         * Конструктор.
         * 
         * @param ptrMesh указатель на меш
         * @param posAttribIndex индекс атрибута позиции
         * @param normalAttribIndx индекс атрибута нормал
         * @param texCoordAttribIndx индекс атрибута текстурной координаты
         * @param tangAttribIndx индекс атрибута касательной
         * @throw runtime_error в случае если не удалось создать дескриптор основного вершинного буфера
        */
        explicit MeshRenderer(const unique_ptr<Mesh>& ptrMesh, uint32_t posAttribIndex = 0, uint32_t normalAttribIndx = 1, uint32_t texCoordAttribIndx = 2, uint32_t tangAttribIndx = 3) :
            MeshRenderer(*ptrMesh, posAttribIndex, normalAttribIndx, texCoordAttribIndx, tangAttribIndx)
        {
        }

        /**
         * Конструктор.
         * 
         * @param ptrMesh указатель на меш
         * @param posAttribIndex индекс атрибута позиции
         * @param normalAttribIndx индекс атрибута нормал
         * @param texCoordAttribIndx индекс атрибута текстурной координаты
         * @param tangAttribIndx индекс атрибута касательной
         * @throw runtime_error в случае если не удалось создать дескриптор основного вершинного буфера
        */
        template<template<typename> typename PtrMesh> 
        explicit MeshRenderer(const PtrMesh<Mesh>& ptrMesh, uint32_t posAttribIndex = 0, uint32_t normalAttribIndx = 1, uint32_t texCoordAttribIndx = 2, uint32_t tangAttribIndx = 3) :
            MeshRenderer(*ptrMesh, posAttribIndex, normalAttribIndx, texCoordAttribIndx, tangAttribIndx)
        {
        }

        MeshRenderer(MeshRenderer&& mr) :
            _ebo{move(mr._ebo)},
            _vao{move(mr._vao)},
            _vbo{0}
        {
            swap(_vbo, mr._vbo);
        }

        MeshRenderer(const MeshRenderer&) = delete;
        MeshRenderer& operator=(const MeshRenderer&) = delete;
        MeshRenderer& operator=(MeshRenderer&&) = delete;

        virtual ~MeshRenderer()
        {
            if (_vbo) {
                glDeleteBuffers(1, &_vbo);
            }
        }

        const IndexBuffer& indices() const noexcept
        {
            return _ebo;
        }

        IndexBuffer& indices() noexcept
        {
            return _ebo;
        }

        const VertexArray& vertexArray() const noexcept
        {
            return _vao;
        }

        VertexArray& vertexArray() noexcept
        {
            return _vao;
        }

        /**
         * Метод делающий основной вершинный буффер текущим.
        */
        inline void bindBaseVertexBuffer() const noexcept
        {
            glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        }

        /**
         * Метод делающий основной вершинный буффер не текущим. 
         * По сути он деалет текущую цель привязки нулевой.
        */
        inline void unbindBaseVertexBuffer() noexcept
        {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        /**
         * Метод позволяющий узнать если дополнительные вершинные буферы.
         * 
         * @return true - если что то есть, иначе false
        */
        bool hasAdditionalBuffers() const noexcept
        {
            return !_buffers.empty();
        }

        /**
         * Метод позволяющий добавлять дополнительные вершинные буферы.
         * 
         * @param buffer вершинный буффер
         * @param vboIndx индекс атрибута 
         * @template NumComponent количество компонентов в атрибуте (напрример в vec4 4-е компонента)
        */
        template<int32_t NumComponent>
        void add(const VertexBuffer<NumComponent>& buffer, int32_t vboIndx)
        {
            size_t i = _buffers.size();

            _buffers.push_back(unique_ptr<IVertexBuffer>(new VertexBuffer<NumComponent>(buffer)));
            _buffers.shrink_to_fit();

            _vao.bind();
            _buffers[i]->bind();

            glEnableVertexAttribArray(vboIndx);
            glVertexAttribPointer(vboIndx, NumComponent, GL_FLOAT, GL_FALSE, 0, nullptr);

            VertexBuffer<NumComponent>::unbind();
            _vao.unbind();
        }

        /**
         * Метод позволяющий добавлять дополнительные вершинные буферы.
         * 
         * @param buffer вершинный буффер
         * @param vboIndx индекс атрибута 
         * @template NumComponent количество компонентов в атрибуте (напрример в vec4 4-е компонента)
        */
        template<int32_t NumComponent>
        void add(VertexBuffer<NumComponent>&& buffer, int32_t vboIndx)
        {
            size_t i = _buffers.size();

            _buffers.push_back(unique_ptr<IVertexBuffer>(new VertexBuffer<NumComponent>(move(buffer))));
            _buffers.shrink_to_fit();

            _vao.bind();
            _buffers[i]->bind();

            glEnableVertexAttribArray(vboIndx);
            glVertexAttribPointer(vboIndx, NumComponent, GL_FLOAT, GL_FALSE, 0, nullptr);

            VertexBuffer<NumComponent>::unbind();
            _vao.unbind();
        }

        IVertexBuffer& additionalVertexBuffer(size_t i) 
        {
            return *_buffers.at(i);
        }

        const IVertexBuffer& additionalVertexBuffer(size_t i) const 
        {
            return *_buffers.at(i);
        }

        void draw(int32_t numberRepetitions) const noexcept
        {
            _vao.bind();
            _ebo.bind();
            glDrawElementsInstanced(GL_TRIANGLES, _ebo.size(), GL_UNSIGNED_INT, nullptr, numberRepetitions);
            _ebo.unbind();
            _vao.unbind();
        }

    private:
        inline void _createVBO()
        {
            glGenBuffers(1, &_vbo);

            if (!_vbo) {
                throw runtime_error("Error create vertex buffer handle");
            }
        }

        IndexBuffer _ebo;
        VertexArray _vao;
        uint32_t _vbo;
        VertexBuffers _buffers;
    };
}
