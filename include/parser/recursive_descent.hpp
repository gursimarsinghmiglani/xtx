// each parsing function barring the start checks at start and advances the input pointer at its end
#pragma once
#include <iostream>
#include "ast.hpp"
#include "../lexer/lexeme.hpp"
struct Parser {
    std::vector<Lexeme>& lexemes;
    size_t pos;
    Parser(std::vector<Lexeme>& lexemes): lexemes(lexemes), pos(0) {}
    const Lexeme& peek() const {
        return lexemes[pos];
    }
    void advance() { pos++; }
    void check() {
        if (pos >= lexemes.size()) error_function();
    }
    void advance_and_check() {
        advance();
        check();
    }
    Token tok() { return peek().tok; }
    [[noreturn]] void error_function() {
        std::cerr << "Parsing error in line " << peek().line_number << " at position " << pos << "\n";
        std::exit(1);
    }
    std::unique_ptr<AST> parse_program();
    std::unique_ptr<AST> parse_global_decl();
    std::unique_ptr<AST> parse_type();
    std::unique_ptr<AST> parse_expr();
    std::unique_ptr<AST> parse_const_decl();
    std::unique_ptr<AST> parse_function_decl();
};
inline std::unique_ptr<AST> Parser::parse_program() {
    auto ast = std::make_unique<AST>();
    ast->node = Node::PROGRAM;
    while (pos < lexemes.size()) {
        switch(tok()) {
            case Token::TOK_GLOBAL:
                ast->children.push_back(std::move(parse_global_decl()));
                break;
            case Token::TOK_CONST:
                ast->children.push_back(std::move(parse_const_decl()));
                break;
            case Token::TOK_FN:
                ast->children.push_back(std::move(parse_function_decl()));
                break;
            default:
                error_function();
        }
    }
    if (pos == lexemes.size()) {
        return ast;
    }
    error_function();
}
inline std::unique_ptr<AST> Parser::parse_global_decl() {
    auto ast = std::make_unique<AST>();
    ast->node = Node::GLOBAL_DECL;
    advance_and_check();
    if (tok() == Token::TOK_ID) {
        auto ast_id = std::make_unique<AST>();
        ast_id->node = Node::ID;
        ast_id->v = peek().s;
        ast->children.push_back(std::move(ast_id));
    } else error_function();
    advance_and_check();
    switch (tok()) {
        case Token::TOK_COLON:
            advance();
            ast->children.push_back(parse_type());
            if (tok() == Token::TOK_ASSIGN) {
                advance();
                //expr code
            }
            break;
        case Token::TOK_ASSIGN:
            advance();
            //expr code
            break;
        default:
            error_function();
    }
    advance_and_check();
    if (tok() == Token::TOK_SEMI) {
        advance();
        return ast;
    }
    error_function();
}
inline std::unique_ptr<AST> Parser::parse_type() {
    check();
    auto ast = std::make_unique<AST>();
    ast->node = Node::TYPE;
    switch(tok()) {
        case Token::TOK_INT:
            ast->v = TypeNode::INT;
            break;
        case Token::TOK_FLOAT:
            ast->v = TypeNode::FLOAT;
            break;
        case Token::TOK_BOOL:
            ast->v = TypeNode::BOOL;
            break;
        case Token::TOK_MATRIX:
            ast->v = TypeNode::MATRIX;
            break;
        case Token::TOK_VECTOR:
            ast->v = TypeNode::VECTOR;
            break;
        default:
            error_function();
    }
    advance();
    return ast;
}
inline std::unique_ptr<AST> Parser::parse_expr() {
    check();

