#ifndef PARSER_H
#define PARSER_H
#include "lexical_analizator.h"
#include "Token.h"
#include "parse_tree.h"
class parser {
private:
    // Лексический анализатор для получения токенов из файла
    lexical_analyzer _tokens;
    parse_tree _parse_tree;
    void check_lexical_error(Token& curr);
    bool begin(parse_tree& begin_tree);
    bool description(parse_tree& description_tree);
    bool small_descriptions(parse_tree& small_description_tree);
    bool var_list(parse_tree& var_list_tree);
    bool operators(parse_tree& operators_tree);
    bool small_operators(parse_tree& small_operators_tree);
    bool end(parse_tree& end_tree);
    bool expr(parse_tree& expr_tree);
    bool term(parse_tree& term_tree);
    bool simple_expr(parse_tree& simple_expr_tree);
public:
    void print_parse_tree(std::ostream& out) { _parse_tree.print(out); }
    parse_tree get_parse_tree() const { return _parse_tree; }
    Hash_table get_hash_table() { return _tokens.tokens(); }
    parser(const char* file_name) : _tokens(file_name), _parse_tree() {}
    bool run();
};
#endif // PARSER_H
