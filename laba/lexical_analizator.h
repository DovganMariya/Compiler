

#include "main.h"
#include "Hash_table.h"
#include "Token.h"

struct position {
    // � �������� ������� � �������� i ����� ���������� �������� � ������ � ������� i + 1.
    vector<int> _data;
    // ��������� ����������� �����.
    string _last_word;
};

class lexical_analyzer {
private:
    // ���� ������ ����� �� �������� ���������� ������ ������
    std::stringstream _input;
    // ������� �� ����� ������������ ��������
    Hash_table _tokens;
    // ������� �������
    position _pos;

    /**
     * ����� ��������� ������� � ����� � ����������� �� �������� �������
     * \param s - ������ �� �������� ����������� ������� � �����
     * */
    void update_position(char s);
    /**
     * ����� ���������, ��� ������ �������� �������� ������������ (�������������� ��������)
     * \param s - ������ ��� ��������
     * */
    bool is_separators(char s);
    /**
     * ����� ���������, ��� ������ �������� �������� ������������ (�������������� ��������)
     * \param s - ������ ��� ��������
     * */
    bool is_separators(string& s);
    /**
     * ����� ���������, �������� �� ����� �������� (��� ����� int, char, return)
     * \param s - ����� ��� ��������
     * */
    bool is_keyword(string& s);
    /**
     * ����� ���������� ��� ������ �����������
     * \param s - ����� ��� ����������� ����
     * */
    Token::Type_lexeme get_separator_type(string& s);
    /**
     * ����� ���������� ��� ��������� �����
     * \param s - ����� ��� ����������� ����
     * */
    Token::Type_lexeme get_keyword_type(string& s);
    /**
     * ����� �������� ��������� �� ������ �����
     * */
    string get_next_word();
public:
    /**
     * ����������� �� ����� �����. ���� ����������� � ����� stringstream
     * \param file_name - ���� ��� ������
     * */
    lexical_analyzer(string file_name);
    /**
     * ����������. � ��� ����������� ����� stringstream
     * */
    ~lexical_analyzer() { _input.clear(); }
    /**
     * ����� ���������� ���-������� � ��������
     * */
    Hash_table tokens() const { return _tokens; }
    /**
     * ����� ���������� ������� �������
     * */
    position position() const { return _pos; }
    /**
     * ����� ���������� ��������� �� ������ �����
     * */
    Token get_next_token();
    /**
     * ����� ���������� � ����� ��������� ����������� �����
     * */
    void return_last_word();
    /**
     * ����� ��������� ����������� ������ �� ����� �����. ����� ��� ������ ���� ����� ���������
     * ��������
     * */
    Hash_table get_all_tokens();
};



