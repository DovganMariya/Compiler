#pragma once
#include "main.h"
#include "Token.h"
#include "Node.h"
class Node {
private:
    Token value;

    bool state;
public:
    // ����������� �� ���������
    Node() : value(Token()), state(true) {};

    // ����������� �� �����, ���������� ������� �� �����
    Node(Token& _value) : value(_value), state(true) {};

    // ����������� �� ����� � ��������� (����� �� ����)
    Node(Token& value, bool state) : value(value), state(state) {};

    Token& getValue() {
        return value;
    }

    void setValue(Token& value) {
        Node::value = value;
    }

    bool isState() {
        return state;
    }

    void setState(bool state) {
        Node::state = state;
    }
};
