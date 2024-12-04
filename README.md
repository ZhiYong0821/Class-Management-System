# Student Records Management System 

This project implements a Command-Line Student Records Management System in C. It allows users to manage student data, including adding, viewing, updating, and deleting records stored in a text file.

# Features

	1.	Load Database: Read student data from a file.
	2.	Display Records: View all student records in a tabular format.
	3.	Insert New Record: Add a new student to the database.
	4.	Query Record: Search for a specific student by ID.
	5.	Update Record: Modify existing student details.
	6.	Delete Record: Remove a student record by ID.
	7.	Save Database: Save all changes back to the file.
	8.	Declaration Compliance: Ensures the team complies with plagiarism and privacy policies.

# Tech Stack

	•	Programming Language: C
	•	Standard Libraries:
	•	<stdio.h>: For input and output.
	•	<stdlib.h>: For dynamic memory management.
	•	<string.h>: For string operations.
	•	<ctype.h>: For character type checking.
	•	Dynamic Memory Management: Efficiently allocates and deallocates memory for student records using malloc, free, and realloc.

# Prerequisites

	•	Compiler: GCC, Clang, or another C compiler.
	•	Operating System: Compatible with Windows, macOS, and Linux.
	•	File: A text file named StudentRecords.txt with the required format must exist in the specified path.

# Setup Instructions

	1.	Clone or download the project.
	2.	Update the filename variable in the source code to match your file path for StudentRecords.txt.
	3.	Compile the code:
    ```
    gcc -o StudentRecordsManagement FINAL(0320).c
    ```
	4.	Run the program:
    ```
    ./StudentRecordsManagement
    ```

# File Format

The StudentRecords.txt file should follow this format:
```
Database Name: P13_3-CMS
Authors: [Your team names]

Table Name: StudentRecords

ID        Name                Programme           Mark
1234567   John Doe            Computer Science    85.0
...
```

# Usage

Main Menu Options:

	1.	Open Database: Load student records from the file.
	2.	Show All Records: View all records in a formatted table.
	3.	Insert Record: Add a new student (with validation for ID and marks).
	4.	Query Record: Search for a student by ID.
	5.	Update Record: Edit details of a student record.
	6.	Delete Record: Remove a student record by ID.
	7.	Save: Save changes to the database file.
	8.	Exit: Save and exit, with cleanup of allocated memory.

# Future Improvements

	•	Add sorting and filtering options for records.
	•	Implement a graphical user interface (GUI).
	•	Encrypt sensitive data before saving it to the file.