// TradingApp.h
// Header file for TradingApp class - main application controller
// TASK 5: User interaction menu and input validation
#pragma once
#include "UserManager.h"
#include "OrderBook.h"
#include <string>

class TradingApp {
public:
    // Constructor: initializes user manager and order book
    TradingApp();
    
    // Main application entry point - displays menu and processes user input
    void init();

private:
    // TASK 5: Menu and input handling
    void printMenu();                    // Displays appropriate menu based on login status
    void processUserOption(int userOption); // Routes user input to appropriate functionality
    
    // TASK 1: Market data display
    void printMarketStats();            // Displays candlestick data for selected product
    
    // TASK 4: Trading operations
    void enterAsk();                    // Allows user to place ask (sell) orders
    void enterBid();                    // Allows user to place bid (buy) orders
    
    // TASK 3: Wallet operations
    void printWallet();                 // Displays current wallet balances
    void addMoneyToWallet();            // Allows user to deposit money
    void withdrawMoney();               // Allows user to withdraw money
    
    void gotoNextTimeframe();           // Placeholder for future functionality

    // Core components
    UserManager userManager;            // Manages user authentication and data
    OrderBook orderBook;                // Manages trading orders and market data
};