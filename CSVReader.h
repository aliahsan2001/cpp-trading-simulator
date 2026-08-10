// CSVReader.h
// Header file for CSVReader class - handles reading and parsing CSV files
// Used to load trading data from CSV files and parse user input
#pragma once
#include "OrderBookEntry.h"
#include <vector>
#include <string>

class CSVReader {
public:
    CSVReader();
    
    // Reads trading orders from CSV file and returns vector of OrderBookEntry objects
    static std::vector<OrderBookEntry> readCSV(std::string csvFilename);
    
    // Tokenizes a string by splitting on a separator character (used for parsing CSV lines and user input)
    static std::vector<std::string> tokenise(std::string csvLine, char separator);
    
    // Converts tokenized string vector to OrderBookEntry object
    static OrderBookEntry stringsToOBE(std::vector<std::string> tokens);
};