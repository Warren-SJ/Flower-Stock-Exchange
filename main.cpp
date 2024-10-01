#include <vector>
#include <iostream>
#include "orders.h"
#include "execution_rep.h"
#include "account.h"
#include "order_book.h"
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
    vector<vector<string>> history;
    for (const auto& order : orders.orders) {

    }
    const string& report_path = path.substr(0, path.find_last_of('/')) + "/execution_rep.csv";
    if (write_execution_rep(report_path, history) == 0) {
        cout << "Execution report written to " << report_path << endl;
        return 0;
    }
    else {
        cerr << "Error writing execution report!" << endl;
        return 1;
    }
}