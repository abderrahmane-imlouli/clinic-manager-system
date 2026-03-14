# Clinic Patient Management System

A simple **C-based patient management system** that uses binary files to store and manage patient records efficiently.

## Features

- Add new patients
- Delete patients
- Modify patient status
- Retrieve patient information by ID
- List patients by status
- Fast access using a secondary index file (`info.dat`) for direct record lookup

## Files

- `ClinicManager.c` – Main C program containing all functionalities.
- `clinic.dat` – Binary file storing all patient records (`struct Patient`).
- `info.dat` – Binary file storing patient IDs and their record numbers for direct access.

## Data Structures

### Patient Record (`struct Patient`)
```c
struct Patient {
    char ID[30];
    char name[30];
    char departement[50];
    int age;
    char sex;     // 'M' or 'F'
    int status;   // 0: Waiting for diagnosis, 1: Diagnosis complete, 2: Waiting for treatment, 3: Under treatment, 4: Treatment complete
};
```
Info Record (struct Info)
```
struct Info {
    char ID[30];
    int record_num;
};
```
Usage

1.Compile the program:
```
gcc ClinicManager.c -o ClinicManager
```
2.Run the program:

./ClinicManager

3.Menu options:

Option	Action
```
1	Retrieve record number of a patient
2	Get patient information by ID
3	Modify patient status
4	Add new patient
5	Delete patient
6	List patients by status
7	Exit
```

Note: First-time users should create empty clinic.dat and info.dat files if they do not exist.

Author

imlouli abderrahmane

License

This project is licensed under the MIT License. See the LICENSE file for details
