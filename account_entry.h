//
// Created by warren on 01-Oct-24.
//

#ifndef ACCOUNT_ENTRY_H
#define ACCOUNT_ENTRY_H

#include <string>

class account_entry {
public:
    account_entry(const std::string& client_order_id, double price, int quantity);

    std::string getClientOrderID() const { return client_order_id; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }

private:
    std::string order_id;
    std::string client_order_id;
    double price;
    int quantity;
};

#endif  // ACCOUNT_ENTRY_H

