# Flower-Stock-Exchange
A simple stock exchange program created as part of the C++ course conducted by the London Stock Exchange Group (LSEG). The main objective of this course was to learn high performance programming in C++.

The requirements for the programs are stated in the presentation.

### Libraries

- iostream
- string
- vector
- unordered_map
- iomanip
- chrono
- ctime
- utility
- sstream
- fstream

**Note**: C++20 or later is required to compile the program.

### Usage

1. Clone the repository to your local machine using the following command:
```bash
git clone https://github.com/Warren-SJ/Flower-Stock-Exchange.git
```
Alternatively, you can download the repository as a zip file and extract it to a directory of your choice.

2. If Visual Studio is installed, open the solution file Flower-Stock-Exchange-UI.sln in Visual Studio. Select either Debug or Release mode, and run the program. Running in Release mode is recommended for better performance. If Visual Studio is not installed, it can be downloaded from Microsoft's official website [here](https://visualstudio.microsoft.com/downloads/)

3. If you are using a different IDE, create a new project, add all source files (`main.cpp`, `account_entry.cpp`, etc.), and configure any dependencies or settings required. Compile and run the program.

4. If an IDE is not used, compile the program using the following command:
```bash
g++ -o Flower_Stock_Exchange main.cpp account_entry.cpp account.cpp logic.cpp order_book.cpp orders.cpp
```
**Note**: You need `g++` installed to use this command. If you’re on Windows, you may need MinGW or a compatible compiler to run this command.

5. Enter the absolute path of the orders file when prompted. The python script names `data_generation.py` is provided to generate a random orders file. Number of orders and error probability can be adjusted in the script.

6. The program will read the orders file and generate the execution report as execution_report.csv and the order book as order_book.csv in the same directory as the orders file.

7. Press any key to exit

### What's next?

I am currently working on a GUI for this project. Check again soon!
