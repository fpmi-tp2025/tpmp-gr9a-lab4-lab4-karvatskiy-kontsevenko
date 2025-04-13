# Tourist Bureau Console Application

## Description

This console-based C/C++ application simulates the work of a tourist bureau, allowing management of excursion routes, buses, crews, and trip records. Users can authenticate and perform data operations such as viewing trips, calculating crew payments, and analyzing performance metrics based on database queries. The app uses SQLite for persistent data storage, including support for image data.

## Installation

To install the application locally, follow these steps:

1. Clone the repository:
   ```bash
   git clone https://github.com/fpmi-tp2025/tpmp-gr9a-lab4-lab4-karvatskiy-kontsevenko.git
2. Navigate to the project directory:
   ```bash
   cd tpmp-gr9a-lab4-lab4-karvatskiy-kontsevenko
3.Build the project using make:
   ```bash
   make
   ```
*Ensure you have a C/C++ compiler and sqlite3 development libraries installed.*  

## Usage
Launch the demo:

   ```bash
   ./bin/demo
   ```
Launch the app:

   ```bash
   ./bin/main
   ```
Authenticate using provided credentials.

Use the menu to:
* View trips by bus
* Insert/update/delete route, bus, crew, and trip data
* Calculate crew earnings for a given period
* Perform data analysis on bus mileage and profitability

## Contributing
Team Members & Roles:

* Aliaksei Kantsavenka — Wiki, Documentation, Tasks, Presentation
* Timofei Karvatskiy — Database design, App coding
