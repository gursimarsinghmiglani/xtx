#pragma once
#include <string>
#include <variant>
#include <vector>
#include "binary_op_node.hpp"
#include "node.hpp"
#include "postfix_op_node.hpp"
#include "print_node.hpp"
#include "type_node.hpp"
#include "unary_op_node.hpp"
struct AST {
    Node node;
    std::variant<
        long long,
        double,
        bool,
        std::string,
        BinaryOpNode,
        PostfixOpNode,
        TypeNode,
        UnaryOpNode,
        PrintNode
    > v;
    std::vector<std::unique_ptr<AST>> children;
};
