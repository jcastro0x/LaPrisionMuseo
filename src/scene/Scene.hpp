// Copyright (c) 2022 Javier Castro - jcastro0x@gmail.com
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NON INFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#pragma once

#include <memory>
#include <list>

#include <SFML/Graphics/Drawable.hpp>

namespace lpm
{
    class Engine;
    class SceneNode;

    /**
     * @brief Represent the current whole of elements to be draw by Engine.
     *
     * Engine can draw one scene at a time. Scenes represents the whole "world" at these moment.
     * In Unity3D, Scene represents his homologue concept "scene", and in Unreal represents an "UWorld".
     *
     * Scene can contains many SceneNodes (aka GameObjects in Unity or Actors in Unreal)
     *
     * SceneNode has an order divided in two sections, both treated as unsigned integer values.
     * Lower values are drawn before higher values:
     * - Group
     * - Internal Z-Depth
     *
     * The first value used by Scene is the node's group, and then, internal z-depth.
     *
     * @example:
     * If NodeA lies in Group 1 and internal 1, and NodeB lies in Group2 and internal 0, then NodeA are drawn BEFORE
     * NodeB, regardless NodeB has lower internal value than NodeA.
     *
     */
    class Scene : public sf::Drawable
    {
        using SceneNodePtr  = std::unique_ptr<class SceneNode>;
        using SceneNodesPtr = std::list<SceneNodePtr>;

    public:
        explicit Scene(Engine* engine);
        ~Scene() override;

        virtual void tick(float deltaTime) = 0;

        /**
         * @brief Create SceneNode and add into Scene.
         * 
         * SceneNodes created thorugh this are memory automatic handled by the scene,
         * for that, this function only returns a raw pointer. DONT DELETE IT YOURSELF. 
         * 
         * @return Pointer to created SceneNode casted to tparam SceneNodeType 
         */
        template<typename SceneNodeType, typename... Args> requires std::is_base_of_v<SceneNode, SceneNodeType>
        SceneNodeType& addSceneNode(Args... args)
        {
            auto* node = addSceneNode_Internal(std::make_unique<SceneNodeType>(args...));
            return *static_cast<SceneNodeType*>(node);
        }

        void destroy();

    public:
        /**
         * Get mouse coords transformed to aspect ratio used in the scene
         * @return Mouse coord in scene's aspect ratio
         */
        [[nodiscard]] sf::Vector2i getSceneMousePos() const;

        /**
         * Get pointer to engine
         * @return Engine pointer
         */
        [[nodiscard]] class Engine* getEngine() const { return engine_; }

        /**
         * Check if this scene is pending to be destroyed
         * @return True if is pending to destroy, false otherwise.
         */
        [[nodiscard]] bool isPendingToDestroy() const;


    protected:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    private:
        SceneNode* addSceneNode_Internal(SceneNodePtr node);

    private:
        Engine* const engine_   = nullptr;
        bool bPendingToDestroy_ = false;

        mutable SceneNodesPtr nodes_;
    };
}
