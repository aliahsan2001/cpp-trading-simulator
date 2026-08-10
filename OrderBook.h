// OrderBook.h
// Header file for OrderBook class - manages trading orders and candlestick computations
#pragma once
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include "Candlestick.h"
#include <vector>
#include <string>

class OrderBook {
public:
    // Constructor: loads orders from CSV file
    OrderBook(std::string filename);
    
    // Returns list of all unique product combinations in the order book
    std::vector<std::string> getKnownProducts();
    
    // Retrieves orders matching specific criteria
    std::vector<OrderBookEntry> getOrders(std::string type, std::string product, std::string timestamp);
    
    // TASK 1: Computes candlestick data (Open, High, Low, Close) for a product
    // Supports daily, monthly, and yearly timeframes with optional date range filtering
    std::vector<Candlestick> computeCandlesticks(std::string product, std::string type, std::string timeframe, std::string startDate = "", std::string endDate = "");    
    
    // TASK 4: Simulates new market activity by creating ask and bid orders for all products
    void simulateMarketActivity(); 

    // Storage for all order book entries
    std::vector<OrderBookEntry> orders;
};