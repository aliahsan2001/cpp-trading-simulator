// UserManager.h
// Header file for UserManager class - handles user registration, login, and data persistence
// TASK 2: User authentication and profile management
// TASK 3: Wallet and transaction data persistence
#pragma once
#include "User.h"
#include <string>
#include <vector>

class UserManager{
    private:
        std::string userFile;      // Path to users.csv file storing user profiles
        std::string walletFile;    // Path to wallet.csv file storing wallet balances
        User* currentUser;          // Pointer to currently logged-in user (nullptr if not logged in)
        
    public:
        // Constructor: initializes file paths
        UserManager(std::string file);
        // Destructor: cleans up current user pointer
        ~UserManager();

        // Checks if a user with given name and email already exists in the database
        bool userExists(const std::string &fullName, const std::string &email);
        
        // TASK 3: Persistence methods - separates user profile and wallet data
        void saveUser(const User &user);  // Saves user profile to users.csv
        void saveWallet();                // Saves wallet balances to wallet.csv
        
        // TASK 2: User registration - creates new user account with validation
        void registerUser();
        
        // TASK 2: User login - authenticates user with username and password
        void loginUser();
        
        // TASK 2: Password reset - allows users to reset forgotten passwords
        void resetPassword();
        
        // Logs out current user and saves wallet data
        void logout();
        
        // Returns true if a user is currently logged in
        bool isLoggedIn();
        
        // Returns pointer to currently logged-in user
        User* getUser(); 
};