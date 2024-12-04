#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


//const char* filename = "C:\\INF1002\\CMS_Folder\\StudentRecords.txt"; 
const char* filename = "/Users/zhiyong/Downloads/StudentRecords.txt";

typedef struct Student {
    int id;
    char* name;
    char* programme;
    float mark;
} Student;

Student* records = NULL;  // Array of student records

int recordCount = 0;
int isDatabaseLoaded = 0;


void openDatabase() {
    // Check if the database is already loaded
    if (isDatabaseLoaded == 1) {
        printf("CMS: Database is already loaded with %d records.\n", recordCount);
        return;
    }

    // Open the file in read mode
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return;
    }

    // Skip header lines to look for valid data
    char buffer[256];
    int foundHeader = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        if (strncmp(buffer, "ID", 2) == 0) {
            foundHeader = 1;
            break;
        }
    }

    if (!foundHeader) {
        printf("Error: Header line starting with 'ID' not found in the file.\n");
        fclose(file);
        return;
    }

    // Clear any previous records before loading new data
    recordCount = 0;

    // Now read the records from the file
    while (fgets(buffer, sizeof(buffer), file)) {
        // Skip separator lines or empty lines
        if (strstr(buffer, "-----") != NULL || buffer[0] == '\n') {
            continue;
        }

        Student temp;
        char nameBuffer[21];       // Fixed width +1 for null terminator
        char programmeBuffer[26];  // Fixed width +1 for null terminator

        // Parse fields using fixed-width format
        int fieldsParsed = sscanf(buffer, "%d %20c %25c %f",
            &temp.id, nameBuffer, programmeBuffer, &temp.mark);

        // Null-terminate the strings and trim trailing newline
        nameBuffer[20] = '\0';
        programmeBuffer[25] = '\0';
        strtok(nameBuffer, "\n");
        strtok(programmeBuffer, "\n");

        // Handle parsing errors
        if (fieldsParsed != 4) {
            printf("Warning: Failed to parse line: %s", buffer);
            continue;
        }

        // Allocate memory for name and programme
        //temp.name = _strdup(nameBuffer);
        //temp.programme = _strdup(programmeBuffer);
        temp.name = strdup(nameBuffer);
        temp.programme = strdup(programmeBuffer);
        if (temp.name == NULL || temp.programme == NULL) {
            printf("Error: Memory allocation failed.\n");
            free(temp.name);        
            free(temp.programme);
            fclose(file);
            return;
        }

        // Add parsed record to the array
        Student* tempRecords = realloc(records, (recordCount + 1) * sizeof(Student));
        if (tempRecords == NULL) {
            printf("Error: Memory reallocation failed.\n");
            free(temp.name);
            free(temp.programme);
            fclose(file);
            return;
        }
        records = tempRecords;
        records[recordCount++] = temp;
    }

    // If no records were loaded, notify the user
    if (recordCount == 0) {
        printf("CMS: The database file \"%s\" was opened but contains no records.\n", filename);
    }
    else {
        printf("CMS: The database file \"%s\" is successfully opened with %d records.\n", filename, recordCount);
    }

    // Mark the database as loaded
    isDatabaseLoaded = 1;

    fclose(file);
}



void showAllRecords() {
    if (recordCount == 0) {
        printf("No records to display.\n");
        return;
    }

    // Print table header with adjusted column widths
    printf("CMS: Here are all the records found in the table \"StudentRecords\":\n");
    printf("+---------+--------------------------+--------------------------------------+-------+\n");
    printf("|   ID    | Name                     | Programme                            | Mark  |\n");
    printf("+---------+--------------------------+--------------------------------------+-------+\n");

    // Print each record with properly aligned columns
    for (int i = 0; i < recordCount; i++) {
        printf("| %-7d | %-24s | %-36s | %5.1f |\n",
            records[i].id,        // Student ID
            records[i].name,      // Name (24-character width)
            records[i].programme, // Programme (36-character width)
            records[i].mark);     // Mark with 1 decimal place
    }

    // Print table footer
    printf("+---------+--------------------------+--------------------------------------+-------+\n");
}
void insertRecord() {
    Student newStudent;
    char nameBuffer[100];
    char programmeBuffer[100];
    char idBuffer[100];
    char markBuffer[100];
    int id;

    // Input validation for ID
    while (1) {
        printf("Enter Student ID (7 digits, or type 'exit' to cancel): ");
        fgets(idBuffer, sizeof(idBuffer), stdin);
        idBuffer[strcspn(idBuffer, "\n")] = '\0'; // Remove newline character

        if (strcmp(idBuffer, "exit") == 0) {
            printf("Insertion process canceled.\n");
            return;
        }

        if (strlen(idBuffer) != 7 || !isdigit(idBuffer[0]) || sscanf(idBuffer, "%d", &id) != 1) {
            printf("Invalid input. Please input a positive 7-digit ID.\n");
            continue;
        }
        newStudent.id = id;
        break;
    }

    // Check for duplicate ID
    for (int i = 0; i < recordCount; i++) {
        if (records[i].id == newStudent.id) {
            printf("CMS: The record with ID=%d already exists.\n", newStudent.id);
            return;
        }
    }

    printf("Enter Student Name: ");
    fgets(nameBuffer, sizeof(nameBuffer), stdin);
    nameBuffer[strcspn(nameBuffer, "\n")] = '\0';

    printf("Enter Programme: ");
    fgets(programmeBuffer, sizeof(programmeBuffer), stdin);
    programmeBuffer[strcspn(programmeBuffer, "\n")] = '\0';

    // Input validation for marks
    while (1) {
        printf("Enter Mark (0-100, or type 'exit' to cancel): ");
        fgets(markBuffer, sizeof(markBuffer), stdin);
        markBuffer[strcspn(markBuffer, "\n")] = '\0';

        if (strcmp(markBuffer, "exit") == 0) {
            printf("Insertion process canceled.\n");
            return;
        }

        float mark;
        char *endptr;
        mark = strtof(markBuffer, &endptr);

        // Check if the input is a valid floating-point number and within the range 0-100
        if (endptr != markBuffer && *endptr == '\0' && mark >= 0 && mark <= 100) {
            newStudent.mark = mark;
            break;
        }
        else {
            printf("Invalid input. Please enter a valid mark between 0 and 100.\n");
        }
    }

    // Allocate memory for name and programme
    newStudent.name = (char*)malloc(strlen(nameBuffer) + 1);
    newStudent.programme = (char*)malloc(strlen(programmeBuffer) + 1);
    if (newStudent.name == NULL || newStudent.programme == NULL) {
        printf("Error: Memory allocation failed.\n");
        return;
    }
    strcpy(newStudent.name, nameBuffer);
    strcpy(newStudent.programme, programmeBuffer);

    // Add the record to the array
    Student* tempRecords = (Student*)realloc(records, (recordCount + 1) * sizeof(Student));
    if (tempRecords == NULL) {
        printf("Error: Memory reallocation failed.\n");
        free(newStudent.name);
        free(newStudent.programme);
        return;
    }
    records = tempRecords;
    records[recordCount] = newStudent;
    recordCount++;

    printf("CMS: A new record with ID=%d is successfully inserted.\n", newStudent.id);
}


void queryRecord() {
    if (recordCount == 0) {
        printf("No records to search.\n");
        return;
    }

    while (1) {
        char idBuffer[100];
        int id;
        printf("Enter Student ID to search (or type 'exit' to cancel): ");
        fgets(idBuffer, sizeof(idBuffer), stdin);
        idBuffer[strcspn(idBuffer, "\n")] = '\0'; // Remove newline character

        // Check if the user wants to exit
        if (strcmp(idBuffer, "exit") == 0) {
            printf("Search process canceled.\n");
            return;
        }

        // Validate ID input
        if (sscanf(idBuffer, "%d", &id) != 1 || id < 1000000 || id > 9999999) {
            printf("Invalid input. Please enter a valid 7-digit ID.\n");
            continue; // Prompt the user again
        }

        // Search for the ID
        for (int i = 0; i < recordCount; i++) {
            if (records[i].id == id) {
                // Adjust the table widths for perfect alignment
                printf("+---------+--------------------------+-----------------------------+-------+\n");
                printf("|   ID    | Name                     | Programme                   | Mark  |\n");
                printf("+---------+--------------------------+-----------------------------+-------+\n");
                printf("| %-7d | %-24s | %-27s | %5.1f |\n",
                    records[i].id,        // ID
                    records[i].name,      // Name (24-character width)
                    records[i].programme, // Programme (27-character width)
                    records[i].mark);     // Mark
                printf("+---------+--------------------------+-----------------------------+-------+\n");
                return; // Exit the function after displaying the record
            }
        }

        // If no record is found
        printf("No student record found with ID=%d.\n", id);
    }
}

void updateRecord() {
    if (recordCount == 0) {
        printf("No records to update.\n");
        return;
    }

    while (1) {
        char idBuffer[100];
        int id;
        printf("Enter Student ID to update (or type 'exit' to cancel): ");
        fgets(idBuffer, sizeof(idBuffer), stdin);
        idBuffer[strcspn(idBuffer, "\n")] = '\0'; // Remove newline character

        // Check if the user wants to exit
        if (strcmp(idBuffer, "exit") == 0) {
            printf("Update process canceled.\n");
            return;
        }

        // Validate ID input
        if (sscanf(idBuffer, "%d", &id) != 1 || id <= 0 || id < 1000000 || id > 9999999 || strlen(idBuffer) != 7) {
            printf("Invalid input. Please enter a valid 7-digit ID.\n");
            continue; // Prompt the user again
        }

        // Check if the ID exists
        for (int i = 0; i < recordCount; i++) {
            if (records[i].id == id) {
                printf("Updating record for ID=%d:\n", id);

                // Prompt for new name
                printf("Enter new name (or press Enter to keep current: %s): ", records[i].name);
                char nameBuffer[100];
                fgets(nameBuffer, sizeof(nameBuffer), stdin);
                if (nameBuffer[0] != '\n') {
                    free(records[i].name);
                    nameBuffer[strcspn(nameBuffer, "\n")] = '\0';
                    records[i].name = (char*)malloc(strlen(nameBuffer) + 1);
                    strcpy(records[i].name, nameBuffer);
                }

                // Prompt for new programme
                printf("Enter new programme (or press Enter to keep current: %s): ", records[i].programme);
                char programmeBuffer[100];
                fgets(programmeBuffer, sizeof(programmeBuffer), stdin);
                if (programmeBuffer[0] != '\n') {
                    free(records[i].programme);
                    programmeBuffer[strcspn(programmeBuffer, "\n")] = '\0';
                    records[i].programme = (char*)malloc(strlen(programmeBuffer) + 1);
                    strcpy(records[i].programme, programmeBuffer);
                }

                // Prompt for new mark
                printf("Enter new mark (or press Enter to keep current: %.1f): ", records[i].mark);
                char markBuffer[100];
                fgets(markBuffer, sizeof(markBuffer), stdin);
                if (markBuffer[0] != '\n') {
                    float newMark;
                    if (sscanf(markBuffer, "%f", &newMark) == 1 && newMark >= 0 && newMark <= 100) {
                        records[i].mark = newMark;
                    }
                    else {
                        printf("Invalid mark. Keeping previous value.\n");
                    }
                }

                printf("CMS: Record for ID=%d successfully updated.\n", id);
                return; // Exit the function after updating
            }
        }

        // If the loop completes without finding a match
        printf("No student record found with ID=%d.\n", id);
    }
}

void deleteRecord() {
    // Check if there are any records in the database
    if (recordCount == 0) {
        printf("There are no records to delete.\n");
        return;
    }

    while (1) {
        char idBuffer[100];
        int id;
        int recordFound = 0;

        // Input validation for ID
        printf("Enter Student ID to delete (or type 'exit' to cancel): ");
        fgets(idBuffer, sizeof(idBuffer), stdin);
        idBuffer[strcspn(idBuffer, "\n")] = '\0'; // Remove newline character

        // Check if the user wants to exit
        if (strcmp(idBuffer, "exit") == 0) {
            printf("Deletion process cancelled.\n");
            return;
        }

        // Validate ID input
        if (sscanf(idBuffer, "%d", &id) != 1 || id <= 0 || id < 1000000 || id > 9999999 || strlen(idBuffer) != 7) {
            printf("Invalid input. Please enter a valid 7-digit ID.\n");
            continue; // Prompt the user again
        }

        // Search for the ID
        for (int i = 0; i < recordCount; i++) {
            if (records[i].id == id) {
                recordFound = 1;

                // Confirmation loop
                while (1) {
                    printf("CMS: Are you sure you want to delete record with ID=%d? Type 'Y' to confirm or 'N' to cancel.\n", id);
                    fgets(idBuffer, sizeof(idBuffer), stdin);
                    char cfm_choice = toupper(idBuffer[0]);

                    if (cfm_choice == 'Y') {
                        // Free the memory for the record being deleted
                        free(records[i].name);
                        free(records[i].programme);

                        // Shift records to remove the deleted one
                        for (int j = i; j < recordCount - 1; j++) {
                            records[j] = records[j + 1];
                        }
                        recordCount--;

                        // Reallocate memory 
                        Student* tempRecords = (Student*)realloc(records, recordCount * sizeof(Student));
                        if (tempRecords != NULL || recordCount == 0) {
                            records = tempRecords;
                        }

                        else {
                            printf("Error: Memory reallocation Failed.\n");
                        }

                        printf("CMS: The record with ID=%d is successfully deleted.\n", id);
                        return;
                    }
                    else if (cfm_choice == 'N') {
                        printf("CMS: The deletion is cancelled.\n");
                        return;
                    }
                    else {
                        printf("Invalid input. Please enter 'Y' to confirm or 'N' to cancel.\n");
                    }
                }
            }
        }

        if (!recordFound) {
            printf("CMS: The record with ID=%d does not exist.\n", id);
        }
    }
}

void saveDatabase() {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error: Could not open file %s for writing.\n", filename);
        return;
    }

    // Write the headers in the correct format
    fprintf(file, "Database Name: P13_3-CMS\n");
    fprintf(file, "Authors: Li Shen, Zhi Yong, Yong Ying, Wei Seng, Vanessa\n\n");
    fprintf(file, "Table Name: StudentRecords\n\n");
    fprintf(file, "%-8s %-20s %-25s %s\n", "ID", "Name", "Programme", "Mark");

    // Write each record with proper formatting
    for (int i = 0; i < recordCount; i++) {
        fprintf(file, "%-8d %-20.20s %-25.25s %.1f\n",
            records[i].id,          // Student ID
            records[i].name,        // Full Name (truncated to 20 characters)
            records[i].programme,   // Full Programme (truncated to 25 characters)
            records[i].mark);       // Mark with one decimal place
    }

    printf("CMS: Database saved to %s successfully with %d records.\n", filename, recordCount);
    fclose(file);
}
// Free all allocated memory before exiting
void freeRecords() {
    for (int i = 0; i < recordCount; i++) {
        free(records[i].name);
        free(records[i].programme);
    }
    free(records);
}

// Exit program with cleanup
void exitProgram() {
    printf("Exiting program.\n");
    saveDatabase();
    freeRecords();
    exit(0);
}


int main() {
    int choice;
    int databaseOpened = 0;
    char inputBuffer[100]; // Buffer for user input

    // Function pointers for menu actions
    void (*menuFunctions[])(void) = {
        openDatabase,
        showAllRecords,
        insertRecord,
        queryRecord,
        updateRecord,
        deleteRecord,
        saveDatabase,
        exitProgram
    };

    int menuSize = sizeof(menuFunctions) / sizeof(menuFunctions[0]);

    while (1) {
        // Display menu
        printf("\n--- Menu ---\n");
        printf("1. Open Database\n");
        printf("2. Show All Records\n");
        printf("3. Insert Record\n");
        printf("4. Query Record\n");
        printf("5. Update Record\n");
        printf("6. Delete Record\n");
        printf("7. Save\n");
        printf("8. Exit\n");
        printf("Enter your choice (P13_3): ");

        // Read user input
        if (fgets(inputBuffer, sizeof(inputBuffer), stdin) == NULL) {
            printf("Error: Input could not be read.\n");
            continue;
        }

        // Remove trailing newline character
        inputBuffer[strcspn(inputBuffer, "\n")] = '\0';

        // Validate and convert input to an integer
        if (strlen(inputBuffer) == 1 && isdigit(inputBuffer[0])) {
            choice = inputBuffer[0] - '0'; // Convert character to integer
        }
        else {
            printf("Error: Invalid input. Please enter a number from 1 to %d.\n", menuSize);
            continue;
        }

        // Handle menu options
        if (choice == 1) {
            openDatabase();
            databaseOpened = 1;
        }
        else if (choice == 8) {
            exitProgram();
        }
        else if (choice >= 2 && choice <= 7) {
            if (!databaseOpened) {
                printf("Please open the database first before performing any other actions.\n");
            }
            else {
                menuFunctions[choice - 1]();
            }
        }
        else {
            printf("Invalid choice. Please enter a number from 1 to %d.\n", menuSize);
        }
    }

    return 0;
}
