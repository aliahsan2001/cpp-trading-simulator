// OrderBookEntry.h
// Header file for OrderBookEntry class - represents a single trading order
#pragma once
#include <string>

class OrderBookEntry {
public:
    std::string timestamp;  // Date and time of the order
    std::string product;    // Product pair (e.g., "ETH/USDT")
    std::string orderType;  // Order type: "ask" (sell) or "bid" (buy)
    double price;           // Price per unit
    double amount;          // Quantity/amount of the order

    // Constructor: initializes order entry with all required fields
    OrderBookEntry(std::string _timestamp, std::string _product, std::string _orderType, double _price, double _amount);
    
    // Static comparison function for sorting orders by timestamp
    static bool compareByTimestamp(OrderBookEntry& e1, OrderBookEntry& e2);
};