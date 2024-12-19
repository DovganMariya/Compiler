#include "main.h"
#include "parser.h"
#include "semantic.h"
//int main() {
//    std::ofstream fout("out.txt");
//    parser par("inp.txt");
//    if (par.run()) {
//        Hash_table res = par.get_hash_table();
//        par.print_parse_tree(fout);
//        fout << "\n\n\n\n\n";
//    }
//    else {
//        std::cout << "Error\n";
//    }
//    fout.close();
//    return 0;
//}
int main() {
    std::ofstream out("out.txt");
    parser par("inp.txt");
    if (par.run()) {
        Hash_table res = par.get_hash_table();

         vector<std::pair<size_t, Token>> res_to_arr = res.to_array();
        par.print_parse_tree(out);

        out << "\n\n\n\n\n";

        semantic sem(par.get_parse_tree(), par.get_hash_table());
        sem.run();
        if (sem.is_not_error())
        {
            out << "\n\n\n" << sem.get_rpn() << '\n';
            cout << "NO ERROR\n";
        }

    }
    else {
        std::cout << "Error\n";
    }
    out.close();
    return 0;
}