
#include "Hash_table.h"
Hash_table::Hash_table()
{
    size = DEFAULT_SIZE;
    arr.resize(size);
    for (int i = 0; i < size; ++i) {
        arr[i] = nullptr;                             //заполн€ем nullptr - то есть если значение отсутствует,
    }                                                 // и никто раньше по этому адресу не обращалс€
}


size_t Hash_table::hash_function(string s, size_t size_hash_table)
{
    size_t hash_result = 0;
    size_t hash_coefficient = 57;
    for (char i : s) {
        hash_result = (hash_coefficient * hash_result + i) % size_hash_table;
    }
    hash_result = (hash_result * 2 + 1) % size_hash_table;
    return hash_result;
}


bool Hash_table::Add(Token value) {
    // ѕровер€ем, не нужно ли делать ре-хеширование
    if (REHASH_SIZE <= (1.0 * size / arr.size())) {
        rehash();
    }
    int index = hash_function(value.getText(), arr.size());
    while (arr[index] != nullptr && arr[index]->isState()) {
        if (arr[index]->getValue().isEqual(value)) {
            return false;
        }
        index += 1;
        index %= arr.size();
    }
    size++;
    // ¬ставка происходить по-разному: если это пустой узел, то просто создаем новый, а если это
    // удаленный узел, то замен€ем значение в нем и поднимаем флаг
    if (arr[index] == nullptr) {
        arr[index] = new Node(value, true);
    }
    else {
        arr[index]->setValue(value);
        arr[index]->setState(true);
    }
    return true;
}


// изменение размера хэш таблицы
void Hash_table::resize()
{
    int past_buffer_size = size;
    size = 0;
    vector<ptr_node> arr2 = vector<ptr_node>(size * 2, nullptr);
    swap(arr, arr2);
    for (int i = 0; i < past_buffer_size; ++i)
    {
        if (arr2[i] && arr2[i]->isState())
        {
            this->Add(arr2[i]->getValue()); 
        }

    }
    // удаление предыдущего массива
    for (int i = 0; i < past_buffer_size; ++i)
        if (arr2[i])
            delete arr2[i];

    arr2.clear();
    arr2.resize(0);
}

void Hash_table::rehash()
{
    size = 0;
    vector<ptr_node> arr2(arr);
    fill(all(arr), nullptr);
    arr.resize(arr.size() * 2, nullptr);

    for (auto i : arr2)
    {
        if (i != nullptr && i->isState())
        {
            Add(i->getValue());
        }
    }

    // удаление предыдущего массива
    arr2.clear();
    arr2.resize(0);
}

Token Hash_table::Find(string& text)
{
    size_t index = hash_function(text, arr.size());

    while (arr[index] != nullptr && arr[index]->getValue().getText() != text)
    {
        index = (index + 1) % arr.size();
    }

    if (arr[index] != nullptr && arr[index]->getValue().getText() == text && arr[index]->isState())
    {
        return arr[index]->getValue();
    }

    return {};
}



void Hash_table::print_table() {
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] == nullptr || !arr[i]->isState()) {
            continue;
        }
        std::cout << "Key: " << i << "\tvalue: " << ' ' << arr[i]->getValue().print_type_token(arr[i]->getValue()) << '\n';
    }
}


int Hash_table::getSize() {
    return size;
}

double Hash_table::getRehashSize() {
    return REHASH_SIZE;
}

void Hash_table::set_type(string& text, string& var_type) {
    size_t idx = hash_function(text, arr.size());
    while (arr[idx] != nullptr && arr[idx]->getValue().get_text() != text) {
        idx = (idx + 1) % arr.size();
    }
    if (arr[idx] != nullptr && arr[idx]->getValue().get_text() == text && arr[idx]->isState()) {
        arr[idx]->getValue().set_var_type(var_type);
    }
}

std::vector<std::pair<size_t, Token>> Hash_table::to_array() const {
    std::vector<std::pair<size_t, Token>> res;
    for (size_t i = 0; i < arr.size(); i++) {
        if (arr[i] == nullptr || arr[i]->isState()) {
            continue;
        }
        res.emplace_back(i, arr[i]->getValue());
    }
    return res;
}
