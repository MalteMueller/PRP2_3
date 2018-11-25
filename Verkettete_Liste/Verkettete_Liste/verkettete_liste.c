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
	exam_t *pfirst_exam;
} student_t;

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
	
	char loop_flag = ' ';
	char read_flag;
	char exam_input_flag;
	char save_flag = ' ';

	//do {

		printf("Wollen Sie Daten aus der Datei liste.dat lesen? [J/N]: ");
		scanf_s(" %c", &read_flag);
		flush_stdin();

		student_t student;

		if (read_flag == 'j') {
			load_list(&student);
			printf("[DEBUG] LOADED STUDENT: Name: %s Nachname: %s Mat-Num: %d *pfirst_exam: %x\n", student.first_name, student.last_name, student.mat_num, student.pfirst_exam);
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
	search_course(&student); 
	search_points(&student);
	
	printf("Wollen Sie die Elemente speichern? ");
	scanf_s(" %c", &save_flag);

	if (save_flag == 'j') {
		save_list(&student);
	}

	free_all(&student);	// error

	system("PAUSE");
	return 0;
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void flush_stdin() {
	int flush_dummy;
	while ((flush_dummy = getchar()) != '\n' && flush_dummy != EOF);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void add_first_exam(student_t *pstudent, exam_t *pfirst_exam) {
	
	enter_exam_data(pfirst_exam);

	pstudent->pfirst_exam = pfirst_exam;
	pfirst_exam->pnext_exam = NULL;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void print_exam(student_t *student) {

	printf("Die Pruefungen des Studenten sind:\n\n");

	exam_t *ptemp_examdb = student->pfirst_exam;
	while (ptemp_examdb != NULL) {	// Läuft bis next-ptr == NULL ist

		printf("Veranstaltung: %s Datum: %s Punkte: %d Pruefer: %s\n", ptemp_examdb->course, ptemp_examdb->date, ptemp_examdb->points, ptemp_examdb->prof);
		ptemp_examdb = ptemp_examdb->pnext_exam;
		
	}

}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void free_all(student_t *student) {	// Löschung läuft nicht

	exam_t *ptemp_examdb = student->pfirst_exam;
	
	while (ptemp_examdb != NULL) {	// Läuft bis next-ptr == NULL ist

		printf("CLEARED: Veranstaltung: %s Datum: %s Punkte: %d Pruefer: %s\n", ptemp_examdb->course, ptemp_examdb->date, ptemp_examdb->points, ptemp_examdb->prof);
		//free(ptemp_examdb);	//Ohne gehts
		ptemp_examdb = ptemp_examdb->pnext_exam;
	}
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void search_course(student_t *pstudent) {
	
	char course[6];

	exam_t *pexam_search = NULL;
	exam_t *ptemp_examdb = pstudent->pfirst_exam;

	//printf("[DEBUG] ptemp_examdb=%x\n", ptemp_examdb);

	printf("Suchen nach Prufung - Eingabe des Veranstaltungsnamen: ");
	gets(course);

	while (ptemp_examdb != NULL) {	// Läuft bis next-ptr == NULL ist

		printf("SEARCH: Veranstaltung: %s Datum: %s Punkte: %d Pruefer: %s\n", ptemp_examdb->course, ptemp_examdb->date, ptemp_examdb->points, ptemp_examdb->prof);
		printf("[DEBUG] ptemp_examdb->course: %s, course: %s\n", ptemp_examdb->course, course);

		if (~strcmp(ptemp_examdb->course, course)) {
			pexam_search = ptemp_examdb;
			break;
		}
		ptemp_examdb = ptemp_examdb->pnext_exam;
	}
	printf("Die gesuchte Veranstaltung ist: %s Datum: %s Note: %d Pruefer: %s\n", pexam_search->course, pexam_search->date, pexam_search->points, pexam_search->prof);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void search_points(student_t *pstudent) {

	int points;

	exam_t *ptemp_examdb = pstudent->pfirst_exam;

	//printf("[DEBUG] ptemp_examdb=%x\n", ptemp_examdb);

	printf("Suchen nach Prufung - Punktzahl kleiner.. ");
	scanf_s(" %d", &points);

	while (ptemp_examdb != NULL) {	// Läuft bis next-ptr == NULL ist

		printf("SEARCH POINTS: Veranstaltung: %s Datum: %s Punkte: %d Pruefer: %s\n", ptemp_examdb->course, ptemp_examdb->date, ptemp_examdb->points, ptemp_examdb->prof);
		printf("[DEBUG] ptemp_examdb->points: %d, points: %d\n", ptemp_examdb->points, points);

		if (ptemp_examdb->points < points) {
			printf("Die gesuchte Veranstaltung ist: %s Datum: %s Note: %d Pruefer: %s\n", ptemp_examdb->course, ptemp_examdb->date, ptemp_examdb->points, ptemp_examdb->prof);
		}
		ptemp_examdb = ptemp_examdb->pnext_exam;
	}
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void save_list(student_t *student) {

	exam_t *ptemp_examdb = student->pfirst_exam;
	
	FILE *f;
	f = fopen("list.dat", "wb");

	if (f == NULL) {
		printf("Datei konnte nicht geoeffnet werden!\n");
	}

	fwrite(student, sizeof(student_t), 1, f);	// ?

	while (ptemp_examdb != NULL) {	// Läuft bis next-ptr == NULL ist

		fwrite(ptemp_examdb, sizeof(exam_t), 1, f);
	
		ptemp_examdb = ptemp_examdb->pnext_exam;
	}
	fclose(f);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void load_list(student_t *student) {		// ???

	student_t tmp;
	exam_t exam_tmp;
	int result;

	FILE *f;
	f = fopen("list.dat", "rb");

	if (f == NULL) {
		printf("Datei konnte nicht geoeffnet werden!\n");
	}

	fread(&tmp, sizeof(student_t), 1, f);	// Read student struct
	printf("tmp->name: %s nachname: %s mat: %d pointer: %x\n", tmp.first_name, tmp.last_name,tmp.mat_num, tmp.pfirst_exam);

	//student->first_name = tmp.first_name;	//WARUM!?
	//student->last_name = tmp.last_name;
	student->mat_num = tmp.mat_num;
	student->pfirst_exam = tmp.pfirst_exam;

	result = fseek(f, sizeof(student_t), SEEK_SET);
	if (result) {
		printf("Fehler!");
	}
	else{
		while (fread(&exam_tmp, sizeof(exam_t), 1, f) != 0) {
			printf("Kurs: %s Points: %d Date: %s Prof: %s Pointer: %x\n", exam_tmp.course, exam_tmp.points, exam_tmp.date, exam_tmp.prof, exam_tmp.pnext_exam);
		}
	}
	
	fclose(f);
}