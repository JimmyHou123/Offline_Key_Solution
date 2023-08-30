#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "generateTOTP.h"

using namespace std;

void helper(string key, string email) {
    // Time step options in seconds and corresponding descriptions
    vector<pair<unsigned int, string>> timeStepOptions = {
        {60, "1 minute"},
        {900, "15 minutes"},
        {3600, "1 hour"},
        {43200, "12 hours"},
        {86400, "24 hours"}
    };

    cout << "\n========= Time Step Options =========\n";
    for (size_t i = 0; i < timeStepOptions.size(); ++i) {
        cout << i + 1 << ". " << timeStepOptions[i].second << endl;
    }

    int choice;
    bool validChoice = false;
    
    do {
        cout << "Enter your choice: ";
        if (cin >> choice) {
            if (choice >= 1 && choice <= static_cast<int>(timeStepOptions.size())) {
                validChoice = true;
            } else {
                cout << "Invalid choice. Please enter a valid option." << endl;
            }
        } else {
            cout << "Invalid input. Please enter a valid option." << endl;
            cin.clear();  // Clear the error flags
        }
    } while (!validChoice);

    unsigned int selectedTimeStep = timeStepOptions[choice - 1].first;

    // Generate and display dynamic password for the selected time step
    string dynamicPassword = generateTOTP(key, email, selectedTimeStep);
    cout << "\nGenerated Dynamic Password (" << timeStepOptions[choice - 1].second << "): " << dynamicPassword << endl;
}

int main() {
    unordered_map<string, string> emailToKey;

    cout << "Welcome to TOTP Dynamic Password Generator!\n";

    while (true) {
        cout << "\nEnter your email (or 'q' to quit): ";
        string email;
        cin >> email;

        if (email == "q") {
            break;
        }

        if (emailToKey.find(email) != emailToKey.end()) {
            string key = emailToKey[email];
            helper(key, email);
        } else {
            cout << "Email not found. Enter key to create a new account: ";
            string newKey;
            cin >> newKey;
            emailToKey[email] = newKey;
            helper(newKey, email);
        }
    }

    cout << "Thank you for using TOTP Dynamic Password Generator!\n";

    return 0;
}
