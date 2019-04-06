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
            if (model.hasBaseColorTexture()) {
                _textureRenderer.reset(new TextureRenderer2D<TextureTexelFormat>(model.texture()));
            }

            if (model.hasNormalMap()) {
                _normalMapRenderer.reset(new TextureRenderer2D<TextureTexelFormat>(model.normalMap()));
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

        /**
         * Этот статический метод используется в случае, если на вход подаётся несколько моджелей.
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
        PtrTexRenderer _textureRenderer;
        PtrTexRenderer _normalMapRenderer;
    };
}
