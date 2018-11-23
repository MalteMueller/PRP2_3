/*
PRP2-2 Aufgabe 7.2
Name: Malte Müller, Fabian Liebold
Date: 22.11.2018
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define PI 3.14159265359
#define FILEPATH "data.csv" 

typedef struct schwingkreis {
	double R;
	double L;
	double C;
	double fmin;
	double fmax;
	int schritte;
	double **ue_fkt;
} schwingkreis_t;

void print_menu();
void print_values(schwingkreis_t schwingkreis);
double **create_ue_fkt(int schritte);
void calculate_values(schwingkreis_t schwingkreis);
void save_values(double **ue_fkt, int schritte);

int main(void) {

	schwingkreis_t schwingkreis;
	schwingkreis.R = 100, schwingkreis.L = 0.015, schwingkreis.C = 0.0000018, schwingkreis.fmin = 100;
	schwingkreis.fmax = 1000, schwingkreis.schritte = 21, schwingkreis.ue_fkt = NULL;

	char user_input;

	do {
		print_menu();
		scanf_s(" %c", &user_input);

		switch (user_input)
		{
		case 'a':
			print_values(schwingkreis);
			break;
		
		case 'b':
			schwingkreis.ue_fkt = create_ue_fkt(schwingkreis.schritte);
			if (schwingkreis.ue_fkt == NULL)return 1;

			calculate_values(schwingkreis);	// nicht bevor alles create-fkt läuft
			//printf("TEST: [4][3]: %f\n", schwingkreis.ue_fkt[4][3]); //funzzzt
			break;

		case 'c':
			break;

		case 'd':
			save_values(schwingkreis.ue_fkt, schwingkreis.schritte);	// Nicht bevor create-fkt und calculate-fkt läuft
			break;

		case 'q':
			break;

		default:
			printf("Ungueltige Eingabe!\n");
			break;
		}

	} while (user_input != 'q');


	system("PAUSE");
	return 0;
}

void print_menu() {
	printf("----------- Serieller Schwingkreis -----------\n\n");
	printf("A - Anzeige der Schwingkreisparameter\n");
	printf("B - Werte berechnen und ausgeben\n");
	printf("C - Frequenzbereich und Schritte abfragen\n");
	printf("D - Werte speichern\n\n");
	printf("----------------------------------------------\n\n");
	printf("Q - Beenden des Programms\n\n");
	printf("----------------------------------------------\n\n");
}

void print_values(schwingkreis_t schwingkreis) {
	printf("++++++++++++++++++++++++++++++++++++++++++++++\n\n");
	printf("Schwingkreisparameter:\n");
	printf("++++++++++++++++++++++++++++++++++++++++++++++\n\n");
	printf("Widerstandswert\t%f Ohm\n", schwingkreis.R);
	printf("Induktionswert\t%f H\n", schwingkreis.L);
	printf("Kapazitätswert\t%f Ohm\n", schwingkreis.C);
	printf("Frequenzintervall - untere Grenze\t%f Hz\n", schwingkreis.fmin);
	printf("Frequenzintervall - obere Grenze\t%f Hz\n", schwingkreis.fmax);
	printf("Anzahl der Schritte\t%d \n", schwingkreis.schritte);
	printf("----------------------------------------------\n\n");
}

double **create_ue_fkt(int schritte) {
	
	double **tmp = (double**)malloc(schritte * sizeof(double*));

	printf("[DEBUG] tmp pointer: %x\n", tmp);
	

	if (tmp == NULL) {
		free(tmp);
		return NULL;
	}

	for (int i = 0; i < schritte; i++) {
		tmp[i] = (double*)malloc(5 * sizeof(double));

		printf("[DEBUG] tmp[%d] pointer: %x\n", i, tmp[i]);

		if (tmp[i] == NULL) {
			for (int j = 0; j <= i; j++) {
				free(tmp[j]);
			}
			free(tmp);
			return NULL;
		}
	}
	return tmp;
}

void calculate_values(schwingkreis_t schwingkreis){
	
	printf("++++++++++++++++++++++++++++++++++++++++++++++\n");
	printf("Schwingkreiswerte:\n");
	printf("++++++++++++++++++++++++++++++++++++++++++++++\n");
	printf("f\tRe\tIm\tBetrag\tPhase\n");

	for (int i = 0; i < schwingkreis.schritte; i++) {																		//Berechnung
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

		for (int y = 0; y < 5; y++) {
			printf("%f\t", schwingkreis.ue_fkt[i][y]);
		}
		printf("\n");
	}
}

void save_values(double **ue_fkt, int schritte) {
	FILE* data_stream;
	data_stream = fopen(FILEPATH, "w");

	fprintf(data_stream, "f, Re, Im, Betrag, Phase\n");

	printf("TEST: [4][3]: %f\n", ue_fkt[4][3]); 

	for (int i = 0; i < schritte; i++) {
		for (int y = 0; y < 5; y++) {
			fprintf(data_stream, "%f", ue_fkt[i][y]);
			if(y != 4) fprintf(data_stream, ",");
		}
		fprintf(data_stream, "\n");
	}
	fclose(data_stream);
}