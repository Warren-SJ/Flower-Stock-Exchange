//
// Created by warren on 30-Sep-24.
//

#ifndef FLOWER_STOCK_EXCHANGE_ORDERS_H
#define FLOWER_STOCK_EXCHANGE_ORDERS_H

#include <vector>
#include <string>
using namespace std;
struct orderStatus{
    vector<vector<string>> orders;
    int status;
};

orderStatus get_orders(const string& path);

#endif //FLOWER_STOCK_EXCHANGE_ORDERS_H
