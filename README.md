# C++ Trading Simulator

**Portfolio focus:** object-oriented design, CSV parsing, and modelling users, wallets, and order books.

**Scope:** an educational trading simulation. It does not connect to a real exchange or execute financial transactions.

A command-line C++ trading simulator built for an object-oriented programming course. The project models users, wallets, order book entries, CSV reading, and trading actions.

## Tech Stack

- C++
- CSV data files
- Object-oriented programming

## Features

- User and wallet management
- Order book parsing
- Trading app controller class
- CSV reader utility
- Simulated order data
- Separate header and implementation files

## How to Compile

Use a C++ compiler such as `g++`:

```bash
g++ main.cpp CSVReader.cpp OrderBook.cpp OrderBookEntry.cpp TradingApp.cpp User.cpp UserManager.cpp Wallet.cpp -o trading_app
```

Then run:

```bash
./trading_app
```

On Windows PowerShell, run:

```powershell
.\trading_app.exe
```

## What I Learned

- Object-oriented C++ design
- Splitting code into classes
- Header/source file organization
- File handling with CSV data
- Building a larger console application
