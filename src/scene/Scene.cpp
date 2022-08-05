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

#include "Scene.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include <Engine.hpp>
#include <scene/SceneNode.hpp>
#include <components/AspectRatio.hpp>
#include <system/Configuration.hpp>

using namespace lpm;

Scene::Scene(Engine* engine)
: engine_(engine)
{
}

Scene::~Scene()
{
    for(auto const& node : nodes_)
    {
        node->destroy();
    } 
}

void Scene::tick(float deltaTime)
{
    for(auto const& node : nodes_)
    {
        node->tick(deltaTime);
    }
}

void Scene::draw(sf::RenderTarget& target, const sf::RenderStates states) const
{
    // Store original view to restore it later
    sf::View const originalView = target.getView();

    target.setView(AspectRatio::getViewportAspectRatio({
        Configuration::BACKGROUND_TEX_SIZE_X,
        Configuration::BACKGROUND_TEX_SIZE_Y},
        target.getSize(),
        AspectRatio::EAspectRatioRule::FitToParent
    ));

    // Before draw, sort all SceneNodes based on his SceneNode::SceneNodeID
    nodes_.sort([](auto& a, auto& b){ return *a < *b; });

    // Then, draw all of them
    for(auto const& node : nodes_)
    {
        node->draw(target, states);
    }

    // Restore original view
    target.setView(originalView);
}

class SceneNode* Scene::addSceneNode_Internal(SceneNodePtr node)
{
    node->setSceneOwner(this);
    node->init();
    return nodes_.emplace_back(std::move(node)).get();
}

sf::Vector2i Scene::getSceneMousePos() const
{
    auto pos = AspectRatio::transformPointToTextureCoords(
        {Configuration::BACKGROUND_TEX_SIZE_X, Configuration::BACKGROUND_TEX_SIZE_Y},
        getEngine()->getWindowSize(),
        AspectRatio::EAspectRatioRule::FitToParent,
        getEngine()->getMousePosition()
    );

    if(pos) return pos.value();
    else return {0, 0};
}

void Scene::destroy()
{
    bPendingToDestroy_ = true;

    for(auto const& node : nodes_)
    {
        node->destroy();
    }
}

bool Scene::isPendingToDestroy() const
{
    return bPendingToDestroy_;
}
