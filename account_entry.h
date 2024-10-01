//
// Created by warren on 01-Oct-24.
//

#ifndef ACCOUNT_ENTRY_H
#define ACCOUNT_ENTRY_H

#include <string>

using namespace std;

class account_entry {
public:
    // Constructor
    account_entry(int order_id, double price, int quantity, std::string  side);

    // Getters
    [[nodiscard]] int getOrderID() const;
    [[nodiscard]] double getPrice() const;
    [[nodiscard]] int getQuantity() const;
    [[nodiscard]] string getSide() const;

    // Setters
    void setOrderID(int order_id);
    void setPrice(double price);
    void setQuantity(int quantity);
    void setSide(const std::string& side);

    // Function to print the account entry details
    void printEntry() const;

private:
    int order_id;
    double price;
    int quantity;
    std::string side;  // Could be "buy" or "sell"
};

#endif  // ACCOUNT_ENTRY_H

