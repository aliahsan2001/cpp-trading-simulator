// CSVReader.cpp
// Implementation of CSVReader class for file I/O and string parsing
#include "CSVReader.h"
#include <iostream>
#include <fstream>

CSVReader::CSVReader() {}

// Reads trading data from CSV file and converts each line to OrderBookEntry
// Handles errors gracefully by skipping invalid lines
std::vector<OrderBookEntry> CSVReader::readCSV(std::string csvFilename) {
    std::vector<OrderBookEntry> entries;
    std::ifstream csvFile{csvFilename};
    std::string line;
    if (csvFile.is_open()) {
        while(std::getline(csvFile, line)) {
            try {
                // Parse each line: tokenize, then convert to OrderBookEntry
                OrderBookEntry obe = stringsToOBE(tokenise(line, ','));
                entries.push_back(obe);
            } catch(const std::exception& e) {
                // Skip invalid lines instead of crashing
                continue;
            }
        }
    }
    return entries;
}

// Splits a string into tokens based on separator character
// Used for parsing CSV lines and user input (e.g., "ETH/BTC,200,0.5")
std::vector<std::string> CSVReader::tokenise(std::string csvLine, char separator) {
    std::vector<std::string> tokens;
    signed int start, end;
    std::string token;
    // Find first non-separator character
    start = csvLine.find_first_not_of(separator, 0);
    do {
        end = csvLine.find_first_of(separator, start);
        if (start == csvLine.length() || start == end) break;
        // Extract token between separators
        if (end >= 0) token = csvLine.substr(start, end - start);
        else token = csvLine.substr(start, csvLine.length() - start);
        tokens.push_back(token);
        start = end + 1;
    } while(end > 0);
    return tokens;
}

// Converts tokenized string vector to OrderBookEntry object
// Expected format: timestamp,product,orderType,price,amount
// Throws exception if format is invalid
OrderBookEntry CSVReader::stringsToOBE(std::vector<std::string> tokens) {
    double price, amount;
    // Validate correct number of tokens
    if (tokens.size() != 5) {
        throw std::exception();
    }
    try {
        // Parse numeric values (price and amount are at indices 3 and 4)
        price = std::stod(tokens[3]);
        amount = std::stod(tokens[4]);
    } catch(const std::exception& e){
        throw;  // Re-throw if conversion fails
    }
    // Create OrderBookEntry: timestamp, product, orderType, price, amount
    OrderBookEntry obe{tokens[0], tokens[1], tokens[2], price, amount};
    return obe;
}