#include <vector>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include "orders.h"
#include "account.h"
#include "account_entry.h"
#include <iomanip>
#include <chrono>
#include <ctime>
#include "logic.h"


using namespace std;

int main() {
    cout << "Enter absolute path of the file: ";
    string path;
    cin >> path;
    replace(path.begin(), path.end(), '\\', '/'); // For Windows path
    orderStatus orders = get_orders(path);
    if (orders.status == 1) {
        cerr << "Error Opening File" << endl;
        return 1;
    } else if (orders.status == 2) {
        cerr << "File is not a CSV file!" << endl;
        return 1;
    } else if (orders.orders.empty()) {
        cerr << "No orders found!" << endl;
        return 1;
    }

    unordered_map<string, account, AccountHash, AccountEqual> order_book = process_orders(orders);

    // Print the order book

    for (const auto &[instrument, acc]: order_book) {
        cout << "Instrument: " << instrument << endl;
        cout << "Buy Entries: " << endl;
        for (const auto &entry: acc.getBuyEntries()) {
            cout << entry.getClientOrderID() << " " << entry.getPrice() << " " << entry.getQuantity() << endl;
        }
        cout << "Sell Entries: " << endl;
        for (const auto &entry: acc.getSellEntries()) {
            cout << entry.getClientOrderID() << " " << entry.getPrice() << " " << entry.getQuantity() << endl;
        }
    }

    system("pause");
    return 0;
}