#include <iostream>
#include <unordered_map>
#include <algorithm>

#include "orders.h"
#include "account.h"
#include "account_entry.h"
#include "logic.h"
#include "order_book.h"

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

    size_t lastSlash = path.find_last_of('/');

    if (lastSlash != string::npos) {
        path = path.substr(0, lastSlash + 1);
    }

    unordered_map<string, account, AccountHash, AccountEqual> order_book = process_orders(orders, path);

    // Write the order book
    int written = write_order_book(order_book, path);
    if(written == 1) {
        cerr << "Error writing to file" << endl;
        return 1;
    }

    system("pause");
    return 0;
}