/*
PRP2-2 Aufgabe 7.2
Name: Malte Müller, Fabian Liebold
Date: 25.11.2018
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define PI 3.14159265359
#define FILEPATH "data.csv" 
#define TZ ","

typedef struct schwingkreis {
	double R;
	double L;
	double C;
	double fmin;
	double fmax;
	int schritte;
	double **ue_fkt;
} schwingkreis_t;

char menu(void);
void print_parameters(FILE* out, schwingkreis_t *schwingkreis);
void print_values(FILE* out, schwingkreis_t *schwingkreis, char tz);
double **create_ue_fkt(int schritte);
void calculate_values(schwingkreis_t schwingkreis);
void change_frequency(schwingkreis_t *schwingkreis);
void free_memory(schwingkreis_t *schwingkreis);

int main(void) {

	schwingkreis_t schwingkreis;
	schwingkreis.R = 100, schwingkreis.L = 0.015, schwingkreis.C = 0.0000018, schwingkreis.fmin = 100;
	schwingkreis.fmax = 1000, schwingkreis.schritte = 21, schwingkreis.ue_fkt = NULL;

	char user_input;

	do {
		user_input = menu();

		switch (user_input)
		{
		case 'a':
			print_parameters(stdout, &schwingkreis);
			break;
		
		case 'b':
			schwingkreis.ue_fkt = create_ue_fkt(schwingkreis.schritte);
			if (schwingkreis.ue_fkt == NULL)return 1;

			calculate_values(schwingkreis);	// nicht bevor alles create-fkt läuft
			//print_values(stdout, &schwingkreis, TZ);

			break;

		case 'c':
			change_frequency(&schwingkreis);
			break;

		case 'd':
			FILE *data_stream;
			data_stream = fopen(FILEPATH, "w");
			print_parameters(data_stream, &schwingkreis, TZ);
			print_values(data_stream, &schwingkreis, TZ);	
			fclose(data_stream);
			break;

		case 'q':
			free_memory(&schwingkreis);
			break;

		default:
			printf("Fehler!\n");
			break;
		}

	} while (user_input != 'q');


	system("PAUSE");
	return 0;
}

char menu(void) {
	

	char tmp = ' ';
	do {
		printf("----------- Serieller Schwingkreis -----------\n\n");
		printf("A - Anzeige der Schwingkreisparameter\n");
		printf("B - Werte berechnen und ausgeben\n");
		printf("C - Frequenzbereich und Schritte abfragen\n");
		printf("D - Werte speichern\n\n");
		printf("----------------------------------------------\n\n");
		printf("Q - Beenden des Programms\n\n");
		printf("----------------------------------------------\n\n");
		printf("Auswahl: ");

		scanf_s(" %c", &tmp);

	} while (tmp != 'a' && tmp != 'b' && tmp != 'c' && tmp != 'd' && tmp != 'q');

	return tmp;

}

void print_parameters(FILE* out, schwingkreis_t *schwingkreis) {
	fprintf(out, "++++++++++++++++++++++++++++++++++++++++++++++\n\n");
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

void change_frequency(schwingkreis_t *schwingkreis) {
	int tmp;
	do {
		printf("Frequenz - Eingabe der unteren Grenze[10, 10000]: ");
		scanf_s(" %d", &tmp);
	} while (tmp<10 || tmp>10000);

	schwingkreis->fmin = tmp;

	do {
		printf("Frequenz - Eingabe der oberen Grenze[300, 100000]: ");
		scanf_s(" %d", &tmp);
	} while (tmp<300 || tmp>100000);

	schwingkreis->fmax = tmp;

	do {
		printf("Frequenz - Eingabe der Schrittanzahl[2, 50]: ");
		scanf_s(" %d", &tmp);
	} while (tmp<2 || tmp>50);

	schwingkreis->schritte = tmp;
}

void print_values(FILE* out, schwingkreis_t *schwingkreis, char tz) {
	

	fprintf(out, "f, Re, Im, Betrag, Phase\n");

	printf("TEST: [4][3]: %f\n", schwingkreis->ue_fkt[4][3]); 

	for (int i = 0; i < schwingkreis->schritte; i++) {
		for (int y = 0; y < 5; y++) {
			fprintf(out, "%f", schwingkreis->ue_fkt[i][y]);
			if(y != 4) fprintf(out, TZ);
		}
		fprintf(out, "\n");
	}
}

void free_memory(schwingkreis_t *schwingkreis) {

	for (int i = 0; i < schwingkreis->schritte; i++) {
		free(schwingkreis->ue_fkt[i]);
	}
	free(schwingkreis->ue_fkt);
}