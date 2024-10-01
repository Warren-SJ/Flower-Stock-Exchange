//
// Created by warren on 01-Oct-24.
//

#ifndef FLOWER_STOCK_EXCHANGE_ACCOUNT_H
#define FLOWER_STOCK_EXCHANGE_ACCOUNT_H

#include <string>
#include <vector>
#include "account_entry.h"

using namespace std;
class account {
public:
    // Constructor: Takes account type ("buy" or "sell")
    account(std::string  account_type);

    // Add and remove entries
    void addEntry(const account_entry& entry);
    void removeEntry(int order_id);

    // Print all entries in the account
    void printAccount() const;

private:
    std::string account_type;  // "buy" or "sell"
    std::vector<account_entry> entries{};

    // Helper function to sort entries
    void sortEntries();

    void addEntry(const account_entry &entry);
};

#endif  // ACCOUNT_H



#endif //FLOWER_STOCK_EXCHANGE_ACCOUNT_H
