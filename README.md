# 🏥 Hospital Management System (C++ Console App)

A simple console-based Hospital Management System written in C++. This system manages patients across multiple medical specializations, handling both urgent and regular cases.

---

## 🚀 Features

- Add new patients with specialization and urgency status
- Automatically handles patient arrival time
- Maintains separate queues for urgent and regular cases
- Doctors are assigned the next patient based on urgency and arrival time
- Displays real-time statistics about the queue state
- User-friendly terminal interface with input validation

---

## 🛠️ How It Works

- Each specialization (1 to 20) has its own queue
- Each queue can hold up to 5 patients
- Urgent patients are prioritized over regular ones
- Patients are timestamped upon addition for sorting and fairness

---

## 📂 Files

- `main.cpp`: The complete source code for the system.
- `.gitignore`: Lists files and directories to be excluded from version control.
- `README.md`: This documentation file.

---

## 📊 Sample Output

```
========================================
   HOSPITAL MANAGEMENT SYSTEM v2.0
========================================

Main Menu:
1) Add new patient
2) Print all patients
3) Get next patient
4) View statistics
5) Clear screen
6) Exit
Enter your choice (1-6):
```

---

## 📌 Requirements

- A C++17 compatible compiler (`g++`, `clang++`, etc.)
- Works on Windows, macOS, and Linux

---

## 📄 License

This project is open-source and available for public use, modification, and distribution. It is provided as-is, without any warranty.
