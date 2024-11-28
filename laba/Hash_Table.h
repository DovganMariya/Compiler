
#pragma once
#include "main.h"
#include "Node.h"
#include "Token.h"

using ptr_node = Node*;

class Hash_table {
private:

    // ����������� ������ �������
    static const int DEFAULT_SIZE = 7;

    // ����������� ���������� ������� (��������� ������ �� � ���������)
    const double REHASH_SIZE = 0.75;

    vector<ptr_node> arr;       // �������������� � ������� ����� �������� ��������� Node*
    int size;                   // ������� ��������� � ��� ������ � ������� (��� ����� deleted)


public:

    // ����������� �� ���������
    Hash_table();

    // ����������
    ~Hash_table() = default;

    size_t hash_function(string s, size_t size_hash_table);

    void resize();

    void rehash();

    bool Add(Token value);
    Token Find(string& value);

    int getSize();

    double getRehashSize();

    void print_table();

    void set_type(string& text, string& var_type);

    vector<pair<size_t, Token>> to_array() const;
};


