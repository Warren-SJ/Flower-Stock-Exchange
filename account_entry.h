//
// Created by warren on 01-Oct-24.
//

#ifndef ACCOUNT_ENTRY_H
#define ACCOUNT_ENTRY_H

#include <string>

using namespace std;

class account_entry {
public:
    account_entry(string  client_order_id, int order_id, double price, int quantity);


    [[nodiscard]] string getClientOrderID() const { return client_order_id; }
    [[nodiscard]] int getOrderID() const { return order_id; }
    [[nodiscard]] double getPrice() const { return price; }
    [[nodiscard]] int getQuantity() const { return quantity; }

private:
    string client_order_id;
    int order_id;
    double price;
    int quantity;
};

#endif  // ACCOUNT_ENTRY_H

