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
    struct Vertex
    {
        vec3 position;
        vec3 normal;
        vec3 tang;
        vec2 uv;

        Vertex& operator=(const Vertex& v)
        {
            position = v.position;
            normal = v.normal;
            tang = v.tang;
            uv = v.uv;

            return *this;
        }
    };

    class Mesh
    {
        using Vertices = vector<Vertex>;
        using Indices = vector<uint32_t>;

        friend class InitializeModelMesh;
        friend class MeshRenderer;

    public:
        /**
         * Конструткор.
         * 
         * @param vertexAttributsCount количество вершин
         * @param indicesCount количество индексов
        */
        explicit Mesh(size_t vertexAttributsCount, size_t indicesCount) :
            _vertices(vertexAttributsCount),
            _indices(indicesCount)
        {
        }

        /**
         * Конструктор.
         * 
         * @param vertexAttributs вершины
         * @param indices индексы
        */
        explicit Mesh(const Vertices& vertexAttributs, const Indices& indices) :
            _vertices{vertexAttributs},
            _indices{indices}
        {
        }

        /**
         * Конструктор.
         * 
         * @param vertexAttributs вершины
         * @param indices индексы
        */
        explicit Mesh(Vertices&& vertexAttributs, Indices&& indices) :
            _vertices{forward<Vertices>(vertexAttributs)},
            _indices{forward<Indices>(indices)}
        {
        }

        /**
         * Конструктор.
         * 
         * @param vertexAttributs контейнер который хранит вершины
         * @param indices контейнер который хранит индексы
        */
        template<template<typename> typename Conteiner>
        explicit Mesh(const Conteiner<Vertices>& vertexAttributs, const Conteiner<uint32_t>& indices) :
            _vertices{vertexAttributs},
            _indices{indices}
        {
        }

        /**
         * Конструктор.
         * 
         * @param vertexAttributs контейнер который хранит вершины
         * @param indices контейнер который хранит индексы
        */
        template<template<typename, typename> typename Conteiner, typename A>
        explicit Mesh(const Conteiner<Vertices, A>& vertexAttributs, const Conteiner<uint32_t, A>& indices) :
            _vertices{vertexAttributs},
            _indices{indices}
        {
        }
        
        /**
         * Конструктор.
         *
         * @param vertexAttributs контейнер который хранит вершины
         * @param indices контейнер который хранит индексы
         */
        template<template<typename> typename Conteiner>
        explicit Mesh(Conteiner<Vertices>&& vertexAttributs, Conteiner<uint32_t>&& indices) :
            _vertices{forward<Conteiner<Vertices>>(vertexAttributs)},
            _indices{forward<Conteiner<uint32_t>>(indices)}
        {
        }

        /**
         * Конструктор.
         *
         * @param vertexAttributs контейнер который хранит вершины
         * @param indices контейнер который хранит индексы
         */
        template<template<typename, typename> typename Conteiner, typename A>
        explicit Mesh(Conteiner<Vertices, A>&& vertexAttributs, Conteiner<uint32_t, A>&& indices) :
            _vertices{forward<Conteiner<Vertices, A>>(vertexAttributs)},
            _indices{forward<Conteiner<uint32_t, A>>(indices)}
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
        explicit Mesh(const Ptr<Vertex[]>& vertexAttributs, size_t size_1, const Ptr<uint32_t[]>& indices, size_t size_2) :
            _vertices(size_1),
            _indices(size_2)
        {
            copy(vertexAttributs.get(), vertexAttributs.get() + size_1, begin(_vertices));
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
        explicit Mesh(const unique_ptr<Vertex[]>& vertexAttributs, size_t size_1, const unique_ptr<uint32_t[]>& indices, size_t size_2) :
            _vertices(size_1),
            _indices(size_2)
        {
            copy(vertexAttributs.get(), vertexAttributs.get() + size_1, begin(_vertices));
            copy(indices.get(), indices.get() + size_2, begin(_indices));
        }

        Mesh(const Mesh& mesh) :
            _vertices{mesh._vertices},
            _indices{mesh._indices}
        {
        }

        Mesh(Mesh&& mesh) :
            _vertices{move(mesh._vertices)},
            _indices{move(mesh._indices)}
        {
        }

        Mesh& operator=(const Mesh&) = delete;
        Mesh& operator=(Mesh&&) = delete;

        const Vertices& vertices() const noexcept
        {
            return _vertices;
        }

        const Indices& indices() const noexcept
        {
            return _indices;
        }

        Vertices& vertices() noexcept
        {
            return _vertices;
        }

        Indices& indices() noexcept
        {
            return _indices;
        }

        size_t vertexCount() const noexcept
        {
            return _vertices.size();
        }

        size_t indexCount() const noexcept
        {
            return _indices.size();
        }

        template<Attribut attr>
        const auto& at(size_t i) const
        {
            if constexpr (attr == Attribut::POSITION) {
                return _vertices.at(i).position;
            } else if constexpr (attr == Attribut::NORMAL) {
                return _vertices.at(i).normal;
            }

            return _vertices.at(i).uv;
        }

        template<Attribut attr>
        auto& at(size_t i)
        {
            if constexpr (attr == Attribut::POSITION) {
                return _vertices.at(i).position;
            } else if constexpr (attr == Attribut::NORMAL) {
                return _vertices.at(i).normal;
            }

            return _vertices.at(i).uv;
        }

    private:
        Vertices _vertices;
        Indices _indices;
    };
}
