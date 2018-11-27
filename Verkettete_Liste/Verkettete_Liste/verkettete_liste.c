/*
PRP2-3 Aufgabe 3.1
Name: Malte Müller, Fabian Liebold
Date: 19.11.2018
*/

#define _CRT_SECURE_NO_WARNINGS	//?

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

 typedef struct exam {		// define exam_t
	char course[5];
	int points;
	char date[8];
	char prof[15];
	struct exam *pnext_exam;
} exam_t;


typedef struct student {	// define student_t
	char first_name[25];
	char last_name[25];
	int mat_num;
	exam_t *pfirst_exam;
} student_t;

// Prototypes:
void flush_stdin();
void create_student(student_t *pstudent);
void enter_exam_data(exam_t *pexam);
void add_first_exam(student_t *pstudent, exam_t *pfirst_exam);
void add_exam(student_t *pstudent, exam_t *pexam);
void print_exam(student_t *student);
void free_all(student_t *student);
void search_course(student_t *pstudent);
void search_points(student_t *pstudent);
void save_list(student_t *student);
void load_list(student_t *student);

int main(void) {
	
	char loop_flag = ' ';	// flags for if-else-routine
	char read_flag;
	char exam_input_flag;
	char save_flag = ' ';

	printf("Wollen Sie Daten aus der Datei liste.dat lesen? [J/N]: ");
	scanf_s(" %c", &read_flag);
	flush_stdin();

	student_t student;

	if (read_flag == 'j') {	// If User wants to read in data by file
		load_list(&student);
		//printf("[DEBUG] LOADED STUDENT: Name: %s Nachname: %s Mat-Num: %d *pfirst_exam: %x\n", student.first_name, student.last_name, student.mat_num, student.pfirst_exam);
	}
	else if (read_flag == 'n') {	// If User wants to type in data
			
		create_student(&student);	// Create student

		//printf("[DEBUG] Name: %s Nachname: %s Matrikelnummer: %d\n", student.first_name, student.last_name, student.mat_num);	//[DEBUG]

		printf("Wollen Sie Pruefungen eingeben? [J/N]: ");	// Ask User to input exam
		scanf_s(" %c", &exam_input_flag);
		flush_stdin();

		if (exam_input_flag == 'j') {	// If user wants to input exam
			do {
				exam_t *pexam = NULL;
				pexam = (struct exam*)malloc(sizeof(struct exam));	// Allocate memory for exam-struct

				if (pexam == NULL) {	// Free memory and raise error if allocation failed
					free(pexam);
					printf("Allocation failed!\n");
				}
					
				//printf("[DEBUG] Erstelltes pexam (malloc): %x\n", pexam);

				if (student.pfirst_exam == NULL) {		// Check if the first exam is available
					add_first_exam(&student, pexam);	// Add fist exam struct (its pointer to student struct)
					}
				else {	// If it's not the first exam
					add_exam(&student, pexam);	// Add exam to list (its pointer to latest exam struct)
				} 
					
				//printf("[DEBUG]  Gerade erstellt: Kurs: %s Punkte: %d Datum: %s Prof: %s Next: %x  (sollte 0 sein)\n", pexam->course, pexam->points, pexam->date, pexam->prof, pexam->pnext_exam);	//[DEBUG]

				printf("Wollen Sie weitere Pruefungen eingeben? [J/N]: ");	// Ask User to input another exam
				scanf_s(" %c", &exam_input_flag);
				flush_stdin();
			} while (exam_input_flag == 'j');	// Exit loop, if user don't want to add another exam 
		}
		else if (read_flag == 'n') {	// If no exam should be added
			
		}
		else printf("Ungueltige Eingabe!\n");
	}
	else printf("Ungueltige Eingabe!\n");
		
	

	print_exam(&student);	// Print list of exams
	search_course(&student);	// Search exam by course-name
	search_points(&student);	// Searce exam by points
	
	printf("Wollen Sie die Elemente speichern? ");	// Ask User to save data
	scanf_s(" %c", &save_flag);

	if (save_flag == 'j') {	// If User wants to save data
		save_list(&student);
	}

	free_all(&student);	// Free memory [ERROR][TODO][PRIO]

	system("PAUSE");
	return 0;
}

/**************************************************************************************************
Function flushes input stream.
Parameters:
	void.
Returns:
	void.
*/
void flush_stdin() {
	int flush_dummy;
	while ((flush_dummy = getchar()) != '\n' && flush_dummy != EOF);
}
/**************************************************************************************************
Function creates a student struct
Parameters:
	student_t *pstundent - student struct pointer
Returns:
	void.
*/
void create_student(student_t *pstudent) {

	printf("Geben Sie einen Studenten ein:\n");
	printf("Nachname: ");
	gets(pstudent->last_name);	// User types in last name
	//flush_stdin();
	printf("Name: ");
	gets(pstudent->first_name);	// User types in first name
	//flush_stdin();
	printf("Matrikelnummer: ");
	scanf_s(" %d", &(pstudent->mat_num));	// User types in mat-number
	//flush_stdin();

	pstudent->pfirst_exam = NULL;	// Pointer to first exam is set to NULL
}

/**************************************************************************************************	
Enter data for exam struct
Parameters:
	exam_t *pexam - exam struct pointer.
Returns:
	void.
*/
void enter_exam_data(exam_t *pexam) {
	
	printf("Eingeben einer Prüfung:\n");
	printf("Name der Veranstaltung: ");
	gets(pexam->course);				// User types in course name
	//flush_stdin();
	printf("Punktzahl: ");
	scanf_s(" %d", &(pexam->points));	// User types in points
	flush_stdin();
	printf("Datum der Pruefung [TTMMJJJJ]: ");
	gets(pexam->date);					// User types in date
	//flush_stdin();
	printf("Name des Pruefers: ");
	gets(pexam->prof);					// User types in professor
	//flush_stdin();
}

/**************************************************************************************************
Functions adds first exam in list and sets its pointer to student.pfirst_exam
Parameters:
	student_t *pstudent
	exam_t *pfirst_exam - exam struct pointer.
Returns:
	void.
*/
void add_first_exam(student_t *pstudent, exam_t *pfirst_exam) {
	
	enter_exam_data(pfirst_exam);	// Data input 

	pstudent->pfirst_exam = pfirst_exam;	// student.pfirst_exam is set to exam pointer
	pfirst_exam->pnext_exam = NULL;			// next-exam-pointer is set to NULL, because its the last element in list
}

/**************************************************************************************************
Functions adds first exam in list and sets its pointer to last element in list
Parameters:
	student_t *pstudent
	exam_t *pexam - exam struct pointer.
Returns:
	void.
*/
void add_exam(student_t *pstudent, exam_t *pexam) {
	
	enter_exam_data(pexam);	// Data input

	exam_t *ptemp_exam = pstudent->pfirst_exam;	// Set temporary exam pointer to the pointer of the first exam struct

	if (ptemp_exam == NULL) {					// Raise error if the poiner in student is NULL
		printf("Keine erste Prueufng angelegt!\n");
		return;
	}

	while (ptemp_exam->pnext_exam != NULL) {	// Loops through until last element with next pointer = NULL
		ptemp_exam = ptemp_exam->pnext_exam;	// Set temporary pointer to next exam struct
		//printf("[DEBUG] Neue Pruefung wird gehängt an: ptemp_exam: %x Pruefungsname:%s\n", ptemp_exam, ptemp_exam->course);
	}
	ptemp_exam->pnext_exam = pexam;	// Set the next-pointer of the last element in list to the new exam struct pointer
	pexam->pnext_exam = NULL;	// Set next-pointer of new struct to NULL -> now the last element in list
	//printf("[DEBUG] pexan wird an letzte Pruefung gehaengt:  pexam: %x ptemp_exam: %x ptemp_exam->pnext_exam: %x\n", pexam, ptemp_exam, ptemp_exam->pnext_exam);
}
/**************************************************************************************************
Functions prints a list with the exams of the student.
Parameters:
	student_t *pstudent
Returns:
	void.
*/
void print_exam(student_t *student) {

	printf("Die Pruefungen des Studenten sind:\n\n");

	exam_t *ptemp_examdb = student->pfirst_exam;	// Set temporary exam pointer to the pointer of the first exam struct
	while (ptemp_examdb != NULL) {	// Loops through until last element with next pointer = NULL

		printf("Veranstaltung: %s Datum: %s Punkte: %d Pruefer: %s\n", ptemp_examdb->course, ptemp_examdb->date, ptemp_examdb->points, ptemp_examdb->prof);
		ptemp_examdb = ptemp_examdb->pnext_exam;	// Set temporary pointer to next exam struct
	}
}
/**************************************************************************************************
Functions clears all the memory.
Parameters:
	student_t *pstudent
Returns:
	void.
*/
void free_all(student_t *student) {	// [TODO][ERROR][PRIO]

	exam_t *ptemp_examdb = student->pfirst_exam; // Set temporary exam pointer to the pointer of the first exam struct
	
	while (ptemp_examdb != NULL) {	// Loops through until last element with next pointer = NULL

		//printf("CLEARED: Veranstaltung: %s Datum: %s Punkte: %d Pruefer: %s\n", ptemp_examdb->course, ptemp_examdb->date, ptemp_examdb->points, ptemp_examdb->prof);	//[DEBUG]
		//free(ptemp_examdb);	//[ERROR]
		ptemp_examdb = ptemp_examdb->pnext_exam; // Set temporary pointer to next exam struct
	}
}
/**************************************************************************************************
Functions searches exam by course name
Parameters:
	student_t *pstudent
Returns:
	void.
*/
void search_course(student_t *pstudent) {
	
	char course[6];

	exam_t *pexam_search = NULL;	
	exam_t *ptemp_examdb = pstudent->pfirst_exam;	// Set temporary exam pointer to the pointer of the first exam struct

	//printf("[DEBUG] ptemp_examdb=%x\n", ptemp_examdb);

	printf("Suchen nach Prufung - Eingabe des Veranstaltungsnamen: ");	// User types in course name
	gets(course);

	while (ptemp_examdb != NULL) {	// Loops through until last element with next pointer = NULL

		//printf("SEARCH: Veranstaltung: %s Datum: %s Punkte: %d Pruefer: %s\n", ptemp_examdb->course, ptemp_examdb->date, ptemp_examdb->points, ptemp_examdb->prof); //[DEBUG]
		//printf("[DEBUG] ptemp_examdb->course: %s, course: %s\n", ptemp_examdb->course, course);	//[DEBUG]

		if (~strcmp(ptemp_examdb->course, course)) {	// If course names are equal -> course found -> break loop
			pexam_search = ptemp_examdb;
			break;
		}
		ptemp_examdb = ptemp_examdb->pnext_exam;// Set temporary pointer to next exam struct
	}
	printf("Die gesuchte Veranstaltung ist: %s Datum: %s Note: %d Pruefer: %s\n", pexam_search->course, pexam_search->date, pexam_search->points, pexam_search->prof);	// Print out exam
}

/**************************************************************************************************
Functions searches exam by points
Parameters:
	student_t *pstudent
Returns:
	void.
*/
void search_points(student_t *pstudent) {

	int points;	

	exam_t *ptemp_examdb = pstudent->pfirst_exam;	// Set temporary exam pointer to the pointer of the first exam struct

	//printf("[DEBUG] ptemp_examdb=%x\n", ptemp_examdb);

	printf("Suchen nach Prufung - Punktzahl kleiner.. ");
	scanf_s(" %d", &points);		// User types in a value

	while (ptemp_examdb != NULL) {	// Loops through until last element with next pointer = NULL

		//printf("SEARCH POINTS: Veranstaltung: %s Datum: %s Punkte: %d Pruefer: %s\n", ptemp_examdb->course, ptemp_examdb->date, ptemp_examdb->points, ptemp_examdb->prof); //[DEBUG]
		//printf("[DEBUG] ptemp_examdb->points: %d, points: %d\n", ptemp_examdb->points, points); //[DEBUG]

		if (ptemp_examdb->points < points) {	// If points of exam is lower than the typed in points, the couse gets printed to console
			printf("Die gesuchte Veranstaltung ist: %s Datum: %s Note: %d Pruefer: %s\n", ptemp_examdb->course, ptemp_examdb->date, ptemp_examdb->points, ptemp_examdb->prof);
		}
		ptemp_examdb = ptemp_examdb->pnext_exam; // Set temporary pointer to next exam struct
	}
}

/**************************************************************************************************
Functions saves data to binary file.
Parameters:
	student_t *student
Returns:
	void.
*/
void save_list(student_t *student) {

	exam_t *ptemp_examdb = student->pfirst_exam;	// Set temporary exam pointer to the pointer of the first exam struct
	
	FILE *f;	// Create stream and open file to write binary
	f = fopen("list.dat", "wb");

	if (f == NULL) {	// Handle errors
		printf("Datei konnte nicht geoeffnet werden!\n");
	}

	fwrite(student, sizeof(student_t), 1, f);	// [UNCLEAR] Write student struct to file

	while (ptemp_examdb != NULL) {	// Loops through until last element with next pointer = NULL

		fwrite(ptemp_examdb, sizeof(exam_t), 1, f); // [UNCLEAR] Writes every exam struct to file
	
		ptemp_examdb = ptemp_examdb->pnext_exam; // Set temporary pointer to next exam struct
	}
	fclose(f);	// Close file
}

/**************************************************************************************************
Functions loads data from binary file.
Parameters:
	student_t *student
Returns:
	void.
*/
void load_list(student_t *student) {		// [UNCLEAR]

	//student_t *tmp;	// initialise temporary student-struct
	exam_t exam_tmp; // initialise temporary exam-struct
	int result;

	FILE *f;	// Create stream and open file to read binary
	f = fopen("list.dat", "rb");

	if (f == NULL) {	// Handle errors
		printf("Datei konnte nicht geoeffnet werden!\n");
	}

	fread(student, sizeof(student_t), 1, f);	// Read student struct data and write it to tmp
	//printf("student->name: %s nachname: %s mat: %d pointer: %x\n", student->first_name, student->last_name,student->mat_num, student->pfirst_exam);	//[DEBUG]

	fseek(f, sizeof(student_t), SEEK_SET); // [TRY]
	int first_exam_flag = 1;
	exam_t *pexam = NULL;
	exam_t *pexam_old = student->pfirst_exam;
	
	while (fread(&exam_tmp, sizeof(exam_t), 1, f) != 0) {
		
		//printf("exam_tmp->course: %s points: %d date: %s prof: %s pointer: %x\n", exam_tmp.course, exam_tmp.points, exam_tmp.date, exam_tmp.prof, exam_tmp.pnext_exam);//[DEBUG] 
		
		pexam = (struct exam*)malloc(sizeof(struct exam));	// Allocate memory for exam-struct
		
		strcpy(pexam->course, exam_tmp.course);
		pexam->points = exam_tmp.points;
		strcpy(pexam->date, exam_tmp.date);	//[ERROR] lvalue must be changeable?
		strcpy(pexam->prof, exam_tmp.prof);
		pexam->pnext_exam = NULL;

		if (first_exam_flag) {
			student->pfirst_exam = pexam;
			first_exam_flag = 0;
		}else pexam_old->pnext_exam = pexam;		// Zuweisung des neuen elements an alten
		
		pexam_old = pexam;	// nächster pointer
	}
	fclose(f);	// Close file
}