// Wallet.h
// Header file for Wallet class - manages user currency balances and transaction history
// TASK 3: Wallet management and trading transaction history
#pragma once
#include <string>
#include <map>
#include <vector>
#include <iostream>

class Wallet {
    // Allow UserManager to access private data to save to wallet.csv
    friend class UserManager; 

public:
    // Constructor
    Wallet();
    
    // Currency management methods
    void insertCurrency(std::string type, double amount);  // Add/deposit currency
    bool removeCurrency(std::string type, double amount);   // Remove/withdraw currency
    bool containsCurrency(std::string type, double amount); // Check if sufficient balance exists
    std::string toString();                                 // Get formatted wallet balance string
    double getBalance(std::string type);                    // Get balance for specific currency
    
    // TASK 3: Transaction history management
    void addHistory(std::string message);                   // Add transaction to history and persist to CSV
    std::string getHistory();                              // Get formatted transaction history
    void printStats(std::string productFilter = "");       // Display user activity statistics
    
    // TASK 3: Statistics tracking
    int askCount = 0;        // Total number of ask orders placed
    int bidCount = 0;        // Total number of bid orders placed
    double totalSpent = 0.0; // Total money spent across all transactions

private:
    std::map<std::string, double> currencies;  // Map of currency type to balance amount
    std::vector<std::string> history;          // In-memory transaction history
};