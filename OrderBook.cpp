// OrderBook.cpp
// Implementation of OrderBook class for managing trading orders and computing candlestick data
#include "OrderBook.h"
#include <map>
#include <algorithm>
#include <iostream>
#include <fstream> 
#include <ctime>

// Constructor: Loads trading orders from CSV file
OrderBook::OrderBook(std::string filename) {
    orders = CSVReader::readCSV(filename);
}

// Returns a list of all unique product combinations found in the order book
std::vector<std::string> OrderBook::getKnownProducts() {
    std::vector<std::string> products;
    std::map<std::string, bool> prodMap;
    // Iterate through all orders and mark unique products
    for(OrderBookEntry& e : orders) prodMap[e.product] = true;
    // Convert map keys to vector
    for(auto const& pair : prodMap) products.push_back(pair.first);
    return products;
}

// Retrieves orders matching specific type, product, and timestamp
std::vector<OrderBookEntry> OrderBook::getOrders(std::string type, std::string product, std::string timestamp) {
    std::vector<OrderBookEntry> orders_sub;
    for(OrderBookEntry& e : orders) {
        if(e.orderType == type && e.product == product && e.timestamp == timestamp) {
            orders_sub.push_back(e);
        }
    }
    return orders_sub;
}

// ============================================================================
// WRITTEN BY ME - TASK 1 IMPLEMENTATION
// ============================================================================
// TASK 1: Computes candlestick data for a given product and order type
// Parameters: product (e.g., "ETH/USDT"), type ("ask" or "bid"), timeframe ("daily", "monthly", "yearly")
// Returns: Vector of Candlestick objects containing Open, High, Low, Close prices for each time period
std::vector<Candlestick> OrderBook::computeCandlesticks(std::string product, std::string type, std::string timeframe, std::string startDate, std::string endDate) {
    std::vector<Candlestick> candlesticks;
    double open = 0, high = 0, low = 0, close = 0;
    std::string currentKey = "";
    bool first = true;
    
    // Iterate through all orders to build candlestick data
    for(OrderBookEntry& e : orders) {
        // Filter by product and order type (ask/bid)
        if(e.product == product && e.orderType == type) {
            // Apply date range filtering if specified
            if(!startDate.empty() && e.timestamp < startDate) continue;
            if(!endDate.empty() && e.timestamp > endDate) continue;

            // Extract date key based on timeframe: daily (full timestamp), monthly (YYYY-MM), yearly (YYYY)
            std::string dateKey = e.timestamp;
            if (timeframe == "monthly" && dateKey.length() >= 7) dateKey = dateKey.substr(0, 7);
            else if (timeframe == "yearly" && dateKey.length() >= 4) dateKey = dateKey.substr(0, 4);
            
            // Initialize first candlestick for a new time period
            if(first) {
                currentKey = dateKey;
                open = high = low = close = e.price; // First price becomes open, high, low, and close
                first = false;
            }
            // When moving to a new time period, save previous candlestick and start new one
            else if(dateKey != currentKey) {
                candlesticks.emplace_back(currentKey, open, high, low, close);
                currentKey = dateKey;
                open = high = low = close = e.price; // Reset for new period
            } else {
                // Update high, low, and close for current time period
                if(e.price > high) high = e.price;
                if(e.price < low) low = e.price;
                close = e.price; // Last price in period becomes close
            }
        }
    }
    // Save the last candlestick if any data was processed
    if(!first) candlesticks.emplace_back(currentKey, open, high, low, close);
    return candlesticks;
}

// ============================================================================
// WRITTEN BY ME - TASK 4 IMPLEMENTATION
// ============================================================================
// TASK 4: Simulates new market trading activity by creating ask and bid orders
// Creates at least 5 ask and 5 bid orders for each available product
// Uses current system timestamp to handle the date gap between historical data (2022) and current date
void OrderBook::simulateMarketActivity() {
    // Get current system timestamp to bridge the gap from historical data (2022) to present
    std::time_t t = std::time(0);
    std::tm* now = std::localtime(&t);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", now);
    std::string timeStr(buf);

    // Append simulated orders to CSV file for persistence
    std::ofstream simFile("simulated_orders.csv", std::ios::app);
    
    // Get all available products from the order book
    std::vector<std::string> products = getKnownProducts();
    // Create 5 ask and 5 bid orders for each product
    for(std::string p : products) {
        for(int i=0; i<5; ++i) { 
            try {
                // Calculate ask price (slightly higher) and bid price (slightly lower)
                // Price calculation: base price of 1000 with random variation
                // This approach ensures realistic price differences between asks and bids
                double p1 = 1000 + (rand() % 100); // Ask price: 1000-1100
                double p2 = 1000 - (rand() % 100); // Bid price: 900-1000
                
                // Create new order entries with current timestamp
                OrderBookEntry newAsk(timeStr, p, "ask", p1, 0.5);
                OrderBookEntry newBid(timeStr, p, "bid", p2, 0.5);
                
                // Add to in-memory order book
                orders.push_back(newAsk);
                orders.push_back(newBid);

                // Persist to CSV file
                if(simFile.is_open()) {
                    simFile << timeStr << "," << p << ",ask," << p1 << ",0.5\n";
                    simFile << timeStr << "," << p << ",bid," << p2 << ",0.5\n";
                }
            } catch(...) {
                // Silently handle any errors during order creation
            }
        }
    }
    simFile.close();
}