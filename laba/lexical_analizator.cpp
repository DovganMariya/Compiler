#include "lexical_analizator.h"
#include <cctype>
#include <algorithm>
#include "DFA.h"

lexical_analyzer::lexical_analyzer(string file_name) {
    ifstream fin(file_name);
    _input << fin.rdbuf();
    fin.close();
    _pos._data = std::vector<int>(1);
}

void lexical_analyzer::update_position(const char s) {
    // ���� ��� ������ �������� ������, �� ������� ����� ������ � ��������� �� ���
    if (s == '\n') {
        _pos._data.push_back(0);
        // ����� ����������� ����� �������� � ������� ������
    }
    else {
        (*_pos._data.rbegin())++;
    }
    // ����������� ��������� ����������� �����
    _pos._last_word += s;
}

bool lexical_analyzer::is_separators(char s) {
    vector<char> spr_sym = { '+', '-', '(', ')', '{', '}', ',', ';', '=', '/', '%', '*' };
    return std::find(spr_sym.begin(), spr_sym.end(), s) != spr_sym.end();
}

bool lexical_analyzer::is_separators(string& s) {
    vector<string> spr_sym = { "+", "-", "(", ")", "{", "}", ",", ";", "=", "/", "%", "*" };
    return std::find(spr_sym.begin(), spr_sym.end(), s) != spr_sym.end();
}

bool lexical_analyzer::is_keyword(string& s) {
    bool flag = false;

    if (s == "int")
    {
        flag = true;
    }
    if (s == "return")
    {
        flag = true;
    }
    if (s == "float")
    {
        flag = true;
    }
    return flag;
}

Token::Type_lexeme lexical_analyzer::get_separator_type(string& s) {
    if (s == "+") {
        return Token::SUM;
    }
    else if (s == "-") {
        return Token::MINUS;
    }
    else if (s == "(") {
        return Token::LEFT_BRACKET;
    }
    else if (s == ")") {
        return Token::RIGHT_BRACKET;
    }
    else if (s == "{") {
        return Token::LEFT_FIGURE_BRACKET;
    }
    else if (s == "}") {
        return Token::RIGHT_FIGURE_BRACKET;
    }
    else if (s == ",") {
        return Token::COMMA;
    }
    else if (s == ";") {
        return Token::SEMICOLON;
    }
    else if (s == "=") {
        return Token::EQUALS;
    }
    else if (s == "/") {
        return Token::DIVIDE;
    }
    else if (s == "%") {
        return Token::MOD;
    }
    else if (s == "*") {
        return Token::MULTIPLY;
    }
    return Token::UNDEFINED;
}

Token::Type_lexeme lexical_analyzer::get_keyword_type(string& s) {
    if (s == "return") {
        return Token::RETURN;
    }
    else if (s == "int") {
        return Token::INT;
    }
    else if (s == "float") {
        return Token::FLOAT;
    }
    return Token::UNDEFINED;
}

std::string lexical_analyzer::get_next_word() {
    std::string word;
    // ������� ��������� ��������� �����, ��� ��� ������ ����� ��������� �����
    _pos._last_word.clear();
    while (true) {
        char s;
        _input.get(s);
        // ���� ��� ����� �����, �� ������������� ����
        if (_input.eof()) {
            break;
        }
        update_position(s);
        // ���� ��� ���������� ������, �� ���������� ����������� �����.
        // ���� ����������� ����� �����, �� ���������� �����������
        if (isspace(s)) {
            if (word.empty()) {
                continue;
            }
            return word;
        }

        if (is_separators(s)) {
            if (word.empty()) {
                word += s;
                return word;
            }
            // ���� ��� �������� ������ �����������, �� ��� ��� �� �������� ��������� ������
            // ��� ���� ������� � ����� � ������� �� ���������� ������������ �����
            _input.unget();

            _pos._last_word = _pos._last_word.substr(0, _pos._last_word.size() - 1);
            (*_pos._data.rbegin())--;
            return word;
        }
        word += s;
    }
    return word;
}

Token lexical_analyzer::get_next_token() {
    // �������� ��������� �� ������ �����
    std::string curr_word = get_next_word();
    // ����� ���������� ��� ������
    if (curr_word.empty()) {
        return {};
    }
    if (is_separators(curr_word)) {
        Token tmp_token(get_separator_type(curr_word), curr_word);
        _tokens.Add(tmp_token);
        return tmp_token;
    }
    if (is_keyword(curr_word)) {
        _tokens.Add({ get_keyword_type(curr_word), curr_word });
        return { get_keyword_type(curr_word), curr_word };
    }
    DFA automat;
    _tokens.Add({ automat.process(curr_word), curr_word });
    return { automat.process(curr_word), curr_word };
}

void lexical_analyzer::return_last_word() {

    std::reverse(_pos._last_word.begin(), _pos._last_word.end());
    for (const char& s : _pos._last_word) {
        // ���� ���������� ������ �������� ������, �� ������� �� ������� ��������� ������
        // ����� ������� ��������� ������, ���� � ��� �� �������� ��������
        if (s == '\n' || (*_pos._data.rbegin()) == 0) {
            _pos._data.pop_back();
        }
        else {
            (*_pos._data.rbegin())--;
        }
        _input.putback(s);
    }
    if ((*_pos._data.rbegin()) == 0) {
        _pos._data.pop_back();
    }
}


//Hash_table lexical_analyzer::get_all_tokens() {
//    DFA automat = DFA();
//    while (true) {
//        Token curr = get_next_token();
//        if (_input.eof()) {
//            break;
//        }
//        if (curr.getText().empty()) {
//            continue;
//        }
//        _tokens.Add(curr);
//    }
//    return _tokens;
//}
