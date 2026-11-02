#include "Utils.h"

#include <iostream>
#include <limits>
#include <cstdlib>

using namespace std;

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

// Очистка экрана: на Windows через cls, на остальных системах через clear
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
