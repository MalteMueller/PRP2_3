/*
PRP2-3 Aufgabe 3.1
Name: Malte Müller, Fabian Liebold
Date: 19.11.2018
*/

#define _CRT_SECURE_NO_WARNINGS	//?

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

 typedef struct exam {
	char course[5];
	int points;
	char date[8];
	char prof[15];
	struct exam *pnext_exam;
} exam_t;


typedef struct student {
	char first_name[25];
	char last_name[25];
	int mat_num;
	struct exam *pfirst_exam;
} student_t;

void flush_stdin();
void create_student(student_t *pstudent);
void enter_exam_data(exam_t *pexam);
void add_first_exam(student_t *pstudent, exam_t *pfirst_exam);
void add_exam(student_t *pstudent, exam_t *pexam);
void print_exam(student_t *student);
void free_all(student_t *student);

int main(void) {
	
	char loop_flag = ' ';
	char read_flag;
	char exam_input_flag;

	//do {

		printf("Wollen Sie Daten aus der Datei liste.dat lesen? [J/N]: ");
		scanf_s(" %c", &read_flag);
		flush_stdin();

		student_t student;

		if (read_flag == 'j') {
		
		}
		else if (read_flag == 'n') {
			
			create_student(&student);

			printf("[DEBUG] Name: %s Nachname: %s Matrikelnummer: %d\n", student.first_name, student.last_name, student.mat_num);	//[DEBUG]

			printf("Wollen Sie Pruefungen eingeben? [J/N]: ");
			scanf_s(" %c", &exam_input_flag);
			flush_stdin();

			if (exam_input_flag == 'j') {
				do {
					exam_t *pexam = NULL;
					pexam = (struct exam*)malloc(sizeof(struct exam));					//MALLOC!?

					if (pexam == NULL) {
						free(pexam);
						printf("Allocation 1 failed!\n");
					}
					
					printf("[DEBUG] Erstelltes pexam (malloc): %x\n", pexam);

					if (student.pfirst_exam == NULL) {		// Check if the first exam is available
						add_first_exam(&student, pexam);
						}
					else {	// If it's not the first exam
						add_exam(&student, pexam);
					} 
					
					printf("[DEBUG]  Gerade erstellt: Kurs: %s Punkte: %d Datum: %s Prof: %s Next: %x  (sollte 0 sein)\n", pexam->course, pexam->points, pexam->date, pexam->prof, pexam->pnext_exam);	//[DEBUG]

					printf("Wollen Sie weitere Pruefungen eingeben? [J/N]: ");
					scanf_s(" %c", &exam_input_flag);
					flush_stdin();
				} while (exam_input_flag == 'j');
			}
			else if (read_flag == 'n') {	// If no exam should be added
			
			}
			else printf("Ungueltige Eingabe!\n");
		}
		else printf("Ungueltige Eingabe!\n");
		
	//} while (loop_flag != 'q');

	print_exam(&student);
	// free_all(&student);	// Hier ist ein wurm
	
	system("PAUSE");
	return 0;
}


void flush_stdin() {
	int flush_dummy;
	while ((flush_dummy = getchar()) != '\n' && flush_dummy != EOF);
}

void create_student(student_t *pstudent) {

	printf("Geben Sie einen Studenten ein:\n");
	printf("Nachname: ");
	gets(pstudent->last_name);
	//flush_stdin();
	printf("Name: ");
	gets(pstudent->first_name);
	//flush_stdin();
	printf("Matrikelnummer: ");
	scanf_s(" %d", &(pstudent->mat_num));
	//flush_stdin();

	pstudent->pfirst_exam = NULL;
}

void enter_exam_data(exam_t *pexam) {
	
	printf("Eingeben einer Prüfung:\n");
	printf("Name der Veranstaltung: ");
	gets(pexam->course);
	//flush_stdin();
	printf("Punktzahl: ");
	scanf_s(" %d", &(pexam->points));
	flush_stdin();
	printf("Datum der Pruefung [TTMMJJJJ]: ");
	gets(pexam->date);
	//flush_stdin();
	printf("Name des Pruefers: ");
	gets(pexam->prof);
	//flush_stdin();
}

void add_first_exam(student_t *pstudent, exam_t *pfirst_exam) {
	
	enter_exam_data(pfirst_exam);

	pstudent->pfirst_exam = pfirst_exam;
	pfirst_exam->pnext_exam = NULL;
}

void add_exam(student_t *pstudent, exam_t *pexam) {
	
	enter_exam_data(pexam);

	exam_t *ptemp_exam = pstudent->pfirst_exam;

	if (ptemp_exam == NULL) {
		printf("Keine erste Prueufng angelegt!\n");
		return;
	}

	while (ptemp_exam->pnext_exam != NULL) {	// Läuft bis next-ptr == NULL ist
		ptemp_exam = ptemp_exam->pnext_exam;
		printf("[DEBUG] Neue Pruefung wird gehängt an: ptemp_exam: %x Pruefungsname:%s\n", ptemp_exam, ptemp_exam->course);
	}
	ptemp_exam->pnext_exam = pexam;	// pexam wird letzter prüfung zugeweisen
	pexam->pnext_exam = NULL;
	printf("[DEBUG] pexan wird an letzte Pruefung gehaengt:  pexam: %x ptemp_exam: %x ptemp_exam->pnext_exam: %x\n", pexam, ptemp_exam, ptemp_exam->pnext_exam);
}

void print_exam(student_t *student) {

	printf("Die Pruefungen des Studenten sind:\n\n");

	exam_t *ptemp_examdb = student->pfirst_exam;
	while (ptemp_examdb != NULL) {	// Läuft bis next-ptr == NULL ist

		printf("Veranstaltung: %s Datum: %s Punkte: %d Pruefer: %s\n", ptemp_examdb->course, ptemp_examdb->date, ptemp_examdb->points, ptemp_examdb->prof);
		ptemp_examdb = ptemp_examdb->pnext_exam;
		
	}

}

void free_all(student_t *student) {	// Löschung läuft nicht

	exam_t *ptemp_examdb = student->pfirst_exam;
	
	while (ptemp_examdb != NULL) {	// Läuft bis next-ptr == NULL ist

		printf("CLEARED: Veranstaltung: %s Datum: %s Punkte: %d Pruefer: %s\n", ptemp_examdb->course, ptemp_examdb->date, ptemp_examdb->points, ptemp_examdb->prof);
		free(ptemp_examdb);
		ptemp_examdb = ptemp_examdb->pnext_exam;
	}
}