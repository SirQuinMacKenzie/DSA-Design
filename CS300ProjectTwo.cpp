/**********************************************************************************
* Project Name: ABCU Course Information System
* File Name: CS300ProjectTwo
* Author: Quinlin MacKenzie
* Date Created: 4/20/24
* Last Modified: 4/20/24
* Description: Loads course data from a file, sorts alphanumerically,
*              and allows the search and display of various course info.
**********************************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

// Course class definition
class Course {
private:
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;

public:
    Course(const string& number, const string& title) : courseNumber(number), courseTitle(title) {}

    const string& getNumber() const { return courseNumber; }
    const string& getTitle() const { return courseTitle; }
    const vector<string>& getPrerequisites() const { return prerequisites; }

    void addPrerequisite(const string& prerequisite) {
        prerequisites.push_back(prerequisite);
    }
};

// Function to load courses from a file into a hashtable
void loadCourses(unordered_map<char, vector<Course>>& coursesHashtable, const string& filename) {
    coursesHashtable.clear(); // Clear existing courses before loading new ones

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string number, title, token;
        getline(ss, number, ','); // Read course number
        getline(ss, title, ',');  // Read course title

        Course course(number, title); // Create a new course object

        while (getline(ss, token, ',')) { // Read remaining tokens (prerequisites)
            if (!token.empty()) { // Ignore empty tokens
                course.addPrerequisite(token);
            }
        }

        char firstChar = number[0];
        coursesHashtable[firstChar].push_back(course); // Add course to hashtable
    }

    file.close();

    // Sort courses by course number after loading
    for (auto& entry : coursesHashtable) {
        vector<Course>& courses = entry.second;
        sort(courses.begin(), courses.end(), [](const Course& a, const Course& b) {
            return a.getNumber() < b.getNumber(); // Sort by course number
            });
    }
}


// Function to print course information
void printCourseInfo(const unordered_map<char, vector<Course>>& coursesHashtable) {
    for (const auto& entry : coursesHashtable) {
        const vector<Course>& courses = entry.second;
        for (const auto& course : courses) {
            cout << course.getNumber() << ", " << course.getTitle() << endl;
        }
    }
}

// Function to print course details
void printCourse(const unordered_map<char, vector<Course>>& coursesHashtable, const string& courseNumber) {
    char firstChar = courseNumber[0];
    if (coursesHashtable.find(firstChar) != coursesHashtable.end()) {
        const vector<Course>& courses = coursesHashtable.at(firstChar);
        auto it = find_if(courses.begin(), courses.end(), [&](const Course& course) {
            return course.getNumber() == courseNumber;
        });
        if (it != courses.end()) {
            const Course& course = *it;
            cout << course.getNumber() << ", " << course.getTitle() << endl;
            const vector<string>& prerequisites = course.getPrerequisites();
            if (!prerequisites.empty()) {
                cout << "Prerequisites: ";
                for (const auto& prereq : prerequisites) {
                    cout << prereq << " ";
                }
                cout << endl;
            }
        } else {
            cout << "Course " << courseNumber << " not found." << endl;
        }
    } else {
        cout << "No courses found." << endl;
    }
}

int main() {
    unordered_map<char, vector<Course>> coursesHashtable;

    // Main menu loop
    int choice;
    do {
        cout << "1. Load Data Structure.\n"
                "2. Print Course List.\n"
                "3. Print Course.\n"
                "9. Exit\n"
                "What would you like to do? ";
        cin >> choice;

        switch (choice) {
            case 1:
                loadCourses(coursesHashtable, "courseInfo.txt");
                break;
            case 2:
                printCourseInfo(coursesHashtable);
                break;
            case 3: {
                string courseNumber;
                cout << "What course do you want to know about? ";
                cin >> courseNumber;
                printCourse(coursesHashtable, courseNumber);
                break;
            }
            case 9:
                cout << "Thank you for using the course planner!" << endl;
                break;
            default:
                cout << choice << " is not a valid option." << endl;
                break;
        }
    } while (choice != 9);

    return 0;
}
