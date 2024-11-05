#pragma once

#include <vector>
#include <string>

using namespace std;

struct orderStatus {
    vector<vector<string>> orders;
    int status;
};

orderStatus get_orders(const string& path);