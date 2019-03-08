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
        vec2 uv;

        Vertex& operator=(const Vertex& va)
        {
            position = va.position;
            normal = va.normal;
            uv = va.uv;

            return *this;
        }
    };

    class Mesh
    {
        using Vertices = vector<Vertex>;
        using Indices = vector<uint32_t>;
        using Tangents = vector<vec2>;

        friend class InitializeModelMesh;
        friend class MeshRenderer;

    public:
        /**
         * Конструткор.
         * 
         * @param vertexAttributsCount количество вершин
         * @param indicesCount количество индексов
         * @param tangentsCount количество касательных
        */
        explicit Mesh(size_t vertexAttributsCount, size_t indicesCount, size_t tangentsCount) :
            _vertices(vertexAttributsCount),
            _indices(indicesCount),
            _tangents(tangentsCount)
        {
        }

        /**
         * Конструктор.
         * 
         * @param vertexAttributs вершины
         * @param indices индексы
         * @param tangents касательыне
        */
        explicit Mesh(const Vertices& vertexAttributs, const Indices& indices, const Tangents& tangents) :
            _vertices{vertexAttributs},
            _indices{indices},
            _tangents(tangents)
        {
        }

        /**
         * Конструктор.
         * 
         * @param vertexAttributs вершины
         * @param indices индексы
         * @param tangents касательыне
        */
        explicit Mesh(Vertices&& vertexAttributs, Indices&& indices, Tangents&& tangents) :
            _vertices{forward<Vertices>(vertexAttributs)},
            _indices{forward<Indices>(indices)},
            _tangents(forward<Tangents>(tangents))
        {
        }

        /**
         * Конструктор.
         * 
         * @param vertexAttributs контейнер который хранит вершины
         * @param indices контейнер который хранит индексы
         * @param tangents контейнер который хранит касательные
        */
        template<template<typename> typename Conteiner>
        explicit Mesh(const Conteiner<Vertices>& vertexAttributs, const Conteiner<uint32_t>& indices, const Conteiner<vec2>& tangents) :
            _vertices{vertexAttributs},
            _indices{indices},
            _tangents(tangents)
        {
        }
        
        /**
         * Конструктор.
         *
         * @param vertexAttributs контейнер который хранит вершины
         * @param indices контейнер который хранит индексы
         * @param tangents контейнер который хранит касательные
         */
        template<template<typename> typename Conteiner>
        explicit Mesh(Conteiner<Vertices>&& vertexAttributs, Conteiner<uint32_t>&& indices, Conteiner<vec2>&& tangents) :
            _vertices{forward<Conteiner<Vertices>>(vertexAttributs)},
            _indices{forward<Conteiner<uint32_t>>(indices)},
            _tangents(forward<Conteiner<vec2>>(tangents))
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
        explicit Mesh(const Ptr<Vertex[]>& vertexAttributs, size_t size_1, const Ptr<uint32_t[]>& indices, size_t size_2, const Ptr<vec4[]>& tangents, size_t size_3) :
            _vertices(size_1),
            _indices(size_2),
            _tangents(size_3)
        {
            copy(vertexAttributs.get(), vertexAttributs.get() + size_1, begin(_vertices));
            copy(indices.get(), indices.get() + size_2, begin(_indices));
            copy(tangents.get(), tangents.get() + size_3, begin(_tangents));
        }

        /**
         * Конструтокр.
         * 
         * @param vertexAttributs указатель на массив с вершинами
         * @param size_1 размер массива vertexAttributs
         * @param indices указатель на массив с индексами
         * @param size_2 размер массива indices
        */
        explicit Mesh(const unique_ptr<Vertex[]>& vertexAttributs, size_t size_1, const unique_ptr<uint32_t[]>& indices, size_t size_2, const unique_ptr<vec4[]>& tangents, size_t size_3) :
            _vertices(size_1),
            _indices(size_2),
            _tangents(size_3)
        {
            copy(vertexAttributs.get(), vertexAttributs.get() + size_1, begin(_vertices));
            copy(indices.get(), indices.get() + size_2, begin(_indices));
            copy(tangents.get(), tangents.get() + size_3, begin(_tangents));
        }

        Mesh(const Mesh& mesh) :
            _vertices{mesh._vertices},
            _indices{mesh._indices},
            _tangents{mesh._tangents}
        {
        }

        Mesh(Mesh&& mesh) :
            _vertices{move(mesh._vertices)},
            _indices{move(mesh._indices)},
            _tangents{move(mesh._tangents)}
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

        const Tangents& tangents() const noexcept
        {
            return _tangents;
        }

        Vertices& vertices() noexcept
        {
            return _vertices;
        }

        Indices& indices() noexcept
        {
            return _indices;
        }

        Tangents& tangents() noexcept
        {
            return _tangents;
        }


        size_t vertexCount() const noexcept
        {
            return _vertices.size();
        }

        size_t indexCount() const noexcept
        {
            return _indices.size();
        }

        size_t tangentsCount() const noexcept
        {
            return _tangents.size();
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
        Tangents _tangents;
    };
}
