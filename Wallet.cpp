// Wallet.cpp
// Implementation of Wallet class for currency management and transaction tracking
// TASK 3: Wallet operations and transaction history persistence
#include "Wallet.h"
#include <fstream> 

// Default constructor
Wallet::Wallet() {}

// ============================================================================
// WRITTEN BY ME - TASK 3 IMPLEMENTATION
// ============================================================================
// TASK 3: Adds currency to wallet (deposit operation)
// Validates amount is non-negative and updates balance
void Wallet::insertCurrency(std::string type, double amount) {
    double balance;
    if (amount < 0) return; // Input validation: reject negative amounts
    // Get current balance or initialize to 0 if currency doesn't exist
    if (currencies.count(type) == 0) balance = 0;
    else balance = currencies[type];
    balance += amount;
    currencies[type] = balance;
    // Log transaction to history
    addHistory("Deposited: " + std::to_string(amount) + " " + type);
}

// ============================================================================
// WRITTEN BY ME - TASK 3 IMPLEMENTATION
// ============================================================================
// TASK 3: Removes currency from wallet (withdrawal/spending operation)
// Validates sufficient balance exists before removing
bool Wallet::removeCurrency(std::string type, double amount) {
    if (amount < 0) return false; // Input validation
    if (currencies.count(type) == 0) return false; // Currency doesn't exist
    else {
        // Check if sufficient balance exists
        if (containsCurrency(type, amount)) {
            currencies[type] -= amount;
            addHistory("Withdrew/Spent: " + std::to_string(amount) + " " + type);
            totalSpent += amount; // Update spending statistics
            return true;
        }
        return false;
    }
}

// Checks if wallet contains sufficient amount of specified currency
bool Wallet::containsCurrency(std::string type, double amount) {
    if (currencies.count(type) == 0) return false;
    else return currencies[type] >= amount;
}

// Returns formatted string representation of all wallet balances
std::string Wallet::toString() {
    std::string s;
    for (std::pair<std::string, double> pair : currencies) {
        s += pair.first + ": " + std::to_string(pair.second) + "\n";
    }
    return s;
}

// Returns balance for a specific currency type (0.0 if currency doesn't exist)
double Wallet::getBalance(std::string type) {
    if (currencies.count(type) == 0) return 0.0;
    return currencies[type];
}

// ============================================================================
// WRITTEN BY ME - TASK 3 IMPLEMENTATION
// ============================================================================
// TASK 3: Adds transaction to history and persists to user_transactions.csv
// All wallet operations automatically log to transaction history
void Wallet::addHistory(std::string message) {
    history.push_back(message);
    
    // TASK 3: Persist transaction to CSV file for permanent record
    std::ofstream file("user_transactions.csv", std::ios::app);
    if(file.is_open()) {
        file << message << "\n";
        file.close();
    }
}

// ============================================================================
// WRITTEN BY ME - TASK 3 IMPLEMENTATION
// ============================================================================
// TASK 3: Returns formatted transaction history
// Displays recent trading transactions made by the user
std::string Wallet::getHistory() {
    std::string s = "\n=== Trading Transaction History ===\n";
    if (history.empty()) {
        s += "No transactions in this session.\n";
    } else {
        for(const std::string& item : history) {
            s += item + "\n";
        }
    }
    return s;
}

// ============================================================================
// WRITTEN BY ME - TASK 3 IMPLEMENTATION
// ============================================================================
// TASK 3: Displays summary statistics of user activity
// Shows number of asks and bids (optionally filtered by product) and total money spent
void Wallet::printStats(std::string productFilter) {
    std::cout << "\n=== User Activity Statistics";
    if(!productFilter.empty()) std::cout << " (Filter: " << productFilter << ")";
    std::cout << " ===\n";

    int localAsks = 0;
    int localBids = 0;
    
    // Scan transaction history for ask/bid keywords
    // If product filter is specified, only count transactions containing that product
    for(const std::string& h : history) {
        if(!productFilter.empty() && h.find(productFilter) == std::string::npos) {
            continue; // Skip transactions that don't match product filter
        }

        if(h.find("Ask") != std::string::npos) localAsks++;
        if(h.find("Bid") != std::string::npos) localBids++;
    }

    std::cout << "Asks: " << localAsks << "\n";
    std::cout << "Bids: " << localBids << "\n";
    
    // Total spending only shown when viewing all products
    if(productFilter.empty()) {
        std::cout << "Total Volume Spent (All): " << totalSpent << "\n";
    }
}