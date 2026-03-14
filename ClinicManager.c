#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Patient {
    char ID[30];
    char name[30];
    char departement[50];
    int age;
    char sex;
    int status; // 0: Waiting for diagnosis, 1: Diagnosis complete, 2: Waiting for treatment, 3: Under treatment, 4: Treatment complete
};

struct Info {
    char ID[30];
    int record_num;
};

// Function to retrieve the record number of a patient
int getRecordNumber(const char* filename, const char* patientID) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file.\n");
        return -1;
    }

    struct Info info;
    while (fread(&info, sizeof(struct Info), 1, file) == 1) {
        if (strcmp(info.ID, patientID) == 0) {
            fclose(file);
            return info.record_num;
        }
    }

    fclose(file);
    return -1; // Patient not found
}

// Function to retrieve patient information from clinic.dat
void getPatientInfo(const char* filename, int record_num) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    struct Patient patient;
    fseek(file, (record_num - 1) * sizeof(struct Patient), SEEK_SET);
    if (fread(&patient, sizeof(struct Patient), 1, file) == 1) {
        printf("ID: %s\n", patient.ID);
        printf("Name: %s\n", patient.name);
        printf("Department: %s\n", patient.departement);
        printf("Age: %d\n", patient.age);
        printf("Sex: %c\n", patient.sex);
        printf("Status: %d\n", patient.status);
    } else {
        printf("Patient not found.\n");
    }

    fclose(file);
}

// Function to modify a patient's status
void modifyPatientStatus(const char* filename, int record_num, int newStatus) {
    FILE *file = fopen(filename, "rb+");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    struct Patient patient;
    fseek(file, (record_num - 1) * sizeof(struct Patient), SEEK_SET);
    if (fread(&patient, sizeof(struct Patient), 1, file) == 1) {
        patient.status = newStatus;
        fseek(file, (record_num - 1) * sizeof(struct Patient), SEEK_SET);
        fwrite(&patient, sizeof(struct Patient), 1, file);
        printf("Patient's status updated successfully.\n");
    } else {
        printf("Patient not found.\n");
    }

    fclose(file);
}

// Function to add a new patient
void addPatient(const char* filename, struct Patient newPatient) {
    FILE *file = fopen(filename, "ab");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    fwrite(&newPatient, sizeof(struct Patient), 1, file);
    fclose(file);
    printf("New patient added successfully.\n");
}

// Function to delete a patient record
void deletePatient(const char* clinicFile, const char* infoFile, const char* patientID) {
    FILE *clinic = fopen(clinicFile, "rb");
    FILE *info = fopen(infoFile, "rb");
    FILE *tempClinic = fopen("temp_clinic.dat", "wb");
    FILE *tempInfo = fopen("temp_info.dat", "wb");

    if (!clinic || !info || !tempClinic || !tempInfo) {
        printf("Error opening file.\n");
        return;
    }

    struct Patient patient;
    struct Info infoRecord;
    int found = 0;

    // Copy records to temporary files, excluding the patient to be deleted
    while (fread(&patient, sizeof(struct Patient), 1, clinic) == 1) {
        if (strcmp(patient.ID, patientID) != 0) {
            fwrite(&patient, sizeof(struct Patient), 1, tempClinic);
        } else {
            found = 1;
        }
    }

    while (fread(&infoRecord, sizeof(struct Info), 1, info) == 1) {
        if (strcmp(infoRecord.ID, patientID) != 0) {
            fwrite(&infoRecord, sizeof(struct Info), 1, tempInfo);
        }
    }

    fclose(clinic);
    fclose(info);
    fclose(tempClinic);
    fclose(tempInfo);

    // Replace original files with temporary files
    remove(clinicFile);
    rename("temp_clinic.dat", clinicFile);
    remove(infoFile);
    rename("temp_info.dat", infoFile);

    if (found) {
        printf("Patient record deleted successfully.\n");
    } else {
        printf("Patient not found.\n");
    }
}

// Function to list patients by status
void listPatientsByStatus(const char* filename, int status) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    struct Patient patient;
    printf("Patients with status %d:\n", status);
    while (fread(&patient, sizeof(struct Patient), 1, file) == 1) {
        if (patient.status == status) {
            printf("ID: %s, Name: %s, Department: %s, Age: %d, Sex: %c\n",
                   patient.ID, patient.name, patient.departement, patient.age, patient.sex);
        }
    }

    fclose(file);
}

int main() {
    char infoFile[] = "info.dat";
    char clinicFile[] = "clinic.dat";
    char patientID[30];
    int record_num, newStatus, choice;
    struct Patient newPatient;

    while (1) {
        printf("\nMenu:\n");
        printf("1. Retrieve record number\n");
        printf("2. Get patient info\n");
        printf("3. Modify patient status\n");
        printf("4. Add new patient\n");
        printf("5. Delete patient\n");
        printf("6. List patients by status\n");
        printf("7. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter patient ID: ");
                scanf("%s", patientID);
                record_num = getRecordNumber(infoFile, patientID);
                if (record_num == -1) {
                    printf("Patient not found in info.dat.\n");
                } else {
                    printf("Record number: %d\n", record_num);
                }
                break;

            case 2:
                printf("Enter patient ID: ");
                scanf("%s", patientID);
                record_num = getRecordNumber(infoFile, patientID);
                if (record_num != -1) {
                    getPatientInfo(clinicFile, record_num);
                } else {
                    printf("Patient not found in info.dat.\n");
                }
                break;

            case 3:
                printf("Enter patient ID: ");
                scanf("%s", patientID);
                record_num = getRecordNumber(infoFile, patientID);
                if (record_num != -1) {
                    printf("Enter new status for patient: ");
                    scanf("%d", &newStatus);
                    modifyPatientStatus(clinicFile, record_num, newStatus);
                } else {
                    printf("Patient not found in info.dat.\n");
                }
                break;

            case 4:
                printf("Enter patient ID: ");
                scanf("%s", newPatient.ID);
                printf("Enter name: ");
                scanf("%s", newPatient.name);
                printf("Enter department: ");
                scanf("%s", newPatient.departement);
                printf("Enter age: ");
                scanf("%d", &newPatient.age);
                printf("Enter sex (M/F): ");
                scanf(" %c", &newPatient.sex);
                printf("Enter status: ");
                scanf("%d", &newPatient.status);
                addPatient(infoFile, newPatient);
                break;

            case 5:
                printf("Enter patient ID to delete: ");
                scanf("%s", patientID);
                deletePatient(clinicFile, infoFile, patientID);
                break;

            case 6:
                printf("Enter status to list patients: ");
                scanf("%d", &newStatus);
                listPatientsByStatus(clinicFile, newStatus);
                break;

            case 7:
                exit(0);

            default:
                printf("Invalid option. Please try again.\n");
        }
    }

    return 0;
}