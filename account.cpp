//
// Created by warren on 01-Oct-24.
//
#include <algorithm>

#include "account.h"

using namespace std;

 account::account(string  instrument) : instrument(std::move(instrument)) {}

void account::addBuyEntry(const account_entry& entry) {
    buy_entries.push_back(entry);
    sortBuyEntries();
}

void account::addSellEntry(const account_entry& entry) {
    sell_entries.push_back(entry);
    sortSellEntries();
}

void account::sortBuyEntries() {
    sort(buy_entries.begin(), buy_entries.end(),
              [](const account_entry& a, const account_entry& b) {
                  return a.getPrice() > b.getPrice();  // Sort in descending order
              });
}

void account::sortSellEntries() {
    sort(sell_entries.begin(), sell_entries.end(),
              [](const account_entry& a, const account_entry& b) {
                  return a.getPrice() < b.getPrice();  // Sort in ascending order
              });
}
void account::popFrontBuyEntries() {
    buy_entries.erase(buy_entries.begin());
}

void account::popFrontSellEntries() {
    sell_entries.erase(sell_entries.begin());
}

void account::replaceEntry(const account_entry& entry, int side) {
    if (side == 1) {
        buy_entries.erase(buy_entries.begin());
        addBuyEntry(entry);
    } else {
        sell_entries.erase(sell_entries.begin());
        addSellEntry(entry);
    }
}
//Getter
string account::getInstrument() const {
    return instrument;
}

