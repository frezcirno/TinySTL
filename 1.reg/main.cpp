#include "Database.h"

int main(int argc, char const* argv[]) {
    database db;
    db.Build();
    while (db.Loop())
        ;
    return 0;
}
