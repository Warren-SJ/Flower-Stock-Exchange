//
// Created by warren on 01-Oct-24.
//

// account_entry.cpp
#include "account_entry.h"

account_entry::account_entry(const std::string& order_id, double price, int quantity)
        : order_id(order_id), price(price), quantity(quantity) {}