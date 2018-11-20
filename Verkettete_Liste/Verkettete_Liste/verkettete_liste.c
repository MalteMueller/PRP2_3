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
void enter_exam_data(exam_t *pexam);
void add_first_exam(student_t *pstudent, exam_t *pfirst_exam);
void add_exam(student_t *pstudent, exam_t *pexam);

int main(void) {
	
	char loop_flag = ' ';
	char read_flag;
	char exam_input_flag;

	//do {

		printf("Wollen Sie Daten aus der Datei liste.dat lesen? [J/N]: ");
		scanf_s(" %c", &read_flag);
		flush_stdin();

		if (read_flag == 'j') {
		
		}
		else if (read_flag == 'n') {
			student_t student;
			create_student(&student);

			printf("[DEBUG] Name: %s Nachname: %s Matrikelnummer: %d\n", student.first_name, student.last_name, student.mat_num);	//[DEBUG]

			printf("Wollen Sie Pruefungen eingeben? [J/N]: ");
			scanf_s(" %c", &exam_input_flag);
			flush_stdin();

			if (exam_input_flag == 'j') {
				do {
					if (student.pfirst_exam == NULL) {		// Check if the first exam is available
						
						exam_t *pfirst_exam = NULL;
						pfirst_exam = (exam_t*)malloc(sizeof(exam_t));					//MALLOC!?

						if (pfirst_exam == NULL) {
							free(pfirst_exam);
							printf("Allocation 1 failed!\n");
						}

						//[DEBUG]
						pfirst_exam->course = "test";		// GEHT NICHT!?

						printf("\n[DEBUG] pfirst_exam: %p\n", pfirst_exam);
						add_first_exam(&student, pfirst_exam);

						printf("[DEBUG]  %s %d %s %s \n", pfirst_exam->course, pfirst_exam->points, pfirst_exam->date, pfirst_exam->prof);	//[DEBUG]
					}
					else {	// If it's not the first exam
						exam_t *pexam = (exam_t*)malloc(sizeof(exam_t));;	// MALLOC?!
						
						if (pexam == NULL) {
							free(pexam);
							printf("Allocation´2 failed!\n");
						}

						add_exam(&student, pexam);

						printf("[DEBUG]  %s %d %s %s %p\n", pexam->course, pexam->points, pexam->date, pexam->prof, pexam->pnext_exam);	//[DEBUG]

					} 
					
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
	printf("\n[DEBUG] pstudent->pfirst_exam: %p, ptemp_exam: %p\n", pstudent->pfirst_exam, ptemp_exam);

	if (ptemp_exam == NULL) {
		printf("Keine erste Prueufng angelegt!\n");
		return;
	}

	while (ptemp_exam->pnext_exam != NULL) {
		ptemp_exam = ptemp_exam->pnext_exam;
		printf("\n[DEBUG] ptemp_exam: %p Pruefungsname:%s\n", ptemp_exam, ptemp_exam->course);
	}
	ptemp_exam->pnext_exam = pexam;
	printf("\n[DEBUG] NACH WHILE: ptemp_exam->pnext_exam: %p pexam: %p\n", ptemp_exam->pnext_exam, pexam);
}