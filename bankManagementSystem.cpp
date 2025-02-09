#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <stdexcept>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iomanip>
#include <random>
#include <memory>

using namespace std;

// Forward declaration of the Account class
class Account;

// Function to generate a random 8-digit account number
string generateAccountNumber(const vector<unique_ptr<Account>>& accounts);

class Account {
protected:
    string accountNumber;
    string accountHolderName;
    double balance;

public:
    Account(string accNum = "", string accHolderName = "", double initialBalance = 0)
        : accountNumber(accNum), accountHolderName(accHolderName), balance(initialBalance) {}

    virtual void deposit(double amount) {
        if (cin.fail() || amount <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw invalid_argument("Deposit amount must be a positive number.");
        }
        balance += amount;
        updateFile();
        cout << "Amount deposited: " << amount << ". Current balance: " << balance << endl;
    }

    virtual void withdraw(double amount) {
        if (cin.fail() || amount <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            throw invalid_argument("Withdrawal amount must be positive.");
        }
        if (amount > balance) {
            throw runtime_error("Insufficient balance!");
        }
        balance -= amount;
        updateFile();
        cout << "Amount withdrawn: " << amount << ". Current balance: " << balance << endl;
    }

    virtual void displayBalance() const {
        cout << "Account Number: " << accountNumber << endl;
        cout << "Account Holder: " << accountHolderName << endl;
        cout << "Current Balance: " << balance << endl;
    }

    string getAccountNumber() const {
        return accountNumber;
    }

    virtual string serialize() const {
        return accountNumber + "," + accountHolderName + "," + to_string(balance);
    }

    virtual void updateFile() {
        // Basic implementation (this can be overridden in derived classes)
        cout << "Updating file for account: " << accountNumber << endl;
    }

    virtual ~Account() {}
};

class SavingsAccount : public Account {
public:
    SavingsAccount(string accNum, string accHolderName, double initialBalance)
        : Account(accNum, accHolderName, initialBalance) {}

    void withdraw(double amount) override {
        if (amount > balance * 0.8) {
            throw runtime_error("Withdrawal amount exceeds 80% of balance in Savings Account!");
        }
        Account::withdraw(amount);
    }

    void updateFile() override {
        // SavingsAccount-specific file update implementation
        cout << "Saving specific update for account: " << accountNumber << endl;
    }
};

class CurrentAccount : public Account {
public:
    CurrentAccount(string accNum, string accHolderName, double initialBalance)
        : Account(accNum, accHolderName, initialBalance) {}

    void updateFile() override {
        // CurrentAccount-specific file update implementation
        cout << "Current account specific update for account: " << accountNumber << endl;
    }
};

class User {
private:
    string username;
    string password; // Store password in plain text
    string role; // "admin" or "user"

public:
    User(string uname = "", string pwd = "", string r = "user")
        : username(uname), password(pwd), role(r) {}

    string getUsername() const {
        return username;
    }

    string getRole() const {
        return role;
    }

    bool authenticate(const string& pwd) const {
        return password == pwd; // Compare plain text passwords
    }

    string serialize() const {
        return username + "," + password + "," + role;
    }
};

class Bank {
private:
    vector<unique_ptr<Account>> accounts;
    map<string, User> users;
    User* currentUser;

public:
    Bank() : currentUser(nullptr) {
        srand(time(0)); // Seed for random number generation
        loadAccounts();
        loadUsers();
    }

    void loadAccounts() {
        ifstream file("accounts.txt");
        if (!file) return;

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string accNum, accHolderName, balanceStr;
            getline(ss, accNum, ',');
            getline(ss, accHolderName, ',');
            getline(ss, balanceStr);
            double balance = stod(balanceStr);
            accounts.push_back(make_unique<SavingsAccount>(accNum, accHolderName, balance));
        }
        file.close();
    }

    void loadUsers() {
        ifstream file("users.txt");
        if (!file) {
            // Create a default admin account if no users file exists
            users["admin"] = User("admin", "admin123", "admin");
            saveUsers();
            return;
        }

        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            string username, password, role;
            getline(ss, username, ',');
            getline(ss, password, ',');
            getline(ss, role);
            users[username] = User(username, password, role);
        }
        file.close();
    }

    void saveAccounts() {
        ofstream file("accounts.txt");
        if (!file) {
            throw runtime_error("Failed to save accounts!");
        }
        for (const auto& acc : accounts) {
            file << acc->serialize() << endl;
        }
        file.close();
    }

    void saveUsers() {
        ofstream file("users.txt");
        if (!file) {
            throw runtime_error("Failed to save users!");
        }
        for (const auto& user : users) {
            file << user.second.serialize() << endl;
        }
        file.close();
    }

    void registerUser() {
        string username, password, role;
        cout << "Enter Username: ";
        cin >> username;
        if (users.find(username) != users.end()) {
            cout << "Username already exists! Please choose a different username.\n";
            return;
        }
        cout << "Enter Password: ";
        cin >> password;
        cout << "Enter Role (admin/user): ";
        cin >> role;
        if (role != "admin" && role != "user") {
            cout << "Invalid role! Role must be 'admin' or 'user'.\n";
            return;
        }
        users[username] = User(username, password, role);
        saveUsers();
        cout << "User registered successfully!\n";
    }

    bool login() {
        string username, password;
        cout << "Enter Username: ";
        cin >> username;
        cout << "Enter Password: ";
        cin >> password;

        if (users.find(username) != users.end() && users[username].authenticate(password)) {
            currentUser = &users[username];
            cout << "Login successful! Welcome, " << username << ".\n";
            return true;
        } else {
            cout << "Login failed! Invalid username or password.\n";
            return false;
        }
    }

    void logout() {
        currentUser = nullptr;
        cout << "Logged out successfully.\n";
    }

    void createAccount() {
        if (currentUser == nullptr || currentUser->getRole() != "admin") {
            cout << "Unauthorized! Only admins can create accounts.\n";
            return;
        }

        string accHolderName;
        double initialBalance;
        cout << "Enter Account Holder Name: ";
        cin.ignore();
        getline(cin, accHolderName);
        cout << "Enter Initial Balance: ";
        cin >> initialBalance;

        string accNum = generateAccountNumber(accounts);
        accounts.push_back(make_unique<SavingsAccount>(accNum, accHolderName, initialBalance));
        saveAccounts();
        cout << "Account created successfully!\n";
        cout << "Generated Account Number: " << accNum << endl;
    }

    void depositToAccount() {
        if (currentUser == nullptr) {
            cout << "Unauthorized! Please login first.\n";
            return;
        }

        string accNum;
        double amount;
        cout << "Enter Account Number: ";
        cin >> accNum;
        cout << "Enter Deposit Amount: ";
        cin >> amount;
        for (auto& account : accounts) {
            if (account->getAccountNumber() == accNum) {
                try {
                    account->deposit(amount);
                    saveAccounts();
                } catch (const exception& e) {
                    cout << "Error: " << e.what() << endl;
                }
                return;
            }
        }
        cout << "Account not found!" << endl;
    }

    void withdrawFromAccount() {
        if (currentUser == nullptr) {
            cout << "Unauthorized! Please login first.\n";
            return;
        }

        string accNum;
        double amount;
        cout << "Enter Account Number: ";
        cin >> accNum;
        cout << "Enter Withdrawal Amount: ";
        cin >> amount;
        for (auto& account : accounts) {
            if (account->getAccountNumber() == accNum) {
                try {
                    account->withdraw(amount);
                    saveAccounts();
                } catch (const exception& e) {
                    cout << "Error: " << e.what() << endl;
                }
                return;
            }
        }
        cout << "Account not found!" << endl;
    }

    void checkBalance() {
        if (currentUser == nullptr) {
            cout << "Unauthorized! Please login first.\n";
            return;
        }

        string accNum;
        cout << "Enter Account Number: ";
        cin >> accNum;
        for (auto& account : accounts) {
            if (account->getAccountNumber() == accNum) {
                account->displayBalance();
                return;
            }
        }
        cout << "Account not found!" << endl;
    }

    void closeAccount() {
        if (currentUser == nullptr || currentUser->getRole() != "admin") {
            cout << "Unauthorized! Only admins can close accounts.\n";
            return;
        }

        string accNum;
        cout << "Enter Account Number: ";
        cin >> accNum;
        auto it = find_if(accounts.begin(), accounts.end(), [&](const unique_ptr<Account>& acc) {
            return acc->getAccountNumber() == accNum;
        });
        if (it != accounts.end()) {
            accounts.erase(it);
            saveAccounts();
            cout << "Account closed successfully.\n";
        } else {
            cout << "Account not found!\n";
        }
    }
};

// Generate a unique 8-digit account number
string generateAccountNumber(const vector<unique_ptr<Account>>& accounts) {
    string accNum;
    bool isUnique;
    do {
        isUnique = true;
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(10000000, 99999999);
        accNum = to_string(dis(gen));
        for (const auto& account : accounts) {
            if (account->getAccountNumber() == accNum) {
                isUnique = false;
                break;
            }
        }
    } while (!isUnique);

    return accNum;
}

int main() {
    Bank bank;
    int choice;

    while (true) {
        cout << "\n1. Register\n2. Login\n3. Create Account\n4. Deposit\n5. Withdraw\n6. Check Balance\n7. Close Account\n8. Logout\n9. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                bank.registerUser();
                break;
            case 2:
                if (bank.login()) {
                    cout << "Login successful!\n";
                }
                break;
            case 3:
                bank.createAccount();
                break;
            case 4:
                bank.depositToAccount();
                break;
            case 5:
                bank.withdrawFromAccount();
                break;
            case 6:
                bank.checkBalance();
                break;
            case 7:
                bank.closeAccount();
                break;
            case 8:
                bank.logout();
                break;
            case 9:
                return 0;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }
    return 0;
}