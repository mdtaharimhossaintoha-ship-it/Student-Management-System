#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 500
#define FILE_NAME "students.txt"

#define ADMIN_USER "toha"
#define ADMIN_PASS "1234"

struct Student {
    int id;
    char name[50];
    char department[30];
    float cgpa;
};

struct Student students[MAX];
int count = 0;


// Function Prototypes
void login();
void menu();
void addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();
void departmentSearch();
void showStatistics();
void loadData();
void saveData();

int readInt(const char *message);
float readFloat(const char *message);
void readString(const char *message, char *str, int size);
int findStudent(int id);


// Input Integer
int readInt(const char *message) {
    char input[100];
    char extra;
    int value;

    while (1) {
        printf("%s", message);

        if (!fgets(input, sizeof(input), stdin))
            continue;

        if (sscanf(input, " %d %c", &value, &extra) == 1)
            return value;

        printf("Invalid input! Enter a valid number.\n");
    }
}


// Input Float
float readFloat(const char *message) {
    char input[100];
    char extra;
    float value;

    while (1) {
        printf("%s", message);

        if (!fgets(input, sizeof(input), stdin))
            continue;

        if (sscanf(input, " %f %c", &value, &extra) == 1)
            return value;

        printf("Invalid input! Enter a valid number.\n");
    }
}


// Input String
void readString(const char *message, char *str, int size) {
    while (1) {
        printf("%s", message);

        if (!fgets(str, size, stdin))
            continue;

        str[strcspn(str, "\n")] = '\0';

        if (str[0] == '\0') {
            printf("Input cannot be empty!\n");
            continue;
        }

        if (strchr(str, '|') != NULL) {
            printf("Special character | is not allowed!\n");
            continue;
        }

        return;
    }
}


// Find Student by ID
int findStudent(int id) {
    for (int i = 0; i < count; i++) {
        if (students[i].id == id)
            return i;
    }

    return -1;
}


// Save Data
void saveData() {
    FILE *fp = fopen(FILE_NAME, "w");

    if (fp == NULL) {
        printf("Error saving data!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d|%s|%s|%.2f\n",
                students[i].id,
                students[i].name,
                students[i].department,
                students[i].cgpa);
    }

    fclose(fp);
}


// Load Data
void loadData() {
    FILE *fp = fopen(FILE_NAME, "r");

    if (fp == NULL)
        return;

    char line[200];

    count = 0;

    while (count < MAX && fgets(line, sizeof(line), fp)) {

        struct Student s;

        if (sscanf(line, "%d|%49[^|]|%29[^|]|%f",
                   &s.id,
                   s.name,
                   s.department,
                   &s.cgpa) == 4) {

            if (s.id > 0 &&
                s.cgpa >= 0 &&
                s.cgpa <= 4 &&
                findStudent(s.id) == -1) {

                students[count] = s;
                count++;
            }
        }
    }

    fclose(fp);
}


// Admin Login
void login() {
    char username[50];
    char password[50];

    int attempts = 3;

    printf("\n====================================\n");
    printf("       STUDENT MANAGEMENT SYSTEM\n");
    printf("====================================\n");

    while (attempts > 0) {

        printf("\nAdmin Login\n");

        readString("Username: ", username, sizeof(username));
        readString("Password: ", password, sizeof(password));

        if (strcmp(username, ADMIN_USER) == 0 &&
            strcmp(password, ADMIN_PASS) == 0) {

            printf("\nLogin Successful!\n");
            return;
        }

        attempts--;

        printf("\nWrong Username or Password!\n");
        printf("Attempts Remaining: %d\n", attempts);
    }

    printf("\nToo many failed attempts. Exiting...\n");
    exit(0);
}


// Add Student
void addStudent() {

    if (count >= MAX) {
        printf("\nStudent limit reached!\n");
        return;
    }

    printf("\n========== ADD NEW STUDENT ==========\n");

    int id = readInt("Enter Student ID: ");

    if (id <= 0) {
        printf("ID must be positive!\n");
        return;
    }

    if (findStudent(id) != -1) {
        printf("Student ID already exists!\n");
        return;
    }

    students[count].id = id;

    readString("Enter Name: ",
               students[count].name, 50);

    readString("Enter Department: ",
               students[count].department, 30);

    float cgpa = readFloat("Enter CGPA: ");

    if (cgpa < 0 || cgpa > 4) {
        printf("CGPA must be between 0 and 4!\n");
        return;
    }

    students[count].cgpa = cgpa;

    count++;

    saveData();

    printf("\nStudent Added Successfully!\n");
}


// Display Students
void displayStudents() {

    if (count == 0) {
        printf("\nNo Student Records Found!\n");
        return;
    }

    printf("\n================ STUDENT RECORDS ================\n");

    printf("%-10s %-20s %-15s %-10s\n",
           "ID", "NAME", "DEPARTMENT", "CGPA");

    printf("-------------------------------------------------\n");

    for (int i = 0; i < count; i++) {

        printf("%-10d %-20s %-15s %-10.2f\n",
               students[i].id,
               students[i].name,
               students[i].department,
               students[i].cgpa);
    }

    printf("-------------------------------------------------\n");
    printf("Total Students: %d\n", count);
}


// Search Student
void searchStudent() {

    int id = readInt("\nEnter Student ID: ");

    int index = findStudent(id);

    if (index == -1) {
        printf("\nStudent Not Found!\n");
        return;
    }

    printf("\n========== STUDENT DETAILS ==========\n");

    printf("ID         : %d\n", students[index].id);
    printf("Name       : %s\n", students[index].name);
    printf("Department : %s\n", students[index].department);
    printf("CGPA       : %.2f\n", students[index].cgpa);
}


// Update Student
void updateStudent() {

    int id = readInt("\nEnter Student ID: ");

    int index = findStudent(id);

    if (index == -1) {
        printf("\nStudent Not Found!\n");
        return;
    }

    printf("\nEnter New Information\n");

    readString("New Name: ",
               students[index].name, 50);

    readString("New Department: ",
               students[index].department, 30);

    float cgpa = readFloat("New CGPA: ");

    if (cgpa < 0 || cgpa > 4) {
        printf("Invalid CGPA! Update cancelled.\n");
        return;
    }

    students[index].cgpa = cgpa;

    saveData();

    printf("\nStudent Updated Successfully!\n");
}


// Delete Student
void deleteStudent() {

    int id = readInt("\nEnter Student ID: ");

    int index = findStudent(id);

    if (index == -1) {
        printf("\nStudent Not Found!\n");
        return;
    }

    char confirm[10];

    readString("Are you sure? (yes/no): ",
               confirm, sizeof(confirm));

    if (strcmp(confirm, "yes") != 0 &&
        strcmp(confirm, "YES") != 0) {

        printf("Delete Cancelled!\n");
        return;
    }

    for (int i = index; i < count - 1; i++) {
        students[i] = students[i + 1];
    }

    count--;

    saveData();

    printf("\nStudent Deleted Successfully!\n");
}


// Department Search
void departmentSearch() {

    char department[30];

    readString("\nEnter Department Name: ",
               department, sizeof(department));

    int found = 0;

    printf("\nStudents in %s Department\n", department);

    for (int i = 0; i < count; i++) {

        if (strcmp(students[i].department, department) == 0) {

            printf("\nID: %d\n", students[i].id);
            printf("Name: %s\n", students[i].name);
            printf("CGPA: %.2f\n", students[i].cgpa);

            found++;
        }
    }

    if (found == 0)
        printf("\nNo Student Found!\n");
    else
        printf("\nTotal Found: %d\n", found);
}


// Statistics
void showStatistics() {

    if (count == 0) {
        printf("\nNo Data Available!\n");
        return;
    }

    float total = 0;
    float highest = students[0].cgpa;
    float lowest = students[0].cgpa;

    int topIndex = 0;

    for (int i = 0; i < count; i++) {

        total += students[i].cgpa;

        if (students[i].cgpa > highest) {
            highest = students[i].cgpa;
            topIndex = i;
        }

        if (students[i].cgpa < lowest) {
            lowest = students[i].cgpa;
        }
    }

    printf("\n========== STUDENT STATISTICS ==========\n");

    printf("Total Students : %d\n", count);

    printf("Average CGPA   : %.2f\n", total / count);

    printf("Highest CGPA   : %.2f\n", highest);

    printf("Lowest CGPA    : %.2f\n", lowest);

    printf("Top Student    : %s\n",
           students[topIndex].name);
}


// Main Menu
void menu() {

    int choice;

    while (1) {

        printf("\n====================================\n");
        printf("       STUDENT MANAGEMENT SYSTEM\n");
        printf("====================================\n");

        printf("1. Add New Student\n");
        printf("2. Display All Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Department-wise Search\n");
        printf("7. Student Statistics\n");
        printf("8. Exit\n");

        printf("====================================\n");

        choice = readInt("Enter Your Choice: ");

        switch (choice) {

            case 1:
                addStudent();
                break;

            case 2:
                displayStudents();
                break;

            case 3:
                searchStudent();
                break;

            case 4:
                updateStudent();
                break;

            case 5:
                deleteStudent();
                break;

            case 6:
                departmentSearch();
                break;

            case 7:
                showStatistics();
                break;

            case 8:
                printf("\nThank You for Using Our System!\n");
                return;

            default:
                printf("\nInvalid Choice! Try Again.\n");
        }
    }
}


// Main Function
int main() {

    loadData();

    login();

    menu();

    return 0;
}
