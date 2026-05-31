// =========================================================
// Assignment 4: Implementing Control Structures
// Employee Schedule Management System
// Language: C++
// =========================================================

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {

    srand(time(0));

    // -----------------------------------------------------
    // Days and Shifts
    // -----------------------------------------------------

    vector<string> days = {
        "Monday", "Tuesday", "Wednesday",
        "Thursday", "Friday", "Saturday", "Sunday"
    };

    vector<string> shifts = {
        "Morning", "Afternoon", "Evening"
    };

    // -----------------------------------------------------
    // Employee Preferences
    // -----------------------------------------------------

    map<string, map<string, string>> employees;

    employees["Alice"] = {
        {"Monday", "Morning"},
        {"Tuesday", "Morning"},
        {"Wednesday", "Afternoon"},
        {"Thursday", "Evening"},
        {"Friday", "Morning"},
        {"Saturday", "Afternoon"},
        {"Sunday", "Evening"}
    };

    employees["Bob"] = {
        {"Monday", "Morning"},
        {"Tuesday", "Afternoon"},
        {"Wednesday", "Morning"},
        {"Thursday", "Evening"},
        {"Friday", "Afternoon"},
        {"Saturday", "Morning"},
        {"Sunday", "Evening"}
    };

    employees["Charlie"] = {
        {"Monday", "Evening"},
        {"Tuesday", "Morning"},
        {"Wednesday", "Afternoon"},
        {"Thursday", "Morning"},
        {"Friday", "Evening"},
        {"Saturday", "Afternoon"},
        {"Sunday", "Morning"}
    };

    employees["David"] = {
        {"Monday", "Afternoon"},
        {"Tuesday", "Evening"},
        {"Wednesday", "Morning"},
        {"Thursday", "Afternoon"},
        {"Friday", "Morning"},
        {"Saturday", "Evening"},
        {"Sunday", "Afternoon"}
    };

    employees["Emma"] = {
        {"Monday", "Morning"},
        {"Tuesday", "Morning"},
        {"Wednesday", "Evening"},
        {"Thursday", "Afternoon"},
        {"Friday", "Morning"},
        {"Saturday", "Morning"},
        {"Sunday", "Afternoon"}
    };

    employees["Frank"] = {
        {"Monday", "Evening"},
        {"Tuesday", "Afternoon"},
        {"Wednesday", "Evening"},
        {"Thursday", "Morning"},
        {"Friday", "Afternoon"},
        {"Saturday", "Morning"},
        {"Sunday", "Morning"}
    };

    employees["Grace"] = {
        {"Monday", "Afternoon"},
        {"Tuesday", "Morning"},
        {"Wednesday", "Morning"},
        {"Thursday", "Evening"},
        {"Friday", "Afternoon"},
        {"Saturday", "Evening"},
        {"Sunday", "Morning"}
    };

    // -----------------------------------------------------
    // Schedule Data Structure
    // -----------------------------------------------------

    map<string, map<string, vector<string>>> schedule;

    for (string day : days) {
        for (string shift : shifts) {
            schedule[day][shift] = {};
        }
    }

    // Track work days
    map<string, int> workDays;

    for (auto emp : employees) {
        workDays[emp.first] = 0;
    }

    const int MAX_EMPLOYEES_PER_SHIFT = 3;
    const int MAX_DAYS_PER_WEEK = 5;

    // -----------------------------------------------------
    // Step 1: Assign Preferred Shifts
    // -----------------------------------------------------

    for (auto emp : employees) {

        string empName = emp.first;

        for (auto pref : emp.second) {

            string day = pref.first;
            string preferredShift = pref.second;

            if (workDays[empName] >= MAX_DAYS_PER_WEEK)
                continue;

            bool alreadyAssigned = false;

            for (string shift : shifts) {

                vector<string> assigned = schedule[day][shift];

                for (string worker : assigned) {
                    if (worker == empName) {
                        alreadyAssigned = true;
                        break;
                    }
                }
            }

            if (alreadyAssigned)
                continue;

            // Assign preferred shift if space available
            if (schedule[day][preferredShift].size() < MAX_EMPLOYEES_PER_SHIFT) {

                schedule[day][preferredShift].push_back(empName);
                workDays[empName]++;
            }

            else {

                // -----------------------------------------
                // Conflict Resolution
                // -----------------------------------------

                bool assigned = false;

                for (string shift : shifts) {

                    if (shift != preferredShift &&
                        schedule[day][shift].size() < MAX_EMPLOYEES_PER_SHIFT) {

                        schedule[day][shift].push_back(empName);
                        workDays[empName]++;
                        assigned = true;
                        break;
                    }
                }

                // Assign next available day
                if (!assigned) {

                    int currentIndex = 0;

                    for (int i = 0; i < days.size(); i++) {
                        if (days[i] == day) {
                            currentIndex = i;
                            break;
                        }
                    }

                    for (int i = currentIndex + 1; i < days.size(); i++) {

                        string nextDay = days[i];

                        bool dayAssigned = false;

                        for (string shift : shifts) {

                            if (schedule[nextDay][shift].size() <
                                MAX_EMPLOYEES_PER_SHIFT) {

                                bool exists = false;

                                for (string s : shifts) {

                                    for (string worker :
                                    schedule[nextDay][s]) {

                                        if (worker == empName) {
                                            exists = true;
                                            break;
                                        }
                                    }
                                }

                                if (!exists) {

                                    schedule[nextDay][shift]
                                        .push_back(empName);

                                    workDays[empName]++;
                                    dayAssigned = true;
                                    break;
                                }
                            }
                        }

                        if (dayAssigned)
                            break;
                    }
                }
            }
        }
    }

    // -----------------------------------------------------
    // Step 2: Ensure minimum 2 employees per shift
    // -----------------------------------------------------

    for (string day : days) {

        for (string shift : shifts) {

            while (schedule[day][shift].size() < 2) {

                vector<string> available;

                for (auto emp : employees) {

                    string empName = emp.first;

                    if (workDays[empName] >= MAX_DAYS_PER_WEEK)
                        continue;

                    bool workingToday = false;

                    for (string s : shifts) {

                        for (string worker : schedule[day][s]) {

                            if (worker == empName) {
                                workingToday = true;
                                break;
                            }
                        }
                    }

                    if (!workingToday)
                        available.push_back(empName);
                }

                if (!available.empty()) {

                    int randomIndex = rand() % available.size();

                    string chosen = available[randomIndex];

                    schedule[day][shift].push_back(chosen);

                    workDays[chosen]++;
                }

                else {
                    break;
                }
            }
        }
    }

    // -----------------------------------------------------
    // Final Schedule Output
    // -----------------------------------------------------

    cout << "\n=================================================\n";
    cout << "       FINAL EMPLOYEE WEEKLY SCHEDULE\n";
    cout << "=================================================\n";

    for (string day : days) {

        cout << "\n" << day << endl;
        cout << "----------------------------------------\n";

        for (string shift : shifts) {

            cout << shift << " Shift:\n";

            if (schedule[day][shift].empty()) {

                cout << "  No Employees Assigned\n";
            }

            else {

                for (string emp : schedule[day][shift]) {

                    cout << "  - " << emp << endl;
                }
            }
        }
    }

    // -----------------------------------------------------
    // Employee Work Summary
    // -----------------------------------------------------

    cout << "\n=================================================\n";
    cout << "          EMPLOYEE WORK SUMMARY\n";
    cout << "=================================================\n";

    for (auto emp : workDays) {

        cout << emp.first << " works "
            << emp.second << " day(s)\n";
    }

    return 0;
}
