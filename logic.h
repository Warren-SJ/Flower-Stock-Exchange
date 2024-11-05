#pragma once

#include <unordered_map>

#include "orders.h"
#include "account.h"
#include "helpers.h"

using namespace std;

unordered_map<string, account, AccountHash, AccountEqual> process_orders(const orderStatus& order_status, const string& path);