#ifndef STATION_H
#define STATION_H

#include <string>
#include <fstream>

// Класс Station описывает сущность "КС / НПС".
// В заголовочном файле не используем "using namespace std", поэтому пишем std:: явно.
class Station {
public:
    std::string name;    // название станции
    int totalShops;      // общее количество цехов
    int workingShops;    // количество цехов в работе
    int stationClass;    // класс станции

    // Считывание данных станции с консоли с проверкой корректности ввода
    void inputFromConsole();

    // Вывод данных станции на консоль
    void display() const;

    // Запуск цеха — увеличиваем счётчик работающих, но только если есть свободные
    void startShop();

    // Остановка цеха — уменьшаем счётчик, но не ниже нуля
    void stopShop();

    // Вложенное меню редактирования КС — работает в цикле, пока пользователь не выберет 0
    void editMenu();

    // Сохранение в файл: маркер "STATION" и все поля построчно
    void saveToFile(std::ofstream& out) const;

    // Загрузка из файла. Маркер "STATION" уже прочитан вызывающим кодом.
    bool loadFromFile(std::ifstream& in);
};

#endif // STATION_H
