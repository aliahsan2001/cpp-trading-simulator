// User.cpp
// Implementation of User class for user profile management and authentication
#include "User.h"
#include <random>

// Default constructor
User::User() {};

// Parameterized constructor: initializes user with provided details
User::User(std::string fullName, std::string email, std::string username, size_t hash){
    this->fullName = fullName;
    this->email = email;
    this->username = username;
    this->passwordHash = hash;
}

// Accessor methods
std::string User::getFullName() const { return fullName; }
std::string User::getEmail() const { return email; }
std::string User::getUsername() const { return username; }
size_t User::getPasswordHash() const { return passwordHash; }

// Returns reference to user's wallet for trading operations
Wallet& User::getWallet() {
    return userWallet;
}

// ============================================================================
// WRITTEN BY ME - TASK 2 IMPLEMENTATION
// ============================================================================
// TASK 2: Generates a unique 10-digit username using random number generation
// Uses C++11 random number generation for better randomness than rand()
std::string User::generateUsername(){
    std::random_device rd;              // Non-deterministic random number generator
    std::mt19937 gen(rd());            // Mersenne Twister engine seeded with random device
    std::uniform_int_distribution<> dist(0,9);  // Distribution for digits 0-9
    std::string id = "";
    // Generate 10 random digits
    for(int i = 0; i < 10; i++){
        id += std::to_string(dist(gen));
    }
    return id;
}

// ============================================================================
// WRITTEN BY ME - TASK 2 IMPLEMENTATION
// ============================================================================
// TASK 2: Hashes password using std::hash function
// This ensures passwords are stored securely and can be compared without storing plain text
size_t User::hashPassword(const std::string &password){
    std::hash<std::string> hasher;
    return hasher(password);
}