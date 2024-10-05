//
// Created by warren on 01-Oct-24.
//

#ifndef FLOWER_STOCK_EXCHANGE_ACCOUNT_H
#define FLOWER_STOCK_EXCHANGE_ACCOUNT_H

#include "account_entry.h"
#include <vector>
#include <string>
#include <algorithm>

using namespace std;
class account {
public:
    account(const string& instrument);


    void addBuyEntry(const account_entry& entry);
    void addSellEntry(const account_entry& entry);

    const vector<account_entry>& getBuyEntries() const { return buy_entries; }
    const vector<account_entry>& getSellEntries() const { return sell_entries; }

    string getInstrument() const;

    void popFrontBuyEntries();
    void popFrontSellEntries();


    void replaceEntry(const account_entry &entry, int side);

private:
    string instrument;
    vector<account_entry> buy_entries;
    vector<account_entry> sell_entries;

    // Helper functions for sorting
    void sortBuyEntries();
    void sortSellEntries();


};



#endif //FLOWER_STOCK_EXCHANGE_ACCOUNT_H
