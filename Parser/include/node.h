#include <iostream>
#include <vector>
#include <memory>

#include "ast_type.h"

class Node {
public:
    explicit Node(Node *parent) {this->parent = parent;}
    Node() {};

    void addChild(std::unique_ptr<Node> node);

    std::string data;
    Type type;
    Node* parent;
    std::vector<std::unique_ptr<Node>> children;
    bool accepted = true;
};
