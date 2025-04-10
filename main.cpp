#include "Headers/PDA.h"
#include "Headers/CFG.h"

using namespace std;

int main() {

    // Test PDA -> CFG
    PDA pda("input-pda2cfg1.json");
    pda.toCFG().print();

    // // Reading a CFG
    // CFG cfg("CFG.json");
    // cfg.print();

    // // CYK algorithm on a cfg
    // CFG cfg("input-cyk1.json");
    // cfg.accepts("accbb"); // has to be true
    // cfg.accepts("aa"); // has to be false

    // // LL(1) parser
    // CFG cfg("input-ll1-3.json");
    // cfg.ll();

    return 0;
}
