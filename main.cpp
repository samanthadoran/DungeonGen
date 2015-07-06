#include <iostream>
#include "include/game.h"

using namespace std;

int main() {
    srand(time(0));

    Game g;
    g.menu();

    cout << "Crash experienced is the fault of SFML, sorry" << endl;

    return 0;
}
