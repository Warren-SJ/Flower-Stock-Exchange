cmake_minimum_required(VERSION 3.29)
project(Flower_Stock_Exchange)

set(CMAKE_CXX_STANDARD 26)

add_executable(Flower_Stock_Exchange main.cpp orders.cpp orders.h
        account_entry.cpp
        account_entry.h
        account.cpp
        account.h
        logic.cpp
        logic.h
)
