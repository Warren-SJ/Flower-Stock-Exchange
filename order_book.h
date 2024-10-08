//
// Created by warren on 08-Oct-24.
//

#ifndef FLOWER_STOCK_EXCHANGE_ORDER_BOOK_H
#define FLOWER_STOCK_EXCHANGE_ORDER_BOOK_H

#include "account.h"
#include <unordered_map>
#include <string>
#include "logic.h"

using namespace std;

int write_order_book(const unordered_map<string, account, AccountHash, AccountEqual>& order_book);

#endif //FLOWER_STOCK_EXCHANGE_ORDER_BOOK_H
