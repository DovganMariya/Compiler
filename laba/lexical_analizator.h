#include "main.h"
#include "Hash_table.h"
#include "Token.h"

struct position {
    // В элементе вектора с индексом i лежит количество символов в строке с номером i + 1.
    vector<int> _data;
    // Последнее прочитанное слово.
    string _last_word;
};

class lexical_analyzer {
private:
    // Поле хранит поток из которого необходимо читать токены
    std::stringstream _input;
    Hash_table _tokens;
    // Текущая позиция
    position _pos;


    void update_position(char s);
    bool is_separators(char s);
    bool is_separators(string& s);
    bool is_keyword(string& s);
    Token::Type_lexeme get_separator_type(string& s);
    Token::Type_lexeme get_keyword_type(string& s);
    string get_next_word();
public:
    lexical_analyzer(string file_name);
    ~lexical_analyzer() { _input.clear(); }
    Hash_table tokens() const { return _tokens; }
    position position() const { return _pos; }
    Token get_next_token();
    void return_last_word();
    Hash_table get_all_tokens();
};



