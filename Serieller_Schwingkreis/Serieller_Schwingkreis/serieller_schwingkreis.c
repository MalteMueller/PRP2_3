/*
PRP2-2 Aufgabe 7.2
Name: Malte Müller, Fabian Liebold
Date: 26.11.2018
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define PI 3.14159265359
#define FILEPATH "data.csv" 
#define TZ ","

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
void print_parameters(FILE* out, schwingkreis_t *schwingkreis);
void print_values(FILE* out, schwingkreis_t *schwingkreis, char tz);
double **create_ue_fkt(int schritte);
void calculate_values(schwingkreis_t schwingkreis);
void change_frequency(schwingkreis_t *schwingkreis);
void free_memory(schwingkreis_t *schwingkreis);

int main(void) {

	schwingkreis_t schwingkreis;		// Initialize schwingkreis and set standard values
	schwingkreis.R = 100, schwingkreis.L = 0.015, schwingkreis.C = 0.0000018, schwingkreis.fmin = 100;
	schwingkreis.fmax = 1000, schwingkreis.schritte = 21, schwingkreis.ue_fkt = NULL;

	char user_input;

	do {
		user_input = menu();	// show menu and get user input

		switch (user_input)
		{
		case 'a':
			print_parameters(stdout, &schwingkreis);	// Print out parameters of schwingkreis to console
			break;
		
		case 'b':
			schwingkreis.ue_fkt = create_ue_fkt(schwingkreis.schritte);	// Allocate memory for ue_fkt-matrix
			if (schwingkreis.ue_fkt == NULL)return 1;	// If an error occures, exit programm with error code: 1

			calculate_values(schwingkreis);	// Calculate schwingkreis-values from given parameters
			//print_values(stdout, &schwingkreis, TZ);

			break;

		case 'c':
			change_frequency(&schwingkreis);	// Set frequency min, frequency max and steps
			break;

		case 'd':
			FILE *data_stream;					// Create a stream and open the csv-file to write values in it
			data_stream = fopen(FILEPATH, "w");
			print_parameters(data_stream, &schwingkreis, TZ);	// Write parameters into file
			print_values(data_stream, &schwingkreis, TZ);	// Write values into file
			fclose(data_stream);	// Close file
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
		printf("----------- Serieller Schwingkreis -----------\n\n");
		printf("A - Anzeige der Schwingkreisparameter\n");
		printf("B - Werte berechnen und ausgeben\n");
		printf("C - Frequenzbereich und Schritte abfragen\n");
		printf("D - Werte speichern\n\n");
		printf("----------------------------------------------\n\n");
		printf("Q - Beenden des Programms\n\n");
		printf("----------------------------------------------\n\n");
		printf("Auswahl: ");

		scanf_s(" %c", &tmp);	// Get user input

	} while (tmp != 'a' && tmp != 'b' && tmp != 'c' && tmp != 'd' && tmp != 'q');

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

void print_parameters(FILE* out, schwingkreis_t *schwingkreis) {
	fprintf(out, "++++++++++++++++++++++++++++++++++++++++++++++\n\n");	// print to given output stream
	fprintf(out, "Schwingkreisparameter:\n");
	fprintf(out, "++++++++++++++++++++++++++++++++++++++++++++++\n\n");
	fprintf(out, "Widerstandswert\t%f Ohm\n", schwingkreis->R);
	fprintf(out, "Induktionswert\t%f H\n", schwingkreis->L);
	fprintf(out, "Kapazitätswert\t%f Ohm\n", schwingkreis->C);
	fprintf(out, "Frequenzintervall - untere Grenze\t%f Hz\n", schwingkreis->fmin);
	fprintf(out, "Frequenzintervall - obere Grenze\t%f Hz\n", schwingkreis->fmax);
	fprintf(out, "Anzahl der Schritte\t%d \n", schwingkreis->schritte);
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

	printf("[DEBUG] tmp pointer: %x\n", tmp);
	

	if (tmp == NULL) {	// Free memory if allocation fails
		free(tmp);
		return NULL;
	}

	for (int i = 0; i < schritte; i++) {				// Allocate rows for each element in column with size of 5 * double
		tmp[i] = (double*)malloc(5 * sizeof(double));

		printf("[DEBUG] tmp[%d] pointer: %x\n", i, tmp[i]);

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
	printf("f\tRe\tIm\tBetrag\tPhase\n");

	for (int i = 0; i < schwingkreis.schritte; i++) {																		//Calculate values according to given equations
		schwingkreis.ue_fkt[i][0] = schwingkreis.fmin + i * (schwingkreis.fmax - schwingkreis.fmin) / schwingkreis.schritte;// f
		schwingkreis.ue_fkt[i][1] = pow((2 * PI*schwingkreis.ue_fkt[i][0] * schwingkreis.R*schwingkreis.C), 2) /
									pow((1 - pow(2 * PI*schwingkreis.ue_fkt[i][0], 2)*schwingkreis.L*schwingkreis.C), 2) +
									pow((2*PI*schwingkreis.ue_fkt[i][0]* schwingkreis.R*schwingkreis.C), 2);				// Re
		schwingkreis.ue_fkt[i][2] = 2*PI*schwingkreis.ue_fkt[i][0]*schwingkreis.R*schwingkreis.C * 
									(1 - pow(2 * PI*schwingkreis.ue_fkt[i][0], 2)*schwingkreis.L*schwingkreis.C) /
									pow((1 - pow(2 * PI*schwingkreis.ue_fkt[i][0], 2)*schwingkreis.L*schwingkreis.C), 2) +
									pow((2 * PI*schwingkreis.ue_fkt[i][0] * schwingkreis.R*schwingkreis.C), 2);				// Im
		schwingkreis.ue_fkt[i][3] = sqrt(pow(schwingkreis.ue_fkt[i][1], 2) + pow(schwingkreis.ue_fkt[i][2], 2));			// Betrag
		schwingkreis.ue_fkt[i][4] = atan(schwingkreis.ue_fkt[i][2] / schwingkreis.ue_fkt[i][1]);							// Phase

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
		printf("Frequenz - Eingabe der unteren Grenze[10, 10000]: ");	//Set f_min
		scanf_s(" %d", &tmp);
	} while (tmp<10 || tmp>10000);

	schwingkreis->fmin = tmp;

	do {
		printf("Frequenz - Eingabe der oberen Grenze[300, 100000]: ");	//Set f_max
		scanf_s(" %d", &tmp);
	} while (tmp<300 || tmp>100000);

	schwingkreis->fmax = tmp;

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
	

	fprintf(out, "f, Re, Im, Betrag, Phase\n");	// Print header	[CHANGE]

	//printf("TEST: [4][3]: %f\n", schwingkreis->ue_fkt[4][3]); 

	for (int i = 0; i < schwingkreis->schritte; i++) {	//Print values to output stream with seperation char
		for (int y = 0; y < 5; y++) {
			fprintf(out, "%f", schwingkreis->ue_fkt[i][y]);
			if(y != 4) fprintf(out, TZ);
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