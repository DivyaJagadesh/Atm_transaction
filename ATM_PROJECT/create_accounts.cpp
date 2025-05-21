#include <iostream>
#include <fstream>
using namespace std;

int main() {
    ofstream file("accounts.csv");
    if (!file) {
        cout << "Error creating file!" << endl;
        return 1;
    }
    file << "AccountNo,Balance,ConcurrentUsers,Status,LastTimestamp" << endl;
    file << "101,5000,0,ACTIVE,0" << endl;
    file << "102,3000,0,ACTIVE,0" << endl;
    file << "103,7000,0,ACTIVE,0" << endl;
    file << "104,2500,0,ACTIVE,0" << endl;
    file << "105,9000,0,ACTIVE,0" << endl;

    file.close();
    cout << "accounts.csv created successfully!" << endl;

    return 0;
}
