#include "orders.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

orderStatus get_orders(const string& path) {
    ifstream file(path);
    if (!file.is_open()) {
        return { {}, 1 };
    }
    // Check if csv file
    if (path.substr(path.find_last_of('.') + 1) != "csv") {
        return { {}, 2 };
    }
    // Define a 2D array to store the CSV data
    vector<vector<string>> orders;
    string line;
    if (getline(file, line)) {
        // First line is read and discarded as it contains headings
    }
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        vector<string> row;
        while (getline(ss, cell, ',')) {
            row.push_back(cell);
        }
        orders.push_back(row);
    }
    file.close();

    // Print the data stored in the 2D array
//    for (const auto& row : orders) {
//        for (const auto& cell : row) {
//            cout << cell << " ";
//        }
//        cout << endl;
//    }
    return { orders, 0 };
}