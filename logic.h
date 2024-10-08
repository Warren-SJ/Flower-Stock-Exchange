//
// Created by warre on 08-Oct-24.
//

#ifndef FLOWER_STOCK_EXCHANGE_LOGIC_H
#define FLOWER_STOCK_EXCHANGE_LOGIC_H

#include "orders.h"
#include "account.h"
#include <unordered_map>
struct AccountHash {
    std::size_t operator()(const account &acc) const {
        return std::hash<std::string>()(acc.getInstrument());  // Hash based on instrument name
    }
};

struct AccountEqual {
    bool operator()(const account &a, const account &b) const {
        return a.getInstrument() == b.getInstrument();  // Compare instruments
    }
};
unordered_map<string, account, AccountHash, AccountEqual> process_orders(orderStatus order_status);

#endif //FLOWER_STOCK_EXCHANGE_LOGIC_H
