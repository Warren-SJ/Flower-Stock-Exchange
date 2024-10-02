#include <vector>
#include <iostream>
#include <unordered_map>
#include "orders.h"
#include "account.h"
#include "account_entry.h"

using namespace std;
int main() {
    cout <<"Enter absolute path of the file: ";
    string path;
    cin >> path;
    replace(path.begin(), path.end(), '\\', '/'); // For Windows path
    orderStatus orders = get_orders(path);
    if (orders.status == 1) {
        cerr << "Error Opening File" << endl;
        return 1;
    }
    else if (orders.status == 2) {
        cerr << "File is not a CSV file!" << endl;
        return 1;
    }
    else if (orders.orders.empty()) {
        cerr << "No orders found!" << endl;
        return 1;
    }

    struct AccountHash {
        std::size_t operator()(const account& acc) const {
            return std::hash<std::string>()(acc.getInstrument());  // Hash based on instrument name
        }
    };

    struct AccountEqual {
        bool operator()(const account& a, const account& b) const {
            return a.getInstrument() == b.getInstrument();  // Compare instruments
        }
    };

    unordered_map<string, account, AccountHash, AccountEqual> order_book;
    for (const auto& row : orders.orders) {
        std::string order_id = row[0];
        std::string instrument = row[1];
        int side = stoi(row[2]);
        int quantity = stoi(row[3]);
        double price = stod(row[4]);

        auto& acc = order_book.emplace(instrument, account(instrument)).first->second; // Get reference to the account

        // Create the account entry
        account_entry entry(order_id, price, quantity);

        // Add entry to the appropriate side
        if (side == 1) {  // Buy side
            acc.addBuyEntry(entry);  // Now using the reference to the account
        } else {  // Sell side
            acc.addSellEntry(entry);  // Using the reference
        }
    }
    // Print the order book
    for (const auto& [instrument, acc] : order_book) {
        cout << "Instrument: " << instrument << endl;
        cout << "Buy Entries: " << endl;
        for (const auto& entry : acc.getBuyEntries()) {
            cout << entry.getOrderID() << " " << entry.getPrice() << " " << entry.getQuantity() << endl;
        }
        cout << "Sell Entries: " << endl;
        for (const auto& entry : acc.getSellEntries()) {
            cout << entry.getOrderID() << " " << entry.getPrice() << " " << entry.getQuantity() << endl;
        }
    }
    return 0;
}