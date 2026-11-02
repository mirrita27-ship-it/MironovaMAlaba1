#ifndef PIPE_H
#define PIPE_H

#include <string>
#include <fstream>

// Класс Pipe описывает сущность "труба".
// Класс объединяет разнородные данные (строка, число, флаг) в единый тип.
// В заголовочном файле не используем "using namespace std", поэтому пишем std:: явно.
class Pipe {
public:
    std::string kmMark;  // километровая отметка (название участка)
    double length;       // длина трубы в км
    int diameter;        // диаметр в мм
    bool isUnderRepair;  // признак "в ремонте"

    // Считывание данных трубы с консоли с проверкой корректности ввода
    void inputFromConsole();

    // Вывод данных трубы на консоль.
    // Метод помечен const, так как он не изменяет состояние объекта.
    void display() const;

    // Редактирование признака "в ремонте" — переключение на противоположный статус
    void editRepairStatus();

    // Сохранение в файл: записываем маркер "PIPE" и все поля построчно
    void saveToFile(std::ofstream& out) const;

    // Загрузка из файла. Маркер "PIPE" уже прочитан вызывающим кодом.
    // Возвращает true, если чтение прошло успешно и значения корректны.
    bool loadFromFile(std::ifstream& in);
};

#endif // PIPE_H
