//
//  Mesh.inl
//  WOGL
//
//  Created by Асиф Мамедов on 11/02/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//
#include <glm/glm.hpp>

#include <vector>

using namespace std;
using namespace glm;

namespace WOGL
{
    enum Attribut
    {
        POSITION,
        NORMAL,
        UV
    };

    struct VertexAttribut
    {
        vec3 position;
        vec3 normal;
        vec2 uv;

        VertexAttribut& operator=(const VertexAttribut& va)
        {
            position = va.position;
            normal = va.normal;
            uv = va.uv;

            return *this;
        }
    };

    class Mesh
    {
        using VertexAttributs = vector<VertexAttribut>;
        using Indices = vector<uint32_t>;

    public:
        /**
         * Конструткор.
         * 
         * @param vertexAttributsCount количество вершин
         * @param indicesCount количество индексов
        */
        explicit Mesh(size_t vertexAttributsCount, size_t indicesCount) :
            _vertexAttributs(vertexAttributsCount),
            _indices(indicesCount)
        {
        }

        /**
         * Конструктор.
         * 
         * @param vertexAttributs контейнер который хранит вершины
         * @param indices контейнер который хранит индексы
        */
        template<template<typename> typename Conteiner>
        explicit Mesh(const Conteiner<VertexAttribut>& vertexAttributs, const Conteiner<uint32_t>& indices) :
            _vertexAttributs{vertexAttributs},
            _indices{indices}
        {
        }

        /**
         * Конструтокр.
         * 
         * @param vertexAttributs указатель на массив с вершинами
         * @param size_1 размер массива vertexAttributs
         * @param indices указатель на массив с индексами
         * @param size_2 размер массива indices
        */
        template<template<typename> typename Ptr>
        explicit Mesh(const Ptr<VertexAttribut[]>& vertexAttributs, size_t size_1, const Ptr<uint32_t[]>& indices, size_t size_2) :
            _vertexAttributs(size_1),
            _indices(size_2)
        {
            copy(vertexAttributs.get(), vertexAttributs.get() + size_1, begin(_vertexAttributs));
            copy(indices.get(), indices.get() + size_2, begin(_indices));
        }

        /**
         * Конструтокр.
         * 
         * @param vertexAttributs указатель на массив с вершинами
         * @param size_1 размер массива vertexAttributs
         * @param indices указатель на массив с индексами
         * @param size_2 размер массива indices
        */
        explicit Mesh(const unique_ptr<VertexAttribut[]>& vertexAttributs, size_t size_1, const unique_ptr<uint32_t[]>& indices, size_t size_2) :
            _vertexAttributs(size_1),
            _indices(size_2)
        {
            copy(vertexAttributs.get(), vertexAttributs.get() + size_1, begin(_vertexAttributs));
            copy(indices.get(), indices.get() + size_2, begin(_indices));
        }

        Mesh(const Mesh& mesh) :
            _vertexAttributs{mesh._vertexAttributs},
            _indices{mesh._indices}
        {
        }

        Mesh(Mesh&& mesh) :
            _vertexAttributs{move(mesh._vertexAttributs)},
            _indices{move(mesh._indices)}
        {
        }

        Mesh& operator=(const Mesh&) = delete;
        Mesh& operator=(Mesh&&) = delete;

        const VertexAttributs& vertexAttributs() const noexcept
        {
            return _vertexAttributs;
        }

        const Indices& indices() const noexcept
        {
            return _indices;
        }

        VertexAttributs& vertexAttributs() noexcept
        {
            return _vertexAttributs;
        }

        Indices& indices() noexcept
        {
            return _indices;
        }

        template<typename T, Attribut attr>
        const T& at(size_t i) const
        {
            VertexAttribut v = _vertexAttributs.at(i);
            T t;

            if constexpr (attr == Attribut::POSITION) {
                t = v.position;
            } else if constexpr (attr == Attribut::NORMAL) {
                t = v.normal;
            } else {
                t = v.uv;
            }

            return t;
        }

        template<typename T, Attribut attr>
        T& at(size_t i)
        {
            VertexAttribut v = _vertexAttributs.at(i);
            T t;

            if constexpr (attr == Attribut::POSITION) {
                t = v.position;
            } else if constexpr (attr == Attribut::NORMAL) {
                t = v.normal;
            } else {
                t = v.uv;
            }

            return t;
        }

    private:
        VertexAttributs _vertexAttributs;
        Indices _indices;
    };
}
