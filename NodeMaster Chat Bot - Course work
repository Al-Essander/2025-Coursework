// OPEN SOURCE CODE


#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <cstdlib>
#include <windows.h>
#include <ctime>
#include <io.h> // Для wcin/wcout
#include <fcntl.h>
#include <fstream>
using namespace std;


struct Node {
    wstring question;
    wstring answer;
    Node* next;

    Node(wstring q = L"", wstring a = L"", Node* n = nullptr)
        : question(q), answer(a), next(n) {}
};

Node* new_elem(wstring q, wstring a) {
    return new Node(q, a);
}

// Очистить экран
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// Обнов/Добавл. Эл.
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

// Очистка списка
void clearList(Node* first) {
    Node* current = first;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

// Очистка файла
void clearFile(const char* filename) {
    ofstream file(filename, ios::trunc); // 'trunc' удаляет всё содержимое файла
    if (!file) {
        cout << "Ошибка открытия файла для очистки!" << endl;
        return;
    }
    file.close(); // Закрываем, чтобы очистить
}

// Сохранение в файл
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

// Загрузка из файла
void loadFromFile(Node** first, const wchar_t* filename) {
    wifstream file(filename);
    if (!file.is_open()) {
        wcout << L"Файл не найден или не может быть открыт." << endl;
        return;
    }

    // Очистка текущего списка
    clearList(*first);
    *first = nullptr;

    wstring q, a;
    while (getline(file, q) && getline(file, a)) {
        update_or_add_element(first, q, a);
    }
    file.close();
}


// Для ввода вопроса-ответа

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
    saveToFile(first, L"chatbot_data.txt");
    wcout << L"Данные сохранены в файл \033[32mchatbot_data.txt\033[0m" << endl<<endl<<endl;
}

int menu() {
    int choice;
    wcout << L"\033[1mДобро пожаловать в чат-бот \033[32mNodeMaster!\033[0m" << endl << endl;
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



//////////////////////////////////////


int main() {
    _setmode(_fileno(stdout), _O_U16TEXT);
    _setmode(_fileno(stdin), _O_U16TEXT);

    

    Node* first = nullptr;

    loadFromFile(&first, L"chatbot_data.txt"); // Загрузка вопросов-ответов при включении

    while (true) {

        int choice = menu();
        switch (choice) {




        case 0:
            return 0;



        case 1: {
            clearScreen();
            wstring q, a;

            wcout << L"(Регистр текста учитывается)" << endl << L"Введите вопрос: ";
            wcin.ignore();
            getline(wcin, q);

            wcout << L"Введите ответ: ";
            getline(wcin, a);

            update_or_add_element(&first, q, a);

            clearScreen();
            wcout << L"Вопрос-ответ '" << q << L"-" << a << L"' добавлен/обновлен в базу. Выберите пункт 2 для просмотра..." << endl;
            break;
        }



        case 2:
            print(first);
            break;



        case 3: {
            wcin.ignore();
            wcout << L"\n\n\033[32mВы вошли в чат. Введите 'exit', чтобы выйти.\033[0m\n\n";
            wstring user_input;
            while (true) {
                // Голубой "Вы: "
                wcout << L"\033[36mВы: \033[0m";
                getline(wcin, user_input);

                if (user_input == L"exit") {
                    wcout << L"\n\n\033[36mЧат завершен.\033[0m\n\n\n";
                    break;
                }

                bool found = false;
                Node* current = first;

                while (current != nullptr) {
                    if (current->question == user_input) {
                        // Красный "Бот: ", белый ответ
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
            break;
        }

        case 9: {
            clearScreen();
            clearFile("chatbot_data.txt");
            wcout << L"\033[31mФайл\033[0m chatbot_data.txt \033[31mочищен.\033[0m" << endl;
            break;
        }


        default:
            wcout << L"\033[31mНеверный выбор!\033[0m" << endl;
            break;
        }
    }
}


