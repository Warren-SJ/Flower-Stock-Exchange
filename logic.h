//
// Created by warren on 08-Oct-24.
//

#ifndef FLOWER_STOCK_EXCHANGE_LOGIC_H
#define FLOWER_STOCK_EXCHANGE_LOGIC_H

#include "orders.h"
#include "account.h"
#include <unordered_map>
#include "helpers.h"

unordered_map<string, account, AccountHash, AccountEqual> process_orders(const orderStatus& order_status);

#endif //FLOWER_STOCK_EXCHANGE_LOGIC_H
