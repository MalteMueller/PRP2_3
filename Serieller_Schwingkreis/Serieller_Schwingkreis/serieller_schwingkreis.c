/*
PRP2-2 Aufgabe 7.2
Name: Malte Müller, Fabian Liebold
Date: 26.11.2018
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define PI 3.14159265359
#define FILEPATH "data.csv" 

typedef struct schwingkreis {	// Define schiwngkreis_t
	double R;
	double L;
	double C;
	double fmin;
	double fmax;
	int schritte;
	double **ue_fkt;
} schwingkreis_t;

//Prototypes:
char menu(void);
void print_parameters(FILE* out, schwingkreis_t *schwingkreis, char tz);
void print_values(FILE* out, schwingkreis_t *schwingkreis, char tz);
double **create_ue_fkt(int schritte);
void calculate_values(schwingkreis_t schwingkreis);
void change_frequency(schwingkreis_t *schwingkreis);
void free_memory(schwingkreis_t *schwingkreis);
void read_parameters(FILE* in, schwingkreis_t *schwingkreis);

int main(void) {

	char tz_csv = ',';	// Define seperation character for csv and cmd
	char tz_cmd= ' ';

	schwingkreis_t schwingkreis;		// Initialize schwingkreis and set standard values
	schwingkreis.R = 100, schwingkreis.L = 0.015, schwingkreis.C = 0.0000018, schwingkreis.fmin = 100;
	schwingkreis.fmax = 1000, schwingkreis.schritte = 21, schwingkreis.ue_fkt = NULL;

	char user_input;

	do {
		user_input = menu();	// show menu and get user input

		switch (user_input)
		{
		case 'a':
			print_parameters(stdout, &schwingkreis, tz_cmd);	// Print out parameters of schwingkreis to console
			break;
		
		case 'b':
			schwingkreis.ue_fkt = create_ue_fkt(schwingkreis.schritte);	// Allocate memory for ue_fkt-matrix
			if (schwingkreis.ue_fkt == NULL)return 1;	// If an error occures, exit programm with error code: 1

			calculate_values(schwingkreis);	// Calculate schwingkreis-values from given parameters

			break;

		case 'c':
			change_frequency(&schwingkreis);	// Set frequency min, frequency max and steps
			break;

		case 'd':
			FILE *data_stream;					// Create a stream and open the csv-file to write values in it
			data_stream = fopen(FILEPATH, "w");
			print_parameters(data_stream, &schwingkreis, tz_csv);	// Write parameters into file
			print_values(data_stream, &schwingkreis, tz_csv);	// Write values into file
			fclose(data_stream);	// Close file
			break;

		case 'e':
			FILE * data_in;					// Create a stream and open the csv-file to read values in it
			data_in = fopen(FILEPATH, "r");

			read_parameters(data_in, &schwingkreis);
			break;

		case 'q':
			free_memory(&schwingkreis);	// Free memory
			break;

		default:
			printf("Fehler!\n");
			break;
		}

	} while (user_input != 'q');	// Exit loop if 'q' is pressed


	system("PAUSE");
	return 0;
}

/**************************************************************************************************
Function prints menu and returns users selection.
Parameters:
	void.
Returns:
	char tmp - user input.
*/

char menu(void) {
	

	char tmp = ' ';
	do {		// Print menu as long as user types in one of the possible selections
		printf("\n----------- Serieller Schwingkreis -----------\n\n");
		printf("A - Anzeige der Schwingkreisparameter\n");
		printf("B - Werte berechnen und ausgeben\n");
		printf("C - Frequenzbereich und Schritte abfragen\n");
		printf("D - Werte speichern\n");
		printf("E - Parameter einlesen\n\n");
		printf("----------------------------------------------\n\n");
		printf("Q - Beenden des Programms\n\n");
		printf("----------------------------------------------\n\n");
		printf("Auswahl: ");

		scanf_s(" %c", &tmp);	// Get user input

		if (tmp < 91) tmp = tmp + 32;

	} while (tmp != 'a' && tmp != 'b' && tmp != 'c' && tmp != 'd' && tmp != 'e' && tmp != 'q');

	return tmp;	
}

/**************************************************************************************************
Function prints parametes to the stream.
Parameters:
	FILE* out - output stream.
	schwingkreis_t *schwingkreis
Returns:
	void.
*/

void print_parameters(FILE* out, schwingkreis_t *schwingkreis, char tz) {
	fprintf(out, "++++++++++++++++++++++++++++++++++++++++++++++\n");	// print to given output stream
	fprintf(out, "Schwingkreisparameter:\n");
	fprintf(out, "++++++++++++++++++++++++++++++++++++++++++++++\n\n");
	fprintf(out, "Widerstandswert%c%lf%cOhm\n", tz, schwingkreis->R,tz);
	fprintf(out, "Induktionswert%c%lf%cH\n",tz, schwingkreis->L, tz);
	fprintf(out, "Kapazitätswert%c%lf%cC\n",tz, schwingkreis->C, tz);
	fprintf(out, "Frequenzintervall - untere Grenze%c%lf%cHz\n", tz, schwingkreis->fmin, tz);
	fprintf(out, "Frequenzintervall - obere Grenze%c%lf%cHz\n", tz, schwingkreis->fmax, tz);
	fprintf(out, "Anzahl der Schritte%c%d \n", tz, schwingkreis->schritte);
	fprintf(out, "----------------------------------------------\n\n");
}

/**************************************************************************************************
Function creates 2d-matrix for schwingkreis-values
Parameters:
	int schritte - size of matrix.
Returns:
	double **tmp - 2d-matrix-pointer.
*/
double **create_ue_fkt(int schritte) {
	
	double **tmp = (double**)malloc(schritte * sizeof(double*));	// Allocate column with size of "schritte"

	if (tmp == NULL) {	// Free memory if allocation fails
		free(tmp);
		return NULL;
	}

	for (int i = 0; i < schritte; i++) {				// Allocate rows for each element in column with size of 5 * double
		tmp[i] = (double*)malloc(5 * sizeof(double));

		//printf("[DEBUG] tmp[%d] pointer: %x\n", i, tmp[i]);

		if (tmp[i] == NULL) {	// Free memory if allocation fails
			for (int j = 0; j <= i; j++) {
				free(tmp[j]);
			}
			free(tmp);
			return NULL;
		}
	}
	return tmp; // return 2d-matrix-pointer
}

/**************************************************************************************************
Function calculates values of schwingkreis
Parameters:
	schiwngkreis_t schwingkreis.
Returns:
	void.
*/
void calculate_values(schwingkreis_t schwingkreis){
	
	printf("++++++++++++++++++++++++++++++++++++++++++++++\n");
	printf("Schwingkreiswerte:\n");
	printf("++++++++++++++++++++++++++++++++++++++++++++++\n");
	printf("f\t\tRe\t\tIm\t\tBetrag\t\tPhase\n");

	for (int i = 0; i < schwingkreis.schritte; i++) {																		//Calculate values according to given equations
		schwingkreis.ue_fkt[i][0] = schwingkreis.fmin + i * (schwingkreis.fmax - schwingkreis.fmin) / (schwingkreis.schritte-1);// f
		schwingkreis.ue_fkt[i][1] = pow((2 * PI*schwingkreis.ue_fkt[i][0] * schwingkreis.R*schwingkreis.C), 2) / (
									pow((1 - pow(2 * PI*schwingkreis.ue_fkt[i][0], 2)*schwingkreis.L*schwingkreis.C), 2) +
									pow((2*PI*schwingkreis.ue_fkt[i][0]* schwingkreis.R*schwingkreis.C), 2));				// Re
		schwingkreis.ue_fkt[i][2] = 2*PI*schwingkreis.ue_fkt[i][0]*schwingkreis.R*schwingkreis.C * 
									(1 - pow(2 * PI*schwingkreis.ue_fkt[i][0], 2)*schwingkreis.L*schwingkreis.C) / (
									pow((1 - pow(2 * PI*schwingkreis.ue_fkt[i][0], 2)*schwingkreis.L*schwingkreis.C), 2) +
									pow((2 * PI*schwingkreis.ue_fkt[i][0] * schwingkreis.R*schwingkreis.C), 2));				// Im
		schwingkreis.ue_fkt[i][3] = sqrt(pow(schwingkreis.ue_fkt[i][1], 2) + pow(schwingkreis.ue_fkt[i][2], 2));			// Betrag
		schwingkreis.ue_fkt[i][4] = atan2(schwingkreis.ue_fkt[i][2] , schwingkreis.ue_fkt[i][1]) * 180 / PI;							// Phase

		for (int y = 0; y < 5; y++) {		// Print out values to console [TEMP]
			printf("%f\t", schwingkreis.ue_fkt[i][y]);
		}
		printf("\n");
	}
}

/**************************************************************************************************
Function changes the frequency parameters and steps.
Parameters:
	schwingkreis_t *schwingkreis
Returns:
	void.
*/
void change_frequency(schwingkreis_t *schwingkreis) {
	int tmp;
	do {
		do {
			printf("Frequenz - Eingabe der unteren Grenze[10, 10000]: ");	//Set f_min
			scanf_s(" %d", &tmp);
		} while (tmp < 10 || tmp>10000);

		schwingkreis->fmin = tmp;

		do {
			printf("Frequenz - Eingabe der oberen Grenze[300, 100000]: ");	//Set f_max
			scanf_s(" %d", &tmp);
		} while (tmp < 300 || tmp>100000);

		schwingkreis->fmax = tmp;
	} while (schwingkreis->fmax<=schwingkreis->fmin);	

	do {
		printf("Frequenz - Eingabe der Schrittanzahl[2, 50]: ");		//Set steps
		scanf_s(" %d", &tmp);
	} while (tmp<2 || tmp>50);

	schwingkreis->schritte = tmp;
}

/**************************************************************************************************
Function prints values to output stream.
Parameters:
	FILE *out - output stream.
	schwingkreis_t *schwingkreis.
	char tz - seperation char.
Returns:
	void.
*/
void print_values(FILE* out, schwingkreis_t *schwingkreis, char tz) {
	

	fprintf(out, "f%c Re%c Im%c Betrag%c Phase\n", tz, tz ,tz ,tz);	// Print header

	//printf("TEST: [4][3]: %f\n", schwingkreis->ue_fkt[4][3]); 

	for (int i = 0; i < schwingkreis->schritte; i++) {	//Print values to output stream with seperation char
		for (int y = 0; y < 5; y++) {
			fprintf(out, "%f", schwingkreis->ue_fkt[i][y]);
			if(y != 4) fprintf(out, "%c",tz);
		}
		fprintf(out, "\n");
	}
}

/**************************************************************************************************
Function clears memory.
Parameters:
	schwingkreis_t *schwingkreis
Returns:
	void.
*/
void free_memory(schwingkreis_t *schwingkreis) {

	for (int i = 0; i < schwingkreis->schritte; i++) {	// Free rows
		free(schwingkreis->ue_fkt[i]);
	}
	free(schwingkreis->ue_fkt);	// Free column
}

void read_parameters(FILE* in, schwingkreis_t *schwingkreis) {
	char temp[1000] = " ";
	
	int row = 0;	// number row
	int column = 0;	// number column

	while (fgets(temp, 1000, in)) {		// skip first 4 rows
		row++;
		if (row == 4) break;
	}
	
	while (fgets(temp, 1000, in) != NULL && temp[0] != '-') {	// get every row until '-'
		if (temp[0] != ' ') {	// if its not a empty line
			
			char * pch;
			pch = strtok(temp, ",");		// stringtoken to slice string 
			
			column = 0;					// couter to get value in row between comma [..,value,..] - reset every new row
			while (pch != NULL)
			{
				if (column == 1) {		// if value in row
					switch (row)		// declare values to parameter struct
					{
					case 4:
						schwingkreis->R = atof(pch);							// string to double
						printf("schwingkreis->R: %lf\n", schwingkreis->R);
						break;
					case 5:
						schwingkreis->L = atof(pch);
						printf("schwingkreis->L: %lf\n", schwingkreis->L);
						break;
					case 6:
						schwingkreis->C = atof(pch);
						printf("schwingkreis->C: %lf\n", schwingkreis->C);
						break;
					case 7:
						schwingkreis->fmin = atof(pch);
						printf("schwingkreis->fmin: %lf\n", schwingkreis->fmin);
						break;
					case 8:
						schwingkreis->fmax = atof(pch);
						printf("schwingkreis->fmax: %lf\n", schwingkreis->fmax);
						break;
					case 9:
						schwingkreis->schritte = atoi(pch);
						printf("schwingkreis->schritte: %d\n", schwingkreis->schritte);
						break;
					default:
						break;
					}
				}
		
				pch = strtok(NULL, ",");	// reset string token
				column++;	// get next column
			}
			row++;	// next row
		} 
	}
}