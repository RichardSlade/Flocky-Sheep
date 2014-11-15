#include <algorithm>
#include <cassert>

#include "SceneNode.hpp"

SceneNode::SceneNode()
: mParent(nullptr)
, mChildren()
{

}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Apply transform of current node
	states.transform *= getTransform();

	// Draw node and children with changed transform
	drawCurrent(target, states);
	drawChildren(target, states);
}

void SceneNode::updateChildren(sf::Time dt)
{
    for(upScNode& node : mChildren)
        node->update(dt);
}

void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
    for(const upScNode& node : mChildren)
        node->draw(target, states);
}

void SceneNode::update(sf::Time dt)
{
    updateCurrent(dt);
    updateChildren(dt);
}

void SceneNode::addChild(upScNode node)
{
    node->mParent = this;
    mChildren.push_back(std::move(node));
}

SceneNode::upScNode SceneNode::deleteChild(const SceneNode& node)
{
    auto found = std::find_if(mChildren.begin(), mChildren.end(),
                                [&] (SceneNode::upScNode& p) -> bool { return p.get() == &node; } );

    assert(found != mChildren.end());

    SceneNode::upScNode result = std::move(*found);
    result->mParent = nullptr;
    mChildren.erase(found);

    return result;
}

sf::Transform SceneNode::getWorldTransform() const
{
    sf::Transform trans = sf::Transform::Identity;

    for(const SceneNode* node = this; node != nullptr; node = node->mParent)
        trans = node->getTransform() * trans;

    return trans;
}

sf::Vector2f SceneNode::getWorldPosition() const
{
    return getWorldTransform() * sf::Vector2f();
}
