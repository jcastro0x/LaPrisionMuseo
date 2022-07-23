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
#include <vector>

#include <SFML/Graphics/Drawable.hpp>

/**
 * @brief Holds elements to be processes (draw, update, ...) in current context.
 *  
 */
class Scene : public sf::Drawable
{
    using SceneNodePtr = std::unique_ptr<class SceneNode>;

public:
    Scene(class Engine* engine);
    ~Scene() override;

    virtual void tick(float deltaTime);

    void addSceneNode(SceneNodePtr node);
    [[nodiscard]] class Engine* getEngine() const { return engine_; }

    /**
     * Get mouse coords transformed to aspect ratio used in the scene
     * @return Mouse coord in scene's aspect ratio
     */
    sf::Vector2i getSceneMousePos() const;

    void destroy();

    [[nodiscard]] bool isPendingToDestroy() const;

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;


private:
    class Engine* engine_ = nullptr;
    std::vector<SceneNodePtr> nodes_;
    bool bPendingToDestroy_ = false;
};
