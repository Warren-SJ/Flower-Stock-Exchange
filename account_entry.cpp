#include "account_entry.h"

using namespace std;

account_entry::account_entry(string  client_order_id, int order_id, double price, int quantity)
    : client_order_id(std::move(client_order_id)), order_id(order_id), price(price), quantity(quantity) {}