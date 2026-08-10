// OrderBookEntry.cpp
// Implementation of OrderBookEntry class
#include "OrderBookEntry.h"

// Constructor: initializes all order entry fields using member initializer list
OrderBookEntry::OrderBookEntry(std::string _timestamp, std::string _product, std::string _orderType, double _price, double _amount)
: timestamp(_timestamp), product(_product), orderType(_orderType), price(_price), amount(_amount) {}

// Comparison function for sorting orders chronologically
// Returns true if e1's timestamp is earlier than e2's timestamp
bool OrderBookEntry::compareByTimestamp(OrderBookEntry& e1, OrderBookEntry& e2) {
    return e1.timestamp < e2.timestamp;
}