//
// Created by warren on 01-Oct-24.
//

#ifndef ACCOUNT_ENTRY_H
#define ACCOUNT_ENTRY_H

#include <string>

using namespace std;

class account_entry {
public:
    account_entry(const string& client_order_id, int order_id, double price, int quantity);


    string getClientOrderID() const { return client_order_id; }
    int getOrderID() const { return order_id; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    void setQuantity(int quantity) { this->quantity = quantity; }

private:
    string client_order_id;
    int order_id;
    double price;
    int quantity;
};

#endif  // ACCOUNT_ENTRY_H

