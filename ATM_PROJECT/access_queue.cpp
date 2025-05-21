#include <iostream>
#include <fstream>
using namespace std;

int main() {
    ofstream file("access_queue.txt");
    if (!file) {
        cout << "Error creating file!" << endl;
        return 1;
    }
    file << "AccountNo,RequestTimestamp\n";
    file.close();
    cout << "access_queue.txt created successfully!" << endl;

    return 0;
}
