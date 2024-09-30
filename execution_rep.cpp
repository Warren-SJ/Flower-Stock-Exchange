#include "orders.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

 int write_execution_rep(string path, const vector<vector<string>>& orders) {
    replace(path.begin(), path.end(), '\\', '/'); // For Windows path
    ofstream file(path);
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }
    int order_id = 1;
    // Write the data to the file
    file << "Order ID,Client Order, Instrument, Side, Exec Status, Quantity, Price" << endl;
    for (const auto& row : orders) {
        file << "ord" << order_id++ << ",";
        for (const auto& cell : row) {
            file << cell << ",";
        }
        file << endl;
    }
    file.close();
    return 0;
}