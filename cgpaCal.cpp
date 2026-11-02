#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstdlib> // for atoi
using namespace std;

// -----------------Convert Letter Grade to Points -------------------
float getGradePoint(string grade) {
    if (grade == "O") return 10;
    else if (grade == "A+") return 9;
    else if (grade == "A") return 8;
    else if (grade == "B+") return 7;
    else if (grade == "B") return 6;
    else if (grade == "C") return 5;
    else return 0; // F or invalid
}

// ------------------- Subject Class -------------------
class Subject {
public:
    string name;
    float gradePoint;
    int credit;

    Subject(string n, float g, int c) : name(n), gradePoint(g), credit(c) {}
};

// ------------------- Semester Class -------------------
class Semester {
public:
    vector<Subject> subjects;

    void addSubject(const Subject &sub) {
        subjects.push_back(sub);
    }

    float getSGPA() {
        float totalPoints = 0;
        int totalCredits = 0;
        for (int i = 0; i < subjects.size(); i++) {
            totalPoints += subjects[i].gradePoint * subjects[i].credit;
            totalCredits += subjects[i].credit;
        }
        return totalCredits ? (totalPoints / totalCredits) : 0;
    }

    int getTotalCredits() {
        int totalCredits = 0;
        for (int i = 0; i < subjects.size(); i++)
            totalCredits += subjects[i].credit;
        return totalCredits;
    }

    float getTotalPoints() {
        float totalPoints = 0;
        for (int i = 0; i < subjects.size(); i++)
            totalPoints += subjects[i].gradePoint * subjects[i].credit;
        return totalPoints;
    }
};

// ------------------- CGPAManager Class -------------------
class CGPAManager {
private:
    vector<Semester> semesters;

public:
    // Load from file
    void loadFromFile() {
        ifstream file("cgpa.txt");
        if (!file) return;

        int semCount;
        file >> semCount;
        file.ignore();
        semesters.clear();

        for (int i = 0; i < semCount; i++) {
            Semester sem;
            int subCount;
            file >> subCount;
            file.ignore();

            for (int j = 0; j < subCount; j++) {
                string line;
                getline(file, line);
                int p1 = line.find('|');
                int p2 = line.find('|', p1 + 1);

                string name = line.substr(0, p1);
                string gradeLetter = line.substr(p1 + 1, p2 - p1 - 1);
                int credit = atoi(line.substr(p2 + 1).c_str());

                float gradePoint = getGradePoint(gradeLetter);
                sem.addSubject(Subject(name, gradePoint, credit));
            }
            semesters.push_back(sem);
        }

        file.close();
    }

    // Save to file
    void saveToFile() {
        ofstream file("cgpa.txt");
        file << semesters.size() << endl;

        for (int i = 0; i < semesters.size(); i++) {
            Semester sem = semesters[i];
            file << sem.subjects.size() << endl;
            for (int j = 0; j < sem.subjects.size(); j++) {
                string gradeLetter;
                float gp = sem.subjects[j].gradePoint;
                if (gp == 10) gradeLetter = "O";
                else if (gp == 9) gradeLetter = "A+";
                else if (gp == 8) gradeLetter = "A";
                else if (gp == 7) gradeLetter = "B+";
                else if (gp == 6) gradeLetter = "B";
                else if (gp == 5) gradeLetter = "C";
                else gradeLetter = "F";

                file << sem.subjects[j].name << "|" << gradeLetter << "|" << sem.subjects[j].credit << endl;
            }
        }

        file.close();
    }

    // ------------------- Add/Update Semester -------------------
    void addSemester() {
        int semNum;
        cout << "Enter semester number to add/update: ";
        cin >> semNum;
        cin.ignore();

        if (semNum < 1) {
            cout << "Invalid semester number!\n";
            return;
        }

        loadFromFile(); // Load existing semesters

        // Resize vector if needed
        while (semesters.size() < semNum)
            semesters.push_back(Semester());

        Semester &sem = semesters[semNum - 1]; // Reference to chosen semester
        sem.subjects.clear(); // Clear existing subjects if overwriting

        int n;
        cout << "Enter number of subjects: ";
        cin >> n;
        cin.ignore();

        for (int i = 0; i < n; i++) {
            string name, gradeLetter;
            int credit;

            cout << "\nSubject name: ";
            getline(cin, name);
            cout << "Grade (O, A+, A, B+, B, C, F): ";
            cin >> gradeLetter;
            cout << "Credits: ";
            cin >> credit;
            cin.ignore();

            float gradePoint = getGradePoint(gradeLetter);
            sem.addSubject(Subject(name, gradePoint, credit));
        }

        saveToFile();
        cout << "Semester " << semNum << " added/updated successfully!\n";
    }

    // ------------------- Edit Semester by Subject -------------------
    void editSemester() {
        int index;
        cout << "Enter semester number to edit: ";
        cin >> index;
        cin.ignore();

        if (index < 1 || index > semesters.size()) {
            cout << "Invalid semester!\n";
            return;
        }

        Semester &sem = semesters[index - 1];

        cout << "\nSubjects in Semester " << index << ":\n";
        for (int i = 0; i < sem.subjects.size(); i++) {
            cout << i + 1 << ". " << sem.subjects[i].name << endl;
        }

        string subName;
        cout << "\nEnter the name of the subject you want to edit: ";
        getline(cin, subName);

        bool found = false;
        for (int i = 0; i < sem.subjects.size(); i++) {
            if (sem.subjects[i].name == subName) {
                found = true;
                string gradeLetter;
                int credit;

                cout << "Enter new grade (O, A+, A, B+, B, C, F): ";
                cin >> gradeLetter;
                cout << "Enter new credits: ";
                cin >> credit;
                cin.ignore();

                float gradePoint = getGradePoint(gradeLetter);
                sem.subjects[i].gradePoint = gradePoint;
                sem.subjects[i].credit = credit;

                cout << "Subject updated successfully!\n";
                break;
            }
        }

        if (!found) {
            cout << "Subject not found in this semester.\n";
        }

        saveToFile();
    }

    // ------------------- Delete Semester -------------------
    void deleteSemester() {
        int index;
        cout << "Enter semester number to delete: ";
        cin >> index;

        if (index < 1 || index > semesters.size()) {
            cout << "Invalid semester!\n";
            return;
        }

        semesters.erase(semesters.begin() + index - 1);
        saveToFile();
        cout << "Semester deleted.\n";
    }

    // ------------------- View Semester SGPA -------------------
    void viewSemester() {
        int semNum;
        cout << "Enter semester number: ";
        cin >> semNum;

        if (semNum < 1 || semNum > semesters.size()) {
            cout << "Invalid semester!\n";
            return;
        }

        Semester &sem = semesters[semNum - 1];
        cout << "\n--- Semester " << semNum << " ---\n";
        for (int i = 0; i < sem.subjects.size(); i++) {
            float gp = sem.subjects[i].gradePoint;
            string gradeLetter;
            if (gp == 10) gradeLetter = "O";
            else if (gp == 9) gradeLetter = "A+";
            else if (gp == 8) gradeLetter = "A";
            else if (gp == 7) gradeLetter = "B+";
            else if (gp == 6) gradeLetter = "B";
            else if (gp == 5) gradeLetter = "C";
            else gradeLetter = "F";

            cout << sem.subjects[i].name << " | Grade: " << gradeLetter << " | Credits: " << sem.subjects[i].credit << endl;
        }

        cout << "SGPA: " << sem.getSGPA() << endl;
    }

    // ------------------- View Year CGPA (2 semesters per year) -------------------
    void viewYear() {
        int year;
        cout << "Enter year number: ";
        cin >> year;

        int start = (year - 1) * 2;
        int end = start + 1;

        if (start >= semesters.size()) {
            cout << "Invalid year!\n";
            return;
        }

        float points = 0;
        int credits = 0;

        cout << "\n--- Year " << year << " ---\n";

        for (int i = start; i <= end && i < semesters.size(); i++) {
            cout << "\nSemester " << i + 1 << endl;
            for (int j = 0; j < semesters[i].subjects.size(); j++) {
                Subject sub = semesters[i].subjects[j];
                float gp = sub.gradePoint;
                string gradeLetter;
                if (gp == 10) gradeLetter = "O";
                else if (gp == 9) gradeLetter = "A+";
                else if (gp == 8) gradeLetter = "A";
                else if (gp == 7) gradeLetter = "B+";
                else if (gp == 6) gradeLetter = "B";
                else if (gp == 5) gradeLetter = "C";
                else gradeLetter = "F";

                cout << sub.name << " | Grade: " << gradeLetter << " | Credits: " << sub.credit << endl;
                points += gp * sub.credit;
                credits += sub.credit;
            }
        }

        cout << "\nYear " << year << " CGPA: " << points / credits << endl;
    }

    // ------------------- Overall CGPA -------------------
    void calculateOverallCGPA() {
        float totalPoints = 0;
        int totalCredits = 0;

        for (int i = 0; i < semesters.size(); i++) {
            totalPoints += semesters[i].getTotalPoints();
            totalCredits += semesters[i].getTotalCredits();
        }

        if (totalCredits == 0) {
            cout << "No data available.\n";
            return;
        }

        cout << "Overall CGPA: " << totalPoints / totalCredits << endl;
    }

    void viewOptions() {
        int choice;
        cout << "\n1. View Semester SGPA\n";
        cout << "2. View Year CGPA\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1)
            viewSemester();
        else if (choice == 2)
            viewYear();
        else
            cout << "Invalid choice!\n";
    }
};

// ------------------- Main Function -------------------
int main() {
    CGPAManager manager;
    manager.loadFromFile();

    int choice;

    do {
        cout << "\n===== CGPA MANAGER =====\n";
        cout << "1. Add/Update Semester\n";
        cout << "2. View SGPA / Year CGPA\n";
        cout << "3. Edit Semester\n";
        cout << "4. Delete Semester\n";
        cout << "5. Overall CGPA\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: manager.addSemester(); break;
            case 2: manager.viewOptions(); break;
            case 3: manager.editSemester(); break;
            case 4: manager.deleteSemester(); break;
            case 5: manager.calculateOverallCGPA(); break;
            case 6: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice!\n";
        }

    } while (choice != 6);

    return 0;
}
