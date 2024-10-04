//
// Created by warren on 01-Oct-24.
//

#include "account.h"

account::account(const std::string& instrument) : instrument(instrument) {}

void account::addBuyEntry(const account_entry& entry) {
    buy_entries.push_back(entry);
    sortBuyEntries();
}

void account::addSellEntry(const account_entry& entry) {
    sell_entries.push_back(entry);
    sortSellEntries();
}

void account::sortBuyEntries() {
    std::sort(buy_entries.begin(), buy_entries.end(),
              [](const account_entry& a, const account_entry& b) {
                  return a.getPrice() > b.getPrice();  // Sort in descending order
              });
}

void account::sortSellEntries() {
    std::sort(sell_entries.begin(), sell_entries.end(),
              [](const account_entry& a, const account_entry& b) {
                  return a.getPrice() < b.getPrice();  // Sort in ascending order
              });
}
//Getter
std::string account::getInstrument() const {
    return instrument;
}
