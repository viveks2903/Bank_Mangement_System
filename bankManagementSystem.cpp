#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Base class: Account
class Account {
protected:
    string accountNumber;
    string accountHolderName;
    double balance;

public:
    Account(string accNum, string accHolderName, double initialBalance)
        : accountNumber(accNum), accountHolderName(accHolderName), balance(initialBalance) {}

    virtual void deposit(double amount) {
        balance += amount;
        cout << "Amount deposited: " << amount << ". Current balance: " << balance << endl;
    }

    virtual void withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
            cout << "Amount withdrawn: " << amount << ". Current balance: " << balance << endl;
        } else {
            cout << "Insufficient balance!" << endl;
        }
    }

    virtual void displayBalance() const {
        cout << "Account Number: " << accountNumber << endl;
        cout << "Account Holder: " << accountHolderName << endl;
        cout << "Current Balance: " << balance << endl;
    }

    string getAccountNumber() const {
        return accountNumber;
    }

    virtual ~Account() {}
};

// Derived class: SavingsAccount
class SavingsAccount : public Account {
public:
    SavingsAccount(string accNum, string accHolderName, double initialBalance)
        : Account(accNum, accHolderName, initialBalance) {}

    void withdraw(double amount) override {
        if (amount > balance * 0.8) {
            cout << "Withdrawal amount exceeds 80% of balance in Savings Account!" << endl;
        } else {
            Account::withdraw(amount);
        }
    }
};

// Derived class: CurrentAccount
class CurrentAccount : public Account {
public:
    CurrentAccount(string accNum, string accHolderName, double initialBalance)
        : Account(accNum, accHolderName, initialBalance) {}

    void withdraw(double amount) override {
        if (amount > balance) {
            cout << "Withdrawal amount exceeds balance in Current Account!" << endl;
        } else {
            Account::withdraw(amount);
        }
    }
};

// Bank System Management Class
class Bank {
private:
    vector<Account*> accounts;

public:
    void createAccount() {
        string accNum, accHolderName;
        double initialBalance;
        int accType;

        cout << "Enter Account Number: ";
        cin >> accNum;
        cout << "Enter Account Holder Name: ";
        cin.ignore();
        getline(cin, accHolderName);
        cout << "Enter Initial Balance: ";
        cin >> initialBalance;
        cout << "Select Account Type (1: Savings, 2: Current): ";
        cin >> accType;

        Account* account = nullptr;
        if (accType == 1) {
            account = new SavingsAccount(accNum, accHolderName, initialBalance);
        } else if (accType == 2) {
            account = new CurrentAccount(accNum, accHolderName, initialBalance);
        }

        if (account != nullptr) {
            accounts.push_back(account);
            cout << "Account created successfully!" << endl;
        }
    }

    void depositToAccount() {
        string accNum;
        double amount;
        cout << "Enter Account Number: ";
        cin >> accNum;
        cout << "Enter Deposit Amount: ";
        cin >> amount;

        for (auto& account : accounts) {
            if (account->getAccountNumber() == accNum) {
                account->deposit(amount);
                return;
            }
        }
        cout << "Account not found!" << endl;
    }

    void withdrawFromAccount() {
        string accNum;
        double amount;
        cout << "Enter Account Number: ";
        cin >> accNum;
        cout << "Enter Withdrawal Amount: ";
        cin >> amount;

        for (auto& account : accounts) {
            if (account->getAccountNumber() == accNum) {
                account->withdraw(amount);
                return;
            }
        }
        cout << "Account not found!" << endl;
    }

    void checkBalance() {
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
        string accNum;
        cout << "Enter Account Number: ";
        cin >> accNum;

        for (auto it = accounts.begin(); it != accounts.end(); ++it) {
            if ((*it)->getAccountNumber() == accNum) {
                delete *it;
                accounts.erase(it);
                cout << "Account closed successfully!" << endl;
                return;
            }
        }
        cout << "Account not found!" << endl;
    }

    ~Bank() {
        for (auto& account : accounts) {
            delete account;
        }
    }
};

// Main function
int main() {
    Bank bank;
    int choice;

    do {
        cout << "\n--- Bank Management System ---\n";
        cout << "1. Create Account\n2. Deposit\n3. Withdraw\n4. Check Balance\n5. Close Account\n6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                bank.createAccount();
                break;
            case 2:
                bank.depositToAccount();
                break;
            case 3:
                bank.withdrawFromAccount();
                break;
            case 4:
                bank.checkBalance();
                break;
            case 5:
                bank.closeAccount();
                break;
            case 6:
                cout << "Thank you for using the Bank Management System!" << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    } while (choice != 6);

    return 0;
}
