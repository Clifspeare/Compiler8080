#include "node.h"

void Node::addChild(std::unique_ptr<Node> node)
{
    node->parent = this;
    if (node->accepted = false) {
        this->accepted = false;
    }
    this->children.push_back(std::move(node));
}