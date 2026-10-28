#include "Database.h"
#include "App.h"

#include <windows.h>
#include <iostream>

int main() {
    SetConsoleOutputCP(CP_UTF8);

    Database database;

    if (!database.Connect("cornell", "login123")) {
        std::cerr << database.GetLastError() << "\n";
        return 1;
    }

    std::cout << "Connected\n";

    App app(database);
    return app.Run();
}