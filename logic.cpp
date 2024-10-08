//
// Created by warren on 08-Oct-24.
//

#include "logic.h"
#include "account.h"
#include "account_entry.h"
#include "orders.h"
#include "helpers.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

vector<string> flowers = {"Rose", "Lavender", "Lotus", "Tulip", "Daisy", "Orchid"};

unordered_map<string, account, AccountHash, AccountEqual> process_orders(const orderStatus& orders){
    auto start_time = chrono::system_clock::now();
    ofstream out("execution_rep.csv");
    if (!out.is_open()) {
        cerr << "Error opening file for writing" << endl;
        return {};
    }
    int order_number = 0;
    out << "Order ID, Client Order ID, Instrument, Side, Exec Status, Price, Quantity, Reason, Time" << endl;
    unordered_map<string, account, AccountHash, AccountEqual> order_book;
    vector<string> client_order_id_history;
    for (const auto &row: orders.orders) {
        const std::string& client_order_id = row[0];
        std::string instrument = row[1];
        int side = stoi(row[2]);
        int quantity = stoi(row[3]);
        double price = stod(row[4]);
        double new_price;
        auto &acc = order_book.emplace(instrument,
                                       account(instrument)).first->second; // Get reference to the account
        string previous_order_id;
        ++order_number;
        bool added = false;

        //validations
        if (client_order_id.empty()) {
            out << "Ord" << order_number << ", " << client_order_id << ", " << instrument << ", " << side << ", "
                << "Reject" << ", " << quantity << ", " << price << ',' << "Invalid client order ID,"
                << getCurrentTimeFormatted() << endl;
            continue;
        }
        if (find(client_order_id_history.begin(), client_order_id_history.end(), client_order_id) !=
            client_order_id_history.end()) {
            out << "Ord" << order_number << ", " << client_order_id << ", " << instrument << ", " << side << ", "
                << "Reject" << ", " << quantity << ", " << price << ',' << "Duplicate client order ID,"
                << getCurrentTimeFormatted() << endl;
            continue;
        }
        if (find(flowers.begin(), flowers.end(), instrument) == flowers.end()) {
            out << "Ord" << order_number << ", " << client_order_id << ", " << instrument << ", " << side << ", "
                << "Reject" << ", " << quantity << ", " << price << ',' << "Invalid instrument,"
                << getCurrentTimeFormatted() << endl;
            continue;
        }
        if (side != 1 && side != 2) {
            out << "Ord" << order_number << ", " << client_order_id << ", " << instrument << ", " << side << ", "
                << "Reject" << ", " << quantity << ", " << price << ',' << "Invalid side,"
                << getCurrentTimeFormatted()
                << endl;
            continue;
        }
        if (quantity <= 0 || quantity > 1000 || quantity % 10 != 0) {
            out << "Ord" << order_number << ", " << client_order_id << ", " << instrument << ", " << side << ", "
                << "Reject" << ", " << quantity << ", " << price << ',' << "Invalid size,"
                << getCurrentTimeFormatted()
                << endl;
            continue;
        }
        if (price <= 0) {
            out << "Ord" << order_number << ", " << client_order_id << ", " << instrument << ", " << side << ", "
                << "Reject" << ", " << quantity << ", " << price << ',' << "Invalid price,"
                << getCurrentTimeFormatted()
                << endl;
            continue;
        }


        if (side == 1) { // If it is a buy entry
            while (!acc.getSellEntries().empty() && quantity > 0) { // Recursively check
                // Execute the trade
                if (price >= acc.getSellEntries().front().getPrice()) {
                    added = true;
                    new_price = acc.getSellEntries().front().getPrice();
                    if (acc.getSellEntries().front().getQuantity() > quantity) {
                        out << "Ord" << order_number << ", " << client_order_id << ", " << instrument << ", "
                            << side
                            << ", " << "Fill" << ", " << quantity << ", " << new_price << endl;
                        out << "Ord" << acc.getSellEntries().front().getOrderID() << ", "
                            << acc.getSellEntries().front().getClientOrderID() << ", " << instrument << ", " << 2
                            << ", " << "PFill" << ", " << quantity << ", " << new_price << ',' << ','
                            << getCurrentTimeFormatted() << endl;
                        acc.replaceEntry(account_entry(acc.getSellEntries().front().getClientOrderID(),
                                                       acc.getSellEntries().front().getOrderID(),
                                                       acc.getSellEntries().front().getPrice(),
                                                       acc.getSellEntries().front().getQuantity() - quantity), 2);
                        account_entry entry(client_order_id, order_number, new_price,
                                            acc.getSellEntries().front().getQuantity() - quantity);
                        acc.addBuyEntry(entry);
                        quantity = 0;
                    } else if (acc.getSellEntries().front().getQuantity() == quantity) {
                        out << "Ord" << order_number << ", " << client_order_id << ", " << instrument << ", "
                            << side
                            << ", " << "Fill" << ", " << quantity << ", " << new_price << ',' << ','
                            << getCurrentTimeFormatted() << endl;
                        out << "Ord" << acc.getSellEntries().front().getOrderID() << ", "
                            << acc.getSellEntries().front().getClientOrderID() << ", " << instrument << ", " << 2
                            << ", " << "Fill" << ", " << quantity << ", " << new_price << ',' << ','
                            << getCurrentTimeFormatted() << endl;
                        acc.popFrontSellEntries();
                        quantity = 0;
                    } else {
                        out << "Ord" << order_number << ", " << client_order_id << ", " << instrument << ", "
                            << side
                            << ", " << "PFill" << ", " << acc.getSellEntries().front().getQuantity() << ", "
                            << new_price << ',' << ',' << getCurrentTimeFormatted() << endl;
                        out << "Ord" << acc.getSellEntries().front().getOrderID() << ", "
                            << acc.getSellEntries().front().getClientOrderID() << ", " << instrument << ", " << 2
                            << ", " << "Fill" << ", " << acc.getSellEntries().front().getQuantity() << ", "
                            << new_price
                            << ',' << ',' << getCurrentTimeFormatted() << endl;
                        quantity -= acc.getSellEntries().front().getQuantity();
                        acc.popFrontSellEntries();
                    }
                } else {
                    account_entry entry(client_order_id, order_number, price, quantity);
                    acc.addBuyEntry(entry);
                    out << "Ord" << order_number << ", " << client_order_id << ", " << instrument << ", " << side
                        << ", " << "New" << ", " << quantity << ", " << price << ',' << ','
                        << getCurrentTimeFormatted()
                        << endl;
                    quantity = 0;
                }
            }
            if (quantity > 0) {
                if (!added) {
                    out << "Ord" << order_number << ", " << client_order_id << ", " << instrument << ", " << side
                        << ", " << "New" << ", " << quantity << ", " << price << ',' << ','
                        << getCurrentTimeFormatted()
                        << endl;
                }
                account_entry entry(client_order_id, order_number, price, quantity);
                acc.addBuyEntry(entry);
            }
        } else { // If it is a sell entry
            while (!acc.getBuyEntries().empty() &&
                   quantity > 0) { // If the top sell entry is less than or equal to the new price
                // Execute the trade
                if (price <= acc.getBuyEntries().front().getPrice()) {
                    added = true;
                    new_price = acc.getBuyEntries().front().getPrice();
                    if (acc.getBuyEntries().front().getQuantity() > quantity) {
                        out << "Ord" << order_number << ", " << client_order_id << ", " << instrument << ", "
                            << side
                            << ", " << "Fill" << ", " << acc.getBuyEntries().front().getQuantity() - quantity
                            << ", "
                            << new_price << ',' << ',' << getCurrentTimeFormatted() << endl;
                        out << "Ord" << acc.getBuyEntries().front().getOrderID() << ", "
                            << acc.getBuyEntries().front().getClientOrderID() << ", " << instrument << ", " << 1
                            << ", "
                            << "Fill" << ", " << acc.getBuyEntries().front().getQuantity() - quantity << ", "
                            << new_price << ',' << ',' << getCurrentTimeFormatted() << endl;
                        acc.replaceEntry(account_entry(acc.getBuyEntries().front().getClientOrderID(),
                                                       acc.getBuyEntries().front().getOrderID(),
                                                       acc.getBuyEntries().front().getPrice(),
                                                       acc.getBuyEntries().front().getQuantity() - quantity), 1);
                        account_entry entry(client_order_id, order_number, price,
                                            acc.getBuyEntries().front().getQuantity() - quantity);
                        acc.addSellEntry(entry);
                        quantity = 0;
                    } else if (acc.getBuyEntries().front().getQuantity() == quantity) {
                        acc.popFrontBuyEntries();
                        out << "Ord" << order_number << ", " << client_order_id << ", " << instrument << ", "
                            << side
                            << ", " << "Fill" << ", " << quantity << ", " << new_price << getCurrentTimeFormatted()
                            << endl;
                        out << "Ord" << acc.getBuyEntries().front().getOrderID() << ", "
                            << acc.getBuyEntries().front().getClientOrderID() << ", " << instrument << ", " << 1
                            << ", "
                            << "Fill" << ", " << quantity << ", " << new_price << ',' << ','
                            << getCurrentTimeFormatted() << endl;
                        quantity = 0;
                    } else {
                        out << "Ord" << order_number << ", " << client_order_id << ", " << instrument << ", "
                            << side
                            << ", " << "PFill" << ", " << acc.getBuyEntries().front().getQuantity() << ", "
                            << new_price
                            << ',' << ',' << getCurrentTimeFormatted() << endl;
                        out << "Ord" << acc.getBuyEntries().front().getOrderID() << ", "
                            << acc.getBuyEntries().front().getClientOrderID() << ", " << instrument << ", " << 1
                            << ", "
                            << "Fill" << ", " << acc.getBuyEntries().front().getQuantity() << ", " << new_price
                            << ','
                            << ',' << getCurrentTimeFormatted() << endl;
                        acc.popFrontBuyEntries();
                        quantity -= acc.getBuyEntries().front().getQuantity();
                    }
                } else {
                    out << "Ord" << order_number << ", " << client_order_id << ", " << instrument << ", " << side
                        << ", " << "New" << ", " << quantity << ", " << price << ',' << ','
                        << getCurrentTimeFormatted()
                        << endl;
                    account_entry entry(client_order_id, order_number, price, quantity);
                    acc.addSellEntry(entry);
                    quantity = 0;
                }
            }
            if (quantity > 0) {
                if (!added) {
                    out << "Ord" << order_number << ", " << client_order_id << ", " << instrument << ", " << side
                        << ", " << "New" << ", " << quantity << ", " << price << ',' << ','
                        << getCurrentTimeFormatted()
                        << endl;
                }
                account_entry entry(client_order_id, order_number, price, quantity);
                acc.addSellEntry(entry);
            }
        }
    }
    auto end_time = chrono::system_clock::now();

    chrono::duration<double> elapsed_seconds = end_time - start_time;

    auto elapsed_milliseconds = chrono::duration_cast<chrono::milliseconds>(elapsed_seconds);
    out << "Elapsed Time:" << ',' << elapsed_milliseconds.count() << "milliseconds" << endl;
    out.close();
    cout << "Done!" << endl;
    return order_book;
}