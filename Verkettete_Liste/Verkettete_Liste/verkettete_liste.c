/*
PRP2-3 Aufgabe 3.1
Name: Malte Müller, Fabian Liebold
Date: 19.11.2018
*/

//#define _CRT_SECURE_NO_WARNINGS	//?

#include <string.h>
#include <stdio.h>

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
void add_first_exam(exam_t *first_exam);


int main(void) {
	
	char loop_flag = ' ';
	char read_flag;
	char exam_input_flag;

	

	do {

		printf("Wollen Sie Daten aus der Datei liste.dat lesen? [J/N]: ");
		scanf_s(" %c", &read_flag);
		flush_stdin();

		if (read_flag == 'j') {
		
		}
		else if (read_flag == 'n') {
			student_t student;
			create_student(&student);

			printf("Name: %s Nachname: %s Matrikelnummer: %d\n", student.first_name, student.last_name, student.mat_num);	//[DEBUG]

			printf("Wollen Sie Pruefungen eingeben? [J/N]: ");
			scanf_s(" %c", &exam_input_flag);
			flush_stdin();

			if (exam_input_flag == 'j') {
				if(student.pfirst_exam == NULL){		// Check if the first exam is available
					exam_t first_exam;
					add_first_exam(&first_exam);

					printf("%s %d %s %s \n", first_exam.course, first_exam.points, first_exam.date, first_exam.prof);	//[DEBUG]

				}
				else {} // If it's not the first exam
			}
			else if (read_flag == 'n') {	// If no exam should be added
			
			}
			else printf("Ungueltige Eingabe!\n");
		}
		else printf("Ungueltige Eingabe!\n");
		
	} while (loop_flag != 'q');

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

void add_first_exam(exam_t *first_exam) {
	
	printf("Eingeben einer Prüfung:\n");
	printf("Name der Veranstaltung: ");
	gets(first_exam->course);
	//flush_stdin();
	printf("Punktzahl: ");
	scanf_s(" %d", &(first_exam->points));
	flush_stdin();
	printf("Datum der Pruefung [TTMMJJJJ]: ");
	gets(first_exam->date);
	//flush_stdin();
	printf("Name des Pruefers: ");
	gets(first_exam->prof);
	//flush_stdin();

	first_exam->pnext_exam = NULL;
}