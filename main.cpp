#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <ctime>

using namespace std;

// Forward declaration of classes
class Book;
class User;

// Library class definition
class Library
{
private:
    vector<Book *> books;    // Vector of book pointers
    vector<User *> users;    // Vector of user pointers
    string databaseFilename; // Filename of database file
    User *loggedInUser;
    // Helper functions for file I/O
    void saveBooksToFile();
    void loadBooksFromFile();
    void saveUsersToFile();
    void loadUsersFromFile();

public:
    Library(string databaseFilename);

    // Admin login functionality
    bool adminLogin(string username, string password);

    // Student login functionality
    bool studentLogin(string username, string password);

    // Student Registration

    void registerUser(string username, string password);
    // Book management functionality
    void addBook(string title, string author, string publisher, int publicationYear, string shelfNumber, bool available);
    void modifyBook(int bookIndex, string title, string author, string publisher, int publicationYear, string shelfNumber, bool available);
    void viewBooks();
    vector<Book *> searchBooks(string query);
    void deleteBook(int bookIndex);

    // Issue and return book functionality
    void issueBook(int bookIndex, User *user);
    void returnBook(int bookIndex, User *user);

    // User management functionality
    void addUser(string username, string password, bool isAdmin);
    void modifyUser(int userIndex, string username, string password, bool isAdmin);
    void viewUsers();
    void deleteUser(int userIndex);
    void setLoggedInUser(User *user);

    // Accessor functions
    vector<Book *> getBooks();
    vector<User *> getUsers();
    User *getLoggedInUser() const;
};

// Book class definition
class Book
{
private:
    string title;
    string author;
    string publisher;
    int publicationYear;
    string shelfNumber;
    bool available;

public:
    Book(string title, string author, string publisher, int publicationYear, string shelfNumber, bool available);

    // Accessor functions
    string getTitle();
    string getAuthor();
    string getPublisher();
    int getPublicationYear();
    string getShelfNumber();
    bool isAvailable();

    // Mutator functions
    void setTitle(string title);
    void setAuthor(string author);
    void setPublisher(string publisher);
    void setPublicationYear(int publicationYear);
    void setShelfNumber(string shelfNumber);
    void setAvailability(bool available);
};

// User class definition
class User
{
private:
    string username;
    string password;
    bool isAdmin;
    vector<Book *> borrowedBooks;

public:
    User(string username, string password, bool isAdmin);

    // Accessor functions
    string getUsername();
    string getPassword();
    bool isAdminUser();
    vector<Book *> getBorrowedBooks();

    // Mutator functions
    void setUsername(string username);
    void setPassword(string password);
    void setIsAdmin(bool isAdmin);
    void borrowBook(Book *book);
    void returnBook(Book *book, int daysLate);
    // Fine charging functionality
    float calculateFine(int daysLate);
};

// Library implementation

// Constructor - loads books and users from file
Library::Library(string databaseFilename)
{
    this->databaseFilename = databaseFilename;
    loadBooksFromFile();
    loadUsersFromFile();
}

// Helper functions for file I/O

void Library::saveBooksToFile()
{
    ofstream outfile;
    outfile.open(databaseFilename + "_books.txt");
    vector<Book *> b = this->getBooks();
    for (int i = 0; i < b.size(); i++)
    {
        Book *book = b[i];
        outfile << book->getTitle() << "," << book->getAuthor() << ","
                << book->getPublisher() << "," << book->getPublicationYear() << ","
                << book->getShelfNumber() << "," << book->isAvailable() << endl;
    }
    outfile.close();
}

void Library::loadBooksFromFile()
{
    ifstream infile;
    infile.open(databaseFilename + "_books.txt");
    if (!infile.is_open())
    {
        return;
    }
    string line;
    while (getline(infile, line))
    {
        stringstream ss(line);
        string title, author, publisher, shelfNumber, availableStr, publicationYear;
        bool available;
        getline(ss, title, ',');
        getline(ss, author, ',');
        getline(ss, publisher, ',');
        // ss >> publicationYear;
        getline(ss, publicationYear, ',');
        getline(ss, shelfNumber, ',');
        getline(ss, availableStr);
        if (availableStr == "1")
        {
            available = true;
        }
        else
        {
            available = false;
        }
        addBook(title, author, publisher, stoi(publicationYear), shelfNumber, available);
    }
    infile.close();
}

void Library::saveUsersToFile()
{
    ofstream outfile;
    outfile.open(databaseFilename + "_users.txt");
    for (int i = 0; i < users.size(); i++)
    {
        User *user = users[i];
        outfile << user->getUsername() << "," << user->getPassword() << ",";
        if (user->isAdminUser())
        {
            outfile << "true";
        }
        else
        {
            outfile << "false";
        }
        outfile << endl;
    }
    outfile.close();
}

void Library::loadUsersFromFile()
{
    ifstream infile;
    infile.open(databaseFilename + "_users.txt");
    if (!infile.is_open())
    {
        return;
    }
    string line;
    while (getline(infile, line))
    {
        stringstream ss(line);
        string username, password, isAdminStr;
        bool isAdmin;
        getline(ss, username, ',');
        getline(ss, password, ',');
        getline(ss, isAdminStr);
        if (isAdminStr == "true")
        {
            isAdmin = true;
        }
        else
        {
            isAdmin = false;
        }
        addUser(username, password, isAdmin);
    }
    infile.close();
}

// Book implementation

Book::Book(string title, string author, string publisher, int publicationYear, string shelfNumber, bool available)
{
    this->title = title;
    this->author = author;
    this->publisher = publisher;
    this->publicationYear = publicationYear;
    this->shelfNumber = shelfNumber;
    this->available = available;
}

string Book::getTitle()
{
    return title;
}

string Book::getAuthor()
{
    return author;
}

string Book::getPublisher()
{
    return publisher;
}

int Book::getPublicationYear()
{
    return publicationYear;
}

string Book::getShelfNumber()
{
    return shelfNumber;
}

bool Book::isAvailable()
{
    return available;
}

void Book::setTitle(string title)
{
    this->title = title;
}

void Book::setAuthor(string author)
{
    this->author = author;
}

void Book::setPublisher(string publisher)
{
    this->publisher = publisher;
}

void Book::setPublicationYear(int publicationYear)
{
    this->publicationYear = publicationYear;
}

void Book::setShelfNumber(string shelfNumber)
{
    this->shelfNumber = shelfNumber;
}

void Book::setAvailability(bool available)
{
    this->available = available;
}

// User implementation

// Constructor
User::User(string username, string password, bool isAdmin)
{
    this->username = username;
    this->password = password;
    this->isAdmin = isAdmin;
}

// Accessor functions
string User::getUsername()
{
    return username;
}

string User::getPassword()
{
    return password;
}

bool User::isAdminUser()
{
    return isAdmin;
}

vector<Book *> User::getBorrowedBooks()
{
    return borrowedBooks;
}

// Mutator functions
void User::setUsername(string username)
{
    this->username = username;
}

void User::setPassword(string password)
{
    this->password = password;
}

void User::setIsAdmin(bool isAdmin)
{
    this->isAdmin = isAdmin;
}

void User::borrowBook(Book *book)
{
    borrowedBooks.push_back(book);
    book->setAvailability(false);
}

void User::returnBook(Book *book, int daysLate)
{
    // Remove the book from the borrowedBooks vector
    for (vector<Book *>::iterator it = borrowedBooks.begin(); it != borrowedBooks.end(); ++it)
    {
        if (*it == book)
        {
            int fine = this->calculateFine(daysLate);
            if (!fine)
            {
                cout << "Your fine is " << fine << " rupees";
            }
            book->setAvailability(true);
            borrowedBooks.erase(it);
            break;
        }
    }
}

// Accessor functions

vector<Book *> Library::getBooks()
{
    return books;
}

vector<User *> Library::getUsers()
{
    return users;
}

User *Library::getLoggedInUser() const
{
    return loggedInUser;
}

// Mutator functions

void Library::setLoggedInUser(User *user)
{
    loggedInUser = user;
}

void Library::addBook(string title, string author, string publisher, int publicationYear, string shelfNumber, bool available)
{
    Book *book = new Book(title, author, publisher, publicationYear, shelfNumber, available);
    books.push_back(book);
    saveBooksToFile();
}

void Library::modifyBook(int bookIndex, string title, string author, string publisher, int publicationYear, string shelfNumber, bool available)
{
    Book *book = books[bookIndex];
    book->setTitle(title);
    book->setAuthor(author);
    book->setPublisher(publisher);
    book->setPublicationYear(publicationYear);
    book->setShelfNumber(shelfNumber);
    book->setAvailability(available);
    saveBooksToFile();
}

void Library::viewBooks()
{
    if (books.empty())
    {
        cout << "No books found in library." << endl;
        return;
    }
    cout << "Books in library: " << endl;
    for (int i = 0; i < books.size(); i++)
    {
        Book *book = books[i];
        cout << i + 1 << ". " << book->getTitle() << " by " << book->getAuthor() << endl;
        cout << "   Publisher: " << book->getPublisher() << "   Publication Year: " << book->getPublicationYear() << endl;
        cout << "   Shelf Number: " << book->getShelfNumber() << "   Available: " << (book->isAvailable() ? "Yes" : "No") << endl;
    }
}

void Library::deleteBook(int bookIndex)
{
    Book *book = books[bookIndex];
    // Remove the book from the borrowedBooks list of all users
    for (int i = 0; i < users.size(); i++)
    {
        User *user = users[i];
        vector<Book *> borrowedBooks = user->getBorrowedBooks();
        for (int j = 0; j < borrowedBooks.size(); j++)
        {
            Book *borrowedBook = borrowedBooks[j];
            if (borrowedBook == book)
            {
                user->returnBook(book, 0);
                break;
            }
        }
    }
    books.erase(books.begin() + bookIndex);
    saveBooksToFile();
}

// void Library::issueBook(int bookIndex, User *user)
// {
//     Book *book = books[bookIndex];
//     if (!book->isAvailable())
//     {
//         cout << "Book is not available." << endl;
//         return;
//     }
//     book->setAvailability(false);
//     user->borrowBook(book);
//     saveBooksToFile();
// }

// void Library::returnBook(int bookIndex, User *user)
// {
//     Book *book = books[bookIndex];
//     if (book->isAvailable())
//     {
//         cout << "Book is already available." << endl;
//         return;
//     }
//     book->setAvailability(true);
//     user->returnBook(book);
//     saveBooksToFile();
// }

vector<Book *> Library::searchBooks(string query)
{
    vector<Book *> results;
    for (int i = 0; i < books.size(); i++)
    {
        Book *book = books[i];
        if (book->getTitle().find(query) != string::npos ||
            book->getAuthor().find(query) != string::npos ||
            book->getPublisher().find(query) != string::npos ||
            book->getShelfNumber().find(query) != string::npos)
        {
            results.push_back(book);
        }
    }
    return results;
}

float User::calculateFine(int daysLate)
{
    return 0.5 * daysLate;
}

void Library::addUser(string username, string password, bool isAdmin)
{
    User *user = new User(username, password, isAdmin);
    users.push_back(user);
    saveUsersToFile();
}

void Library::modifyUser(int userIndex, string username, string password, bool isAdmin)
{
    if (userIndex >= 0 && userIndex < users.size())
    {
        User *user = users[userIndex];
        user->setUsername(username);
        user->setPassword(password);
        user->setIsAdmin(isAdmin);
        saveUsersToFile();
    }
}
// View all registered users
void Library::viewUsers()
{
    cout << "Registered users:\n";
    for (int i = 0; i < users.size(); i++)
    {
        User *user = users[i];
        cout << "Username: " << user->getUsername() << ", Is Admin: " << (user->isAdminUser() ? "Yes" : "No") << endl;
    }
    cout << endl;
}
// Delete a registered user
void Library::deleteUser(int userIndex)
{
    User *user = users[userIndex];

    // Remove all borrowed books from the user
    vector<Book *> borrowedBooks = user->getBorrowedBooks();
    for (int i = 0; i < borrowedBooks.size(); i++)
    {
        Book *book = borrowedBooks[i];
        book->setAvailability(true);
    }

    // Delete the user from the vector of users
    users.erase(users.begin() + userIndex);

    // Save the updated vector of users to file
    saveUsersToFile();

    cout << "User " << user->getUsername() << " has been deleted.\n\n";
}

// Admin login functionality
bool Library::adminLogin(string username, string password)
{
    for (int i = 0; i < users.size(); i++)
    {
        User *user = users[i];
        if (user->getUsername() == username && user->getPassword() == password && user->isAdminUser())
        {
            return true;
        }
    }
    return false;
}

// Student login functionality
bool Library::studentLogin(string username, string password)
{
    for (int i = 0; i < users.size(); i++)
    {
        User *user = users[i];
        if (user->getUsername() == username && user->getPassword() == password && !user->isAdminUser())
        {
            this->setLoggedInUser(user);
            return true;
        }
    }
    return false;
}

// void Library::registerUser(string username, string password)
// {
//     // Check if username already exists
//     for (int i = 0; i < users.size(); i++)
//     {
//         if (users[i]->getUsername() == username)
//         {
//             cout << "Username already exists. Please choose another username." << endl;
//             return;
//         }
//     }

//     // Create new user and add to users vector
//     User *user = new User(username, password, false);
//     users.push_back(user);

//     // Save updated user list to file
//     saveUsersToFile();

//     cout << "User registered successfully." << endl;
// }

int main()
{
    // Check if the database files exist, create them if not
    string databaseFilename = "library";
    ifstream infile(databaseFilename + "_books.txt");
    if (!infile.good())
    {
        ofstream outfile(databaseFilename + "_books.txt");
        outfile.close();
    }
    infile.close();
    infile.open(databaseFilename + "_users.txt");
    if (!infile.good())
    {
        ofstream outfile(databaseFilename + "_users.txt");
        // Add default admin user
        outfile << "admin,admin,true\n";
        outfile.close();
    }
    infile.close();
    // Create library object
    Library library(databaseFilename);

    // Prompt user to login as admin or student
    string username, password;
    bool loggedIn = false;
    bool isAdmin = false;
    while (!loggedIn)
    {
        cout << "Login as admin or student?\n"
             << "Enter quit to exit\n";
        string userType;
        cin >> userType;
        if (userType == "admin")
        {
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            if (library.adminLogin(username, password))
            {
                isAdmin = true;
                loggedIn = true;
            }
            else
            {
                cout << "Invalid login credentials.\n";
            }
        }
        else if (userType == "student")
        {
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            if (library.studentLogin(username, password))
            {
                isAdmin = false;
                loggedIn = true;
            }
            else
            {
                cout << "Invalid login credentials.\n";
            }
        }
        else if (userType == "quit" || userType == "q")
        {
            return 0;
        }
        else
        {
            cout << "Invalid user type.\n";
        }
    }

    // Handle user interaction based on whether the user is an admin or student
    if (isAdmin)
    {
        // Admin menu
        while (true)
        {
            cout << "\nAdmin Menu:\n";
            cout << "1. Add book\n";
            cout << "2. Modify book\n";
            cout << "3. View books\n";
            cout << "4. Delete book\n";
            cout << "5. Add user\n";
            cout << "6. Modify user\n";
            cout << "7. View users\n";
            cout << "8. Delete user\n";
            cout << "9. Logout\n";
            int choice;
            cin >> choice;
            switch (choice)
            {
            case 1:
            {
                string title, author, publisher, shelfNumber;
                int publicationYear;
                bool available;
                cout << "Enter book details:\n";
                cout << "Title: ";
                cin.ignore();
                getline(cin, title);
                cout << "Author: ";
                getline(cin, author);
                cout << "Publisher: ";
                getline(cin, publisher);
                cout << "Publication year: ";
                cin >> publicationYear;
                cout << "Shelf number: ";
                cin >> shelfNumber;
                cout << "Available? (1/0): ";
                cin >> available;
                library.addBook(title, author, publisher, publicationYear, shelfNumber, available);
                cout << "Book added successfully.\n";
                break;
            }
            case 2:
            {
                int bookIndex;
                cout << "Enter index of book to modify: ";
                cin >> bookIndex;
                if (bookIndex >= 0 && bookIndex < library.getBooks().size())
                {
                    Book *book = library.getBooks()[bookIndex];
                    string title, author, publisher, shelfNumber;
                    int publicationYear;
                    bool available;
                    cout << "Enter new book details:\n";
                    cout << "Title: ";
                    cin.ignore();
                    getline(cin, title);
                    cout << "Author: ";
                    getline(cin, author);
                    cout << "Publisher: ";
                    getline(cin, publisher);
                    cout << "Publication year: ";
                    cin >> publicationYear;
                    cout << "Shelf number: ";
                    cin >> shelfNumber;
                    cout << "Available? (1/0): ";
                    cin >> available;
                    library.modifyBook(bookIndex, title, author, publisher, publicationYear, shelfNumber, available);
                    cout << "Book modified successfully.\n";
                }
                else
                {
                    cout << "Invalid book index.\n";
                }
                break;
            }
            case 3:
            {
                library.viewBooks();
                break;
            }
            case 4:
            {
                int bookIndex;
                cout << "Enter index of book to delete: ";
                cin >> bookIndex;
                if (bookIndex >= 0 && bookIndex < library.getBooks().size())
                {
                    library.deleteBook(bookIndex);
                    cout << "Book deleted successfully.\n";
                }
                else
                {
                    cout << "Invalid book index.\n";
                }
                break;
            }
            case 5:
            {
                string username, password;
                bool isAdmin;
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                cout << "Is admin? (1/0): ";
                cin >> isAdmin;
                library.addUser(username, password, isAdmin);
                cout << "User added successfully.\n";
                break;
            }
            case 6:
            {
                int userIndex;
                cout << "Enter index of user to modify: ";
                cin >> userIndex;
                if (userIndex >= 0 && userIndex < library.getUsers().size())
                {
                    User *user = library.getUsers()[userIndex];
                    string username, password;
                    bool isAdmin;
                    cout << "Enter new username: ";
                    cin >> username;
                    cout << "Enter new password: ";
                    cin >> password;
                    cout << "Is admin? (1/0): ";
                    cin >> isAdmin;
                    library.modifyUser(userIndex, username, password, isAdmin);
                    cout << "User modified successfully.\n";
                }
                else
                {
                    cout << "Invalid user index.\n";
                }
                break;
            }

            case 7:
            {
                library.viewUsers();
                break;
            }
            case 8:
            {
                int userIndex;
                cout << "Enter index of user to delete: ";
                cin >> userIndex;
                if (userIndex >= 0 && userIndex < library.getUsers().size())
                {
                    library.deleteUser(userIndex);
                    cout << "User deleted successfully.\n";
                }
                else
                {
                    cout << "Invalid user index.\n";
                }
                break;
            }
            case 9:
            {
                cout << "Logging out...\n";
                loggedIn = false;
                break;
            }
            default:
            {
                cout << "Invalid choice.\n";
                break;
            }
            }
            if (!loggedIn)
            {
                break;
            }
        }
    }
    else
    {
        // Student menu
        while (true)
        {
            cout << "\nStudent Menu:\n";
            cout << "1. View books\n";
            cout << "2. Search books\n";
            cout << "3. Borrow book\n";
            cout << "4. Return book\n";
            cout << "5. Logout\n";
            int choice;
            cin >> choice;
            switch (choice)
            {
            case 1:
            {
                // View books
                vector<Book *> books = library.getBooks();
                cout << "\nBooks:\n";
                for (int i = 0; i < books.size(); i++)
                {
                    Book *book = books[i];
                    if (book->isAvailable())
                    {
                        cout << i << ". " << book->getTitle() << " by " << book->getAuthor() << " (available)\n";
                    }
                    else
                    {
                        cout << i << ". " << book->getTitle() << " by " << book->getAuthor() << " (not available)\n";
                    }
                }
                break;
            }
            case 2:
            {
                string query;
                cout << "Enter search query: ";
                cin.ignore();
                getline(cin, query);
                vector<Book *> books = library.searchBooks(query);
                if (books.size() == 0)
                {
                    cout << "No books found.\n";
                }
                else
                {
                    for (int i = 0; i < books.size(); i++)
                    {
                        Book *book = books[i];
                        cout << i << ". " << book->getTitle() << " by " << book->getAuthor() << " (";
                        if (book->isAvailable())
                        {
                            cout << "available";
                        }
                        else
                        {
                            cout << "not available";
                        }
                        cout << ")\n";
                    }
                }
            }
            case 3:
            {
                // Borrow book
                vector<Book *> books = library.getBooks();
                cout << "\nBooks:\n";
                for (int i = 0; i < books.size(); i++)
                {
                    Book *book = books[i];
                    if (book->isAvailable())
                    {
                        cout << i << ". " << book->getTitle() << " by " << book->getAuthor() << " (available)\n";
                    }
                }
                cout << "Enter index of book to borrow: ";
                int bookIndex;
                cin >> bookIndex;
                if (bookIndex >= 0 && bookIndex < books.size())
                {
                    Book *book = books[bookIndex];
                    if (book->isAvailable())
                    {
                        User *user = library.getLoggedInUser();
                        user->borrowBook(book);
                        cout << "Book borrowed successfully.\n";
                    }
                    else
                    {
                        cout << "Book not available for borrowing.\n";
                    }
                }
                else
                {
                    cout << "Invalid book index.\n";
                }
                break;
            }
            case 4:
            {
                // Return book
                User *user = library.getLoggedInUser();
                vector<Book *> borrowedBooks = user->getBorrowedBooks();
                if (borrowedBooks.size() == 0)
                {
                    cout << "You have no borrowed books.\n";
                }
                else
                {
                    cout << "\nBorrowed Books:\n";
                    for (int i = 0; i < borrowedBooks.size(); i++)
                    {
                        Book *book = borrowedBooks[i];
                        cout << i << ". " << book->getTitle() << " by " << book->getAuthor() << "\n";
                    }
                    cout << "Enter index of book to return: ";
                    int bookIndex;
                    cin >> bookIndex;
                    if (bookIndex >= 0 && bookIndex < borrowedBooks.size())
                    {
                        int daysLate;
                        cout << "Enter number of days late:\n";
                        cin >> daysLate;
                        User *user = library.getLoggedInUser();
                        Book *book = borrowedBooks[bookIndex];
                        user->returnBook(book, daysLate);
                        cout << "Book returned successfully.\n";
                    }
                    else
                    {
                        cout << "Invalid book index.\n";
                    }
                }
                break;
            }
            case 5:
            {
                // Logout
                cout << "Logging out...\n";
                loggedIn = false;
                break;
            }
            default:
                cout << "Invalid choice.\n";
                break;
            }
            if (!loggedIn)
            {
                break;
            }
        }
    }
}
