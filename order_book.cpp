//
// Created by warren on 08-Oct-24.
//
#include <fstream>
#include <iostream>
#include <algorithm>

#include "order_book.h"
#include "account.h"
#include "logic.h"

using namespace std;

int write_order_book(const unordered_map<string, account, AccountHash, AccountEqual>& order_book, const string& path) {
    ofstream file(path + "order_book.csv");

    if (!file.is_open()) {
        std::cerr << "Failed to open file!" << std::endl;
        return 1;
    }

// Loop through the order book and write to CSV
    for (const auto &[instrument, acc]: order_book) {
        file << instrument << std::endl;  // Write the instrument name

// Buy and Sell entries in one row
        const auto &buyEntries = acc.getBuyEntries();
        const auto &sellEntries = acc.getSellEntries();

// Determine the max number of entries between buy and sell
        size_t maxEntries = max(buyEntries.size(), sellEntries.size());

// Write buy and sell entries side by side
        for (size_t i = 0; i < maxEntries; ++i) {
            if (i < buyEntries.size()) {
                file << buyEntries[i].getClientOrderID() << ","
                     << buyEntries[i].getPrice() << ","
                     << buyEntries[i].getQuantity();
            } else {
                file << ",,";
            }

            file << ",";  // Separator between buy and sell

            if (i < sellEntries.size()) {
                file << sellEntries[i].getClientOrderID() << ","
                     << sellEntries[i].getPrice() << ","
                     << sellEntries[i].getQuantity();
            } else {
                file << ",,";
            }

            file << endl;  // Move to next row
        }
    }

    file.close();
    return 0;
}