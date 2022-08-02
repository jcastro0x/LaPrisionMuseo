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

#include <Engine.hpp>
#include <scene/SceneNode.hpp>
#include <components/AspectRatio.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

using namespace lpm;

Scene::Scene(Engine* engine)
: engine_(engine)
{
}

Scene::~Scene()
{
    for(auto& node : nodes_)
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

void Scene::addSceneNode(SceneNodePtr node)
{
    nodes_.emplace_back(std::move(node));
}

void Scene::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // Store original view to restore it later
    sf::View const originalView = target.getView();

    // Calculate viewport needed to fit to a aspet ratio of 1.333333
    // TODO: Replace 640,480 for 1.3333
    target.setView(AspectRatio::getViewportAspectRatio({640, 480}, target.getSize(),
                                                       AspectRatio::EAspectRatioRule::FitToParent));

    for(auto& node : nodes_)
    {
        node->draw(target, states);
    }

    // Restore original view
    target.setView(originalView);
}

sf::Vector2i Scene::getSceneMousePos() const
{
    // TODO: Replace 640,480 for 1.3333
    auto pos = AspectRatio::transformPointToTextureCoords(
        {640, 480},
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
}

bool Scene::isPendingToDestroy() const
{
    return bPendingToDestroy_;
}
