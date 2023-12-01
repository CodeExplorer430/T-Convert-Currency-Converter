# T-Convert: Currency Converter v1.0
T-Convert is a command-line based currency converter application developed in C programming language. This application allows users to convert between different currencies using live exchange rates fetched from the FX Rates API.

**Features:**<br>
- **Currency Conversion:** Convert an amount from one currency to another based on the latest exchange rates.
- **View Supported Currencies:** Display a list of supported currencies available for conversion.
- **Date Selection:** Users can select a specific date to view historical exchange rates.

**How It Works:**<br>
- Utilizes the FX Rates API to fetch real-time exchange rates.
- Offers a user-friendly interface with step-by-step instructions for currency conversion.
- Validates user inputs to ensure accurate and error-free conversions.

**Requirements:**<br>
- **CURL Library:** Utilizes the libcurl library for making HTTP requests to the FX Rates API.
- **cJSON Library:** Employs cJSON for parsing JSON data received from the API.

**Usage:**<br>
1. Run the executable file and choose an option from the displayed menu.
2. Enter the source and target currencies, the amount to convert, and the date if required.
3. Receive instant conversion results or view supported currencies.

**Development:**<br>
This application was developed by a group of mine from BSIT 1B at the University of Caloocan City (UCC) as a case study project for learning the fundamentals of programming using C.

**How to Contribute:**<br>
Feel free to contribute to T-Convert by opening issues for bugs or feature requests. Pull requests with improvements or new features are welcome!

**License:**<br>
This project is licensed under the MIT License.
