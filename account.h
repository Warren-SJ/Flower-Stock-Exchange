//
// Created by warren on 01-Oct-24.
//

#ifndef FLOWER_STOCK_EXCHANGE_ACCOUNT_H
#define FLOWER_STOCK_EXCHANGE_ACCOUNT_H

#include <vector>
#include <string>

#include "account_entry.h"


using namespace std;
class account {
public:
    account(string instrument);


    void addBuyEntry(const account_entry& entry);
    void addSellEntry(const account_entry& entry);

    [[nodiscard]] const vector<account_entry>& getBuyEntries() const { return buy_entries; }
    [[nodiscard]] const vector<account_entry>& getSellEntries() const { return sell_entries; }

    [[nodiscard]] string getInstrument() const;

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
