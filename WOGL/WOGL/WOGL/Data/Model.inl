//
//  Model.inl
//  WOGL
//
//  Created by Асиф Мамедов on 14/02/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "Texture2D.hpp"
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

            _meshes.reserve(node->mNumMeshes);

            for (size_t i{0}; i < node->mNumMeshes; i++) {
                mesh = scene->mMeshes[node->mMeshes[i]];
                stayTexture = (mesh->mTextureCoords[0] ? true : false);

                _meshes.push_back(Mesh(mesh->mNumVertices, mesh->mNumFaces * 3));

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
                    
                    vertex.tang = {
                        mesh->mTangents[j].x,
                        mesh->mTangents[j].y,
                        mesh->mTangents[j].z
                    };

                    _meshes[i]._vertices[j] = vertex;
                }

                for (size_t j{0}, k{0}; j < mesh->mNumFaces; j++, k += 3) {
                    _meshes[i]._indices[k] = mesh->mFaces[j].mIndices[0];
                    _meshes[i]._indices[k + 1] = mesh->mFaces[j].mIndices[1];
                    _meshes[i]._indices[k + 2] = mesh->mFaces[j].mIndices[2];
                }
            }
        }

        Meshes _meshes;
    };

    template<typename TextureDataType, TexelType Tx>
    class Model :
        public InitializeModelMesh
    {
        using Meshes = vector<Mesh>;
        using Models = vector<Model>;
        using TextureType = Texture2D<TextureDataType, Tx>;
        using TextureAndTextureSlot = pair<TextureType, int32_t>;

        friend class InitializeModelRenderer;

    public:
        /**
         * Конструктор который загружает меши модели, располагающегося по пути path.
         * 
         * @param path путь к загруженной модели
         * @param numTextures количество текстур в модели
         * @throw invalid_argument в случае если не удалось загрузить модель из файла
        */
        Model(const string_view path, size_t numTextures = 1) :
            InitializeModelMesh(path)
        {
            _textures.reserve(numTextures);
        }

        Model(Model&& model) :
            InitializeModelMesh(move(model._meshes)),
            _textures{move(model._textures)}
        {
        }

        Model(const Model&) = delete;
        Model& operator=(const Model&) = delete;

    private:
        Model(aiNode* node, const aiScene* scene) :
            InitializeModelMesh(node, scene)
        {
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

        /**
         * Метод, необходимый для добавления текстуры.
         * 
         * При добавление текстуры так же заранее необходимо предусмотреть, к какому текстурному слоту
         * будет привязана текстура во время рендеринга.
         * 
         * @param path путь до 2d текстуры
         * @param slot текстурный слот
        */
        inline void pushTexture(const string_view path, int32_t slot)
        {
            _textures.push_back(TextureAndTextureSlot(Texture2D<TextureDataType, Tx>::loadTexture(path), slot));
        }

        /**
         * Метод, необходимый для добавления текстуры.
         * 
         * При добавление текстуры так же заранее необходимо предусмотреть, к какому текстурному слоту
         * будет привязана текстура во время рендеринга.
         * 
         * @param ptrTexture указатель на 2d текстуру
         * @param slot текстурный слот
        */
        template<template<typename> typename Ptr>
        inline void pushTexture(const Ptr<Texture2D<TextureDataType, Tx>>& ptrTexture, int32_t slot)
        {
            _textures.push_back(TextureAndTextureSlot(*ptrTexture), slot);
        }

        /**
         * Метод, необходимый для добавления текстуры.
         * 
         * При добавление текстуры так же заранее необходимо предусмотреть, к какому текстурному слоту
         * будет привязана текстура во время рендеринга.
         * 
         * @param ptrTexture указатель на 2d текстуру
         * @param slot текстурный слот
        */
        template<typename DelType, template<typename, typename> typename Ptr>
        inline void pushTexture(const Ptr<Texture2D<TextureDataType, Tx>, DelType>& ptrTexture, int32_t slot)
        {
            _textures.push_back(TextureAndTextureSlot(*ptrTexture), slot);
        }

        /**
         * Метод, необходимый для добавления текстуры.
         * 
         * При добавление текстуры так же заранее необходимо предусмотреть, к какому текстурному слоту
         * будет привязана текстура во время рендеринга.
         * 
         * @param texture 2d текстура
         * @param slot текстурный слот
        */
        inline void pushTexture(Texture2D<TextureDataType, Tx>&& texture, int32_t slot)
        {
            _textures.push_back(TextureAndTextureSlot(texture), slot);
        }

        /**
         * Метод, необходимый для добавления текстуры.
         * 
         * При добавление текстуры так же заранее необходимо предусмотреть, к какому текстурному слоту
         * будет привязана текстура во время рендеринга.
         * 
         * @param texture 2d текстура
         * @param slot текстурный слот
        */
        inline void pushTexture(const Texture2D<TextureDataType, Tx>& texture, int32_t slot)
        {
            _textures.push_back(TextureAndTextureSlot(texture, slot));
        }

        /**
         * Метод удаляющий последнюю добавленную 2d текстуру.
        */
        inline void popTexture()
        {
            _textures.pop_back();
        }

        /**
         * @return true - если нет 2d текстур, иначе false
        */
        bool hasTextures() const noexcept
        {
            return _textures.empty();
        }

        /**
         * Метод возвращающий вектор текстур с текстурными слотами.
         * Каждый объект этого вектора имеет тип pair<...>. В first хранится сама текстура, а в second - текстурный слот.
         * 
         * @return вектор текстур с текстурными слотами
        */
        auto& texturesAndTexturesSlot() noexcept
        {
            return _textures;
        }

        /**
         * Метод возвращающий вектор текстур с текстурными слотами.
         * Каждый объект этого вектора имеет тип pair<...>. В first хранится сама текстура, а в second - текстурный слот.
         * 
         * @return константный вектор текстур с текстурными слотами
        */
        const auto& texturesAndTexturesSlot() const noexcept
        {
            return _textures;
        }

        /**
         * Метод возвращающий i'ю 2d текстуру.
         * 
         * @param i порядковый индекс текстуры 
         * @return i'ая 2d текстура
        */
        auto& texture(size_t i) 
        {
            return _textures.at(i).first;
        }

        /**
         * Метод возвращающий i'ю 2d текстуру.
         * 
         * @param i порядковый индекс текстуры 
         * @return константная i'ая 2d текстура
        */
        const auto& texture(size_t i) const
        {
            return _textures.at(i).first;
        }

        /**
         * Метод возвращающий i'ый текстурный слот.
         * 
         * @param i порядковый индекс текстурного слота 
         * @return i'ый текстурный слот.
        */
        int32_t slot(size_t i) const 
        {
            return _textures.at(i).second;
        }

        /**
         * Статический метод необходимый для загрузки модели в виде нескольких моделей.
         * Данный статический метод будет удобен в случае если в одной модели используется сразу
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

                for (size_t i{0}, size{node->mNumChildren}; i < size; i++) {
                    s.push(node->mChildren[i]);
                }
            }

            import.FreeScene();
            models.shrink_to_fit();

            return models;
        }

    private:
        vector<TextureAndTextureSlot> _textures;
    };
}
