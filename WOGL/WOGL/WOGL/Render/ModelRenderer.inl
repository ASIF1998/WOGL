//
//  ModelRenderer.inl
//  WOGL
//
//  Created by Асиф Мамедов on 18/02/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "MeshRenderer.hpp"
#include "Texture/TextureRenderer2D.hpp"

#include <optional>

namespace WOGL
{
    class InitializeModelRenderer 
    {
        using MeshRenderers = vector<MeshRenderer>;

    public:
        /**
         * Конструктор.
         *
         * @param model модель
         * @param posAttibIndx индекс атрибута позиции
         * @param normalAttribIndx индекс атрибута нормали
         * @param texCoordAttribIndx индекс атрибута текстурной координаты
         * @param tangAttribIndx индекс атрибута касательной
        */
        template<typename ModelType>
        explicit InitializeModelRenderer(const ModelType& model, uint32_t posAttibIndx = 0, uint32_t normalAttribIndx = 1, uint32_t texCoordAttribIndx = 2, uint32_t tangAttribIndx = 3)
        {
            _meshRenderers.reserve(model._meshes.size());

            for (size_t i{0}; i < model._meshes.size(); i++) {
                _meshRenderers.push_back(MeshRenderer{model._meshes[i], posAttibIndx, normalAttribIndx, texCoordAttribIndx, tangAttribIndx});
            }
        }

    protected:
        MeshRenderers _meshRenderers;
    };

    template<TexelFormat TextureTexelFormat>
    class ModelRenderer :
        public InitializeModelRenderer
    {
        using PtrTexRenderer = unique_ptr<TextureRenderer2D<TextureTexelFormat>>;
        using TextureRendererAndSlot = pair<TextureRenderer2D<TextureTexelFormat>, int32_t>;

        friend class Context;

    public:
        /**
         * Конструктор.
         * 
         * @param model модель
         * @param posAttibIndx индекс атрибута позиции
         * @param normalAttribIndx индекс атрибута нормали
         * @param texCoordAttribIndx индекс атрибута текстурной координаты
         * @param tangAttribIndx индекс атрибута касательной
        */
        template<typename Model>
        explicit ModelRenderer(const Model& model, uint32_t posAttibIndx = 0, uint32_t normalAttribIndx = 1, uint32_t texCoordAttribIndx = 2, uint32_t tangAttribIndx = 3) :
           InitializeModelRenderer(model, posAttibIndx, normalAttribIndx, texCoordAttribIndx, tangAttribIndx)
        {
            const auto& textures = model.texturesAndTexturesSlot();
            size_t size = textures.size();

            _texturersRenderer.reserve(size);

            for (size_t i{0}; i < size; i++) {
                _texturersRenderer.push_back(TextureRendererAndSlot(TextureRenderer2D<TextureTexelFormat>(textures[i].first), textures[i].second));
            }   
        }

        const MeshRenderer& at(size_t i) const 
        {
            return _meshRenderers.at(i);
        }

        MeshRenderer& at(size_t i) 
        {
            return _meshRenderers.at(i);
        }

        const MeshRenderer& operator[](size_t i) const noexcept
        {
            return _meshRenderers[i];
        }

        MeshRenderer& operator[](size_t i) noexcept
        {
            return _meshRenderers[i];
        }

        auto& textureRenderer(size_t i) noexcept
        {
            return _texturersRenderer.at(i).first;
        }

        const auto& textureRenderer(size_t i) const noexcept 
        {
            return _texturersRenderer.at(i).first;
        }

        int32_t textureRendererSlot(size_t i)  noexcept
        {
            return _texturersRenderer.at(i).second;
        }

        /**
         * Этот статический метод используется в случае, если на вход подаётся несколько моделей.
         *
         * @param models некоторый контейнер с моделями
         * @param posAttibIndx индекс атрибута позиции
         * @param normalAttribIndx индекс атрибута нормали
         * @param texCoordAttribIndx индекс атрибута текстурной координаты
         * @param tangAttribIndx индекс атрибута касательной
         * @return вектор с объектами типа ModelRenderer
        */
        template<typename Models>
        static auto makeModelsRenderer(const Models& models, uint32_t posAttibIndx = 0, uint32_t normalAttribIndx = 1, uint32_t texCoordAttribIndx = 2, uint32_t tangAttribIndx = 3)
        {
            vector<ModelRenderer<TextureTexelFormat>> modelsRenderer;
            modelsRenderer.reserve(models.size());

            for (size_t i{0}; i < models.size(); i++) {
                modelsRenderer.push_back(ModelRenderer{models[i], posAttibIndx, normalAttribIndx, texCoordAttribIndx, tangAttribIndx});
            }

            return modelsRenderer;
        }

    private:
        vector<TextureRendererAndSlot> _texturersRenderer;
    };
}
