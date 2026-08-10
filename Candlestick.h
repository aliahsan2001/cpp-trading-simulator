// Candlestick.h
// ============================================================================
// WRITTEN BY ME - TASK 1 IMPLEMENTATION
// ============================================================================
// Header file for Candlestick class - represents OHLC (Open, High, Low, Close) data
// TASK 1: Class structure for storing and displaying candlestick summary data
#pragma once
#include <string>
#include <iostream>

class Candlestick {
public:
    std::string date;      // Time period identifier (date, month, or year depending on timeframe)
    double open;           // First trading price in the time period
    double high;           // Highest price seen during the time period
    double low;            // Lowest price seen during the time period
    double close;          // Last trading price in the time period

    // Constructor: initializes all candlestick values
    Candlestick(std::string date, double open, double high, double low, double close)
    : date(date), open(open), high(high), low(low), close(close) {}

    // Displays candlestick data in formatted output
    void print() {
        std::cout << "Date: " << date 
                  << " | Open: " << open 
                  << " | High: " << high 
                  << " | Low: " << low 
                  << " | Close: " << close << std::endl;
    }
};