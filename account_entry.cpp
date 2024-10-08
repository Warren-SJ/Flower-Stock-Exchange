//
// Created by warren on 01-Oct-24.
//

// account_entry.cpp
#include "account_entry.h"

#include <utility>

using namespace std;

account_entry::account_entry(string  client_order_id, int order_id, double price, int quantity)
        : client_order_id(std::move(client_order_id)), order_id(order_id), price(price), quantity(quantity) {}
