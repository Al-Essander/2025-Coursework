// Внимание! Это отладочная версия; не для демонстрации!

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <cstdlib>
#include <windows.h>
#include <ctime>
#include <io.h>
#include <fcntl.h>
#include <fstream>
#include <chrono>
#include <functional>
using namespace std;

// Объявление структуры Node
struct Node {
    wstring question;
    wstring answer;
    Node* next;

    Node(wstring q = L"", wstring a = L"", Node* n = nullptr)
        : question(q), answer(a), next(n) {}
};

// Прототипы функций
void print_current_time();
void debug_time(const string& action, function<void()> func);
void debug_search_time(Node* first, const wstring& query);
Node* new_elem(wstring q, wstring a);
void debug_print(Node* first);
void clearScreen();
void update_or_add_element(Node** first, wstring q, wstring a);
void clearList(Node* first);
void printFromFile(const wchar_t* filename);
void clearFile(const char* filename);
void saveToFile(Node* first, const wchar_t* filename);
void loadFromFile(Node** first, const wchar_t* filename);
void print(Node* first);
int menu();

// Реализации функций

void print_current_time() {
    time_t now = time(0);
    tm* local_time = localtime(&now);

    wcout << L"[DEBUG] Время ОС: "
        << (local_time->tm_hour) << L":"
        << (local_time->tm_min) << L":"
        << (local_time->tm_sec) << endl;
}

void debug_time(const string& action, function<void()> func) {
    auto start = chrono::high_resolution_clock::now();
    func();
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    wcout << L"[DEBUG] занял " << duration << L" мс" << endl;
}

void debug_search_time(Node* first, const wstring& query) {
    auto start = chrono::high_resolution_clock::now();

    Node* current = first;
    bool found = false;

    while (current != nullptr) {
        if (current->question == query) {
            found = true;
            break;
        }
        current = current->next;
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    if (found) {
        wcout << L"[DEBUG] Q found for " << duration << L" ms" << endl;
    }
    else {
        wcout << L"[DEBUG] Q wasnt found for " << duration << L" ms" << endl;
    }
}

Node* new_elem(wstring q, wstring a) {
    return new Node(q, a);
}

void debug_print(Node* first) {
    wcout << L"[DEBUG] Текущий список:\n";
    Node* current = first;
    while (current != nullptr) {
        wcout << L"Вопрос: " << current->question << L" | Ответ: " << current->answer << endl;
        current = current->next;
    }
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void update_or_add_element(Node** first, wstring q, wstring a) {
    Node* current = *first;
    Node* prev = nullptr;

    while (current != nullptr) {
        if (current->question == q) {
            current->answer = a;
            return;
        }
        prev = current;
        current = current->next;
    }

    Node* new_node = new_elem(q, a);

    if (*first == nullptr) {
        *first = new_node;
    }
    else {
        prev->next = new_node;
    }
}

void clearList(Node* first) {
    Node* current = first;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

void printFromFile(const wchar_t* filename) {
    clearScreen();

    wifstream file(filename);
    if (!file.is_open()) {
        wcout << L"\033[31mErr: Файл не найден или не может быть открыт.\033[0m" << endl;
        return;
    }

    wstring q, a;
    bool isEmpty = true;

    while (getline(file, q) && getline(file, a)) {
        int length = 17 + q.length() + a.length();
        wstring dashes(length, L'-');
        wcout << dashes << endl;
        wcout << L"Вопрос: " << q << L"  Ответ: " << a << endl;
        wcout << dashes << endl << endl;
        isEmpty = false;
    }

    if (isEmpty) {
        wcout << L"\033[31mФайл пуст. Нет данных для отображения.\033[0m" << endl;
    }

    file.close();
}

void clearFile(const char* filename) {
    ofstream file(filename, ios::trunc);
    if (!file) {
        cout << "Ошибка открытия файла для очистки!" << endl;
        return;
    }
    file.close();
}

void saveToFile(Node* first, const wchar_t* filename) {
    wofstream file(filename);
    if (!file.is_open()) {
        wcout << L"Ошибка открытия файла для записи!" << endl;
        return;
    }

    Node* current = first;
    while (current != nullptr) {
        file << current->question << L"\n";
        file << current->answer << L"\n";
        current = current->next;
    }
    file.close();
}

void loadFromFile(Node** first, const wchar_t* filename) {
    wifstream file(filename);
    if (!file.is_open()) {
        wcout << L"Файл не найден или не может быть открыт." << endl;
        return;
    }

    clearList(*first);
    *first = nullptr;

    wstring q, a;
    while (getline(file, q) && getline(file, a)) {
        update_or_add_element(first, q, a);
    }
    file.close();
}

void print(Node* first) {
    clearScreen();

    if (first != nullptr) {
        Node* current = first;

        while (current != nullptr) {
            int length = 17 + current->question.length() + current->answer.length();
            wstring dashes(length, L'-');
            wcout << dashes << endl;
            wcout << L"Вопрос: " << current->question << L"  Ответ: " << current->answer << endl;
            wcout << dashes << endl << endl;
            current = current->next;
        }
    }
    else {
        wcout << L"\033[31mErr: Нет базы данных с ответами...\033[0m" << endl;
    }
}

int menu() {
    int choice;
    wcout << L"\033[1mДобро пожаловать в чат-бот \033[32mNodeMaster (Версия отладки)!\033[0m" << endl << endl;
    wcout << L"\n\033[36mМеню:\033[0m\n\n";
    wcout << L"1. Добавить/Обновить элемент\n\n";
    wcout << L"2. Вывести список\n\n";
    wcout << L"3. Войти в чат\n\n";
    wcout << L"0. Выход\n\n";
    wcout << L"\033[31m9. Удалить данные бота\033[0m\n\n";
    wcout << L"Выберите действие: ";
    wcin >> choice;
    return choice;
}

int main() {
    // Установка режима работы с Unicode
    if (_setmode(_fileno(stdout), _O_U16TEXT) == -1) {
        cerr << "Ошибка установки режима вывода\n";
        return 1;
    }
    if (_setmode(_fileno(stdin), _O_U16TEXT) == -1) {
        cerr << "Ошибка установки режима ввода\n";
        return 1;
    }

    Node* first = nullptr;
    loadFromFile(&first, L"chatbot_data.txt");

    while (true) {
        int choice = menu();
        wstring q, a;

        switch (choice) {
        case 0:
            clearList(first);
            return 0;

        case 1: {
            debug_time("Добавление/обновление элемента", [&]() {
                clearScreen();
                wcout << L"(Регистр текста учитывается)" << endl << L"Введите вопрос: ";
                wcin.ignore();
                getline(wcin, q);

                wcout << L"Введите ответ: ";
                getline(wcin, a);

                update_or_add_element(&first, q, a);
                debug_print(first);
                });

            clearScreen();
            wcout << L"Вопрос-ответ добавлен/обновлен в базу. Выберите пункт 2 для просмотра..." << endl;
            break;
        }

        case 2: {
            debug_time("Вывод списка", [&]() {
                debug_print(first);
                print(first);
                });
            break;
        }

        case 3: {
            debug_time("Вход в чат", [&]() {
                wcin.ignore();
                wcout << L"\n\n\033[32mВы вошли в чат. Введите 'exit', чтобы выйти.\033[0m\n\n";
                wstring user_input;
                while (true) {
                    wcout << L"\033[36mВы: \033[0m";
                    getline(wcin, user_input);

                    if (user_input == L"exit") {
                        wcout << L"\n\n\033[36mЧат завершен.\033[0m\n\n\n";
                        break;
                    }

                    debug_search_time(first, user_input);

                    bool found = false;
                    Node* current = first;

                    while (current != nullptr) {
                        if (current->question == user_input) {
                            wcout << L"\033[31mБот: \033[0m" << current->answer << L"\n";
                            found = true;
                            break;
                        }
                        current = current->next;
                    }

                    if (!found) {
                        wcout << L"\033[31mБот: \033[0mЯ не знаю ответа на этот вопрос.\n";
                    }
                }
                });
            break;
        }

        case 9: {
            debug_time("Очистка данных", [&]() {
                clearScreen();
                clearFile("chatbot_data.txt");
                clearList(first);
                first = nullptr;
                saveToFile(first, L"chatbot_data.txt");
                wcout << L"\033[31mФайл и данные в памяти очищены.\033[0m" << endl;
                });
            break;
        }

        default:
            wcout << L"\033[31mНеверный выбор!\033[0m" << endl;
            break;
        }
    }

    return 0;
}
