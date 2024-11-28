#include "main.h"
#include "parser.h"
int main() {
    std::ofstream fout("out.txt");
    parser par("inp.txt");
    if (par.run()) {
        Hash_table res = par.get_hash_table();
//        res.print_table();
        par.print_parse_tree(fout);
        fout << "\n\n\n\n\n";
    }
    else {
        std::cout << "Error\n";
    }
    fout.close();
    return 0;
}
