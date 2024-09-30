#include <vector>
#include <iostream>
#include "orders.h"
#include "execution_rep.h"
using namespace std;
int main() {
    cout <<"Enter absolute path of the file: ";
    string path;
    cin >> path;
    replace(path.begin(), path.end(), '\\', '/'); // For Windows path
    vector<vector<string>> orders = get_orders(path);
    if (orders.empty()) {
        cerr << "File is empty!" << endl;
        return 1;
    }
    const string& report_path = path.substr(0, path.find_last_of('/')) + "/execution_rep.csv";
    if (write_execution_rep(report_path, orders) == 0) {
        cout << "Execution report written to " << report_path << endl;
        return 0;
    }
    else {
        cerr << "Error writing execution report!" << endl;
        return 1;
    }
}