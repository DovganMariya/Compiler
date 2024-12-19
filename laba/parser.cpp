#include "parser.h"

void parser::check_lexical_error(Token& curr) {
    if (curr.get_type() == Token::UNDEFINED && !curr.get_text().empty()) {
        std::cout << "UNDEFINED lexeme " << curr.get_text() << '\n';
    }
}

bool parser::run() {

    _parse_tree.add_product(terminal::FUNCTION, parse_tree::FUNCTION);

    parse_tree begin_tree(terminal::BEGIN);

    if (!begin(begin_tree)) {
        return false;
    }

    _parse_tree.insert_tree(terminal::BEGIN, begin_tree);

    parse_tree description_tree(terminal::DESCRIPTION);

    if (!description(description_tree)) {
        return false;
    }
    _parse_tree.insert_tree(terminal::DESCRIPTION, description_tree);

    parse_tree operators_tree(terminal::OPERATORS);

    if (!operators(operators_tree)) {
        return false;
    }

    _parse_tree.insert_tree(terminal::OPERATORS, operators_tree);

    parse_tree end_tree(terminal::END);

    if (!end(end_tree)) {
        return false;
    }

    _parse_tree.insert_tree(terminal::END, end_tree);
 
    Token curr = _tokens.get_next_token();

    if (curr.get_type() == Token::UNDEFINED && curr.get_text().empty()) {
        return true;
    }
    // Если же было прочитано что то не пустое, то выводим ошибку
    position curr_pos = _tokens.position();
    std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
    std::cout << "Error: no more one function\n";
    return false;
}

bool parser::begin(parse_tree& begin_tree) {

    begin_tree.add_product(terminal::BEGIN, parse_tree::BEGIN);

    position curr_pos = _tokens.position();
    Token curr = _tokens.get_next_token();
    if (curr.get_type() != Token::INT && curr.get_type() != Token::FLOAT) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error in the type of function: the type should be int or float\n";
        return false;
    }

    if (curr.get_type() == Token::INT) {
        begin_tree.add_product(terminal::TYPE, parse_tree::TYPE_1);
    }
    else if (curr.get_type() == Token::FLOAT) {
        begin_tree.add_product(terminal::TYPE, parse_tree::TYPE_2);
    }

    curr_pos = _tokens.position();
    curr = _tokens.get_next_token();
    if (curr.get_type() != Token::ID) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error in the function name: the name must be identifier\n";
        return false;
    }

    begin_tree.add_product(terminal::FUNCTION_NAME, parse_tree::FUNCTION_NAME);
    begin_tree.add_token(terminal::ID, curr);

    curr_pos = _tokens.position();
    curr = _tokens.get_next_token();
    if (curr.get_type() != Token::LEFT_BRACKET) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error int the function description: not find (\n";
        return false;
    }

    curr_pos = _tokens.position();
    curr = _tokens.get_next_token();
    if (curr.get_type() != Token::RIGHT_BRACKET) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error int the function description: not find )\n";
        return false;
    }

    curr_pos = _tokens.position();
    curr = _tokens.get_next_token();
    if (curr.get_type() != Token::LEFT_FIGURE_BRACKET) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error int||float the function description: not find {\n";
        return false;
    }
    return true;
}

bool parser::description(parse_tree& description_tree) {
    parse_tree small_description_tree(terminal::DESCR);

    if (!small_descriptions(small_description_tree)) {
        return false;
    }

    while (true) {
        Token curr = _tokens.get_next_token();

        if (curr.get_type() != Token::INT && curr.get_type() != Token::FLOAT) {

            _tokens.return_last_word();

            description_tree.add_tree(terminal::DESCRIPTION, small_description_tree);
            return true;
        }

        description_tree.add_product(terminal::DESCRIPTION, parse_tree::DESCRIPTION_2);
        description_tree.insert_tree(terminal::DESCR, small_description_tree);
        small_description_tree.clear();

        _tokens.return_last_word();
        if (!small_descriptions(small_description_tree)) {
            return false;
        }
    }
}

bool parser::small_descriptions(parse_tree& small_description_tree) {

    small_description_tree.add_product(terminal::DESCR, parse_tree::DESCR);

    position curr_pos = _tokens.position();
    Token curr = _tokens.get_next_token();
    if (curr.get_type() != Token::INT && curr.get_type() != Token::FLOAT) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error in the description: "
            << "the description should start with the type of variable\n";
    }

    if (curr.get_type() == Token::INT) {
        small_description_tree.add_product(terminal::TYPE, parse_tree::TYPE_1);
    }
    else if (curr.get_type() == Token::FLOAT) {
        small_description_tree.add_product(terminal::TYPE, parse_tree::TYPE_2);
    }

    parse_tree var_list_tree(terminal::VAR_LIST);

    if (!var_list(var_list_tree)) {
        return false;
    }

    small_description_tree.insert_tree(terminal::VAR_LIST, var_list_tree);

    curr_pos = _tokens.position();
    curr = _tokens.get_next_token();
    if (curr.get_type() != Token::SEMICOLON) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "error in the descriptions ending: no ;\n";
        return false;
    }
    return true;
}

bool parser::var_list(parse_tree& var_list_tree) {

    position curr_pos = _tokens.position();
    Token curr = _tokens.get_next_token();
    if (curr.get_type() != Token::ID) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error in the description: no identifier in description\n";
        return false;
    }

    Token prev = curr;

    curr = _tokens.get_next_token();
    if (curr.get_type() != Token::COMMA) {

        _tokens.return_last_word();

        var_list_tree.add_product(terminal::VAR_LIST, parse_tree::VAR_LIST_1);
        var_list_tree.add_token(terminal::ID, prev);

        return true;
    }

    var_list_tree.add_product(terminal::VAR_LIST, parse_tree::VAR_LIST_2);
    var_list_tree.add_token(terminal::ID, prev);
    return var_list(var_list_tree);
}

bool parser::operators(parse_tree& operators_tree) {

    parse_tree small_operators_tree(terminal::OP);

    if (!small_operators(small_operators_tree)) {
        return false;
    }

    while (true) {
        Token curr = _tokens.get_next_token();
 
        if (curr.get_type() != Token::ID) {
         
            _tokens.return_last_word();
         
            operators_tree.add_tree(terminal::OPERATORS, small_operators_tree);
            return true;
        }

        operators_tree.add_product(terminal::OPERATORS, parse_tree::OPERATORS_2);
        operators_tree.insert_tree(terminal::OP, small_operators_tree);
        small_operators_tree.clear();
   
        _tokens.return_last_word();
        if (!small_operators(small_operators_tree)) {
            return false;
        }
    }
}

bool parser::small_operators(parse_tree& small_operators_tree) {

    position curr_pos = _tokens.position();
    Token curr = _tokens.get_next_token();
    if (curr.get_type() != Token::ID) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error in operators: expression must be assigned to the variable\n";
        return false;
    }

    Token id_token = curr;

    curr_pos = _tokens.position();
    curr = _tokens.get_next_token();
    if (curr.get_type() != Token::EQUALS) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error in operators: not find =\n";
        return false;
    }
    curr_pos = _tokens.position();
    curr = _tokens.get_next_token();

    if (curr.get_type() == Token::ID || curr.get_type() == Token::INT_NUMBER || curr.get_type() == Token::FLOAT_NUMBER || curr.get_type() == Token::LEFT_BRACKET) {

        _tokens.return_last_word();

        small_operators_tree.add_product(terminal::OP, parse_tree::OP_1);

        small_operators_tree.add_token(terminal::ID, id_token);

        parse_tree expr_tree(terminal::EXPR);

        if (!expr(expr_tree)) {
            return false;
        }

        small_operators_tree.insert_tree(terminal::EXPR, expr_tree);

    }
    else {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error in operators: invalid value " << curr.get_text() << "\n";
        return false;
    }
    // Отслеживаем наличие ; в потоке
    curr_pos = _tokens.position();
    curr = _tokens.get_next_token();
    if (curr.get_type() != Token::SEMICOLON) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error in operators: not find ;\n";
        return false;
    }
    return true;
}

bool parser::end(parse_tree& end_tree) {
    end_tree.add_product(terminal::END, parse_tree::END);
    position curr_pos = _tokens.position();
    Token curr = _tokens.get_next_token();
    if (curr.get_type() != Token::RETURN) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "error in the function ending: no keyword return\n";
        return false;
    }
    // Идентификатор
    curr_pos = _tokens.position();
    curr = _tokens.get_next_token();
    if (curr.get_type() != Token::ID) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "error in the function ending: no identifier\n";
        return false;
    }
    // Добавим прочитанный идентификатор в дерево
    end_tree.add_token(terminal::ID, curr);
    // Точка с запятой
    curr_pos = _tokens.position();
    curr = _tokens.get_next_token();
    if (curr.get_type() != Token::SEMICOLON) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "error in the function ending: no ;\n";
        return false;
    }
    // Фигурная скобка
    curr_pos = _tokens.position();
    curr = _tokens.get_next_token();
    if (curr.get_type() != Token::RIGHT_FIGURE_BRACKET) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "error in the function ending: no }\n";
        return false;
    }
    return true;
}

bool parser::simple_expr(parse_tree& simple_expr_tree) {

    position curr_pos = _tokens.position();
    Token curr = _tokens.get_next_token();
    if (curr.get_type() == Token::LEFT_BRACKET) {

        simple_expr_tree.add_product(terminal::SIMPLE_EXPR, parse_tree::SIMPLE_EXPR_3);
        parse_tree expr_tree(terminal::EXPR);
        // Вызываем разбор expr
        if (!expr(expr_tree)) {
            return false;
        }
        // Если все хорошо, то добавляем дерево expr в дерево SimpleExpr
        simple_expr_tree.insert_tree(terminal::EXPR, expr_tree);
        // Проверяем наличие закрывающей скобки
        curr_pos = _tokens.position();
        curr = _tokens.get_next_token();
        if (curr.get_type() != Token::RIGHT_BRACKET) {
            check_lexical_error(curr);
            std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
            std::cout << "Error in simple number expression: not find )\n";
            return false;
        }
        return true;
    }

    if (curr.get_type() != Token::ID && curr.get_type() != Token::INT_NUMBER && curr.get_type() != Token::FLOAT_NUMBER) {
        check_lexical_error(curr);
        std::cout << curr_pos._data.size() << ':' << *curr_pos._data.rbegin() << ' ';
        std::cout << "Error in simple expression: " << curr.get_text()
            << " must be a number or a variable\n";
        return false;
    }
    
    if (curr.get_type() == Token::ID) {
        simple_expr_tree.add_product(terminal::SIMPLE_EXPR, parse_tree::SIMPLE_EXPR_1);
        simple_expr_tree.add_token(terminal::ID, curr);
    }
    else if (curr.get_type() == Token::INT_NUMBER || curr.get_type() == Token::FLOAT_NUMBER) {
        simple_expr_tree.add_product(terminal::SIMPLE_EXPR, parse_tree::SIMPLE_EXPR_2);
        simple_expr_tree.add_token(terminal::CONST, curr);
    }
    return true;
}

bool parser::expr(parse_tree& expr_tree) {

    parse_tree term_tree(terminal::TERM);

    if (!term(term_tree)) {
        return false;
    }
    Token curr = _tokens.get_next_token();
 
    if (curr.get_type() != Token::SUM && curr.get_type() != Token::MINUS) {
     
        _tokens.return_last_word();
     
        expr_tree.add_tree(terminal::EXPR, term_tree);
        return true;
    }
    // Если мы прочитали сумму, то добавляем вторую продукцию
    if (curr.get_type() == Token::SUM) {
        expr_tree.add_product(terminal::EXPR, parse_tree::EXPR_2);
       
        expr_tree.insert_tree(terminal::TERM, term_tree);
        
    }
    else if (curr.get_type() == Token::MINUS) {
        expr_tree.add_product(terminal::EXPR, parse_tree::EXPR_3);
 
        expr_tree.insert_tree(terminal::TERM, term_tree);
    }

    return expr(expr_tree);
}


bool parser::term(parse_tree& term_tree) {

    parse_tree simple_expr_tree(terminal::SIMPLE_EXPR);

    if (!simple_expr(simple_expr_tree)) {
        return false;
    }


    Token curr = _tokens.get_next_token();

    if (curr.get_type() != Token::MULTIPLY && curr.get_type() != Token::DIVIDE && curr.get_type() != Token::MOD) {

        _tokens.return_last_word();

        term_tree.add_tree(terminal::TERM, simple_expr_tree);
        return true;
    }

    if (curr.get_type() == Token::MULTIPLY) {
        term_tree.add_product(terminal::TERM, parse_tree::TERM_1);

        term_tree.insert_tree(terminal::SIMPLE_EXPR, simple_expr_tree);

    }
    else if (curr.get_type() == Token::DIVIDE) {
        term_tree.add_product(terminal::TERM, parse_tree::TERM_2);

        term_tree.insert_tree(terminal::SIMPLE_EXPR, simple_expr_tree);
    }
    else if (curr.get_type() == Token::MOD) {
        term_tree.add_product(terminal::TERM, parse_tree::TERM_3);

        term_tree.insert_tree(terminal::SIMPLE_EXPR, simple_expr_tree);
    }


    return term(term_tree);
};
