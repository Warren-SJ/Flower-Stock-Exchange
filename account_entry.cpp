//
// Created by warren on 01-Oct-24.
//

// account_entry.cpp
#include "account_entry.h"

using namespace std;

account_entry::account_entry(const string& client_order_id, int order_id, double price, int quantity)
        : client_order_id(client_order_id), order_id(order_id), price(price), quantity(quantity) {}
