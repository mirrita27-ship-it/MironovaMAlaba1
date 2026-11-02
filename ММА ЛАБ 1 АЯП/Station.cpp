#include "Station.h"
#include "Utils.h"

#include <iostream>
#include <limits>

using namespace std;

void Station::inputFromConsole() {
    cout << "--- Enter CS/PS Data ---\n";

    cout << "Enter station name: ";
    clearBuffer();
    getline(cin, name);
    while (name.empty()) {
        cout << "Name cannot be empty. Please re-enter: ";
        getline(cin, name);
    }

    cout << "Enter total number of shops (> 0): ";
    while (!(cin >> totalShops) || totalShops <= 0) {
        cout << "Error! Number of shops must be greater than 0. Retry: ";
        clearBuffer();
    }

    // Проверка: количество работающих цехов не может превышать общее число
    cout << "Enter number of shops in operation (0 to " << totalShops << "): ";
    while (!(cin >> workingShops) || workingShops < 0 || workingShops > totalShops) {
        cout << "Error! Value must be between 0 and " << totalShops << ". Retry: ";
        clearBuffer();
    }

    cout << "Enter station class (integer): ";
    while (!(cin >> stationClass)) {
        cout << "Error! Enter an integer: ";
        clearBuffer();
    }

    cout << "CS/PS successfully added/updated.\n";
}

void Station::display() const {
    cout << "\n[CS / PS]\n";
    cout << "Name: " << name << "\n";
    cout << "Station Class: " << stationClass << "\n";
    cout << "Total Shops: " << totalShops << "\n";
    cout << "Shops Working: " << workingShops << "\n";
}

void Station::startShop() {
    cout << "\n--- Start Shop ---\n";
    if (workingShops >= totalShops) {
        cout << "Error! All shops are already running (" << workingShops << " of " << totalShops << ").\n";
    }
    else {
        workingShops++;
        cout << "Shop started successfully. Currently working: " << workingShops << " of " << totalShops << ".\n";
    }
}

void Station::stopShop() {
    cout << "\n--- Stop Shop ---\n";
    if (workingShops <= 0) {
        cout << "Error! All shops are already stopped.\n";
    }
    else {
        workingShops--;
        cout << "Shop stopped successfully. Currently working: " << workingShops << " of " << totalShops << ".\n";
    }
}

void Station::editMenu() {
    int choice = -1;
    while (choice != 0) {
        cout << "\n--- Edit CS/PS ---\n";
        cout << "1. Start a shop\n";
        cout << "2. Stop a shop\n";
        cout << "0. Return to main menu\n";
        cout << "Select action: ";

        while (!(cin >> choice) || (choice != 0 && choice != 1 && choice != 2)) {
            cout << "Error! Enter 0, 1, or 2: ";
            clearBuffer();
        }

        if (choice == 1) startShop();
        else if (choice == 2) stopShop();
    }
}

void Station::saveToFile(ofstream& out) const {
    out << "STATION\n";
    out << name << "\n";
    out << totalShops << "\n";
    out << workingShops << "\n";
    out << stationClass << "\n";
}

bool Station::loadFromFile(ifstream& in) {
    if (!getline(in, name)) return false;
    if (!(in >> totalShops >> workingShops >> stationClass)) return false;
    in.ignore(numeric_limits<streamsize>::max(), '\n');  // дочитываем строку (именно из файла!)
    if (name.empty() || totalShops <= 0 || workingShops < 0 || workingShops > totalShops) return false;
    return true;
}
