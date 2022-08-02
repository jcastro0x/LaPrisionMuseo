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

#include <string>
#include <limits>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>


namespace lpm
{
    class Scene;

    /**
     * @brief Child element of a lpm::Scene.
     *
     */
    class SceneNode : public sf::Drawable, public sf::Transformable
    {
    public:
        struct SceneNodeID
        {
            uint16_t group;
            uint16_t depth;

            /**
             * Returns the depth value of this SceneNode to sort it before draw by lpm::Scene
             * @return Depth value
             */
            [[nodiscard]] size_t calculateDepth() const;
        };

        friend Scene;

        using groupType = decltype(SceneNodeID::group);
        using depthType = decltype(SceneNodeID::depth);

    public:
        explicit SceneNode();
        ~SceneNode() override = default;

        bool operator<(SceneNode const& other) const
        {
            return id_.calculateDepth() < other.id_.calculateDepth();
        }

    public:
        SceneNode& setSceneOwner(Scene* owner);
        SceneNode& setName(std::string_view name);
        SceneNode& setDrawOrder(groupType group, depthType depth = {});

    private:
        void generateAutomaticNodeName();

    protected:
        virtual void init() {};
        virtual void tick(float /*deltaTime*/) {};
        virtual void destroy() {};

    protected:
        Scene* getSceneOwner() const;
        std::string getName() const;
        const SceneNodeID& getSceneNodeID() const;
        SceneNodeID& getSceneNodeID();

    private:
        Scene* owner_ = nullptr;
        std::string name_;
        SceneNodeID id_;
    };

    struct CommonDepths
    {
        static constexpr SceneNode::groupType BACKGROUND = 0;
        static constexpr SceneNode::groupType MIDDLE     = 0;
        static constexpr SceneNode::groupType FOREGROUND = 0;
    };
}
