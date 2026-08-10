// User.h
// Header file for User class - manages user profile and authentication
// TASK 2: User registration and login functionality
#ifndef USER_H
#define USER_H

#include <string>
#include "Wallet.h"

class User {
    private:
        std::string fullName;      // User's full name
        std::string email;          // User's email address
        std::string username;       // Unique 10-digit username generated during registration
        size_t passwordHash;        // Hashed password using std::hash for security
        Wallet userWallet;          // TASK 3: User's wallet linked to track trading activities

    public:
        // Constructors
        User();
        User(std::string fullName, std::string email, std::string username, size_t hash);

        // Accessor methods
        std::string getFullName() const;
        std::string getEmail() const;
        std::string getUsername() const;
        size_t getPasswordHash() const;
        
        // TASK 3: Returns reference to user's wallet
        Wallet& getWallet();

        // TASK 2: Generates unique 10-digit username using random number generation
        static std::string generateUsername();
        
        // TASK 2: Hashes password using std::hash function for secure storage
        static size_t hashPassword(const std::string &password);
};

#endif