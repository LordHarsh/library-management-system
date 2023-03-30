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
class Library {
    private:
        vector<Book*> books; // Vector of book pointers
        vector<User*> users; // Vector of user pointers
        string databaseFilename; // Filename of database file

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
        void deleteBook(int bookIndex);

        // Issue and return book functionality
        void issueBook(int bookIndex, User* user);
        void returnBook(int bookIndex, User* user);

        // Fine charging functionality
        float calculateFine(int daysLate);

        // User management functionality
        void addUser(string username, string password, bool isAdmin);
        void modifyUser(int userIndex, string username, string password, bool isAdmin);
        void viewUsers();
        void deleteUser(int userIndex);

        // Accessor functions
        vector<Book*> getBooks();
        vector<User*> getUsers();
};

// Book class definition
class Book {
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
class User {
    private:
        string username;
        string password;
        bool isAdmin;
        vector<Book*> borrowedBooks;

    public:
        User(string username, string password, bool isAdmin);

        // Accessor functions
        string getUsername();
        string getPassword();
        bool isAdminUser();
        vector<Book*> getBorrowedBooks();

        // Mutator functions
        void setUsername(string username);
        void setPassword(string password);
        void setIsAdmin(bool isAdmin);
        void borrowBook(Book* book);
        void returnBook(Book* book);
};

// Library implementation

// Constructor - loads books and users from file
Library::Library(string databaseFilename) {
    this->databaseFilename = databaseFilename;
    loadBooksFromFile();
    loadUsersFromFile();
}

// Helper functions for file I/O

void Library::saveBooksToFile() {
ofstream outfile;
outfile.open(databaseFilename + "_books.txt");
for (int i = 0; i < books.size(); i++) {
Book* book = books[i];
outfile << book->getTitle() << "," << book->getAuthor() << ","
<< book->getPublisher() << "," << book->getPublicationYear() << ","
<< book->getShelfNumber() << "," << book->isAvailable() << endl;
}
outfile.close();
}

void Library::loadBooksFromFile() {
    ifstream infile;
    infile.open(databaseFilename + "_books.txt");
    if (!infile.is_open()) {
        return;
    }
    string line;
    while (getline(infile, line)) {
        stringstream ss(line);
        string title, author, publisher, shelfNumber, availableStr;
        int publicationYear;
        bool available;
        getline(ss, title, ',');
        getline(ss, author, ',');
        getline(ss, publisher, ',');
        ss >> publicationYear;
        getline(ss, shelfNumber, ',');
        getline(ss, availableStr, ',');
        if (availableStr == "true") {
            available = true;
        } else {
            available = false;
        }
        addBook(title, author, publisher, publicationYear, shelfNumber, available);
    }
    infile.close();
}

void Library::saveUsersToFile() {
    ofstream outfile;
    outfile.open(databaseFilename + "_users.txt");
    for (int i = 0; i < users.size(); i++) {
        User* user = users[i];
        outfile << user->getUsername() << "," << user->getPassword() << ",";
        if (user->isAdminUser()) {
            outfile << "true";
        } else {
            outfile << "false";
        }
        outfile << endl;
    }
    outfile.close();
}

void Library::loadUsersFromFile() {
    ifstream infile;
    infile.open(databaseFilename + "_users.txt");
    if (!infile.is_open()) {
        return;
    }
    string line;
    while (getline(infile, line)) {
        stringstream ss(line);
        string username, password, isAdminStr;
        bool isAdmin;
        getline(ss, username, ',');
        getline(ss, password, ',');
        getline(ss, isAdminStr);
        if (isAdminStr == "true") {
            isAdmin = true;
        } else {
            isAdmin = false;
        }
        addUser(username, password, isAdmin);
    }
    infile.close();
}

// Book implementation

Book::Book(string title, string author, string publisher, int publicationYear, string shelfNumber, bool available) {
    this->title = title;
    this->author = author;
    this->publisher = publisher;
    this->publicationYear = publicationYear;
    this->shelfNumber = shelfNumber;
    this->available = available;
}

string Book::getTitle() {
    return title;
}

string Book::getAuthor() {
    return author;
}

string Book::getPublisher() {
    return publisher;
}

int Book::getPublicationYear() {
    return publicationYear;
}

string Book::getShelfNumber() {
    return shelfNumber;
}

bool Book::isAvailable() {
    return available;
}

void Book::setTitle(string title) {
    this->title = title;
}

void Book::setAuthor(string author) {
    this->author = author;
}

void Book::setPublisher(string publisher) {
    this->publisher = publisher;
}

void Book::setPublicationYear(int publicationYear) {
    this->publicationYear = publicationYear;
}

void Book::setShelfNumber(string shelfNumber) {
    this->shelfNumber = shelfNumber;
}

void Book::setAvailability(bool available) {
    this->available = available;
}

// User implementation

// Constructor
User::User(string username, string password, bool isAdmin) {
    this->username = username;
    this->password = password;
    this->isAdmin = isAdmin;
}

// Accessor functions
string User::getUsername() {
    return username;
}

string User::getPassword() {
    return password;
}

bool User::isAdminUser() {
    return isAdmin;
}

vector<Book*> User::getBorrowedBooks() {
    return borrowedBooks;
}

// Mutator functions
void User::setUsername(string username) {
    this->username = username;
}

void User::setPassword(string password) {
    this->password = password;
}

void User::setIsAdmin(bool isAdmin) {
    this->isAdmin = isAdmin;
}

void User::borrowBook(Book* book) {
    borrowedBooks.push_back(book);
}

void User::returnBook(Book* book) {
    // Remove the book from the borrowedBooks vector
    for (vector<Book*>::iterator it = borrowedBooks.begin(); it != borrowedBooks.end(); ++it) {
        if (*it == book) {
            borrowedBooks.erase(it);
            break;
        }
    }
}

// Accessor functions

vector<Book*> Library::getBooks() {
    return books;
}

vector<User*> Library::getUsers() {
    return users;
}

// Mutator functions

void Library::addBook(string title, string author, string publisher, int publicationYear, string shelfNumber, bool available) {
    Book* book = new Book(title, author, publisher, publicationYear, shelfNumber, available);
    books.push_back(book);
    saveBooksToFile();
}

void Library::modifyBook(int bookIndex, string title, string author, string publisher, int publicationYear, string shelfNumber, bool available) {
    Book* book = books[bookIndex];
    book->setTitle(title);
    book->setAuthor(author);
    book->setPublisher(publisher);
    book->setPublicationYear(publicationYear);
    book->setShelfNumber(shelfNumber);
    book->setAvailability(available);
    saveBooksToFile();
}

void Library::viewBooks() {
    if (books.empty()) {
        cout << "No books found in library." << endl;
        return;
    }
    cout << "Books in library: " << endl;
    for (int i = 0; i < books.size(); i++) {
        Book* book = books[i];
        cout << i + 1 << ". " << book->getTitle() << " by " << book->getAuthor() << endl;
        cout << "   Publisher: " << book->getPublisher() << "   Publication Year: " << book->getPublicationYear() << endl;
        cout << "   Shelf Number: " << book->getShelfNumber() << "   Available: " << (book->isAvailable() ? "Yes" : "No") << endl;
    }
}

void Library::deleteBook(int bookIndex) {
    Book* book = books[bookIndex];
    // Remove the book from the borrowedBooks list of all users
    for (int i = 0; i < users.size(); i++) {
        User* user = users[i];
        vector<Book*> borrowedBooks = user->getBorrowedBooks();
        for (int j = 0; j < borrowedBooks.size(); j++) {
            Book* borrowedBook = borrowedBooks[j];
            if (borrowedBook == book) {
                user->returnBook(book);
                break;
            }
        }
    }
    books.erase(books.begin() + bookIndex);
    saveBooksToFile();
}

void Library::issueBook(int bookIndex, User* user) {
    Book* book = books[bookIndex];
    if (!book->isAvailable()) {
        cout << "Book is not available." << endl;
        return;
    }
    book->setAvailability(false);
    user->borrowBook(book);
    saveBooksToFile();
}

void Library::returnBook(int bookIndex, User* user) {
    Book* book = books[bookIndex];
    if (book->isAvailable()) {
        cout << "Book is already available." << endl;
        return;
    }
    book->setAvailability(true);
    user->returnBook(book);
    saveBooksToFile();
}

float Library::calculateFine(int daysLate) {
    return 0.5 * daysLate;
}

void Library::addUser(string username, string password, bool isAdmin) {
    User* user = new User(username, password, isAdmin);
    users.push_back(user);
    saveUsersToFile();
}

void Library::modifyUser(int userIndex, string username, string password, bool isAdmin) {
    if (userIndex >= 0 && userIndex < users.size()) {
        User* user = users[userIndex];
        user->setUsername(username);
        user->setPassword(password);
        user->setIsAdmin(isAdmin);
        saveUsersToFile();
    }
}
// View all registered users
void Library::viewUsers() {
    cout << "Registered users:\n";
    for (int i = 0; i < users.size(); i++) {
        User* user = users[i];
        cout << "Username: " << user->getUsername() << ", Is Admin: " << (user->isAdminUser() ? "Yes" : "No") << endl;
    }
    cout << endl;
}
// Delete a registered user
void Library::deleteUser(int userIndex) {
    User* user = users[userIndex];

    // Remove all borrowed books from the user
    vector<Book*> borrowedBooks = user->getBorrowedBooks();
    for (int i = 0; i < borrowedBooks.size(); i++) {
        Book* book = borrowedBooks[i];
        book->setAvailability(true);
    }

    // Delete the user from the vector of users
    users.erase(users.begin() + userIndex);

    // Save the updated vector of users to file
    saveUsersToFile();

    cout << "User " << user->getUsername() << " has been deleted.\n\n";
}

// Get the vector of books
vector<Book*> Library::getBooks() {
    return books;
}

// Get the vector of users
vector<User*> Library::getUsers() {
    return users;
}

// Admin login functionality
bool Library::adminLogin(string username, string password) {
    for (int i = 0; i < users.size(); i++) {
        User* user = users[i];
        if (user->getUsername() == username && user->getPassword() == password && user->isAdminUser()) {
            return true;
        }
    }
    return false;
}

// Student login functionality
bool Library::studentLogin(string username, string password) {
    for (int i = 0; i < users.size(); i++) {
        User* user = users[i];
        if (user->getUsername() == username && user->getPassword() == password && !user->isAdminUser()) {
            return true;
        }
    }
    return false;
}
void Library::registerUser(string username, string password) {
    // Check if username already exists
    for (int i = 0; i < users.size(); i++) {
        if (users[i]->getUsername() == username) {
            cout << "Username already exists. Please choose another username." << endl;
            return;
        }
    }

    // Create new user and add to users vector
    User* user = new User(username, password, false);
    users.push_back(user);

    // Save updated user list to file
    saveUsersToFile();

    cout << "User registered successfully." << endl;
}



int main() {
    // Create library object
    Library library("library_database");

    // Admin login
    string adminUsername = "admin";
    string adminPassword = "admin123";
    bool isAdminLoggedIn = library.adminLogin(adminUsername, adminPassword);

    if (isAdminLoggedIn) {
        cout << "Admin logged in successfully." << endl;

        // Add a book
        library.addBook("The Great Gatsby", "F. Scott Fitzgerald", "Charles Scribner's Sons", 1925, "A123", true);

        // View all books
        library.viewBooks();

        // Modify a book
        library.modifyBook(0, "The Great Gatsby", "F. Scott Fitzgerald", "Charles Scribner's Sons", 1925, "B456", false);

        // View all books
        library.viewBooks();

        // Delete a book
        library.deleteBook(0);

        // View all books
        library.viewBooks();

        // Add a user
        library.addUser("johndoe", "password", false);

        // View all users
        library.viewUsers();

        // Modify a user
        library.modifyUser(0, "johndoe", "newpassword", true);

        // View all users
        library.viewUsers();

        // Delete a user
        library.deleteUser(0);

        // View all users
        library.viewUsers();

        // Logout
        isAdminLoggedIn = false;
    } else {
        // Student login
        string studentUsername = "johndoe";
        string studentPassword = "password";
        bool isStudentLoggedIn = library.studentLogin(studentUsername, studentPassword);

        if (isStudentLoggedIn) {
            cout << "Student logged in successfully." << endl;

            // View all books
            library.viewBooks();

            // Issue a book
            User* student = library.getUsers()[0];
            library.issueBook(0, student);

            // View borrowed books for student
            vector<Book*> borrowedBooks = student->getBorrowedBooks();
            cout << "Borrowed books for " << student->getUsername() << ":" << endl;
            for (int i = 0; i < borrowedBooks.size(); i++) {
                cout << i + 1 << ". " << borrowedBooks[i]->getTitle() << endl;
            }

            // Return a book
            library.returnBook(0, student);

            // View borrowed books for student
            borrowedBooks = student->getBorrowedBooks();
            cout << "Borrowed books for " << student->getUsername() << ":" << endl;
            for (int i = 0; i < borrowedBooks.size(); i++) {
                cout << i + 1 << ". " << borrowedBooks[i]->getTitle() << endl;
            }

            // Logout
            isStudentLoggedIn = false;
        } else {
            cout << "Invalid username or password." << endl;
        }
    }

    return 0;
}
