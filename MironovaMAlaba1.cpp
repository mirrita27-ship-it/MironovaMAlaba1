#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <iomanip>

// Директива using позволяет не писать std:: перед каждым объектом стандартной библиотеки
using namespace std;

// Функция очистки буфера ввода.
// Нужна, чтобы при некорректном вводе (например, буквы вместо числа)
// "мусор" в потоке cin не ломал дальнейший ввод.
void clearBuffer() {
    cin.clear();                                            // сбрасываем флаги ошибок потока
    cin.ignore(numeric_limits<streamsize>::max(), '\n');    // удаляем всё до конца строки
}

// Функция паузы — ждёт нажатия Enter, чтобы пользователь успел прочитать вывод
void pause() {
    cout << "\nPress Enter to continue...";
    clearBuffer();
    cin.get();
}

// Структура Pipe описывает сущность "труба".
// Структура объединяет разнородные данные (строка, число, флаг) в единый тип.
struct Pipe {
    string kmMark;       // километровая отметка (название участка)
    double length;       // длина трубы в км
    int diameter;        // диаметр в мм
    bool isUnderRepair;  // признак "в ремонте"

    // Считывание данных трубы с консоли с проверкой корректности ввода
    void inputFromConsole() {
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

    // Вывод данных трубы на консоль.
    // Метод помечен const, так как он не изменяет состояние объекта.
    void display() const {
        cout << "\n[PIPE]\n";
        cout << "Kilometer Mark: " << kmMark << "\n";
        // setprecision(2) + fixed — вывод длины с двумя знаками после запятой
        cout << "Length (km): " << fixed << setprecision(2) << length << "\n";
        cout << "Diameter (mm): " << diameter << "\n";
        cout << "Status: " << (isUnderRepair ? "UNDER REPAIR" : "OPERATIONAL") << "\n";
    }

    // Редактирование признака "в ремонте" — переключение на противоположный статус
    void editRepairStatus() {
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

    // Сохранение в файл: записываем маркер "PIPE" и все поля построчно
    void saveToFile(ofstream& out) const {
        out << "PIPE\n";
        out << kmMark << "\n";
        out << length << "\n";
        out << diameter << "\n";
        out << (isUnderRepair ? 1 : 0) << "\n";
    }

    // Загрузка из файла. Возвращает true, если чтение прошло успешно.
    // Маркер "PIPE" используется для проверки корректности формата файла.
    bool loadFromFile(ifstream& in) {
        string marker;
        in >> marker;
        if (marker != "PIPE") return false;

        clearBuffer();
        getline(in, kmMark);
        in >> length;
        in >> diameter;
        int repairFlag;
        in >> repairFlag;
        isUnderRepair = (repairFlag == 1);
        clearBuffer();

        return in.good();  // good() возвращает true, если с потоком всё в порядке
    }
};

// Структура Station описывает сущность "КС / НПС"
struct Station {
    string name;         // название станции
    int totalShops;      // общее количество цехов
    int workingShops;    // количество цехов в работе
    int stationClass;    // класс станции

    void inputFromConsole() {
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

    void display() const {
        cout << "\n[CS / PS]\n";
        cout << "Name: " << name << "\n";
        cout << "Station Class: " << stationClass << "\n";
        cout << "Total Shops: " << totalShops << "\n";
        cout << "Shops Working: " << workingShops << "\n";
    }

    // Запуск цеха — увеличиваем счётчик работающих, но только если есть свободные
    void startShop() {
        cout << "\n--- Start Shop ---\n";
        if (workingShops >= totalShops) {
            cout << "Error! All shops are already running (" << workingShops << " of " << totalShops << ").\n";
        }
        else {
            workingShops++;
            cout << "Shop started successfully. Currently working: " << workingShops << " of " << totalShops << ".\n";
        }
    }

    // Остановка цеха — уменьшаем счётчик, но не ниже нуля
    void stopShop() {
        cout << "\n--- Stop Shop ---\n";
        if (workingShops <= 0) {
            cout << "Error! All shops are already stopped.\n";
        }
        else {
            workingShops--;
            cout << "Shop stopped successfully. Currently working: " << workingShops << " of " << totalShops << ".\n";
        }
    }

    // Вложенное меню редактирования КС — работает в цикле, пока пользователь не выберет 0
    void editMenu() {
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

    void saveToFile(ofstream& out) const {
        out << "STATION\n";
        out << name << "\n";
        out << totalShops << "\n";
        out << workingShops << "\n";
        out << stationClass << "\n";
    }

    bool loadFromFile(ifstream& in) {
        string marker;
        in >> marker;
        if (marker != "STATION") return false;

        clearBuffer();
        getline(in, name);
        in >> totalShops;
        in >> workingShops;
        in >> stationClass;
        clearBuffer();

        return in.good();
    }
};

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
        system("cls");  // очистка экрана консоли (для Windows)
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
        while (!(cin >> choice)) {
            cout << "Input error! Enter a number from 0 to 7: ";
            clearBuffer();
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
                // Считываем первый маркер, чтобы понять, что в файле
                string firstMarker;
                inFile >> firstMarker;

                if (firstMarker == "PIPE") {
                    // Возвращаем указатель чтения в начало,
                    // чтобы функция loadFromFile прочитала маркер сама
                    inFile.seekg(0);
                    if (myPipe.loadFromFile(inFile)) {
                        isPipeAdded = true;
                        cout << "\nPipe successfully loaded.\n";
                    }
                }

                // Если файл не закончился — читаем вторую сущность
                if (!inFile.eof()) {
                    string secondMarker;
                    inFile >> secondMarker;
                    if (secondMarker == "STATION") {
                        // Откатываем позицию назад на длину маркера + символ перевода строки
                        inFile.seekg(-((int)secondMarker.length() + 1), ios::cur);
                        if (myStation.loadFromFile(inFile)) {
                            isStationAdded = true;
                            cout << "CS/PS successfully loaded.\n";
                        }
                    }
                }

                if (!isPipeAdded && !isStationAdded) {
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