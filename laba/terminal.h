#ifndef TERMINAL_H
#define TERMINAL_H

#include <iostream>
#include <string>
#include "lexical_item.h"

/**
 * ����� ���������. ����������� �� ���������� lexical_item
 * \param _type - ��� ���������
 * */
class terminal : public lexical_item {
public:
    // ���� ����������
    enum type_terminal {
        FUNCTION, BEGIN, END, FUNCTION_NAME,
        DESCRIPTION, DESCR,
        TYPE, VAR_LIST,
        OPERATORS, OP,
        ID, CONST,
        TERM, EXPR,
        LET_NUM, INT_NUMBER, FLOAT_NUMBER, SIMPLE_EXPR
    };
    /**
     * ����������� �� ���������. �������� ���������������� ����� FUNCTION
     * */
    terminal() : _type(FUNCTION) {}
    /**
     * ����������� �� ���� ���������
     * */
    terminal(type_terminal type) : _type(type) {}
    /**
     * ����������� �����������
     * */
    terminal(const terminal& v) : _type(v._type) {}
    /**
     * ����� ���������� ��� ���������
     * */
    type_terminal type() const { return _type; }

    /**
     * ���������� ������ �� ���������� lexical_item
     * ����� ���������� ��� ������
     * \return "terminal"
     * */
    std::string class_name() const override { return { "terminal" }; }

    friend std::ostream& operator<<(std::ostream& out, const terminal& v);

private:
    type_terminal _type;
};

#endif // TERMINAL_H
