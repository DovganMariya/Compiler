#ifndef PARSE_TREE_H
#define PARSE_TREE_H

#include "lexical_item.h"
#include "Token.h"
#include "terminal.h"
#include <iostream>
#include <utility>
#include <vector>

/*
 *  ласс дерева разбора
 * ƒерево хранит в себе токены и терминалы в последовательности, соответствующей левому разбору строки
 * */

using ptr_lexi = std::shared_ptr<lexical_item>;
class parse_tree;

/**
 *  ласс узла дерева разбора
 * \param _value - ссылка на интерфейс lexical_item
 * \param _children - вектор детей узла
 * */
class tree_node {
public:
    /**
     *  онструктор по умолчанию
     * ¬ектор с детьми будет создан пустым
     * */
    tree_node() : _value{ nullptr }, _children{ std::vector<std::shared_ptr<tree_node>>() }, _rpn{} {}

    /**
     *  онструктор по терминалу
     * \param v - терминал, которым будет инициализировано поле _value
     * ¬ектор с детьми будет создан пустым
     * */
    tree_node(const terminal& v) :
        _value{ std::static_pointer_cast<lexical_item>(std::make_shared<terminal>(v)) },
        _children{ std::vector<std::shared_ptr<tree_node>>() } {};

    /**
     *  онструктор по токену
     * \param v - токен, которым будет инициализировано поле _value
     * ¬ектор с детьми будет создан пустым
     * */
    tree_node(const Token& v) :
        _value{ std::dynamic_pointer_cast<lexical_item>(std::make_shared<Token>(v)) },
        _children{ std::vector<std::shared_ptr<tree_node>>() } {};

    /**
     *  онструктор по ссылке на интерфейс lexical_item
     * \param v - ссылке на интерфейс, которой будет инициализировано поле _value
     * ¬ектор с детьми будет создан пустым
     * */
    tree_node(ptr_lexi v) :
        _value{ std::move(v) },
        _children{ std::vector<std::shared_ptr<tree_node>>() } {};

    /**
     *  онструктор копировани€
     * ¬ектор детей будет скопирован
     * \param v - узел дл€ копировани€
     * */
    tree_node(const tree_node& v);

    /**
     *  онструктор по умолчанию
     * */
    ~tree_node() = default;

    //  ласс дерева объ€влен дружеским дл€ доступа к пол€м узла
    friend class parse_tree;
    //  ласс семантического анализа объ€влен дружественным, чтобы легче реализовать обход дерева
    friend class semantic;

    string _rpn;
private:
    ptr_lexi _value;
    std::vector<std::shared_ptr<tree_node>> _children;

};

using ptree_node = shared_ptr<tree_node>;

class parse_tree {
public:
    enum type_product {
        FUNCTION,
        BEGIN,
        END,
        FUNCTION_NAME,
        DESCRIPTION_1, DESCRIPTION_2,
        DESCR,
        TYPE_1, TYPE_2,
        VAR_LIST_1, VAR_LIST_2,
        OPERATORS_1, OPERATORS_2,
        OP_1, EXPR_1, EXPR_2, EXPR_3,
        TERM_1, TERM_2, TERM_3,
        SIMPLE_EXPR_1, SIMPLE_EXPR_2, SIMPLE_EXPR_3,
    };


    parse_tree() : _root(make_shared<tree_node>(tree_node{ terminal{terminal::FUNCTION} })) {}
    parse_tree(const terminal& root) : _root(make_shared<tree_node>(tree_node{ root })) {}
    parse_tree(const parse_tree& v) { copy_tree(_root, v._root); }
    ~parse_tree() = default;
    /**
     * ћетод чистит дерево, не трога€ корень дерева
     * */
    void clear() { _root->_children.clear(); }

    void print(std::ostream& out) { print(out, _root, 0); }

     //ћетод вставки дерева в поддерево, корень которого содержит переданный терминал. ¬ставка
     //осуществл€етс€ в самый левый терминал без детей.
    void insert_tree(const terminal& to_add, const parse_tree& tree) {
        insert_tree(to_add, tree, _root);
    }

    void add_tree(const terminal& to_add, const parse_tree& tree) {
        add_tree(to_add, tree, _root);
    }

    void add_token(const terminal& to_add, const Token& v) {
        add_token(to_add, v, _root);
    }

    void add_product(const terminal& to_add, const type_product& product) {
        add_product(to_add, product, _root);
    }

    friend class semantic;
private:
    ptree_node _root;

    void print(std::ostream& out, const ptree_node& pos, size_t level);

    void insert_tree(const terminal& to_add, const parse_tree& tree, ptree_node& pos);

    void add_tree(const terminal& to_add, const parse_tree& tree, const ptree_node& pos);

    void copy_tree(ptree_node& to, const ptree_node& from);

    bool add_token(const terminal& to_add, const Token& v, ptree_node& pos);

    bool add_product(const terminal& to_add, type_product product, ptree_node& pos);

    void push_product_item(std::vector<std::shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_function(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_begin(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_end(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_function_name(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_description_1(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_description_2(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_descr(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_type_1(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_type_2(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_var_list_1(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_var_list_2(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_var_operators_1(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_var_operators_2(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_var_op_1(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_expr_1(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_expr_2(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_expr_3(vector<shared_ptr<tree_node>>& children, type_product product);

    void push_product_item_term_1(vector<shared_ptr<tree_node>>& children, parse_tree::type_product product);

    void push_product_item_term_2(vector<shared_ptr<tree_node>>& children, parse_tree::type_product product);

    void push_product_item_term_3(vector<shared_ptr<tree_node>>& children, parse_tree::type_product product);

    void push_product_item_simple_expr_1(vector<shared_ptr<tree_node>>& children, parse_tree::type_product product);

    void push_product_item_simple_expr_2(vector<shared_ptr<tree_node>>& children, parse_tree::type_product product);

    void push_product_item_simple_expr_3(vector<shared_ptr<tree_node>>& children, parse_tree::type_product product);

};


#endif // PARSE_TREE_H
