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

#include "SceneNode.hpp"

#include <cassert>

#include <scene/Scene.hpp>



using namespace lpm;


size_t SceneNode::SceneNodeID::calculateDepth() const
{
    // Check overflow
    {
        constexpr size_t max_group  = std::numeric_limits<decltype(group)>::max();
        constexpr size_t max_depth  = std::numeric_limits<decltype(depth)>::max();
        constexpr size_t max_return = std::numeric_limits<decltype(calculateDepth())>::max();

        static_assert(max_depth + max_group * max_group < max_return
        , "group and depth must return values lower than getZIndex return type");
    }

    return depth + group * std::numeric_limits<decltype(group)>::max();
}




SceneNode::SceneNode()
{
    generateAutomaticNodeName();
}

bool SceneNode::operator<(SceneNode const& other) const
{
    return id_.calculateDepth() < other.id_.calculateDepth();
}

SceneNode& SceneNode::setSceneOwner(Scene* owner)
{
    owner_ = owner;
    return *this;
}

SceneNode& SceneNode::setName(std::string_view name)
{
    name_ = std::string(name.data());
    return *this;
}

SceneNode& SceneNode::setDrawOrder(groupType group, depthType depth/*= 0*/)
{
    id_.group = group;
    id_.depth = depth;
    return *this;
}

void SceneNode::generateAutomaticNodeName()
{
    static size_t counter = std::numeric_limits<size_t>::max();

    ++counter;
    setName(std::to_string(counter) + "_node");
}

Scene* SceneNode::getSceneOwner() const
{
    assert(owner_ != nullptr && "Trying to get scene owner when it's null");
    return owner_;
}

std::string SceneNode::getName() const
{
    return name_;
}

const SceneNode::SceneNodeID& SceneNode::getSceneNodeID() const
{
    return id_;
}

SceneNode::SceneNodeID& SceneNode::getSceneNodeID()
{
    return id_;
}
