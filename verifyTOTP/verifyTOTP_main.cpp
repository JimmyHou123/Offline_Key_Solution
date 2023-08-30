#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <chrono>
#include <thread>
#include "verifyTOTP.h"

using namespace std;

void helper(string email) {
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

    int trials = 0;
    bool locked = false;

    while (trials < MAX_TRIALS && !locked) {
        string inputPassword;
        cout << "\nEnter the password to verify: ";
        cin >> inputPassword;

        unsigned int selectedTimeStep = timeStepOptions[choice - 1].first;
        if (verifyTOTP(email, inputPassword, selectedTimeStep)) {
            cout << "Password is valid." << endl;
            break;
        } else {
            trials++;

            if (trials >= MAX_TRIALS) {
                locked = true;
                cout << "Too many unsuccessful verification attempts. Program will be locked for " << LOCK_TIME << " seconds." << endl;

                // Lock the program for LOCK_TIME seconds
                this_thread::sleep_for(chrono::seconds(LOCK_TIME));
                locked = false;
                trials = 0; // Reset trials after unlocking
            } else {
                cout << "Password is not valid. Please try again." << endl;
            }
        }
    }
}

int main() {
    unordered_set<string> user_emails;

    cout << "Welcome to TOTP Dynamic Password Verifier!\n";

    while (true) {
        cout << "\nEnter your email (or 'q' to quit): ";
        string email;
        cin >> email;

        if (email == "q") {
            break;
        }

        if (user_emails.find(email) != user_emails.end()) {
            helper(email);
        } else {
            cout << "Email not found. Enter authentication password to create a new account." << endl;
            
            int trials = 0;
            bool locked = false;

            while (trials < MAX_TRIALS && !locked) {
                string authen;
                cout << "Enter authentication password: ";
                cin >> authen;

                if (authen == AUTHEN) {
                    cout << "Authentication successful. Access granted." << endl;
                    break;
                } else {
                    trials++;

                    if (trials >= MAX_TRIALS) {
                        locked = true;
                        cout << "Too many unsuccessful trials. Program will be locked for " << LOCK_TIME << " seconds." << endl;

                        // Lock the program for LOCK_TIME seconds
                        this_thread::sleep_for(chrono::seconds(LOCK_TIME));
                        locked = false;
                        trials = 0; // Reset trials after unlocking
                    } else {
                        cout << "Authentication failed. Please try again." << endl;
                    }
                }
            }

            user_emails.insert(email);
            
            helper(email);
        }
    }

    cout << "Thank you for using TOTP Dynamic Password Verifier!\n";

    return 0;
}
