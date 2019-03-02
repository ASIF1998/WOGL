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

#include <queue>

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

            queue<aiNode*> s;
            aiNode* node;

            s.push(scene->mRootNode);

            while(!s.empty()) {
                node = s.front();
                s.pop();

                if(node->mNumMeshes) {
                    _addMeshes(node, scene);
                }

                for (size_t i{0}, size = node->mNumChildren; i < size; i++) {
                    s.push(node->mChildren[i]);
                }
            }

            import.FreeScene();
        }

        InitializeModelMesh(Meshes&& meshes) :
            _meshes{move(meshes)}
        {
        }

        InitializeModelMesh(const InitializeModelMesh&) = delete;
        InitializeModelMesh& operator=(const InitializeModelMesh&) = delete;
        InitializeModelMesh& operator=(InitializeModelMesh&&) = delete;

        InitializeModelMesh(aiNode* node, const aiScene* scene)
        {
            _addMeshes(node, scene);
        }

        void _addMeshes(aiNode* node, const aiScene* scene)
        {
            Vertex vertex;
            aiMesh* mesh;
            bool stayTexture;
            vector<Vertex> vertices;
            vector<uint32_t> indices;
            vector<vec2> tangets;

            for (size_t i{0}; i < node->mNumMeshes; i++) {
                mesh = scene->mMeshes[node->mMeshes[i]];
                stayTexture = (mesh->mTextureCoords[0] ? true : false);

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

                vertices.shrink_to_fit();
                tangets.shrink_to_fit();
                indices.shrink_to_fit();

                for (uint32_t j{0}; j < mesh->mNumFaces; j++) {
                    indices.push_back(mesh->mFaces[j].mIndices[0]);
                    indices.push_back(mesh->mFaces[j].mIndices[1]);
                    indices.push_back(mesh->mFaces[j].mIndices[2]);
                }

                _meshes.push_back(Mesh(move(vertices), move(indices), move(tangets)));
            }
        }

        Meshes _meshes;
    };

    template<typename TextureDataType, TexelType Tx>
    class Model :
        public InitializeModelMesh
    {
        using TextureType = Texture<TextureDataType, Tx>;
        using PtrTexture = unique_ptr<TextureType>;
        using Meshes = vector<Mesh>;
        using Models = vector<Model>;

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

        Model(Model&& model) :
            InitializeModelMesh(move(model._meshes))
        {
            swap(_baseColorTexture, model._baseColorTexture);
            swap(_normalMap, model._normalMap);
        }

        Model(const Model&) = delete;
        Model& operator=(const Model&) = delete;

    private:
        Model(aiNode* node, const aiScene* scene) :
            InitializeModelMesh(node, scene)
        {
            _baseColorTexture = nullptr;
            _normalMap = nullptr;
        }

    public:
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

        /**
         * Статический метод необходимый для загрузки модели в виде нескольких моделей.
         * Данный статический метод будет удобен в случае если в одной модели спользуется сразу
         * несколько текстур (например несколько текстур с базовым цветом).
         * 
         * При загрузке модели текстуры не устанавливаются !!!
         * 
         * @param path путь до модели
         * @return вектор моделей
        */
        static Models makeModels(const string_view path)
        {
            Importer import;
            auto* scene = import.ReadFile(path.data(), aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
                throw invalid_argument(import.GetErrorString());
            }

            queue<aiNode*> s;
            aiNode* node;
            Models models;

            s.push(scene->mRootNode);
            
            while(!s.empty()) {
                node = s.front();
                s.pop();

                if (node->mNumMeshes) {
                    models.push_back(Model{node, scene});
                }

                for (size_t i{0}, size = node->mNumChildren; i < size; i++) {
                    s.push(node->mChildren[i]);
                }
            }

            import.FreeScene();
            models.shrink_to_fit();

            return models;
        }

    private:
        PtrTexture _baseColorTexture;
        PtrTexture _normalMap;
    };
}
