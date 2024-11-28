

#include "main.h"
#include "Token.h"
#include "Utils.h"

class DFA {
private:
    enum states {
        q_0_start,
        q_1_end_id,
        q_2_end_int,
        q_3_end_float,
        q_4 
    };

    enum transition {
        symbol_letter,
        symbol_digit,
        symbol_point,
        symbol_error
    };

    using Table_states = vector<vector<states>>;

    Table_states _table;



    /**
    * ����� ��������� "���" �������� - �� ��������� �� ������� ��������� � ����� ���������.
    * \param from - ��������� �� �������� ������ ���
    * \param curr_symbol - �� ������ ������� ������ �������
    * */

    states function_step(states from, transition curr_symbol);


    transition get_transition(char s);

public:

    // ����������� �� ���������
    DFA();

    // �������� ��� ������ �� ��� ���������� �������������
    // lex - ��������� ������������� ������
    Token::Type_lexeme process(string& lex);

};


