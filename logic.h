//
// Created by warren on 08-Oct-24.
//

#ifndef FLOWER_STOCK_EXCHANGE_LOGIC_H
#define FLOWER_STOCK_EXCHANGE_LOGIC_H

#include <unordered_map>

#include "orders.h"
#include "account.h"
#include "helpers.h"

using namespace std;

unordered_map<string, account, AccountHash, AccountEqual> process_orders(const orderStatus& order_status, const string& path);

#endif //FLOWER_STOCK_EXCHANGE_LOGIC_H
