#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
using namespace std;

struct Account {
    int account_number;
    int balance;
    int concurrent_users;
    string status;
    string last_timestamp; 
};
string getISTTime() {
    time_t now = time(nullptr);
    tm *utc = gmtime(&now);
    utc->tm_hour += 5;    
    utc->tm_min  += 30;
    mktime(utc);         
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", utc);
    return string(buf);
}
void updateAccountFile(const vector<Account>& accounts) {
    ofstream out("accounts.csv");
    if (!out) {
        cerr << "Error: cannot write accounts.csv\n";
        return;
    }
    out << "AccountNo,Balance,ConcurrentUsers,Status,LastTimestamp\n";
    for (auto &a : accounts) {
        out
          << a.account_number << ","
          << a.balance        << ","
          << a.concurrent_users << ","
          << a.status         << ","
          << "\"" << a.last_timestamp << "\"\n";
    }
}
void logAccessAttempt(int acctNo) {
    bool exists = ifstream("access_queue.txt").good();
    ofstream q("access_queue.txt", ios::app);
    if (!exists) {
        q << "AccountNo,RequestTimestamp\n";
    }
    q << acctNo << ",\"" << getISTTime() << "\"\n";
}
bool tryToAccessAccount(vector<Account>& accounts, int acctNo) {
    for (auto &a : accounts) {
        if (a.account_number == acctNo) {
            if (a.concurrent_users == 0) {
                a.concurrent_users = 1;
                a.status           = "LOCKED";
                a.last_timestamp   = getISTTime();
                updateAccountFile(accounts);
                return true;
            } else {
                cout << "Account " << acctNo
                     << " is LOCKED — your request has been queued.\n";
                logAccessAttempt(acctNo);
                return false;
            }
        }
    }
    cout << "Account Locked!103!\n";
    return false;
}
void releaseAccount(vector<Account>& accounts, int acctNo) {
    for (auto &a : accounts) {
        if (a.account_number == acctNo) {
            a.concurrent_users = 0;
            a.status           = "ACTIVE";
            a.last_timestamp   = getISTTime();
            updateAccountFile(accounts);
            return;
        }
    }
}
void deposit(vector<Account>& accounts, int acctNo, int amt) {
    for (auto &a : accounts) {
        if (a.account_number == acctNo) {
            a.balance         += amt;
            a.last_timestamp   = getISTTime();
            updateAccountFile(accounts);
            cout << "Deposited " << amt
                 << ". New balance: " << a.balance << "\n";
            return;
        }
    }
}
void withdraw(vector<Account>& accounts, int acctNo, int amt) {
    for (auto &a : accounts) {
        if (a.account_number == acctNo) {
            if (amt <= a.balance) {
                a.balance       -= amt;
                a.last_timestamp = getISTTime();
                updateAccountFile(accounts);
                cout << "Withdrew " << amt
                     << ". New balance: " << a.balance << "\n";
            } else {
                cout << "Insufficient balance!\n";
            }
            return;
        }
    }
}

int main() {
    ifstream in("accounts.csv");
    if (!in) {
        cerr << "Cannot open accounts.csv\n";
        return 1;
    }
    string line, field;
    getline(in, line);  

    vector<Account> accounts;
    while (getline(in, line)) {
        stringstream ss(line);
        Account acc;
        getline(ss, field, ','); acc.account_number   = stoi(field);
        getline(ss, field, ','); acc.balance          = stoi(field);
        getline(ss, field, ','); acc.concurrent_users = stoi(field);
        getline(ss, field, ','); acc.status           = field;
        getline(ss, field);
        if (field.size() >= 2 && field.front() == '"' && field.back() == '"')
            field = field.substr(1, field.size() - 2);
        acc.last_timestamp = field;
        accounts.push_back(acc);
    }
    in.close();
    int acctNo, choice, amt;
    cout << "Enter your Account Number: ";
    cin  >> acctNo;
    if (!tryToAccessAccount(accounts, acctNo)) {
        return 0;
    }
    while (true) {
        cout << "\n--- User Menu ---\n"
             << "1. Check Balance\n"
             << "2. Deposit\n"
             << "3. Withdraw\n"
             << "4. Exit\n"
             << "Choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                for (auto &a : accounts)
                    if (a.account_number == acctNo)
                        cout << "Balance: " << a.balance << "\n";
                break;
            case 2:
                cout << "Amount to deposit: ";
                cin  >> amt;
                deposit(accounts, acctNo, amt);
                break;
            case 3:
                cout << "Amount to withdraw: ";
                cin  >> amt;
                withdraw(accounts, acctNo, amt);
                break;
            case 4:
                releaseAccount(accounts, acctNo);
                cout << "Session ended. Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice!\n";
        }
    }
}
