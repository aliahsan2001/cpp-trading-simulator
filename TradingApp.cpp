// TradingApp.cpp
// Implementation of TradingApp class - main application controller
// TASK 5: User interaction menu with robust input validation
#include "TradingApp.h"
#include <iostream>
#include <limits>
#include <vector>
#include <ctime>
#include "Candlestick.h" 

// Constructor: initializes user manager with users.csv and order book with market data file
TradingApp::TradingApp() 
: userManager("users.csv"), orderBook("20200601.csv") 
{
}

// ============================================================================
// WRITTEN BY ME - TASK 5 IMPLEMENTATION
// ============================================================================
// Main application loop - displays menu and processes user input continuously
// TASK 5: Implements graceful input handling to prevent program termination on invalid input
void TradingApp::init() {
    int input;
    while(true) {
        printMenu();
        std::cin >> input;
        
        // TASK 5: Input validation - handle non-numeric input gracefully
        if(std::cin.fail()) {
            std::cin.clear();  // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // Discard invalid input
            std::cout << "Invalid input. Please enter a number.\n";
            continue;  // Return to menu instead of terminating
        }
        std::cin.ignore();  // Clear newline from input buffer
        processUserOption(input);
    }
}

// ============================================================================
// WRITTEN BY ME - TASK 5 IMPLEMENTATION
// ============================================================================
// TASK 5: Displays appropriate menu based on user login status
// Shows authentication options when logged out, trading options when logged in
void TradingApp::printMenu() {
    std::cout << "\n=== Trading Platform ===\n";
    
    if(!userManager.isLoggedIn()) {
        // TASK 2: Authentication menu
        std::cout << "1: Register\n";
        std::cout << "2: Login\n";
        std::cout << "3: Reset Password\n";
        std::cout << "4: Exit\n";
    } else {
        // TASK 1, 3, 4: Trading and wallet management menu
        std::cout << "1: Print Market Stats (Candlesticks)\n";
        std::cout << "2: Make an Ask\n";
        std::cout << "3: Make a Bid\n";
        std::cout << "4: Print Wallet\n";
        std::cout << "5: Print Transaction History\n";
        std::cout << "6: Add Money to Wallet\n";
        std::cout << "7: Simulate Market Activity\n";
        std::cout << "8: Print User Statistics\n";
        std::cout << "9: Withdraw Money\n";
        std::cout << "10: Logout\n";
    }
}

// ============================================================================
// WRITTEN BY ME - TASK 5 IMPLEMENTATION
// ============================================================================
// TASK 5: Routes user input to appropriate functionality based on menu selection
// Handles both logged-out and logged-in states with appropriate options
void TradingApp::processUserOption(int userOption) {
    if(!userManager.isLoggedIn()) {
        // TASK 2: Authentication options
        if(userOption == 1) userManager.registerUser();
        else if(userOption == 2) userManager.loginUser();
        else if(userOption == 3) userManager.resetPassword();
        else if(userOption == 4) exit(0);
        else std::cout << "Invalid choice\n";  // TASK 5: Graceful error handling
    } else {
        // TASK 1, 3, 4: Trading and wallet operations
        switch(userOption) {
            case 1: printMarketStats(); break;  // TASK 1: Display candlestick data
            case 2: enterAsk(); break;         // TASK 4: Place ask order
            case 3: enterBid(); break;         // TASK 4: Place bid order
            case 4: printWallet(); break;       // TASK 3: Display wallet balance
            case 5: std::cout << userManager.getUser()->getWallet().getHistory(); break;  // TASK 3: Transaction history
            case 6: addMoneyToWallet(); break;  // TASK 3: Deposit money
            case 7: 
                // TASK 4: Simulate market activity
                orderBook.simulateMarketActivity(); 
                std::cout << "Market simulated! 10 new orders generated.\n";
                break;
            case 8: 
            {
                // TASK 3: Display user statistics with optional product filter
                std::string filter;
                std::cout << "Enter product to filter stats (or press Enter for all): ";
                std::getline(std::cin, filter);
                userManager.getUser()->getWallet().printStats(filter);
                break;
            }
            case 9: withdrawMoney(); break;     // TASK 3: Withdraw money
            case 10: 
                userManager.logout(); 
                std::cout << "Logged out successfully.\n";
                break;
            default: std::cout << "Invalid choice\n";  // TASK 5: Graceful error handling
        }
    }
}

// ============================================================================
// WRITTEN BY ME - TASK 1 IMPLEMENTATION
// ============================================================================
// TASK 1: Displays candlestick data for a selected product
// Allows user to specify product, order type (ask/bid), timeframe (daily/monthly/yearly),
// and optional date range filtering. Default timeframe is yearly.
void TradingApp::printMarketStats() {
    std::string product, type, timeframe, startD, endD;
    std::cout << "Enter product (e.g., ETH/BTC): ";
    std::getline(std::cin, product);
    std::cout << "Enter type (ask/bid): ";
    std::getline(std::cin, type);
    std::cout << "Enter timeframe (daily/monthly/yearly) [Default: yearly]: ";
    std::getline(std::cin, timeframe); 
    // TASK 1: Default to yearly if no timeframe specified
    if(timeframe.empty()) timeframe = "yearly";

    // TASK 1: Optional date range filtering
    std::cout << "Enter Start Date (YYYY/MM/DD) or Enter to skip: ";
    std::getline(std::cin, startD);
    std::cout << "Enter End Date (YYYY/MM/DD) or Enter to skip: ";
    std::getline(std::cin, endD);

    // Compute candlestick data based on user inputs
    std::vector<Candlestick> candles = orderBook.computeCandlesticks(product, type, timeframe, startD, endD);
    std::cout << "\n--- Candlestick Data (" << timeframe << ") ---\n";
    if(candles.empty()) std::cout << "No data found for these criteria.\n";
    else for(auto& c : candles) c.print();
}

// ============================================================================
// WRITTEN BY ME - TASK 4 & TASK 5 IMPLEMENTATION
// ============================================================================
// TASK 4: Allows user to place an ask (sell) order
// TASK 5: Validates input format and checks wallet balance before placing order
// TASK 3: Logs transaction and updates wallet statistics
void TradingApp::enterAsk() {
    std::cout << "Make an ask (Sell) - enter: product,price,amount (e.g., ETH/BTC,200,0.5)\n";
    std::string input;
    std::getline(std::cin, input);
    
    // TASK 5: Input validation - parse and verify format
    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if(tokens.size() != 3) {
        std::cout << "Bad input! Format: ETH/BTC,200,0.5\n";
        return;  // Graceful error handling - return to menu instead of crashing
    }
    
    std::string product = tokens[0];
    double price = std::stod(tokens[1]);
    double amount = std::stod(tokens[2]);
    
    // TASK 5: Validate sufficient balance exists for the currency being sold
    std::vector<std::string> currs = CSVReader::tokenise(product, '/');
    if(currs.size() >= 1) {
        // For ask orders, user must have the base currency (e.g., ETH in ETH/USDT)
        if(!userManager.getUser()->getWallet().containsCurrency(currs[0], amount)) {
            std::cout << "Insufficient " << currs[0] << "\n";
            return;
        }
    }
    
    // TASK 4: Use current system timestamp for new orders
    std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", now);
    OrderBookEntry obe(std::string(buf), product, "ask", price, amount);
    orderBook.orders.push_back(obe);
    
    // TASK 3: Log transaction and update statistics
    userManager.getUser()->getWallet().addHistory("Placed Ask: " + input);
    userManager.getUser()->getWallet().askCount++;
    
    // TASK 3: Persist wallet data to CSV file
    userManager.saveWallet(); 
    
    std::cout << "Ask placed successfully!\n";
}

// ============================================================================
// WRITTEN BY ME - TASK 4 & TASK 5 IMPLEMENTATION
// ============================================================================
// TASK 4: Allows user to place a bid (buy) order
// TASK 5: Validates input format and checks wallet balance before placing order
// TASK 3: Logs transaction and updates wallet statistics
void TradingApp::enterBid() {
    std::cout << "Make a bid (Buy) - enter: product,price,amount (e.g., ETH/BTC,200,0.5)\n";
    std::string input;
    std::getline(std::cin, input);
    
    // TASK 5: Input validation - parse and verify format
    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if(tokens.size() != 3) {
        std::cout << "Bad input! Format: ETH/BTC,200,0.5\n";
        return;  // Graceful error handling - return to menu instead of crashing
    }
    
    std::string product = tokens[0];
    double price = std::stod(tokens[1]);
    double amount = std::stod(tokens[2]);
    
    // TASK 5: Validate sufficient balance exists for the quote currency being used to buy
    std::vector<std::string> currs = CSVReader::tokenise(product, '/');
    if(currs.size() >= 2) {
        // For bid orders, user must have the quote currency (e.g., USDT in ETH/USDT)
        double cost = price * amount;
        if(!userManager.getUser()->getWallet().containsCurrency(currs[1], cost)) {
            std::cout << "Insufficient " << currs[1] << "\n";
            return;
        }
    }

    // TASK 4: Use current system timestamp for new orders
    std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", now);
    OrderBookEntry obe(std::string(buf), product, "bid", price, amount);
    orderBook.orders.push_back(obe);
    
    // TASK 3: Log transaction and update statistics
    userManager.getUser()->getWallet().addHistory("Placed Bid: " + input);
    userManager.getUser()->getWallet().bidCount++;
    
    // TASK 3: Persist wallet data to CSV file
    userManager.saveWallet();
    
    std::cout << "Bid placed successfully!\n";
}

// ============================================================================
// WRITTEN BY ME - TASK 3 IMPLEMENTATION
// ============================================================================
// TASK 3: Displays current wallet balances for all currencies
void TradingApp::printWallet() {
    if(userManager.isLoggedIn()) {
        std::cout << "\nMy Wallet: \n";
        std::cout << userManager.getUser()->getWallet().toString();
    }
}

// ============================================================================
// WRITTEN BY ME - TASK 3 & TASK 5 IMPLEMENTATION
// ============================================================================
// TASK 3: Allows user to deposit money into their wallet
// TASK 5: Validates input to ensure amount is non-negative numeric value
void TradingApp::addMoneyToWallet() {
    if(!userManager.isLoggedIn()) return;
    std::string currency;
    double amount;
    
    std::cout << "\n==== Add Money to Wallet ====\n";
    std::cout << "Enter currency type (e.g., USDT, BTC): ";
    std::getline(std::cin, currency);
    std::cout << "Enter amount to add: ";
    std::cin >> amount;
    
    // TASK 5: Input validation - check for invalid input or negative amounts
    if(std::cin.fail() || amount < 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid amount.\n";
        return;  // Graceful error handling
    }
    std::cin.ignore();
    userManager.getUser()->getWallet().insertCurrency(currency, amount);
    
    // TASK 3: Persist updated wallet balance to CSV file
    userManager.saveWallet();
    
    std::cout << "Funds added successfully.\n";
}

// ============================================================================
// WRITTEN BY ME - TASK 3 & TASK 5 IMPLEMENTATION
// ============================================================================
// TASK 3: Allows user to withdraw money from their wallet
// TASK 5: Validates input and checks sufficient balance before withdrawal
void TradingApp::withdrawMoney() {
    if(!userManager.isLoggedIn()) return;
    std::string currency;
    double amount;
    
    std::cout << "\n==== Withdraw Money ====\n";
    std::cout << "Enter currency type (e.g., USDT, BTC): ";
    std::getline(std::cin, currency);
    std::cout << "Enter amount to withdraw: ";
    std::cin >> amount;
    
    // TASK 5: Input validation - check for invalid input or negative amounts
    if(std::cin.fail() || amount < 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid amount.\n";
        return;  // Graceful error handling
    }
    std::cin.ignore();

    // TASK 5: Validate sufficient balance exists before withdrawal
    if(userManager.getUser()->getWallet().removeCurrency(currency, amount)) {
        // TASK 3: Persist updated wallet balance to CSV file
        userManager.saveWallet();
        std::cout << "Withdrawal successful! New balance updated.\n";
    } else {
        std::cout << "Insufficient funds to withdraw " << amount << " " << currency << "\n";
    }
}

// Placeholder method for future functionality
void TradingApp::gotoNextTimeframe() {}