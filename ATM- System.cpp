#include <fstream>
#include <iostream>
#include <string>
#include <ctime>
#include <conio.h> 

using namespace std;

string get_hidden_input() {
    string input;
    char ch;
    while (true) {
        ch = _getch(); 
        if (ch == '\r') 
            break;
        else if (ch == '\b') { 
            if (!input.empty()) {
                cout << "\b \b"; 
                input.pop_back();
            }
        }
        else {
            input.push_back(ch); 
            cout << '*'; 
        }
    }
    cout << endl;
    return input;
}

class TransactionHistory {
public:
    void log_transaction(int account_number, const string& transaction_type, double amount, double balance_after) {
        ofstream file(to_string(account_number) + "_history.txt", ios::app); 
        if (!file.is_open()) {
            cout << "Error: Could not open transaction history file for account " << account_number << "." << endl;
            return;
        }

        time_t now = time(0);
        char dt[26]; 
        ctime_s(dt, sizeof(dt), &now); 

        file << dt; 
        file << "Transaction Type: " << transaction_type << endl;
        file << "Amount: " << amount << endl;
        file << "Balance After Transaction: " << balance_after << endl;
        file << "-------------------------------------" << endl;

        file.close();
    }

    void display_transaction_history(int account_number) {
        ifstream file(to_string(account_number) + "_history.txt");
        if (!file.is_open()) {
            cout << "No transaction history found for account " << account_number << "." << endl;
            return;
        }

        cout << "\nTransaction History for Account " << account_number << ":" << endl;
        cout << "-------------------------------------" << endl;

        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
        cout << "-------------------------------------" << endl;
    }
};

class Accounts
{
public:
    int* account_number;
    double* balance;
    string* name;
    int* pins;
    int* failed_attempts;
    string* account_type;
    double* daily_transaction_total; 
    const double DAILY_LIMIT = 25000.0; 
    int count;

    Accounts() {
        count = 0;
        account_number = new int[100];
        balance = new double[100];
        name = new string[100];
        pins = new int[100];
        failed_attempts = new int[100];
        account_type = new string[100];
        daily_transaction_total = new double[100]; 
        for (int i = 0; i < 100; i++) {
            daily_transaction_total[i] = 0.0; 
        }
    }

    bool operator==(int other_account_number) const {
        for (int i = 0; i < count; ++i) {
            if (account_number[i] == other_account_number) {
                return true;
            }
        }
        return false;
    }

    int getAccountIndex(int account_number) const {
        for (int i = 0; i < count; ++i) {
            if (this->account_number[i] == account_number) {
                return i;
            }
        }
        return -1; 
    }

    void reset_daily_limit() {
        for (int i = 0; i < count; i++) {
            daily_transaction_total[i] = 0.0; 
        }
        cout << "Daily transaction limits have been reset." << endl;
    }

    bool can_transact(int accountIndex, double amount) {
        if (daily_transaction_total[accountIndex] + amount > DAILY_LIMIT) {
            cout << "Transaction exceeds the daily limit of 25,000." << endl;
            return false;
        }
        return true;
    }

    void update_daily_transaction(int accountIndex, double amount) {
        daily_transaction_total[accountIndex] += amount;
    }

    void read_accounts()
    {
        ifstream file("accounts.txt");
        if (!file.is_open()) {
            cout << "Error: Could not open accounts.txt file." << endl;
            return;
        }
        string line;
        while (getline(file, line))
        {
            size_t pos = 0;
            pos = line.find(',');
            account_number[count] = stoi(line.substr(0, pos));
            line.erase(0, pos + 1);

            pos = line.find(',');
            name[count] = line.substr(0, pos);
            line.erase(0, pos + 1);

            pos = line.find(',');
            pins[count] = stoi(line.substr(0, pos));
            line.erase(0, pos + 1);

            pos = line.find(',');
            account_type[count] = line.substr(0, pos);
            line.erase(0, pos + 1);

            balance[count] = stod(line);
            failed_attempts[count] = 0;
            count++;
        }
        file.close();
    }

    void write_accounts()
    {
        ofstream file("accounts.txt");
        if (!file.is_open()) {
            cout << "Error: Could not open accounts.txt file for writing." << endl;
            return;
        }
        for (int i = 0; i < count; i++) {
            file << account_number[i] << "," << name[i] << "," << pins[i] 
                << "," << account_type[i] << "," << balance[i] << endl;
        }
        file.close();
    }

    void create_account() {
        if (count >= 100) {
            cout << "Error: Cannot create more accounts." << endl;
            return;
        }

        int new_account_number;
        cout << "Enter new account number: ";
        cin >> new_account_number;

        for (int i = 0; i < count; i++) {
            if (account_number[i] == new_account_number) {
                cout << "Error: Account number already exists." << endl;
                return;
            }
        }

        account_number[count] = new_account_number;

        cout << "Enter name: ";
        cin.ignore();
        getline(cin, name[count]);

        cout << "Enter PIN: ";
        pins[count] = stoi(get_hidden_input());

        cout << "Enter initial balance: ";
        cin >> balance[count];

        cout << "Select account type (1 for Savings, 2 for Checking): ";
        int account_choice;
        cin >> account_choice;
        if (account_choice == 1) {
            account_type[count] = "Savings";
        }
        else {
            account_type[count] = "Checking";
        }

        count++;
        write_accounts();

        cout << "Account created successfully." << endl;
    }

    void delete_account()
    {
        int account_to_delete;
        cout << "Enter account number to delete: ";
        cin >> account_to_delete;
        int index = -1;
        for (int i = 0; i < count; i++) {
            if (account_number[i] == account_to_delete) {
                index = i;
                break;
            }
        }
        if (index == -1) {
            cout << "Account not found." << endl;
            return;
        }
        for (int i = index; i < count - 1; i++) {
            account_number[i] = account_number[i + 1];
            name[i] = name[i + 1];
            pins[i] = pins[i + 1];
            account_type[i] = account_type[i + 1];
            balance[i] = balance[i + 1];
        }
        count--;
        write_accounts();
        cout << "Account deleted successfully." << endl;
    }

    void reset_failed_attempts(int accountIndex)
    {
        failed_attempts[accountIndex] = 0;
    }

    bool is_account_locked(int accountIndex)
    {
        return failed_attempts[accountIndex] >= 3;
    }

    void increment_failed_attempts(int accountIndex)
    {
        failed_attempts[accountIndex]++;
    }

    void apply_interest(int accountIndex)
    {
        if (account_type[accountIndex] == "Savings") {
            double interest_rate = 0.05; 
            balance[accountIndex] += balance[accountIndex] * interest_rate;
            cout << "Interest applied. New balance: " << balance[accountIndex] << endl;
        }
    }
};

class LoanManagement {
public:
    void apply_for_loan(int account_number) {
        if (has_active_loan(account_number)) {
            cout << "You already have an active loan. Please pay it off before applying for a new one." << endl;
            return;
        }

        double loan_amount;
        int tenure_in_years;
        string repayment_method;

        cout << "Enter the loan amount (between 100 and 100,000): ";
        cin >> loan_amount;

        if (loan_amount < 100 || loan_amount > 100000) {
            cout << "Invalid loan amount. Loan must be between 100 and 100,000." << endl;
            return;
        }

        cout << "Enter the loan tenure (in years): ";
        cin >> tenure_in_years;

        cout << "How would you like to repay the loan? (monthly/yearly): ";
        cin >> repayment_method;

        double total_repayable = loan_amount;  
        double installment = 0;

        if (repayment_method == "monthly") {
            int total_months = tenure_in_years * 12;
            installment = total_repayable / total_months;
            cout << "Your monthly installment will be: " << installment << endl;
        }
        else if (repayment_method == "yearly") {
            installment = total_repayable / tenure_in_years;
            cout << "Your yearly installment will be: " << installment << endl;
        }
        else {
            cout << "Invalid repayment method. Loan application failed!" << endl;
            return;
        }

        ofstream file("loan.txt", ios::app);
        if (!file.is_open()) {
            cout << "Error: Could not open loan file." << endl;
            return;
        }

        file << account_number << ","
            << loan_amount << ","
            << tenure_in_years << ","
            << total_repayable << ","
            << repayment_method << ","
            << installment << endl;

        file.close();

        cout << "\nLoan applied successfully!" << endl;
        cout << "Loan Amount: " << loan_amount << endl;
        cout << "Tenure: " << tenure_in_years << " years" << endl;
        cout << "Repayment Method: " << repayment_method << endl;
        cout << "Total Repayable Amount: " << total_repayable << endl;
    }

    void view_loans() {
        ifstream file("loan.txt");
        if (!file.is_open()) {
            cout << "No loan records found." << endl;
            return;
        }

        cout << "Account No   Loan Amount   Tenure   Total Repayable   Repayment Method   Installment" << endl;
        cout << "------------------------------------------------------------------------" << endl;

        string line;
        while (getline(file, line)) {
            size_t pos = 0;
            string token;
            string data[6]; 
            int index = 0;

            while ((pos = line.find(',')) != string::npos) {
                token = line.substr(0, pos);
                data[index++] = token;
                line.erase(0, pos + 1);
            }
            data[index] = line;

            cout << data[0] << "           " << data[1] << "           "
                << data[2] << " years   " << data[3] << "           "
                << data[4] << "           " << data[5] << endl;
        }
        file.close();
    }

    void clear_loans() {
        ofstream file("loan.txt", ios::trunc); 
        if (file.is_open()) {
            file.close();
            cout << "All loan records cleared successfully!" << endl;
        }
        else {
            cout << "Error: Could not clear loan file." << endl;
        }
    }

    bool has_active_loan(int account_number) {
        ifstream file("loan.txt");
        if (!file.is_open()) {
            return false; 
        }

        string line;
        while (getline(file, line)) {
            size_t pos = line.find(',');
            int file_account_number = stoi(line.substr(0, pos));
            if (file_account_number == account_number) {
                file.close();
                return true; 
            }
        }
        file.close();
        return false;
    }

    void pay_loan(int account_number) {
        ifstream file("loan.txt");
        if (!file.is_open()) {
            cout << "No active loans found." << endl;
            return;
        }

        ofstream temp_file("temp.txt"); 
        if (!temp_file.is_open()) {
            cout << "Error: Could not create a temporary file." << endl;
            return;
        }

        bool loan_found = false;
        string line;

        while (getline(file, line)) {
            size_t pos = 0;
            string token;
            string data[6];
            int index = 0;

            while ((pos = line.find(',')) != string::npos) {
                token = line.substr(0, pos);
                data[index++] = token;
                line.erase(0, pos + 1);
            }
            data[index] = line;

            int file_account_number = stoi(data[0]);
            if (file_account_number == account_number) {
                loan_found = true;
                cout << "\nLoan Details:\n";
                cout << "Loan Amount: " << data[1] << endl;
                cout << "Total Repayable: " << data[3] << endl;

                double amount_to_pay;
                cout << "Enter the amount to pay: ";
                cin >> amount_to_pay;

                double remaining_balance = stod(data[3]) - amount_to_pay;
                if (remaining_balance <= 0) {
                    cout << "Loan fully paid off. Congratulations!" << endl;
                }
                else {
                    cout << "Remaining Balance: " << remaining_balance << endl;
                    temp_file << data[0] << ","
                        << data[1] << ","
                        << data[2] << ","
                        << remaining_balance << ","
                        << data[4] << ","
                        << data[5] << endl;
                }
            }
            else {
                temp_file << line << endl;
            }
        }

        file.close();
        temp_file.close();

        remove("loan.txt");
        rename("temp.txt", "loan.txt");

        if (!loan_found) {
            cout << "No active loan found for account number " << account_number << "." << endl;
        }
    }

    void check_loan_status(int account_number) {
        ifstream file("loan.txt");
        if (!file.is_open()) {
            cout << "No loan records found." << endl;
            return;
        }

        string line;
        bool loan_found = false;

        while (getline(file, line)) {
            size_t pos = 0;
            string token;
            string data[6];
            int index = 0;

            while ((pos = line.find(',')) != string::npos) {
                token = line.substr(0, pos);
                data[index++] = token;
                 line.erase(0, pos + 1);
            }
             data[index] = line;

            int file_account_number = stoi(data[0]);
            if (file_account_number == account_number) {
                loan_found = true;
                cout << "\nLoan Status:\n";
                cout << "Loan Amount: " << data[1] << endl;
                cout << "Remaining Balance: " << data[3] << endl;
                cout << "Repayment Method: " << data[4] << endl;
                cout << "Installment Amount: " << data[5] << endl;
                break;
            }
        }

        file.close();

        if (!loan_found) {
            cout << "No active loan found for account number " << account_number << "." << endl;
        }
    }
};

class ATM : public Accounts
{
public:
    TransactionHistory history;
    LoanManagement loanManager;
    void clear_transaction_history() {
        for (int i = 0; i < count; i++) {
            string file_name = to_string(account_number[i]) + "_history.txt";
            ofstream file(file_name, ios::trunc);
            if (!file) {
                cout << "Error: Could not open transaction history file for account " << account_number[i] << " for clearing." << endl;
            }
            file.close();
        }
    }

    void initialize()
    {
        read_accounts();
        if (count == 0) {
            cout << "Error: No accounts found." << endl;
        }
    }

    void deposit(int accountIndex) {
        int deposit_amount;
        cout << "Enter amount to deposit: ";
        cin >> deposit_amount;
        if (deposit_amount > 0) {
            if (can_transact(accountIndex, deposit_amount)) {
                balance[accountIndex] += deposit_amount;
                update_daily_transaction(accountIndex, deposit_amount);
                cout << "Deposit successful." << endl;
                write_accounts();

                history.log_transaction(account_number[accountIndex], "Deposit", deposit_amount, balance[accountIndex]);
            }
        }
        else {
            cout << "Invalid deposit amount." << endl;
        }
    }

    void withdraw(int accountIndex) {
        int withdraw_amount;
        cout << "Enter amount to withdraw: ";
        cin >> withdraw_amount;
        if (withdraw_amount > 0 && balance[accountIndex] >= withdraw_amount) {
            if (can_transact(accountIndex, withdraw_amount)) {
                balance[accountIndex] -= withdraw_amount;
                update_daily_transaction(accountIndex, withdraw_amount);
                cout << "Withdrawal successful." << endl;
                write_accounts();

                history.log_transaction(account_number[accountIndex], "Withdrawal", withdraw_amount, balance[accountIndex]);
            }
        }
        else {
            cout << "Insufficient funds or invalid withdrawal amount." << endl;
        }
    }

    void transfer(int accountIndex) {
        int target_account_number, transfer_amount;
        cout << "Enter account number to transfer to: ";
        cin >> target_account_number;

        if (*this == target_account_number) {
            cout << "Enter amount to transfer: ";
            cin >> transfer_amount;

            if (transfer_amount > 0 && balance[accountIndex] >= transfer_amount) {
                if (can_transact(accountIndex, transfer_amount)) {
                    for (int i = 0; i < count; ++i) {
                        if (account_number[i] == target_account_number) {
                            balance[accountIndex] -= transfer_amount;
                            balance[i] += transfer_amount;
                            update_daily_transaction(accountIndex, transfer_amount);

                            cout << "Transfer successful." << endl;
                            write_accounts();

                            history.log_transaction(account_number[accountIndex], "Transfer Out", transfer_amount, balance[accountIndex]);
                            history.log_transaction(account_number[i], "Transfer In", transfer_amount, balance[i]);
                            break;
                        }
                    }
                }
            }
            else {
                cout << "Insufficient funds or invalid transfer amount." << endl;
            }
        }
        else {
            cout << "Target account not found." << endl;
        }
    }

    void balance_check(int accountIndex)
    {
        cout << "Balance: " << balance[accountIndex] << endl;
    }

    void bill_payment(int accountIndex)
    {
        int bill_amount;
        cout << "Enter bill amount: ";
        cin >> bill_amount;
        if (bill_amount > 0 && balance[accountIndex] >= bill_amount) {
            balance[accountIndex] -= bill_amount;
            cout << "Bill payment successful." << endl;
            write_accounts();
        }
        else {
            cout << "Insufficient funds or invalid bill amount." << endl;
        }
    }

    void challan_payment(int accountIndex)
    {
        int challan_amount;
        cout << "Enter challan amount: ";
        cin >> challan_amount;
        if (challan_amount > 0 && balance[accountIndex] >= challan_amount) {
            balance[accountIndex] -= challan_amount;
            cout << "Challan payment successful." << endl;
            write_accounts();
        }
        else {
            cout << "Insufficient funds or invalid challan amount." << endl;
        }
    }

    void pin_change(int accountIndex)
    {
        int old_pin, new_pin;
        cout << "Enter current PIN: ";
        old_pin = stoi(get_hidden_input());
        if (pins[accountIndex] == old_pin) {
            cout << "Enter new PIN: ";
            new_pin = stoi(get_hidden_input());
            pins[accountIndex] = new_pin;
            write_accounts();
            cout << "PIN changed successfully." << endl;
        }
        else {
            cout << "Incorrect current PIN." << endl;
        }
    }

    void ask_for_rating() {
        int rating;
        cout << "\nThank you for using our ATM service!" << endl;
        cout << "Please rate our service (1 - Poor, 5 - Excellent): ";
        cin >> rating;

        if (rating >= 1 && rating <= 5) {
            cout << "Thank you for your feedback!" << endl;
        }
        else {
            cout << "Invalid rating. Please provide a rating between 1 and 5." << endl;
        }
    }

    void menu(int accountIndex) {
      int choice;
      do {
        cout << "\nWelcome to the ATM" << endl;
        cout << "Account: " << account_number[accountIndex] << "\tName: " << name[accountIndex]
             << "\tAccount Type: " << account_type[accountIndex] << endl;
        cout << "1. Withdraw" << endl;
        cout << "2. Deposit" << endl;
        cout << "3. Transfer" << endl;
        cout << "4. Balance Check" << endl;
        cout << "5. Bill Payment" << endl;
        cout << "6. Challan Payment" << endl;
        cout << "7. Change PIN" << endl;
        cout << "8. Apply Interest" << endl;
        cout << "9. View Transaction History" << endl;
        cout << "10. Apply for Loan" << endl;
        cout << "11. Pay Loan" << endl;
        cout << "12. Check Loan Status" << endl; 
        cout << "13. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            withdraw(accountIndex);
            break;
        case 2:
            deposit(accountIndex);
            break;
        case 3:
            transfer(accountIndex);
            break;
        case 4:
            balance_check(accountIndex);
            break;
        case 5:
            bill_payment(accountIndex);
            break;
        case 6:
            challan_payment(accountIndex);
            break;
        case 7:
            pin_change(accountIndex);
            break;
        case 8:
            apply_interest(accountIndex);
            break;
        case 9:
            history.display_transaction_history(account_number[accountIndex]);
            break;
        case 10: 
            loanManager.apply_for_loan(account_number[accountIndex]);
            break;
        case 11: 
            loanManager.pay_loan(account_number[accountIndex]);
            break;
        case 12:
            loanManager.check_loan_status(account_number[accountIndex]);
            break;
        case 13:
            ask_for_rating();
            clear_transaction_history();
            cout << "Goodbye! We hope to see you again soon." << endl;
            break;
        default:
            cout << "Invalid choice." << endl;
        }
      } while (choice != 13);
    }

    bool admin_login() {
        const string ADMIN_USERNAME = "admin";
        const string ADMIN_PIN = "admin123";

        string username, pin;
        cout << "Enter Admin Username: ";
        cin >> username;
        cout << "Enter Admin PIN: ";
        pin = get_hidden_input(); 

        if (username == ADMIN_USERNAME && pin == ADMIN_PIN) {
            cout << "Admin login successful!" << endl;
            return true;
        }
        else {
            cout << "Invalid Admin credentials!" << endl;
            return false;
        }
    }

    void adminMenu() {
        int adminChoice;
        do {
            cout << "\nAdmin Menu:\n";
            cout << "1. Create New Account\n";
            cout << "2. Delete Account\n";
            cout << "3. Reset Daily Limit\n";
            cout << "4. Clear Transaction History\n";
            cout << "5. View All Loans\n"; 
            cout << "6. Clear All Loans\n";
            cout << "7. Compare Accounts\n";
            cout << "8. Logout\n";
            cout << "Enter your choice: ";
            cin >> adminChoice;

            switch (adminChoice) {
            case 1:
                create_account();
                break;
            case 2:
                delete_account();
                break;
            case 3:
                reset_daily_limit();
                break;
            case 4:
                clear_transaction_history();
                break;
            case 5:
                loanManager.view_loans();
                break;
            case 6: 
                loanManager.clear_loans();
                break;
            case 7: { 
                int account1, account2;
                cout << "Enter the first account number to compare: ";
                cin >> account1;
                cout << "Enter the second account number to compare: ";
                cin >> account2;

                int index1 = getAccountIndex(account1);
                int index2 = getAccountIndex(account2);

                if (index1 != -1 && index2 != -1) {
                    cout << "Both accounts exist in the system." << endl;
                    cout << "Account " << account1 << " has balance: " << balance[index1] << endl;
                    cout << "Account " << account2 << " has balance: " << balance[index2] << endl;

                    if (balance[index1] > balance[index2]) {
                        cout << "Account " << account1 << " has more balance." << endl;
                    }
                    else if (balance[index1] < balance[index2]) {
                        cout << "Account " << account2 << " has more balance." << endl;
                    }
                    else {
                        cout << "Both accounts have the same balance." << endl;
                    }
                }
                else {
                    if (index1 == -1) {
                        cout << "Account " << account1 << " does not exist." << endl;
                    }
                    if (index2 == -1) {
                        cout << "Account " << account2 << " does not exist." << endl;
                    }
                }
                break;
            }
            case 8:
                cout << "Logging out...\n";
                break;
            default:
                cout << "Invalid choice! Try again.\n";
            }
        } while (adminChoice != 8);
    }

    void starting_menu()
    {
        int choice;
        do {
            cout << "\n1. Access Account" << endl;
            cout << "2. Create Account (Admin Only)" << endl;
            cout << "3. Delete Account (Admin Only)" << endl;
            cout << "4. Admin Login" << endl;
            cout << "5. Exit" << endl;
            cout << "Enter choice: ";
            cin >> choice;

            switch (choice) {
            case 1: {
                int account_number_input;
                cout << "Enter account number: ";
                cin >> account_number_input;
                int accountIndex = -1;

                for (int i = 0; i < count; i++) {
                    if (account_number[i] == account_number_input) {
                        accountIndex = i;
                        break;
                    }
                }

                if (accountIndex != -1) {
                    int pin_attempts = 0;
                    int pin;
                    while (pin_attempts < 3) {
                        cout << "Enter PIN: ";
                        pin = stoi(get_hidden_input());
                        if (pins[accountIndex] == pin) {
                            menu(accountIndex);
                            return;
                        }
                        else {
                            pin_attempts++;
                            cout << "Incorrect PIN. Please try again." << endl;
                        }
                    }
                    cout << "Account locked due to 3 incorrect PIN attempts." << endl;
                }
                else {
                    cout << "Account number not found." << endl;
                }
                break;
            }
            case 2: {
                cout << "Admin authentication required to create an account.\n";
                if (admin_login()) { 
                    create_account();
                }
                else {
                    cout << "Admin authentication failed. Access denied." << endl;
                }
                break;
            }
            case 3: {
                cout << "Admin authentication required to delete an account.\n";
                if (admin_login()) { 
                    delete_account();
                }
                else {
                    cout << "Admin authentication failed. Access denied." << endl;
                }
                break;
            }
            case 4: {
                cout << "Admin Login Menu\n";
                if (admin_login()) { 
                    adminMenu(); 
                }
                else {
                    cout << "Admin authentication failed. Returning to main menu." << endl;
                }
                break;
            }
            case 5:
                ask_for_rating();
                clear_transaction_history();
                cout << "Goodbye! We hope to see you again soon." << endl;
                break;
            default:
                cout << "Invalid choice." << endl;
            }
        } while (choice != 5);
    }
};

int main()
{
    ofstream file("accounts.txt");
    if (file.is_open()) {
        file << "101,hassan Khokhar,123,Savings,50000\n";
        file << "102,Mustufa Hashmi,456,Checking,60000\n";
        file << "103,Abdul Rafay Abbasi,789,Savings,70000\n";
        file << "104,Umer Baig,1011,Checking,80000\n";
        file << "105,Hazam Mehmood,1213,Savings,90000\n";
        file << "106,Muhammad Tauqeer Ul Hassan,2456,Checking,100000\n";
        file.close();
        cout << "accounts.txt file has been created with sample data." << endl;
    }
    else {
        cerr << "Error: Could not create accounts.txt file." << endl;
    }

    ATM atm;
    atm.initialize();
    atm.starting_menu();

    return 0;
}
