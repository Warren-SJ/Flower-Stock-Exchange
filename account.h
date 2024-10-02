//
// Created by warren on 01-Oct-24.
//

#ifndef FLOWER_STOCK_EXCHANGE_ACCOUNT_H
#define FLOWER_STOCK_EXCHANGE_ACCOUNT_H

#include "account_entry.h"
#include <vector>
#include <string>
#include <algorithm>

class account {
public:
    account(const std::string& instrument);

    void addBuyEntry(const account_entry& entry);
    void addSellEntry(const account_entry& entry);

    const std::vector<account_entry>& getBuyEntries() const { return buy_entries; }
    const std::vector<account_entry>& getSellEntries() const { return sell_entries; }

    std::string getInstrument() const;

private:
    std::string instrument;
    std::vector<account_entry> buy_entries;
    std::vector<account_entry> sell_entries;

    // Helper functions for sorting
    void sortBuyEntries();
    void sortSellEntries();


};



#endif //FLOWER_STOCK_EXCHANGE_ACCOUNT_H
