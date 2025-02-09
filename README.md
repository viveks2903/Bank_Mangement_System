# Bank Management System

## Overview 📝

### Project Name: 
**Bank Management System**

### Description:
The **Bank Management System** is a C++ console application that simulates the core operations of a banking system. The system allows **users** to create accounts, deposit and withdraw money, check balances, and perform administrative functions like managing user roles and account closure. The application uses a **text file-based storage** mechanism to persist account and user data between sessions.

### Core Features:
- **User Authentication:**  
  Users can register with a username and password. There are two roles: `admin` (who can manage accounts) and `user` (who can perform transactions on their own accounts).
  
- **Account Management:**  
  Admins can create new accounts, close existing accounts, and manage the user base.

- **Transactions:**  
  - **Deposits:** Users can deposit money into their accounts.  
  - **Withdrawals:** Users can withdraw funds, with specific withdrawal limits based on account type.  
  - **Balance Check:** Users can view the balance of their account.

- **Role-based Access Control (RBAC):**  
  The system uses role-based access to ensure that only authorized users (admins) can create or close accounts. Regular users can only access their own account information and perform transactions.

- **Data Persistence:**  
  Account and user information is saved to text files (`accounts.txt`, `users.txt`). This ensures data persists across multiple sessions of the program.

- **Unique Account Number Generation:**  
  A random 8-digit account number is generated for each new account to ensure uniqueness.

- **Error Handling:**  
  The program includes basic error handling for invalid inputs, such as incorrect login attempts, invalid deposit or withdrawal amounts, and insufficient funds for withdrawals.

### Technologies Used:
- C++ for core functionality  
- File handling (text files) for data persistence  
- Standard Library functions (e.g., `iostream`, `vector`, `map`, `fstream`, `sstream`, etc.)

### Why It’s Useful:
- **Simulates real-world banking operations:** Allows users to perform real-world banking tasks like managing accounts and handling transactions.
- **Role Management:** Implements user role functionality (admin/user), showing the basics of authentication and authorization in applications.
- **File Handling Practice:** Demonstrates how to use file streams to read and write data to persistent storage in a C++ program.
- **Hands-on Learning:** Helps developers learn and apply key concepts of object-oriented programming (OOP) such as inheritance, polymorphism, and encapsulation.
- **Customizable:** Admins have the ability to customize the user base and accounts via file-based storage, making the system flexible and easy to extend.

### How to Run:
1. Clone the repository:  
   
- git clone <repository_url>
- g++ -o bank_management_system main.cpp
- ./bank_management_system
