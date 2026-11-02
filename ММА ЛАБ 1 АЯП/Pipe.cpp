#include "Pipe.h"
#include "Utils.h"

#include <iostream>
#include <iomanip>
#include <limits>

using namespace std;

void Pipe::inputFromConsole() {
    cout << "--- Enter Pipe Data ---\n";

    cout << "Enter kilometer mark (name): ";
    clearBuffer();
    getline(cin, kmMark);
    // Защита от пустого названия
    while (kmMark.empty()) {
        cout << "Name cannot be empty. Please re-enter: ";
        getline(cin, kmMark);
    }

    // Проверка: введено ли число И больше ли оно нуля
    cout << "Enter pipe length (km, > 0): ";
    while (!(cin >> length) || length <= 0) {
        cout << "Error! Length must be a number greater than 0. Retry: ";
        clearBuffer();
    }

    cout << "Enter pipe diameter (mm, > 0): ";
    while (!(cin >> diameter) || diameter <= 0) {
        cout << "Error! Diameter must be an integer greater than 0. Retry: ";
        clearBuffer();
    }

    cout << "Is the pipe under repair? (1 - yes, 0 - no): ";
    int repairFlag;
    while (!(cin >> repairFlag) || (repairFlag != 0 && repairFlag != 1)) {
        cout << "Error! Enter 1 or 0: ";
        clearBuffer();
    }
    isUnderRepair = (repairFlag == 1);

    cout << "Pipe successfully added/updated.\n";
}

void Pipe::display() const {
    cout << "\n[PIPE]\n";
    cout << "Kilometer Mark: " << kmMark << "\n";
    // setprecision(2) + fixed — вывод длины с двумя знаками после запятой
    cout << "Length (km): " << fixed << setprecision(2) << length << "\n";
    cout << "Diameter (mm): " << diameter << "\n";
    cout << "Status: " << (isUnderRepair ? "UNDER REPAIR" : "OPERATIONAL") << "\n";
}

void Pipe::editRepairStatus() {
    cout << "\n--- Edit Pipe Status ---\n";
    cout << "Current status: " << (isUnderRepair ? "Under Repair" : "Operational") << "\n";
    cout << "Toggle status? (1 - yes, 0 - no): ";

    int choice;
    while (!(cin >> choice) || (choice != 0 && choice != 1)) {
        cout << "Error! Enter 1 or 0: ";
        clearBuffer();
    }

    if (choice == 1) {
        isUnderRepair = !isUnderRepair;  // логическое отрицание — переключение флага
        cout << "Status successfully changed to: " << (isUnderRepair ? "Under Repair" : "Operational") << "\n";
    }
    else {
        cout << "Changes cancelled.\n";
    }
}

void Pipe::saveToFile(ofstream& out) const {
    out << "PIPE\n";
    out << kmMark << "\n";
    out << setprecision(10);   // чтобы длина не теряла точность при записи
    out << length << "\n";
    out << diameter << "\n";
    out << (isUnderRepair ? 1 : 0) << "\n";
}

bool Pipe::loadFromFile(ifstream& in) {
    int repairFlag;
    if (!getline(in, kmMark)) return false;
    if (!(in >> length >> diameter >> repairFlag)) return false;
    in.ignore(numeric_limits<streamsize>::max(), '\n');  // дочитываем строку (именно из файла!)
    if (kmMark.empty() || length <= 0 || diameter <= 0 || (repairFlag != 0 && repairFlag != 1)) return false;
    isUnderRepair = (repairFlag == 1);
    return true;
}
