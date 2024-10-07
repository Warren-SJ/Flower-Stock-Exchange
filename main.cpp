#include <vector>
#include <iostream>
#include <unordered_map>
#include <fstream>
#include "orders.h"
#include "account.h"
#include "account_entry.h"
#include <iomanip>
#include <chrono>
#include <ctime>

string getCurrentTimeFormatted() {
    // Get the current time
    auto now = chrono::system_clock::now();
    auto currentTime = chrono::system_clock::to_time_t(now);

    // Convert to tm structure for local time
    tm localTime = *localtime(&currentTime);

    // Get the milliseconds part
    auto duration = now.time_since_epoch();
    auto millis = chrono::duration_cast<chrono::milliseconds>(duration) % 1000;

    // Format the time as YYYYMMDD-HHMMSS.sss
    ostringstream oss;
    oss << put_time(&localTime, "%Y%m%d-%H%M%S")
        << '.' << std::setfill('0') << std::setw(3) << millis.count();

    return oss.str();
}

using namespace std;
int main() {
    cout <<"Enter absolute path of the file: ";
    string path;
    cin >> path;
    replace(path.begin(), path.end(), '\\', '/'); // For Windows path
    auto start_time = chrono::system_clock::now();
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

    vector<string> flowers = {"Rose", "Lavender", "Lotus", "Tulip", "Daisy", "Orchid"};

    ofstream out("execution_rep.csv");
    if (!out.is_open()) {
        cerr << "Error opening file for writing" << endl;
        return 1;
    }
    int order_number = 0;
    out << "Order ID, Client Order ID, Instrument, Side, Exec Status, Price, Quantity, Reason, Time" << endl;
    unordered_map<string, account, AccountHash, AccountEqual> order_book;
    vector<string>client_order_id_history;
    for (const auto& row : orders.orders) {
        std::string client_order_id = row[0];
        std::string instrument = row[1];
        int side = stoi(row[2]);
        int quantity = stoi(row[3]);
        double price = stod(row[4]);
        double new_price;
        auto& acc = order_book.emplace(instrument, account(instrument)).first->second; // Get reference to the account
        string previous_order_id;
        ++order_number;
        bool added = false;

        //validations
        if(client_order_id.empty()){
            out << "Ord" << order_number << ", " << client_order_id << ", " << instrument << ", " << side << ", " << "Reject" << ", " << quantity << ", " << price << ',' << "Invalid client order ID," << getCurrentTimeFormatted()<<endl;
            continue;
        }
        if (find(client_order_id_history.begin(), client_order_id_history.end(), client_order_id) != client_order_id_history.end()){
            out << "Ord" << order_number << ", " << client_order_id << ", " << instrument << ", " << side << ", " << "Reject" << ", " << quantity << ", " << price << ',' << "Duplicate client order ID," << getCurrentTimeFormatted()<<endl;
            continue;
        }
        if (find(flowers.begin(), flowers.end(), instrument) == flowers.end()){
            out << "Ord" << order_number << ", " << client_order_id << ", " << instrument << ", " << side << ", " << "Reject" << ", " << quantity << ", " << price << ',' << "Invalid instrument," << getCurrentTimeFormatted()<<endl;
            continue;
        }
        if(side != 1 && side != 2){
            out << "Ord" << order_number << ", " << client_order_id << ", " << instrument << ", " << side << ", " << "Reject" << ", " << quantity << ", " << price << ',' << "Invalid side," << getCurrentTimeFormatted()<<endl;
            continue;
        }
        if(quantity <= 0 || quantity > 1000 || quantity % 10 != 0){
            out << "Ord" << order_number << ", " << client_order_id << ", " << instrument << ", " << side << ", " << "Reject" << ", " << quantity << ", " << price << ',' << "Invalid size," << getCurrentTimeFormatted()<<endl;
            continue;
        }
        if (price <= 0){
            out << "Ord" << order_number << ", " << client_order_id << ", " << instrument << ", " << side << ", " << "Reject" << ", " << quantity << ", " << price << ',' << "Invalid price," << getCurrentTimeFormatted()<<endl;
            continue;
        }


        if(side == 1) { // If it is a buy entry
            while (!acc.getSellEntries().empty() && quantity > 0) { // Recursively check
                // Execute the trade
                if (price >= acc.getSellEntries().front().getPrice()){
                    added = true;
                    new_price = acc.getSellEntries().front().getPrice();
                    if (acc.getSellEntries().front().getQuantity() > quantity){
                        out << "Ord" << order_number << ", " << client_order_id << ", " << instrument << ", " << side << ", " << "Fill" << ", " << quantity << ", " <<new_price << endl;
                        out << "Ord" << acc.getSellEntries().front().getOrderID() << ", " << acc.getSellEntries().front().getClientOrderID() << ", " << instrument << ", " << 2 << ", " << "PFill" << ", " << quantity << ", " << new_price << ',' <<getCurrentTimeFormatted()<<endl;
                        acc.replaceEntry(account_entry(acc.getSellEntries().front().getClientOrderID(), acc.getSellEntries().front().getOrderID(), acc.getSellEntries().front().getPrice(), acc.getSellEntries().front().getQuantity() - quantity), 2);
                        account_entry entry(client_order_id, order_number, new_price, acc.getSellEntries().front().getQuantity() - quantity);
                        acc.addBuyEntry(entry);
                        quantity = 0;
                    }
                    else if (acc.getSellEntries().front().getQuantity() == quantity){
                        out << "Ord" << order_number << ", " << client_order_id << ", " << instrument << ", " << side << ", " << "Fill" << ", " << quantity << ", " << new_price << ','<< getCurrentTimeFormatted()<< endl;
                        out << "Ord" << acc.getSellEntries().front().getOrderID() << ", " << acc.getSellEntries().front().getClientOrderID() << ", " << instrument << ", " << 2 << ", " << "Fill" << ", " << quantity << ", " << new_price <<',' << getCurrentTimeFormatted()<<endl;
                        acc.popFrontSellEntries();
                        quantity = 0;
                    }
                    else{
                        out << "Ord" << order_number << ", " << client_order_id << ", " << instrument << ", " << side << ", " << "PFill" << ", " << acc.getSellEntries().front().getQuantity() << ", " << new_price << ',' <<getCurrentTimeFormatted()<<endl;
                        out << "Ord" << acc.getSellEntries().front().getOrderID() << ", " << acc.getSellEntries().front().getClientOrderID() << ", " << instrument << ", " << 2 << ", " << "Fill" << ", " << acc.getSellEntries().front().getQuantity()<< ", " << new_price << ',' <<getCurrentTimeFormatted()<<endl;
                        quantity -= acc.getSellEntries().front().getQuantity();
                        acc.popFrontSellEntries();
                    }
                }else{
                    account_entry entry(client_order_id, order_number, price, quantity);
                    acc.addBuyEntry(entry);
                    out << "Ord" << order_number << ", " << client_order_id << ", " << instrument << ", " << side << ", " << "New" << ", " << quantity << ", " << price << ',' <<getCurrentTimeFormatted()<<endl;
                    quantity = 0;
                }
            }
            if (quantity > 0){
                if (!added) {
                    out << "Ord" << order_number << ", " << client_order_id << ", " << instrument << ", " << side
                        << ", " << "New" << ", " << quantity << ", " << price << ',' << getCurrentTimeFormatted()
                        << endl;
                }
                account_entry entry(client_order_id, order_number, price, quantity);
                acc.addBuyEntry(entry);
            }
        } else { // If it is a sell entry
            while (!acc.getBuyEntries().empty() && quantity > 0) { // If the top sell entry is less than or equal to the new price
                // Execute the trade
                if (price <= acc.getBuyEntries().front().getPrice()) {
                    added = true;
                    new_price = acc.getBuyEntries().front().getPrice();
                    if (acc.getBuyEntries().front().getQuantity() > quantity){
                        out << "Ord" << order_number << ", " << client_order_id << ", " << instrument << ", " << side << ", " << "Fill" << ", " << acc.getBuyEntries().front().getQuantity() - quantity << ", " << new_price << ',' << getCurrentTimeFormatted() <<endl;
                        out << "Ord" << acc.getBuyEntries().front().getOrderID() << ", " << acc.getBuyEntries().front().getClientOrderID() << ", " << instrument << ", " << 1 << ", " << "Fill" << ", " << acc.getBuyEntries().front().getQuantity() - quantity << ", " << new_price << ',' << getCurrentTimeFormatted()<<endl;
                        acc.replaceEntry(account_entry(acc.getBuyEntries().front().getClientOrderID(), acc.getBuyEntries().front().getOrderID(), acc.getBuyEntries().front().getPrice(), acc.getBuyEntries().front().getQuantity() - quantity), 1);
                        account_entry entry(client_order_id, order_number, price, acc.getBuyEntries().front().getQuantity() - quantity);
                        acc.addSellEntry(entry);
                        quantity = 0;
                    }
                    else if (acc.getBuyEntries().front().getQuantity() == quantity){
                        acc.popFrontBuyEntries();
                        out << "Ord" << order_number << ", " << client_order_id << ", " << instrument << ", " << side << ", " << "Fill" << ", " << quantity << ", " << new_price << getCurrentTimeFormatted()<<endl;
                        out << "Ord" << acc.getBuyEntries().front().getOrderID() << ", " << acc.getBuyEntries().front().getClientOrderID() << ", " << instrument << ", " << 1 << ", " << "Fill" << ", " << quantity << ", " << new_price << ',' << getCurrentTimeFormatted()<< endl;
                        quantity = 0;
                    }
                    else{
                        out << "Ord" << order_number << ", " << client_order_id << ", " << instrument << ", " << side << ", " << "PFill" << ", " << acc.getBuyEntries().front().getQuantity() << ", " << new_price << ',' << getCurrentTimeFormatted()<<endl;
                        out << "Ord" << acc.getBuyEntries().front().getOrderID() << ", " << acc.getBuyEntries().front().getClientOrderID() << ", " << instrument << ", " << 1 << ", " << "Fill" << ", " << acc.getBuyEntries().front().getQuantity()<< ", " << new_price << ',' << getCurrentTimeFormatted()<<endl;
                        acc.popFrontBuyEntries();
                        quantity -= acc.getBuyEntries().front().getQuantity();
                    }
                }
                else{
                    out << "Ord" << order_number << ", " << client_order_id << ", " << instrument << ", " << side << ", " << "New" << ", " << quantity << ", " << price <<',' <<  getCurrentTimeFormatted()<< endl;
                    account_entry entry(client_order_id, order_number,price, quantity);
                    acc.addSellEntry(entry);
                    quantity = 0;
                }
            }
            if (quantity > 0){
                if (!added){
                    out << "Ord" << order_number << ", " << client_order_id << ", " << instrument << ", " << side << ", " << "New" << ", " << quantity << ", " << price << ',' << getCurrentTimeFormatted()<<endl;
                }
                account_entry entry(client_order_id, order_number, price, quantity);
                acc.addSellEntry(entry);
            }
        }
    }

    // Print the order book

    for (const auto& [instrument, acc] : order_book) {
        cout << "Instrument: " << instrument << endl;
        cout << "Buy Entries: " << endl;
        for (const auto& entry : acc.getBuyEntries()) {
            cout << entry.getClientOrderID() << " " << entry.getPrice() << " " << entry.getQuantity() << endl;
        }
        cout << "Sell Entries: " << endl;
        for (const auto& entry : acc.getSellEntries()) {
            cout << entry.getClientOrderID() << " " << entry.getPrice() << " " << entry.getQuantity() << endl;
        }
    }
    auto end_time = chrono::system_clock::now();

    chrono::duration<double> elapsed_seconds = end_time - start_time;

    auto elapsed_milliseconds = chrono::duration_cast<chrono::milliseconds>(elapsed_seconds);
    out << "Elapsed Time:" << elapsed_milliseconds.count() <<"milliseconds"<<endl;
    out.close();


    return 0;
}