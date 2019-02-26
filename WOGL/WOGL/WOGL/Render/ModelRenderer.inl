//
//  ModelRenderer.inl
//  WOGL
//
//  Created by Асиф Мамедов on 18/02/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "MeshRenderer.hpp"
#include "TextureRenderer.hpp"

namespace WOGL
{
    class InitializeModelRenderer 
    {
        using MeshRenderers = vector<MeshRenderer>;

    public:
        template<typename ModelType>
        explicit InitializeModelRenderer(const ModelType& model)
        {
            _meshRenderers.reserve(model._meshes.size());

            for (size_t i{0}; i < model._meshes.size(); i++) {
                _meshRenderers.push_back(MeshRenderer(model._meshes[i]));
            }
        }

    protected:
        MeshRenderers _meshRenderers;
    };

    template<TexelFormat TextureTexelFormat>
    class ModelRenderer :
        public InitializeModelRenderer
    {
        using PtrTexRenderer = unique_ptr<TextureRenderer<TextureTexelFormat>>;

        friend class Context;

    public:
        /**
         * Конструктор.
         * 
         * @param model модель
        */
        template<typename ModelType>
        explicit ModelRenderer(const ModelType& model) :
           InitializeModelRenderer(model)
        {
            if (model.hasBaseColorTexture()) {
                _textureRenderer.reset(new TextureRenderer<TextureTexelFormat>(model.texture()));
            }

            if (model.hasNormalMap()) {
                _normalMapRenderer.reset(new TextureRenderer<TextureTexelFormat>(model.normalMap()));
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

    private:
        PtrTexRenderer _textureRenderer;
        PtrTexRenderer _normalMapRenderer;
    };
}
