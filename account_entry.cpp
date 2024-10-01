//
// Created by warren on 01-Oct-24.
//

// account_entry.cpp
#include "account_entry.h"
#include <iostream>
#include <utility>

using namespace std;
// Constructor
account_entry::account_entry(int order_id,
                             double price,
                             int quantity,
                             string  side)
                            : order_id(order_id),
                            price(price),
                            quantity(quantity),
                            side(std::move(side)) {}

// Getters
int account_entry::getOrderID() const {
    return order_id;
}

double account_entry::getPrice() const {
    return price;
}

int account_entry::getQuantity() const {
    return quantity;
}

std::string account_entry::getSide() const {
    return side;
}

// Setters
void account_entry::setOrderID(int new_order_id) {
    this->order_id = new_order_id;
}

void account_entry::setPrice(double new_price) {
    this->price = new_price;
}

void account_entry::setQuantity(int new_quantity) {
    this->quantity = new_quantity;
}

void account_entry::setSide(const std::string& transaction_side) {
    this->side = transaction_side;
}

// Print entry details
void account_entry::printEntry() const {
    std::cout << "Order ID: " << order_id
              << ", Price: " << price
              << ", Quantity: " << quantity
              << ", Side: " << side << std::endl;
}
