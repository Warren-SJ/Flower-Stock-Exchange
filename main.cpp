#include <vector>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include "orders.h"
#include "account.h"
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

    struct AccountHash {
        std::size_t operator()(const account& acc) const {
            return std::hash<std::string>()(acc.getInstrument());  // Hash based on instrument name
        }
    };

    struct AccountEqual {
        bool operator()(const account& a, const account& b) const {
            return a.getInstrument() == b.getInstrument();  // Compare instruments
        }
    };

    vector<vector<string>> execution_rep;

//string output_path = path.substr(0, path.find_last_of('/', path.find_last_of('/') + 1)) + "execution_rep.csv";
    ofstream out("execution_rep.csv");
    if (!out.is_open()) {
        cerr << "Error opening file for writing" << endl;
        return 1;
    }
    out << "Order ID, Client Order ID, Instrument, Side, Exec Status, Price, Quantity" << endl;
    int i = 1;
    unordered_map<string, account, AccountHash, AccountEqual> order_book;
    for (const auto& row : orders.orders) {
        std::string order_id = row[0];
        std::string instrument = row[1];
        int side = stoi(row[2]);
        int quantity = stoi(row[3]);
        double price = stod(row[4]);

        execution_rep.push_back({"Ord" + to_string(i),order_id, instrument, to_string(side), "New", to_string(quantity), to_string(price)});
        auto& acc = order_book.emplace(instrument, account(instrument)).first->second; // Get reference to the account
//        cout << acc.getInstrument() << endl;
//        cout <<"Before" << acc.getBuyEntries().size() << " " << acc.getSellEntries().size() << endl;
        string previous_order_id;


        // Check if the account already exists. If it does, check if the top price is same as new price
        if(side == 1) { // If it is a buy entry
            if (!acc.getSellEntries().empty()) { // If the top sell entry is less than or equal to the new price
                previous_order_id = acc.getSellEntries().front().getClientOrderID();
                // Execute the trade
                if (price >= acc.getSellEntries().front().getPrice()){
                    price = acc.getSellEntries().front().getPrice();
                    if (acc.getSellEntries().front().getQuantity() > quantity){
                        acc.replaceEntry(account_entry(acc.getSellEntries().front().getClientOrderID(), acc.getSellEntries().front().getPrice(), acc.getSellEntries().front().getQuantity() - quantity), 2);
                        out << "Ord" << i << ", " << order_id << ", " << instrument << ", " << side << ", " << "Fill" << ", " << price << ", " <<acc.getSellEntries().front().getQuantity()- quantity << endl;
                        out << "Ord" << i++ << ", " << previous_order_id << ", " << instrument << ", " << 2 << ", " << "PFill" << ", " << price << ", " <<acc.getSellEntries().front().getQuantity()- quantity << endl;
//                  execution_rep.push_back({"Ord" + to_string(i),order_id, instrument, row[2], "Fill", row[3], row[4]});
                        account_entry entry(order_id, price, quantity - acc.getSellEntries().front().getQuantity());
                        acc.addBuyEntry(entry);
                    }
                    else if (acc.getSellEntries().front().getQuantity() == quantity){
                        acc.popFrontSellEntries();
                        out << "Ord" << i++ << ", " << order_id << ", " << instrument << ", " << side << ", " << "Fill" << ", " << price << ", " << quantity << endl;
                        out << "Ord" << i++ << ", " << previous_order_id << ", " << instrument << ", " << 2 << ", " << "Fill" << ", " << price << ", " << quantity << endl;
                    }
                    else{
                        acc.popFrontSellEntries();
                        out << "Ord" << i++ << ", " << previous_order_id << ", " << instrument << ", " << 2 << ", " << "Fill" << ", " << quantity - acc.getSellEntries().front().getQuantity()<< ", " << price << endl;
                        out << "Ord" << i++ << ", " << order_id << ", " << instrument << ", " << side << ", " << "PFill" << ", " << quantity -acc.getSellEntries().front().getQuantity() << ", " << price << endl;
                        account_entry entry(order_id, price, quantity - acc.getSellEntries().front().getQuantity());
                        acc.addBuyEntry(entry);
                    }
                }
                else{
                    account_entry entry(order_id, price, quantity);
                    acc.addBuyEntry(entry);
                    out << "Ord" << i++ << ", " << order_id << ", " << instrument << ", " << side << ", " << "New" << ", " << quantity << ", " << price << endl;
                }
            }
            else{
                account_entry entry(order_id, price, quantity);
                acc.addBuyEntry(entry);
//                cout <<"Empty Sell Triggered" << endl;
                out << "Ord" << i++ << ", " << order_id << ", " << instrument << ", " << side << ", " << "New" << ", " << quantity << ", " << price << endl;
            }
        } else {
            if (!acc.getBuyEntries().empty()) { // If the top sell entry is less than or equal to the new price
                previous_order_id = acc.getBuyEntries().front().getClientOrderID();
                // Execute the trade
                if (price <= acc.getBuyEntries().front().getPrice()) {
                    cout <<price << endl;
                    cout <<acc.getBuyEntries().front().getPrice() << endl;
                    price = acc.getBuyEntries().front().getPrice();
                    if (acc.getBuyEntries().front().getQuantity() > quantity){
                        acc.replaceEntry(account_entry(acc.getBuyEntries().front().getClientOrderID(), acc.getBuyEntries().front().getPrice(), acc.getBuyEntries().front().getQuantity() - quantity), 1);
//                        cout <<"No 1"<<endl;
                        out << "Ord" << i << ", " << order_id << ", " << instrument << ", " << side << ", " << "Fill" << ", " << acc.getBuyEntries().front().getQuantity() - quantity << ", " << price << endl;
                        out << "Ord" << i++ << ", " << previous_order_id << ", " << instrument << ", " << 1 << ", " << "Fill" << ", " << acc.getBuyEntries().front().getQuantity() - quantity << ", " << price << endl;
//                  execution_rep.push_back({"Ord" + to_string(i),order_id, instrument, row[2], "Fill", row[3], row[4]});
                        account_entry entry(order_id, price, quantity - acc.getBuyEntries().front().getQuantity());
                        acc.addSellEntry(entry);
                    }
                    else if (acc.getBuyEntries().front().getQuantity() == quantity){
                        acc.popFrontBuyEntries();
//                        cout <<"No 2"<<endl;
                        out << "Ord" << i++ << ", " << order_id << ", " << instrument << ", " << side << ", " << "Fill" << ", " << quantity << ", " << price << endl;
                        out << "Ord" << i++ << ", " << previous_order_id << ", " << instrument << ", " << 1 << ", " << "Fill" << ", " << quantity << ", " << price << endl;
                    }
                    else{
                        acc.popFrontBuyEntries();
//                        cout <<"No 3"<<endl;
                        out << "Ord" << i++ << ", " << previous_order_id << ", " << instrument << ", " << 1 << ", " << "Fill" << ", " << quantity - acc.getBuyEntries().front().getQuantity()<< ", " << price << endl;
                        out << "Ord" << i++ << ", " << order_id << ", " << instrument << ", " << side << ", " << "PFill" << ", " << quantity - acc.getBuyEntries().front().getQuantity() << ", " << price << endl;
                        account_entry entry(order_id, price, quantity - acc.getBuyEntries().front().getQuantity());
                        acc.addSellEntry(entry);
                    }
                }
                else{
                    account_entry entry(order_id, price, quantity);
                    acc.addSellEntry(entry);
//                    cout <<"No 4"<<endl;
                    out << "Ord" << i++ << ", " << order_id << ", " << instrument << ", " << side << ", " << "New" << ", " << quantity << ", " << price << endl;
                }

            }
            else{
                account_entry entry(order_id, price, quantity);
                acc.addSellEntry(entry);
//                cout <<"No 5"<<endl;
//                cout <<"Empty Buy Triggered" << endl;
                out << "Ord" << i++ << ", " << order_id << ", " << instrument << ", " << side << ", " << "New" << ", " << quantity << ", " << price << endl;
            }
        }
//        cout << "After" << acc.getBuyEntries().size() << " " << acc.getSellEntries().size() << endl;

    }



    // Print the order book

    for (const auto& [instrument, acc] : order_book) {
//        cout << "Instrument: " << instrument << endl;
//        cout << "Buy Entries: " << endl;
        for (const auto& entry : acc.getBuyEntries()) {
//            cout << entry.getClientOrderID() << " " << entry.getPrice() << " " << entry.getQuantity() << endl;
        }
//        cout << "Sell Entries: " << endl;
        for (const auto& entry : acc.getSellEntries()) {
//            cout << entry.getClientOrderID() << " " << entry.getPrice() << " " << entry.getQuantity() << endl;
        }
    }
    // Write the order book to a file

    return 0;
}