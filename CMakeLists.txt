cmake_minimum_required(VERSION 3.29)
project(Flower_Stock_Exchange)

set(CMAKE_CXX_STANDARD 26)

add_executable(Flower_Stock_Exchange main.cpp orders.cpp
        execution_rep.cpp
        execution_rep.h
        order_book.cpp
        order_book.h
        account_entry.cpp
        account_entry.h
        account.cpp
        account.h)
