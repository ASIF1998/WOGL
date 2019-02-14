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

namespace WOGL
{
    class InitializeModelMesh
    {
        using Meshes = vector<Mesh>;

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
            for (uint32_t i{0}; i < node->mNumMeshes; i++) {
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

                auto numVertexcis = mesh->mNumVertices;
                auto numIndecis = mesh->mNumFaces;

                Vertex vert;
                vec3 tangets;

                Mesh subMesh(numVertexcis, numIndecis, numVertexcis);

                bool stayTexture = (mesh->mTextureCoords[0] ? true : false);

                for (uint32_t j{0}; j < numVertexcis; j++) {
                    vert.position.x = mesh->mVertices[j].x;
                    vert.position.y = mesh->mVertices[j].y;
                    vert.position.z = mesh->mVertices[j].z;

                    vert.normal.x = mesh->mNormals[j].x;
                    vert.normal.y = mesh->mNormals[j].y;
                    vert.normal.z = mesh->mNormals[j].z;

                    tangets.x = mesh->mTangents[j].x;
                    tangets.y = mesh->mTangents[j].y;

                    if (stayTexture) {
                        vert.uv.x = mesh->mTextureCoords[0][j].x;
                        vert.uv.y = mesh->mTextureCoords[0][j].y;
                    }

                    subMesh._vertices.push_back(vert);
                    subMesh._tangents.push_back(tangets);
                }

                for (uint32_t j{0}; j < mesh->mNumFaces; j++) {
                    for (uint32_t k{0}; k < mesh->mFaces[j].mNumIndices; k++) {
                        subMesh._indices.push_back(mesh->mFaces[j].mIndices[k]);
                    }
                }

                _meshes.push_back(move(subMesh));
            }

            for (size_t i{0}; i < node->mNumChildren; i++) {
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
            _texture = nullptr;
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
            return _texture.get();
        }

        const TextureType& texture() const noexcept
        {
            return _texture.get();
        }
        
        inline void texture(const Texture<TextureDataType, Tx>& texture)  noexcept
        {
            _texture.reset(new Texture<TextureDataType, Tx>(texture));
        }
        
        inline void texture(Texture<TextureDataType, Tx>&& texture) noexcept
        {
            _texture.reset(new Texture<TextureDataType, Tx>(move(texture)));
        }

    private:
        PtrTexture _texture;
    };
}
