//
//  Model.inl
//  WOGL
//
//  Created by Асиф Мамедов on 14/02/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "Texture.hpp"
#include "Mesh.hpp"

#include <string_view>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace Assimp;
using namespace std;

namespace WOGL
{
    class InitializeModelMesh
    {
        using Meshes = vector<Mesh>;
        
        friend class InitializeModelRenderer;

    protected:
        /**
         * Конструктор который загружает меши модели, располагающегося по пути path.
         * 
         * @param path путь к загруженной модели
         * @throw invalid_argument в случае если не удалось загрузить модель из файла
        */
        explicit InitializeModelMesh(const string_view path)
        {
            Importer import;
            auto* scene = import.ReadFile(path.data(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
                throw invalid_argument(import.GetErrorString());
            }

            _nodeProcessing(scene->mRootNode, scene);

            import.FreeScene();
        }

        InitializeModelMesh(const InitializeModelMesh&) = delete;
        InitializeModelMesh& operator=(const InitializeModelMesh&) = delete;
        InitializeModelMesh& operator=(InitializeModelMesh&&) = delete;

    private:
        void _nodeProcessing(aiNode* node, const aiScene* scene)
        {
            Vertex vertex;

            vector<Vertex> vertices;
            vector<uint32_t> indices;
            vector<vec2> tangets;

            for (size_t i{0}; i < node->mNumMeshes; i++) {
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                bool stayTexture = (mesh->mTextureCoords[0] ? true : false);

                for (size_t j{0}; j < mesh->mNumVertices; j++) {
                    vertex.position = {
                        mesh->mVertices[j].x,
                        mesh->mVertices[j].y,
                        mesh->mVertices[j].z
                    };

                    vertex.normal = {
                        mesh->mNormals[j].x,
                        mesh->mNormals[j].y,
                        mesh->mNormals[j].z
                    };

                    if (stayTexture) {
                        vertex.uv = {
                            mesh->mTextureCoords[0][j].x,
                            mesh->mTextureCoords[0][j].y
                        };
                    }

                    vertices.push_back(Vertex{vertex});
                    tangets.push_back(vec2{mesh->mTangents[j].x, mesh->mTangents[j].y});
                }

                for (uint32_t j{0}; j < mesh->mNumFaces; j++) {
                    for (uint32_t k{0}; k < mesh->mFaces[j].mNumIndices; k++) {
                        indices.push_back(mesh->mFaces[j].mIndices[k]);
                    }
                }

                vertices.shrink_to_fit();
                tangets.shrink_to_fit();
                indices.shrink_to_fit();

                _meshes.push_back(Mesh(move(vertices), move(indices), move(tangets)));
            }

            for (size_t i{0}, size = node->mNumChildren; i < size; i++) {
                _nodeProcessing(node->mChildren[i], scene);
            }
        }

    protected:
        Meshes _meshes;
    };

    template<typename TextureDataType, TexelType Tx>
    class Model :
        public InitializeModelMesh
    {
        using TextureType = Texture<TextureDataType, Tx>;
        using PtrTexture = unique_ptr<TextureType>;
        using Meshes = vector<Mesh>;

        friend class InitializeModelRenderer;

    public:
        /**
         * Конструктор который загружает меши модели, располагающегося по пути path.
         * 
         * @param path путь к загруженной модели
         * @throw invalid_argument в случае если не удалось загрузить модель из файла
        */
        Model(const string_view path) :
            InitializeModelMesh(path)
        {
            _baseColorTexture = nullptr;
            _normalMap = nullptr;
        }

        size_t amountMesh() const noexcept
        {
            return _meshes.size();
        }

        Mesh& at(size_t i)
        {
            return _meshes.at(i);
        }

        const Mesh& at(size_t i) const
        {
            return _meshes.at(i);
        }

        Mesh& operator[](size_t i) noexcept
        {
            return _meshes[i];
        }

        const Mesh& operator[](size_t i) const noexcept
        {
            return _meshes[i];
        }

        Meshes& meshes() noexcept
        {
            return _meshes;
        }

        const Meshes& meshes() const noexcept
        {
            return _meshes;
        }

        TextureType& texture() noexcept
        {
            return *_baseColorTexture.get();
        }

        const TextureType& texture() const noexcept
        {
            return *_baseColorTexture.get();
        }

        TextureType& normalMap() noexcept
        {
            return *_normalMap.get();
        }

        const TextureType& normalMap() const noexcept
        {
            return *_normalMap.get();
        }

        inline void setBaseColorTexture(const Texture<TextureDataType, Tx>& baseColorTexture)  noexcept
        {
            _baseColorTexture.reset(new Texture<TextureDataType, Tx>(baseColorTexture));
        }
        
        inline void setBaseColorTexture(Texture<TextureDataType, Tx>&& baseColorTexture) noexcept
        {
            _baseColorTexture.reset(new Texture<TextureDataType, Tx>(move(baseColorTexture)));
        }

        template<template<typename> typename Ptr>
        inline void setBaseColorTexture(const Ptr<Texture<TextureDataType, Tx>>& baseColorTexture)
        {
            _baseColorTexture.reset(new Texture<TextureDataType, Tx>(*baseColorTexture));
        } 

        inline void setBaseColorTexture(const string_view path) 
        {
            setBaseColorTexture(Texture<TextureDataType, Tx>::loadTexture(path));
        }

        inline void setNormalMap(const Texture<TextureDataType, Tx>& normalMap)  noexcept
        {
            _normalMap.reset(new Texture<TextureDataType, Tx>(normalMap));
        }
        
        inline void setNormalMap(Texture<TextureDataType, Tx>&& normalMap) noexcept
        {
            _normalMap.reset(new Texture<TextureDataType, Tx>(move(normalMap)));
        }

        template<template<typename> typename Ptr>
        inline void setNormalMap(const Ptr<Texture<TextureDataType, Tx>>& normalMap)
        {
            _normalMap.reset(new Texture<TextureDataType, Tx>(*normalMap));
        }

        inline void setNormalMap(const string_view path) 
        {
            setNormalMap(Texture<TextureDataType, Tx>::loadTexture(path));
        } 

        /**
         * Метод сообщающий о наличии текстуры с базовыми цветами.
         * 
         * @return true в случае если имеется текстура, иначе false
        */
        bool hasBaseColorTexture() const noexcept
        {
            return static_cast<bool>(_baseColorTexture);
        }

        /**
         * Метод сообщающий о наличии карты нормалей.
         * 
         * @return true в случае если имеется карта нормалей, иначе false
        */
        bool hasNormalMap() const noexcept
        {
            return static_cast<bool>(_normalMap);
        }

    private:
        PtrTexture _baseColorTexture;
        PtrTexture _normalMap;
    };
}
