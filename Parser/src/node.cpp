#include "node.h"

void Node::addChild(std::shared_ptr<Node> node) {
  node->parent = this;
  if (node->accepted == false) {
    this->accepted = false;
  }
  this->children.push_back(node);
}//