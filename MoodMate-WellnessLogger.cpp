#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <limits>

using namespace std;

// ==========================================
// 1. MoodEntry Base Class
// ==========================================
class MoodEntry {
private:
    string date;
    int rating;
    string note;

public:
    MoodEntry();
    MoodEntry(string d, int r, string n);

    // Getters and Setters
    string getDate() const;
    int getRating() const;
    string getNote() const;

    void setDate(string d);
    void setRating(int r);
    void setNote(string n);

    void printEntry() const;
};

// Implementations using scope resolution operator
MoodEntry::MoodEntry() {
    date = "Unspecified";
    rating = 3;
    note = "No note";
}

MoodEntry::MoodEntry(string d, int r, string n) {
    setDate(d);
    setRating(r);
    setNote(n);
}

string MoodEntry::getDate() const { return date; }
int MoodEntry::getRating() const { return rating; }
string MoodEntry::getNote() const { return note; }

void MoodEntry::setDate(string d) {
    if (d.empty()) {
        date = "Unspecified";
    } else {
        date = d;
    }
}

void MoodEntry::setRating(int r) {
    if (r >= 1 && r <= 5) {
        rating = r;
    } else {
        rating = 3; // Fallback value
    }
}

void MoodEntry::setNote(string n) {
    if (n.empty()) {
        note = "No note";
    } else {
        note = n;
    }
}

void MoodEntry::printEntry() const {
    cout << "Date: " << date << "\nRating: " << rating << "\nNote: " << note << endl;
}


// ==========================================
// 2. WellnessEntry Derived Class
// ==========================================
class WellnessEntry : public MoodEntry {
private:
    double sleepHours;
    int exerciseMinutes;

public:
    WellnessEntry();
    WellnessEntry(string d, int r, string n, double sleep, int exercise);

    // Getters and Setters
    double getSleepHours() const;
    int getExerciseMinutes() const;

    void setSleepHours(double sleep);
    void setExerciseMinutes(int exercise);

    void printEntry() const; // Overrides/extends base function
};

WellnessEntry::WellnessEntry() : MoodEntry() {
    sleepHours = 0.0;
    exerciseMinutes = 0;
}

WellnessEntry::WellnessEntry(string d, int r, string n, double sleep, int exercise) 
    : MoodEntry(d, r, n) {
    setSleepHours(sleep);
    setExerciseMinutes(exercise);
}

double WellnessEntry::getSleepHours() const { return sleepHours; }
int WellnessEntry::getExerciseMinutes() const { return exerciseMinutes; }

void WellnessEntry::setSleepHours(double sleep) {
    if (sleep >= 0.0 && sleep <= 24.0) {
        sleepHours = sleep;
    } else {
        sleepHours = 0.0; // Fallback value
    }
}

void WellnessEntry::setExerciseMinutes(int exercise) {
    if (exercise >= 0 && exercise <= 1440) {
        exerciseMinutes = exercise;
    } else {
        exerciseMinutes = 0; // Fallback value
    }
}

void WellnessEntry::printEntry() const {
    MoodEntry::printEntry(); // Reuse base portion
    cout << "Sleep Hours: " << sleepHours << "\nExercise Minutes: " << exerciseMinutes << endl;
}


// ==========================================
// 3 & 4. WellnessLogger Class (Composition & Persistence)
// ==========================================
class WellnessLogger {
private:
    WellnessEntry entries[20];
    int entryCount;
    const string filename = "moodmate.txt";

public:
    WellnessLogger();

    int getEntryCount() const;
    bool addEntry(const WellnessEntry& entry);
    void displayEntries() const;
    bool updateEntry(int index, double sleep, int exercise, int rating, string date, string note);
    void displayAverageRating() const;
    bool saveToFile() const;
    void loadFromFile();
};

WellnessLogger::WellnessLogger() {
    entryCount = 0;
}

int WellnessLogger::getEntryCount() const {
    return entryCount;
}

bool WellnessLogger::addEntry(const WellnessEntry& entry) {
    if (entryCount >= 20) {
        return false; // Capacity reached
    }
    entries[entryCount] = entry;
    entryCount++;
    return true;
}

void WellnessLogger::displayEntries() const {
    if (entryCount == 0) {
        cout << "The log is currently empty." << endl;
        return;
    }
    for (int i = 0; i < entryCount; i++) {
        cout << "\n--- Entry #" << (i + 1) << " ---" << endl;
        entries[i].printEntry();
    }
}

bool WellnessLogger::updateEntry(int index, double sleep, int exercise, int rating, string date, string note) {
    if (index < 0 || index >= entryCount) {
        return false; // Invalid entry number leaves log unchanged
    }
    entries[index].setDate(date);
    entries[index].setRating(rating);
    entries[index].setNote(note);
    entries[index].setSleepHours(sleep);
    entries[index].setExerciseMinutes(exercise);
    return true;
}

void WellnessLogger::displayAverageRating() const {
    if (entryCount == 0) {
        cout << "Cannot calculate average: Log is empty." << endl;
        return;
    }
    double total = 0;
    for (int i = 0; i < entryCount; i++) {
        total += entries[i].getRating();
    }
    cout << fixed << setprecision(1);
    cout << "Average Mood Rating: " << (total / entryCount) << endl;
}

bool WellnessLogger::saveToFile() const {
    ofstream outFile(filename);
    if (!outFile) {
        return false;
    }
    for (int i = 0; i < entryCount; i++) {
        outFile << entries[i].getDate() << "\n";
        outFile << entries[i].getRating() << "\n";
        outFile << entries[i].getNote() << "\n";
        outFile << entries[i].getSleepHours() << "\n";
        outFile << entries[i].getExerciseMinutes() << "\n";
    }
    return true;
}

void WellnessLogger::loadFromFile() {
    ifstream inFile(filename);
    if (!inFile) {
        cout << "No existing data file found. Starting with an empty log." << endl;
        return;
    }

    entryCount = 0;
    string dateLine, ratingLine, noteLine, sleepLine, exerciseLine;
    int skippedRecords = 0;

    while (entryCount < 20 && getline(inFile, dateLine)) {
        if (!getline(inFile, ratingLine) || 
            !getline(inFile, noteLine) || 
            !getline(inFile, sleepLine) || 
            !getline(inFile, exerciseLine)) {
            cout << "Warning: Detected an incomplete or malformed record at the end of the file. Stopping file read." << endl;
            break;
        }

        try {
            size_t idx;
            int r = stoi(ratingLine, &idx);
            if (idx != ratingLine.length()) throw invalid_argument("Extra chars");

            double s = stod(sleepLine, &idx);
            if (idx != sleepLine.length()) throw invalid_argument("Extra chars");

            int e = stoi(exerciseLine, &idx);
            if (idx != exerciseLine.length()) throw invalid_argument("Extra chars");

            // Reconstruct through constructor to enforce all safety validation
            WellnessEntry newEntry(dateLine, r, noteLine, s, e);
            entries[entryCount] = newEntry;
            entryCount++;
        }
        catch (...) {
            cout << "Warning: Skipped a malformed record containing invalid or nonnumeric data." << endl;
            // Retain valid records already loaded, do not update entryCount for this slot
        }
    }

    if (getline(inFile, dateLine)) {
        cout << "Notice: Maximum capacity of 20 records reached. Some records were not loaded." << endl;
    }
    cout << "Successfully loaded " << entryCount << " records." << endl;
}


// ==========================================
// Helper functions for safe user input
// ==========================================
void clearInputBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}


// ==========================================
// 5. Main Application Menu
// ==========================================
int main() {
    WellnessLogger logger;
    logger.loadFromFile(); // Startup load

    int choice = 0;
    while (choice != 6) {
        cout << "\n===== MoodMate Menu =====" << endl;
        cout << "1. Add wellness entry" << endl;
        cout << "2. View entries" << endl;
        cout << "3. Update an entry" << endl;
        cout << "4. View average mood rating" << endl;
        cout << "5. Save entries" << endl;
        cout << "6. Save and exit" << endl;
        cout << "Enter your choice (1-6): ";

        if (!(cin >> choice)) {
            cout << "Invalid choice. Please enter a number between 1 and 6." << endl;
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();

        switch (choice) {
            case 1: {
                if (logger.getEntryCount() >= 20) {
                    cout << "Error: The log is full (maximum 20 entries)." << endl;
                    break;
                }
                string d, n;
                int r;
                double s;
                int e;

                cout << "Enter date (YYYY-MM-DD): ";
                getline(cin, d);

                cout << "Enter rating (1-5): ";
                while (!(cin >> r)) {
                    cout << "Nonnumeric input. Please enter a whole number: ";
                    clearInputBuffer();
                }
                clearInputBuffer();

                cout << "Enter note: ";
                getline(cin, n);

                cout << "Enter sleep hours (0-24): ";
                while (!(cin >> s)) {
                    cout << "Nonnumeric input. Please enter a number: ";
                    clearInputBuffer();
                }
                clearInputBuffer();

                cout << "Enter exercise minutes (0-1440): ";
                while (!(cin >> e)) {
                    cout << "Nonnumeric input. Please enter a whole number: ";
                    clearInputBuffer();
                }
                clearInputBuffer();

                WellnessEntry entry(d, r, n, s, e);
                logger.addEntry(entry);
                cout << "Entry added successfully!" << endl;
                break;
                }
                case 2:
                logger.displayEntries();
                break;
                case 3: {
                if (logger.getEntryCount() == 0) {
                cout << "The log is empty. Nothing to update." << endl;
                break;
                }
    int entryNum;
        cout << "Enter entry number to update (1-" << logger.getEntryCount() << "): ";
        if (!(cin >> entryNum)) {
        cout << "Invalid entry selection. Log remains unchanged." << endl;
        clearInputBuffer();
        break;
        }
        clearInputBuffer();
    int idx = entryNum - 1;
        if (idx < 0 || idx >= logger.getEntryCount()) {
        cout << "Error: Invalid entry number. Log remains unchanged." << endl;
        break;
        }
        string d, n;
    int r;
    double s;
    int e;
        cout << "Enter new date (YYYY-MM-DD): ";
        getline(cin, d);
        cout << "Enter new rating (1-5): ";
        while (!(cin >> r)) {
        cout << "Nonnumeric input. Please enter a whole number: ";
        clearInputBuffer();
        }
        clearInputBuffer();
        cout << "Enter new note: ";
        getline(cin, n);
        cout << "Enter new sleep hours (0-24): ";
        while (!(cin >> s)) {
        cout << "Nonnumeric input. Please enter a number: ";
        clearInputBuffer();
        }
        clearInputBuffer();
        cout << "Enter new exercise minutes (0-1440): ";
        while (!(cin >> e)) {
        cout << "Nonnumeric input. Please enter a whole number: ";
        clearInputBuffer();
        }
        clearInputBuffer();
        logger.updateEntry(idx, s, e, r, d, n);
        cout << "Entry updated successfully!" << endl;
        break;
        }
        case 4:
        logger.displayAverageRating();
        break;
        case 5:
        if (logger.saveToFile()) {
        cout << "Entries saved successfully to moodmate.txt." << endl;
        } else {
        cout << "Error: Could not open file to save data." << endl;
        }
        break;
        case 6:
    if (logger.saveToFile()) {
        cout << "Entries saved successfully. Goodbye!" << endl;
        } else {
        cout << "Error saving entries before exiting. Application closing." << endl;
        }
        break;
        default:
        cout << "Invalid option. Please choose between 1 and 6." << endl;
        break;
        }
}
return 0;
}
