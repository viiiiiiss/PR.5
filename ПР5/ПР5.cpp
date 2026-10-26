#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <Windows.h> 
#pragma execution_character_set("utf-8")
using namespace std;

//Класс Читатель (поля private)
class Reader {
private:
    string name, phone;
public:
    void set(string n, string p) { name = n; phone = p; }
    string getName() { return name; }
    string getPhone() { return phone; }
};

//Класс Книга
class Book {
private:
    string title, author;
    int year = 0;
    Reader* holder = nullptr; // у кого книга (nullptr - в библиотеке)
public:
    void set(string t, string a, int y) { title = t; author = a; year = y; }
    string getTitle() { return title; }
    string getAuthor() { return author; }
    int getYear() { return year; }
    Reader* getHolder() { return holder; }
    void setHolder(Reader* r) { holder = r; }
};

vector<shared_ptr<Book>> books; // книги - смарт-указатели
vector<Reader*> readers;        // читатели - динамические объекты (new/delete)

void addBook(string t, string a, int y) {
    shared_ptr<Book> b = make_shared<Book>();
    b->set(t, a, y);
    books.push_back(b);
}

void addReader(string n, string p) {
    Reader* r = new Reader();
    r->set(n, p);
    readers.push_back(r);
}

void showBooks() {
    for (int i = 0; i < (int)books.size(); i++) {
        cout << i + 1 << ". " << books[i]->getTitle() << ", " << books[i]->getAuthor()
            << ", " << books[i]->getYear() << " - ";
        if (books[i]->getHolder() == nullptr) cout << "в наличии\n";
        else cout << "выдана: " << books[i]->getHolder()->getName() << "\n";
    }
}

void showReaders() {
    for (int i = 0; i < (int)readers.size(); i++)
        cout << i + 1 << ". " << readers[i]->getName() << ", " << readers[i]->getPhone() << "\n";
}

// спрашивает номер книги, возвращает индекс или -1
int askBook() {
    showBooks();
    int n;
    cout << "Номер книги: ";
    cin >> n;
    if (n < 1 || n >(int)books.size()) { cout << "Нет такой книги\n"; return -1; }
    return n - 1;
}

int askReader() {
    showReaders();
    int n;
    cout << "Номер читателя: ";
    cin >> n;
    if (n < 1 || n >(int)readers.size()) { cout << "Нет такого читателя\n"; return -1; }
    return n - 1;
}

int main() {
    SetConsoleCP(65001); // устанавливаем кодировку входящего потока данных на UTF-8 
    SetConsoleOutputCP(65001); // устанавливаем кодировку исходящего потока данных на UTF-8
    // начальные данные
    addBook("Мастер_и_Маргарита", "Булгаков", 1967);
    addBook("Война_и_мир", "Толстой", 1869);
    addReader("Соседова_Варвара", "111-11-11");
    addReader("Потебенько_Анастасия", "444-55-66");
    books[0]->setHolder(readers[0]);

    int choice = -1;
    while (choice != 0) {
        cout << "\nБИБЛИОТЕКА\n"
            << "1. Список книг        7. Изменить читателя\n"
            << "2. Добавить книгу     8. Удалить читателя\n"
            << "3. Изменить книгу     9. Выдать книгу\n"
            << "4. Удалить книгу     10. Вернуть книгу\n"
            << "5. Список читателей  11. Книги читателя\n"
            << "6. Добавить читателя 12. Статистика\n"
            << "0. Выход\nВыбор: ";
        cin >> choice;
        cout << "\n";

        string a, b;
        int y, i, j;
        switch (choice) {
        case 1:
            showBooks();
            break;
        case 2:
            cout << "Название, автор, год: ";
            cin >> a >> b >> y;
            addBook(a, b, y);
            break;
        case 3:
            i = askBook();
            if (i >= 0) {
                cout << "Новые название, автор, год: ";
                cin >> a >> b >> y;
                books[i]->set(a, b, y);
            }
            break;
        case 4:
            i = askBook();
            if (i >= 0 && books[i]->getHolder() != nullptr) cout << "Книга выдана, удалить нельзя\n";
            else if (i >= 0) books.erase(books.begin() + i);
            break;
        case 5:
            showReaders();
            break;
        case 6:
            cout << "ФИО, телефон: ";
            cin >> a >> b;
            addReader(a, b);
            break;
        case 7:
            i = askReader();
            if (i >= 0) {
                cout << "Новые ФИО, телефон: ";
                cin >> a >> b;
                readers[i]->set(a, b);
            }
            break;
        case 8:
            i = askReader();
            if (i >= 0) {
                bool hasBooks = false;
                for (int k = 0; k < (int)books.size(); k++)
                    if (books[k]->getHolder() == readers[i]) hasBooks = true;
                if (hasBooks) cout << "У читателя есть книги, удалить нельзя\n";
                else {
                    delete readers[i];                 // освобождаем память
                    readers.erase(readers.begin() + i);
                }
            }
            break;
        case 9:
            i = askBook();
            if (i >= 0 && books[i]->getHolder() != nullptr) cout << "Книга уже выдана\n";
            else if (i >= 0) {
                j = askReader();
                if (j >= 0) books[i]->setHolder(readers[j]);
            }
            break;
        case 10:
            i = askBook();
            if (i >= 0) books[i]->setHolder(nullptr);
            break;
        case 11:
            j = askReader();
            if (j >= 0)
                for (int k = 0; k < (int)books.size(); k++)
                    if (books[k]->getHolder() == readers[j])
                        cout << books[k]->getTitle() << ", " << books[k]->getAuthor() << "\n";
            break;
        case 12: {
            int* issued = new int(0);                  // динамический счётчик
            for (int k = 0; k < (int)books.size(); k++)
                if (books[k]->getHolder() != nullptr) (*issued)++;
            cout << "Всего книг: " << books.size() << "\n"
                << "Выдано: " << *issued << "\n"
                << "В наличии: " << books.size() - *issued << "\n"
                << "Читателей: " << readers.size() << "\n";
            delete issued;                             // освобождает память
            break;
        }
        }
    }

    // перед выходом освобождает всех читателей (книги удалятся сами
    for (int k = 0; k < (int)readers.size(); k++) delete readers[k];
    return 0;
}