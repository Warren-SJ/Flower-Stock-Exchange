#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;
int main() {
    cout <<"Enter absolute path of the file: ";
    string path;
    cin >> path;
    replace(path.begin(), path.end(), '\\', '/'); // For Windows path
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
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
    for (const auto& row : orders) {
        for (const auto& cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }
    return 0;
}
