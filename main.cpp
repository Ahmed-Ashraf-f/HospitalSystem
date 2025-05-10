#include <iostream>
#include <queue>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <limits>
#include <algorithm>
#include <iomanip>

using namespace std;

class Patient {
private:
    string name;
    bool isUrgent;
    time_t arrivalTime;

public:
    Patient(const string &name, bool isUrgent)
            : name(name), isUrgent(isUrgent), arrivalTime(time(nullptr)) {}

    string getName() const { return name; }
    bool isUrgentCase() const { return isUrgent; }
    time_t getArrivalTime() const { return arrivalTime; }

    string getFormattedArrivalTime() const {
        char buffer[20];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&arrivalTime));
        return string(buffer);
    }
};

class PatientQueue {
private:
    queue<Patient> regularQueue;
    queue<Patient> urgentQueue;
    const int maxSize;
    int specializationId;

public:
    PatientQueue(int specializationId, int maxSize = 5)
            : specializationId(specializationId), maxSize(maxSize) {}

    bool addPatient(const string &name, bool isUrgent) {
        if (getTotalSize() >= maxSize) {
            cout << "Sorry, we can't add more patients for specialization " << specializationId << ".\n\n";
            return false;
        }

        if (isUrgent)
            urgentQueue.emplace(name, true);
        else
            regularQueue.emplace(name, false);

        return true;
    }

    Patient nextPatient() {
        if (!urgentQueue.empty()) {
            Patient patient = urgentQueue.front();
            urgentQueue.pop();
            return patient;
        } else if (!regularQueue.empty()) {
            Patient patient = regularQueue.front();
            regularQueue.pop();
            return patient;
        }
        throw runtime_error("No patients in queue");
    }

    int getTotalSize() const {
        return urgentQueue.size() + regularQueue.size();
    }

    int getUrgentSize() const { return urgentQueue.size(); }
    int getRegularSize() const { return regularQueue.size(); }
    int getSpecializationId() const { return specializationId; }

    void printQueue() const {
        vector<Patient> urgentPatients;
        vector<Patient> regularPatients;

        // Copy urgent patients
        queue<Patient> tempUrgent = urgentQueue;
        while (!tempUrgent.empty()) {
            urgentPatients.push_back(tempUrgent.front());
            tempUrgent.pop();
        }

        // Copy regular patients
        queue<Patient> tempRegular = regularQueue;
        while (!tempRegular.empty()) {
            regularPatients.push_back(tempRegular.front());
            tempRegular.pop();
        }

        // Sort by arrival time (oldest first)
        auto sortByArrival = [](const Patient &a, const Patient &b) {
            return a.getArrivalTime() < b.getArrivalTime();
        };

        sort(urgentPatients.begin(), urgentPatients.end(), sortByArrival);
        sort(regularPatients.begin(), regularPatients.end(), sortByArrival);

        // Print urgent patients first
        for (const auto &patient : urgentPatients) {
            cout << left << setw(20) << patient.getName()
                 << " (Urgent, Arrived: " << patient.getFormattedArrivalTime() << ")\n";
        }

        // Then regular patients
        for (const auto &patient : regularPatients) {
            cout << left << setw(20) << patient.getName()
                 << " (Regular, Arrived: " << patient.getFormattedArrivalTime() << ")\n";
        }
    }

    bool isEmpty() const {
        return urgentQueue.empty() && regularQueue.empty();
    }
};

class HospitalSystem {
private:
    vector<PatientQueue> specializations;
    const int maxQueueSize;

    void clearInputBuffer() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    bool isValidSpecialization(int spec) const {
        return spec >= 1 && spec < static_cast<int>(specializations.size());
    }

public:
    HospitalSystem(int numSpecializations = 20, int maxQueueSize = 5)
            : specializations(), maxQueueSize(maxQueueSize) {
        // Initialize with empty queues for each specialization (1-based index)
        specializations.reserve(numSpecializations + 1);
        specializations.emplace_back(0, maxQueueSize); // Dummy for index 0
        for (int i = 1; i <= numSpecializations; ++i) {
            specializations.emplace_back(i, maxQueueSize);
        }
    }

    void addPatient() {
        int specialization;
        string name;
        int status;

        cout << "Enter specialization (1-" << specializations.size()-1 << "): ";
        while (!(cin >> specialization) || !isValidSpecialization(specialization)) {
            cout << "Invalid input. Please enter a number between 1 and "
                 << specializations.size()-1 << ": ";
            clearInputBuffer();
        }

        cout << "Enter patient name: ";
        clearInputBuffer();
        getline(cin, name);

        // Validate name (basic check)
        if (name.empty() || all_of(name.begin(), name.end(), [](char c) { return isspace(c); })) {
            cout << "Invalid name. Please enter a valid name.\n\n";
            return;
        }

        cout << "Enter status (0 for regular, 1 for urgent): ";
        while (!(cin >> status) || (status != 0 && status != 1)) {
            cout << "Invalid input. Please enter 0 for regular or 1 for urgent: ";
            clearInputBuffer();
        }

        specializations[specialization].addPatient(name, status == 1);
        cout << "Patient added successfully.\n\n";
    }

    void getNextPatient() {
        int specialization;

        cout << "Enter specialization (1-" << specializations.size()-1 << "): ";
        while (!(cin >> specialization) || !isValidSpecialization(specialization)) {
            cout << "Invalid input. Please enter a number between 1 and "
                 << specializations.size()-1 << ": ";
            clearInputBuffer();
        }

        try {
            Patient patient = specializations[specialization].nextPatient();
            cout << "\n" << patient.getName() << ", please go with the Doctor.\n";
            cout << "(" << (patient.isUrgentCase() ? "Urgent" : "Regular")
                 << " case, arrived at " << patient.getFormattedArrivalTime() << ")\n\n";
        } catch (const runtime_error &e) {
            cout << "No patients in specialization " << specialization
                 << " at the moment. Have rest, Doctor.\n\n";
        }
    }

    void printAllPatients() const {
        bool anyPatients = false;

        for (int i = 1; i < specializations.size(); ++i) {
            if (!specializations[i].isEmpty()) {
                anyPatients = true;
                cout << "Specialization " << i << " ("
                     << specializations[i].getTotalSize() << " patients):\n";
                cout << "----------------------------------------\n";
                specializations[i].printQueue();
                cout << "----------------------------------------\n\n";
            }
        }

        if (!anyPatients) {
            cout << "No patients in any specialization at the moment.\n\n";
        }
    }

    void printStatistics() const {
        cout << "\nHospital Statistics:\n";
        cout << "========================================\n";
        cout << left << setw(15) << "Specialization"
             << setw(10) << "Urgent"
             << setw(10) << "Regular"
             << setw(10) << "Total"
             << "Status\n";
        cout << "----------------------------------------\n";

        for (int i = 1; i < specializations.size(); ++i) {
            int urgent = specializations[i].getUrgentSize();
            int regular = specializations[i].getRegularSize();
            int total = urgent + regular;

            cout << left << setw(15) << i
                 << setw(10) << urgent
                 << setw(10) << regular
                 << setw(10) << total;

            if (total == 0) {
                cout << "Empty";
            } else if (total >= maxQueueSize) {
                cout << "Full";
            } else if (total >= maxQueueSize * 0.8) {
                cout << "Busy";
            } else {
                cout << "Available";
            }

            cout << "\n";
        }
        cout << "========================================\n\n";
    }
};

class Menu {
public:
    static void displayWelcome() {
        cout << "========================================\n";
        cout << "   HOSPITAL MANAGEMENT SYSTEM v2.0\n";
        cout << "========================================\n\n";
    }

    static int getChoice() {
        int choice;
        cout << "\nMain Menu:\n"
             << "1) Add new patient\n"
             << "2) Print all patients\n"
             << "3) Get next patient\n"
             << "4) View statistics\n"
             << "5) Clear screen\n"
             << "6) Exit\n"
             << "Enter your choice (1-6): ";

        while (!(cin >> choice) || choice < 1 || choice > 6) {
            cout << "Invalid input. Please enter a number between 1 and 6: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << endl;
        return choice;
    }

    static void clearScreen() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif
    }
};

int main() {
    Menu::displayWelcome();
    HospitalSystem hospital;

    while (true) {
        int choice = Menu::getChoice();
        switch (choice) {
            case 1:hospital.addPatient();break;
            case 2:hospital.printAllPatients();break;
            case 3:hospital.getNextPatient();break;
            case 4:hospital.printStatistics();break;
            case 5:Menu::clearScreen();Menu::displayWelcome();break;
            case 6:
                cout << "Exiting program...\n";
                this_thread::sleep_for(chrono::seconds(2));
                return 0;
        }
    }
}