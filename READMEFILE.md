## ATM Management System
A comprehensive C++ application simulating ATM operations with extended banking features, including transaction history, loan management, and admin controls.

## Features

### User Features
- 💳 **Account Access** with secure PIN entry (hidden input)
- 🏦 **Transactions**: Withdraw, Deposit, Transfer funds
- 📊 **Balance Check** & **Transaction History**
- 🔐 **PIN Change** functionality
- 📅 **Daily Transaction Limit** (25,000)
- 🏦 **Loan Management**: Apply, Pay, Check Loan Status
- 💰 **Interest Application** for Savings accounts
- 📝 **Bill/Challan Payment**

### Admin Features
- 👨💼 **Admin Panel** with secure login
- ➕ **Create/Delete Accounts**
- 🔄 **Reset Daily Limits** & **Clear Transaction History**
- 📋 **View/Clear All Loans**
- ⚖️ **Compare Account Balances**

## Tech Stack
- **Language**: C++
- **Libraries**: `<fstream>`, `<iostream>`, `<ctime>`, `<conio.h>`
- **Data Storage**: Text files (`accounts.txt`, `loan.txt`, transaction history files)

## Installation & Usage

### Prerequisites
- C++ compiler (GCC recommended)
- Windows OS (for `conio.h` support)

Compilation
```bash
g++ FileName.cpp -o atm
```

### Run Program
```bash
./atm   # Linux/Mac
atm.exe # Windows
```

## File Structure
```
├── accounts.txt          # Stores account details
├── loan.txt              # Stores loan records
├── [account#]_history.txt # Transaction history per account
└── FileName.cpp          # Main source code
```

## Usage Example
1. **Main Menu**:
   ```
   1. Access Account
   2. Create Account (Admin)
   3. Delete Account (Admin)
   4. Admin Login
   5. Exit
   ```
2. **ATM Operations**:
   - Withdraw/Deposit funds
   - Transfer between accounts
   - Check balance & history
   - Manage loans

3. **Admin Controls** (Username: `admin`, PIN: `admin123`):
   - Create/delete accounts
   - Reset system limits
   - View all financial records

## Dependencies
- Windows-only: `conio.h` for hidden PIN input
- For Linux/Mac: Consider using `termios.h` alternative

## Contributing
Contributions welcome! Please fork the repository and create a pull request.

## License
[MIT](https://choosealicense.com/licenses/mit/)

## Future Improvements
- [ ] Add GUI interface
- [ ] Implement database integration
- [ ] Add encryption for sensitive data
- [ ] Support multiple currency types
- [ ] Enhance loan calculation logic
```
