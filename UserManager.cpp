// UserManager.cpp
// Implementation of UserManager class for user authentication and data persistence
#include "UserManager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>

// Constructor: initializes file paths for user data and wallet storage
UserManager::UserManager(std::string file) 
: userFile(file), walletFile("wallet.csv"), currentUser(nullptr) 
{}

// Destructor: cleans up dynamically allocated current user
UserManager::~UserManager() {
    if(currentUser != nullptr) {
        delete currentUser;
    }
}

// TASK 2: Checks if a user with the given full name and email already exists
// Prevents duplicate registrations by validating against existing records
bool UserManager::userExists(const std::string &fullName, const std::string &email){
    std::ifstream file(userFile);
    if(!file.is_open()) return false;
    std::string line;
    // Read through all user records and check for matching name and email
    while(getline(file, line)){
        std::stringstream ss(line);
        std::string name, mail;
        getline(ss, name, ',');
        getline(ss, mail, ',');
        if(name == fullName && mail == email) return true;
    }
    return false;
}

// ============================================================================
// WRITTEN BY ME - TASK 2 IMPLEMENTATION
// ============================================================================
// TASK 2: Saves user profile data to users.csv file
// Format: fullName,email,username,passwordHash
void UserManager::saveUser(const User &user){
    std::ofstream file(userFile, std::ios::app);
    file << user.getFullName() << ","
         << user.getEmail() << ","
         << user.getUsername() << ","
         << user.getPasswordHash() << "\n";
    file.close();
}

// ============================================================================
// WRITTEN BY ME - TASK 3 IMPLEMENTATION
// ============================================================================
// TASK 3: Saves wallet data to wallet.csv file
// Implements update strategy: reads all existing data, removes current user's old entries,
// adds current user's latest wallet state, then rewrites the entire file
void UserManager::saveWallet() {
    if(currentUser == nullptr) return;
    
    // Step 1: Read all existing wallet data from file
    std::vector<std::string> lines;
    std::ifstream fileIn(walletFile);
    std::string line;
    
    if(fileIn.is_open()) {
        while(getline(fileIn, line)) {
            // File format: username,currency,amount
            std::string lineUser = line.substr(0, line.find(','));
            // Keep all lines that do NOT belong to current user (preserve other users' data)
            if(lineUser != currentUser->getUsername()) {
                lines.push_back(line);
            }
        }
        fileIn.close();
    }

    // Step 2: Add current user's latest wallet data for all currencies
    for(auto const& pair : currentUser->getWallet().currencies) {
        std::string currency = pair.first;
        double amount = pair.second;
        std::string newLine = currentUser->getUsername() + "," + currency + "," + std::to_string(amount);
        lines.push_back(newLine);
    }

    // Step 3: Rewrite entire file with updated data
    std::ofstream fileOut(walletFile); // Overwrite mode
    for(const std::string& l : lines) {
        fileOut << l << "\n";
    }
    fileOut.close();
}

// ============================================================================
// WRITTEN BY ME - TASK 2 IMPLEMENTATION
// ============================================================================
// TASK 2: Registers a new user account
// Validates that user doesn't already exist, generates unique username, hashes password,
// and links user with their wallet for tracking trading activities
void UserManager::registerUser(){
    std::string fullName, email, password;
    std::cout<<"\n==== Register User =====\n";
    std::cout<<"Enter Full Name: ";
    std::getline(std::cin, fullName);
    std::cout<<"Enter Email: ";
    std::getline(std::cin, email);
    
    // TASK 2: Check for duplicate users to prevent re-registration
    if(userExists(fullName, email)){
        std::cout << "User already exists!\n";
        return;
    }
    
    std::cout<<"Enter Password: ";
    std::getline(std::cin, password);
    
    // TASK 2: Generate unique 10-digit username
    std::string username = User::generateUsername();
    // TASK 2: Hash password using std::hash for secure storage
    size_t hash = User::hashPassword(password);
    
    User newUser(fullName, email, username, hash);
    
    // TASK 3: Initialize wallet with default starting balance
    newUser.getWallet().insertCurrency("USDT", 0.0); 
    
    // Save user profile to users.csv
    saveUser(newUser);
    
    // Temporarily set as current user to save initial wallet state
    currentUser = new User(fullName, email, username, hash);
    currentUser->getWallet().insertCurrency("USDT", 0.0); 
    saveWallet();
    delete currentUser;
    currentUser = nullptr;
    
    std::cout << "Registration Successful! Your username is: " << username << "\n";
}

// ============================================================================
// WRITTEN BY ME - TASK 2 IMPLEMENTATION
// ============================================================================
// TASK 2: Authenticates user login with username and password
// Compares hashed password input against stored hash, then loads user's wallet data
void UserManager::loginUser(){
    std::string usernameInput, passwordInput;
    std::cout<<"===== User Login ========= \n";
    std::cout<<"Enter username: ";
    std::getline(std::cin, usernameInput);
    std::cout<<"Enter Password: ";
    std::getline(std::cin, passwordInput);
    
    // Hash the input password to compare with stored hash
    size_t hashedInput = User::hashPassword(passwordInput);
    std::ifstream file(userFile);
    if(!file.is_open()) return;
    
    // Search through user records for matching username and password hash
    std::string line;
    bool found = false;
    while(getline(file, line)){
        std::stringstream ss(line);
        std::string name, mail, uName, hashStr;
        getline(ss, name, ',');
        getline(ss, mail, ',');
        getline(ss, uName, ',');
        getline(ss, hashStr, ',');
        
        // TASK 2: Verify credentials by comparing username and hashed password
        if(uName == usernameInput && std::stoull(hashStr) == hashedInput){
            currentUser = new User(name, mail, uName, hashedInput);
            found = true;
            break;
        }
    }
    file.close();
    
    if(found) {
        // ========================================================================
        // WRITTEN BY ME - TASK 3 IMPLEMENTATION
        // ========================================================================
        // TASK 3: Load wallet data from wallet.csv for the logged-in user
        std::ifstream wFile(walletFile);
        if(wFile.is_open()) {
            while(getline(wFile, line)) {
                std::stringstream ss(line);
                std::string wUser, curr, amtStr;
                getline(ss, wUser, ',');
                getline(ss, curr, ',');
                getline(ss, amtStr, ',');
                
                // Restore wallet balances for this user
                if(wUser == usernameInput) {
                    try {
                        double amt = std::stod(amtStr);
                        currentUser->getWallet().insertCurrency(curr, amt);
                    } catch(...) {
                        // Skip invalid entries
                    }
                }
            }
        }
        std::cout << "Login Successful!\n";
    }
    else std::cout << "Invalid credentials.\n";
}

// ============================================================================
// WRITTEN BY ME - TASK 2 IMPLEMENTATION
// ============================================================================
// TASK 2: Allows users to reset forgotten passwords
// Verifies user identity with username and email, then updates password hash
void UserManager::resetPassword() {
    std::string username, email, newPass;
    std::cout << "\n===== Reset Password =====\n";
    std::cout << "Enter Username: ";
    std::getline(std::cin, username);
    std::cout << "Enter Email: ";
    std::getline(std::cin, email);

    std::ifstream fileIn(userFile);
    if (!fileIn.is_open()) {
        std::cout << "Error: database not found.\n";
        return;
    }

    // Read all user records and update the matching one
    std::vector<std::string> lines;
    std::string line;
    bool found = false;

    while (std::getline(fileIn, line)) {
        std::stringstream ss(line);
        std::string name, mail, uName, hash;
        std::getline(ss, name, ',');
        std::getline(ss, mail, ',');
        std::getline(ss, uName, ',');
        std::getline(ss, hash, ',');

        // Verify username and email match before allowing password reset
        if (uName == username && mail == email) {
            found = true;
            std::cout << "User found. Enter new password: ";
            std::getline(std::cin, newPass);
            // Hash the new password and create updated record
            size_t newHash = User::hashPassword(newPass);
            std::string newLine = name + "," + mail + "," + uName + "," + std::to_string(newHash);
            lines.push_back(newLine);
        } else {
            // Keep other users' records unchanged
            lines.push_back(line);
        }
    }
    fileIn.close();

    // Rewrite file with updated password if user was found
    if (found) {
        std::ofstream fileOut(userFile);
        for (const auto& l : lines) fileOut << l << "\n";
        fileOut.close();
        std::cout << "Password reset successfully!\n";
    } else {
        std::cout << "Credentials do not match any record.\n";
    }
}

// Logs out current user and saves wallet data before clearing session
void UserManager::logout() {
    if(currentUser != nullptr) {
        saveWallet(); // TASK 3: Persist wallet data before logout
        delete currentUser;
        currentUser = nullptr;
    }
}

// Returns true if a user is currently logged in
bool UserManager::isLoggedIn() { return currentUser != nullptr; }

// Returns pointer to currently logged-in user (nullptr if not logged in)
User* UserManager::getUser() { return currentUser; }