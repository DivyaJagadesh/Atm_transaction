//admin.cpp

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

struct Account {
    int    account_number;
    int    balance;
    int    concurrent_users;
    string status;
    string last_timestamp;
};
void ensureQueueFile() {
    bool exists = ifstream("access_queue.txt").good();
    ofstream q("access_queue.txt", ios::app);
    if (!exists) {
        q << "AccountNo,RequestTimestamp\n";
    }
    
}
void displayAccounts() {
    ifstream in("accounts.csv");
    if (!in) {
        cerr << "Error: cannot open accounts.csv\n";
        return;
    }

    string header;
    getline(in, header);  

    cout << "\n--- Accounts ---\n";
    cout << "AccNo | Balance | Users |  Status  |     LastTimestamp\n";

    string line;
    while (getline(in, line)) {
        stringstream ss(line);
        Account a;
        string field;

        getline(ss, field, ','); a.account_number   = stoi(field);
        getline(ss, field, ','); a.balance          = stoi(field);
        getline(ss, field, ','); a.concurrent_users = stoi(field);
        getline(ss, field, ','); a.status           = field;
        getline(ss, field);   
        if (field.size() >= 2 && field.front() == '"' && field.back() == '"')
            field = field.substr(1, field.size() - 2);
        a.last_timestamp = field;

        cout << a.account_number
             << "     " << a.balance
             << "      " << a.concurrent_users
             << "      " << a.status
             << "      " << a.last_timestamp
             << "\n";
    }
    in.close();
}
void displayQueue() {
    ifstream q("access_queue.txt");
    if (!q) {
        cout << "\n(No queued requests yet)\n";
        return;
    }

    cout << "\n--- Access Queue ---\n";
    string line;
    bool first = true;
    while (getline(q, line)) {
        if (first) {
            cout << line << "\n";  
            first = false;
        } else {
            cout << line << "\n";
        }
    }
    q.close();
}

int main() {
    ensureQueueFile();
    displayAccounts();
    displayQueue();

    return 0;
}
