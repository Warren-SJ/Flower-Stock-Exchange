#pragma once

#include <unordered_map>
#include <string>

#include "account.h"
#include "logic.h"

using namespace std;

int write_order_book(const unordered_map<string, account, AccountHash, AccountEqual>& order_book, const string& path);

