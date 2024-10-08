//
// Created by warren on 08-Oct-24.
//

#ifndef FLOWER_STOCK_EXCHANGE_HELPERS_H
#define FLOWER_STOCK_EXCHANGE_HELPERS_H

#include <string>
#include <chrono>
#include <iomanip>
#include "account.h"

using namespace std;

struct AccountHash {
    size_t operator()(const account &acc) const {
        return hash<string>()(acc.getInstrument());  // Hash based on instrument name
    }
};

struct AccountEqual {
    bool operator()(const account &a, const account &b) const {
        return a.getInstrument() == b.getInstrument();  // Compare instruments
    }
};

inline string getCurrentTimeFormatted() {
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
#endif //FLOWER_STOCK_EXCHANGE_HELPERS_H
