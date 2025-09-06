    #include <chrono>
    #include <bits/stdc++.h>
    using namespace std;

    struct User{
        User(string Name_, int Id_, string email_):Name(Name_), Id(Id_), email(email_){};
        User() : Name(""), Id(0), email("") {}
        virtual int getMaxBook();
        virtual int getBorrowdays();
        virtual int getFinePerDay();
        bool canBorrow(){
            return borrowed.size() < this->getMaxBook();
        };
        string Name;
        int Id;
        string email;
        set<int> borrowed;
    };

    struct Student : User{
        int getMaxBook() override{
            return 3;
        };
        
        int getBorrowdays() override{
            return 14;
        };

        int getFinePerDay() override{
            return 1;
        };
    };

    struct Facility : User{
        int getMaxBook() override{
            return 10;
        };
        
        int getBorrowdays() override{
            return 30;
        };

        int getFinePerDay() override{
            return 10;
        };
    };

    struct Guest : User{
        int getMaxBook() override{
            return 1;
        };
        
        int getBorrowdays() override{
            return 7;
        };

        int getFinePerDay() override{
            return 1;
        };
    };

    struct Book{
        string title;
        string author;
        int isbn;
        string genre;
        int owner;
        chrono::system_clock::duration LP;
        Book() : title(""), author(""), isbn(0), genre(""), owner(0), LP(chrono::system_clock::duration::zero()) {}
        Book(string title, string author, int isbn, string genre):title(title), author(author), isbn(isbn), genre(genre){};

    };

    class DB{
        public:
            void registerUser(string Name_, string email_, int Type){
                try
                {
                    switch (Type)
                    {
                    case 0:
                        Number_of_users++;
                        Users[Number_of_users] = Guest::User(Name_, Number_of_users, email_);
                        break;
                    case 1:
                        Users[Number_of_users] = Student::User(Name_, Number_of_users, email_);
                        break;
                    case 2:
                        Users[Number_of_users] = Facility::User(Name_, Number_of_users, email_);
                        break;
                    default:
                        cout << "Неверный тип пользователя";
                        return;
                    } 
                }
                catch(const std::exception& e)
                {
                    std::cerr << "Неверный запрос проверьте формат запроса." << '\n';
                }
                
            };

            User findUser(int Id){
                return Users[Id];
            };

            void borrowBook(int userId, int isbn){
                if (!Users.count(userId)){
                    cout << "Пользователя с таким id не существует." << endl;
                    return;
                }
                if (Books.count(isbn)){
                    if (Books[isbn].owner == 0){
                        Books[isbn].owner = userId;
                        Books[isbn].LP = now.time_since_epoch();
                        Users[userId].borrowed.insert(isbn);
                        cout << "Вот" << Books[isbn].title << ". Вы должны вернуть книги в пределе" << Users[userId].canBorrow() << " дней." << endl;
                        return;
                    }
                    else{
                        cout << "Даннаяя книга сейчас у другого читателя." << endl;
                        return;
                    }
                }
                else{
                    cout << "Книги с таким ISBN нету в нашей библиотеке." << endl;
                    return;
                }
            };

            void returnBook(int userId, int isbn){
                if (Users.count(userId) && Books.count(isbn)){
                    if (Users[userId].borrowed.count(isbn)){
                        Books[isbn].owner = 0;
                        Users[userId].borrowed.erase(isbn);
                        cout << "Книга успешно возвращена." << endl;
                        return;
                    }
                    else{
                        cout << "Вы не брали книгу с таким isbn, проверьте входные данные" << endl;
                        return;
                    }
                }
                else{
                    cout << "Пользователя или книги не существует." << endl;
                    return;
                }
            };

            void addBook(string title, string author, int isbn, string genre){
                if (Books.count(isbn)){
                    cout << "Книга с таким isbn уже существует" << endl;
                    return;
                }
                Books[isbn] = Book(title, author, isbn, genre);
                cout << "Книга успешно добавлена." << endl;
                    return;
            };

            void removeBook(int isbn){
                if (!Books.count(isbn)){
                    cout << "Книги с таким isbn не существует" << endl;
                    return;
                }
                Books.erase(isbn);
                cout << "Книга успешно добавлена." << endl;
            };

            void trackOverdue(){
                for (auto [isbn, Book] : Books){
                    if (Book.owner != 0){
                        std::chrono::days day_borrowed = chrono::duration_cast<chrono::days>(Book.LP);
                        std::chrono::days today = chrono::duration_cast<chrono::days>(now.time_since_epoch());
                        int day_b = static_cast<int>(day_borrowed.count());
                        int day_t = static_cast<int>(today.count());
                        if (day_t - day_b <= Users[Book.owner].getBorrowdays())
                            continue;
                        else{
                            cout << "Задолжность по книге " << Book.title << " за пользователем с id" << Book.owner << endl; 
                        }

                    }
            }};

        private:
            chrono::system_clock::time_point now = chrono::system_clock::now();
            int Number_of_users = 0; 
            map<int, Book> Books;
            map<int, User> Users;
    };

    int main() {
        cout << "=== Вас приветствует Библиотека ===" << endl;
        DB db;

        string command;

        cout << "Введите команду (help - список команд, 0 - выход)" << endl;

        while (true) {
            cout << "> ";               
            cin >> command;             

            if (command == "0") {       
                cout << "Выход из программы..." << endl;
                break;
            }
            else if (command == "help") {
                cout << "Доступные команды:" << endl;
                cout << " addbook    - добавить книгу" << endl;
                cout << " removebook - удалить книгу" << endl;
                cout << " register   - зарегистрировать пользователя" << endl;
                cout << " borrow     - взять книгу" << endl;
                cout << " return     - вернуть книгу" << endl;
                cout << " overdue    - проверить задолженности" << endl;
                cout << " 0          - выход" << endl;
            }
            else if (command == "addbook") {
                string title, author, genre;
                int isbn;
                cout << "Введите название: ";
                cin >> title;
                cout << "Введите автора: ";
                cin >> author;
                cout << "Введите ISBN (число): ";
                cin >> isbn;
                cout << "Введите жанр: ";
                cin >> genre;
                db.addBook(title, author, isbn, genre);
            }
            else if (command == "removebook") {
                int isbn;
                cout << "Введите ISBN: ";
                cin >> isbn;
                db.removeBook(isbn);
            }
            else if (command == "register") {
                string name, email;
                int type;
                cout << "Введите имя: ";
                cin >> name;
                cout << "Введите email: ";
                cin >> email;
                cout << "Введите тип (0 - гость, 1 - студент, 2 - сотрудник): ";
                cin >> type;
                db.registerUser(name, email, type);
            }
            else if (command == "borrow") {
                int userId, isbn;
                cout << "Введите ID пользователя: ";
                cin >> userId;
                cout << "Введите ISBN книги: ";
                cin >> isbn;
                db.borrowBook(userId, isbn);
            }
            else if (command == "return") {
                int userId, isbn;
                cout << "Введите ID пользователя: ";
                cin >> userId;
                cout << "Введите ISBN книги: ";
                cin >> isbn;
                db.returnBook(userId, isbn);
            }
            else if (command == "overdue") {
                db.trackOverdue();
            }
            else {
                // Если команда неизвестна
                cout << "Неизвестная команда. Введите 'help' для справки." << endl;
            }
        }

        return 0;
    }
