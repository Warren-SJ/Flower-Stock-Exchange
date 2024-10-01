//
// Created by warren on 01-Oct-24.
//

#include "account.h"
#include "account_entry.h"
#include <algorithm>
#include <iostream>
#include <utility>

using namespace std;
// Constructor: Initialize account type
account::account(std::string  account_type) : account_type(std::move(account_type)) {}

// Add a new entry and sort the account
void account::addEntry(const account_entry& entry) {
    entries.push_back(entry);
    sortEntries();  // Sort based on account type
}

// Remove an entry by order_id and sort the account
void account::removeEntry(int order_id) {
    entries.erase(
            std::remove_if(entries.begin(), entries.end(),
                           [order_id](const account_entry& entry) {
                               return entry.getOrderID() == order_id;
                           }),
            entries.end());
    sortEntries();  // Sort again after removing the entry
}

// Print all entries
void account::printAccount() const {
    cout << "Account (" << account_type << "):" << endl;
    for (const auto& entry : entries) {
        entry.printEntry();
    }
}

// Private helper function to sort entries
void account::sortEntries() {
    if (account_type == "buy") {
        // Sort in descending order (highest price first)
        sort(entries.begin(), entries.end(),
                  [](const account_entry& a, const account_entry& b) {
                      return a.getPrice() > b.getPrice();
                  });
    } else if (account_type == "sell") {
        // Sort in ascending order (lowest price first)
        sort(entries.begin(), entries.end(),
                  [](const account_entry& a, const account_entry& b) {
                      return a.getPrice() < b.getPrice();
                  });
    }
}
