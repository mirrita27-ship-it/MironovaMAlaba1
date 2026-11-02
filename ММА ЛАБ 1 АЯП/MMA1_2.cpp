#include <iostream>
#include <string>
#include <fstream>
#include <limits>

#include "Pipe.h"
#include "Station.h"
#include "Utils.h"

// Директива using позволяет не писать std:: перед каждым объектом стандартной библиотеки
using namespace std;

// Точка входа в программу
int main() {
    // Создаём по одному экземпляру трубы и станции (по условию задачи)
    Pipe myPipe;
    Station myStation;

    // Флаги, показывающие, были ли объекты инициализированы пользователем.
    // Нужны, чтобы запретить просмотр/редактирование/сохранение пустых данных.
    bool isPipeAdded = false;
    bool isStationAdded = false;

    const string FILENAME = "pipeline_data.txt";

    int choice = -1;
    // Бесконечный цикл меню — прерывается только через return в пункте 0
    while (true) {
        clearScreen();  // очистка экрана консоли
        cout << "=====================================\n";
        cout << "   PIPELINE TRANSPORT MANAGEMENT\n";
        cout << "=====================================\n";
        cout << "1. Add Pipe\n";
        cout << "2. Add CS/PS\n";
        cout << "3. View All Objects\n";
        cout << "4. Edit Pipe\n";
        cout << "5. Edit CS/PS\n";
        cout << "6. Save to File\n";
        cout << "7. Load from File\n";
        cout << "0. Exit\n";
        cout << "=====================================\n";
        cout << "Select menu item: ";

        // Защита от ввода не-числа в главное меню
        while (true) {
            if (cin >> choice) {
                // Проверяем, что после числа в строке нет "мусора" (букв и т.п.)
                int nextChar = cin.peek();  // peek() возвращает int, поэтому и храним в int
                if (nextChar == '\n' || nextChar == EOF) {
                    break;
                }
                cout << "Input error! Enter a number from 0 to 7: ";
                clearBuffer();
            }
            else {
                if (cin.eof()) {  // ввод закрыт (Ctrl+Z / Ctrl+D) — выходим, чтобы не зациклиться
                    cout << "\nInput closed. Exiting.\n";
                    return 0;
                }
                cout << "Input error! Enter a number from 0 to 7: ";
                clearBuffer();
            }
        }

        // Переключатель по пунктам меню
        switch (choice) {
        case 1:
            myPipe.inputFromConsole();
            isPipeAdded = true;
            pause();
            break;

        case 2:
            myStation.inputFromConsole();
            isStationAdded = true;
            pause();
            break;

        case 3:
            if (!isPipeAdded && !isStationAdded) {
                cout << "\nNo data to display. Please add objects first.\n";
            }
            else {
                if (isPipeAdded) myPipe.display();
                if (isStationAdded) myStation.display();
            }
            pause();
            break;

        case 4:
            if (!isPipeAdded) {
                cout << "\nError! Please add a pipe first (Option 1).\n";
            }
            else {
                myPipe.editRepairStatus();
            }
            pause();
            break;

        case 5:
            if (!isStationAdded) {
                cout << "\nError! Please add a CS/PS first (Option 2).\n";
                pause();
            }
            else {
                myStation.editMenu();
            }
            break;

        case 6:
            if (!isPipeAdded && !isStationAdded) {
                cout << "\nError! Nothing to save. Add at least one object.\n";
                pause();
                break;
            }
            {
                // Открываем файл для записи; ofstream автоматически создаёт файл
                ofstream outFile(FILENAME);
                if (!outFile) {
                    cout << "\nError opening file for writing!\n";
                }
                else {
                    if (isPipeAdded) myPipe.saveToFile(outFile);
                    if (isStationAdded) myStation.saveToFile(outFile);
                    cout << "\nData successfully saved to " << FILENAME << "\n";
                }
            }
            pause();
            break;

        case 7:
        {
            ifstream inFile(FILENAME);
            if (!inFile) {
                cout << "\nError! File " << FILENAME << " not found.\n";
            }
            else {
                bool loadedAny = false;  // удалось ли загрузить хоть что-то из этого файла
                string marker;
                // Читаем файл по маркерам, без перемотки (seekg)
                while (inFile >> marker) {
                    inFile.ignore(numeric_limits<streamsize>::max(), '\n');  // дочитываем строку с маркером
                    if (marker == "PIPE") {
                        if (!myPipe.loadFromFile(inFile)) break;
                        isPipeAdded = true;
                        loadedAny = true;
                        cout << "\nPipe successfully loaded.\n";
                    }
                    else if (marker == "STATION") {
                        if (!myStation.loadFromFile(inFile)) break;
                        isStationAdded = true;
                        loadedAny = true;
                        cout << "CS/PS successfully loaded.\n";
                    }
                    else {
                        break;  // неизвестный маркер — файл повреждён
                    }
                }
                if (!loadedAny) {
                    cout << "\nFile is corrupted or has invalid format.\n";
                }
            }
        }
        pause();
        break;

        case 0:
            cout << "\nExiting program. Goodbye!\n";
            return 0;  // завершение программы

        default:
            cout << "\nInvalid menu item. Try again.\n";
            pause();
            break;
        }
    }

    return 0;
}
