#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main (int argc, char *argv[])
{
	int  numtasks, rank;
	int linii, limita1, limita2, limita3;
	char task[500];
	//char *task = malloc(9 * sizeof(char));
	
	int vocale = 0, consoane = 0;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (numtasks != 6) {
		printf("please run with: mpirun --oversubscribe -np 6 %s\n", argv[0]);
		MPI_Finalize();	
		exit(0);
	}
	
	if (rank == 0) {
	
		FILE* file = fopen(argv[1], "r"); 

		if(!file){
			printf("\n Unable to open : %s ", argv[1]);
			return -1;
		}

		char line[500][500];
		int k = 0;

		while (fgets(line[k], sizeof(line[0]), file)) {
			//printf("%s\n", line[k]);
			k++; 
		}

		linii = atoi(line[0]);
		limita1 = linii / 3, limita2 = limita1 + (linii / 3), limita3 = linii;
		
		MPI_Send (&limita1, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
		MPI_Send (&limita1, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
		MPI_Send (&limita2, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
		MPI_Send (&limita2, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);
		MPI_Send (&limita3, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);

		for (int i = 1; i <= limita1; i++) {
			MPI_Send (line[i], sizeof(line[i]), MPI_CHAR, 1, 0, MPI_COMM_WORLD);
		}

		for (int i = limita1 + 1; i <= limita2; i++) {
			MPI_Send (line[i], sizeof(line[i]), MPI_CHAR, 2, 0, MPI_COMM_WORLD);
		}

		for (int i = limita2 + 1; i <= limita3; i++) {
			MPI_Send (line[i], sizeof(line[i]), MPI_CHAR, 3, 0, MPI_COMM_WORLD);
		}

		fclose(file);
	}
	
	if (rank == 1) {
		MPI_Status status;
		MPI_Recv (&limita1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		for (int i = 1; i <= limita1; i++) {
			MPI_Recv (task, sizeof(task), MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
			printf ("Proces[1] : %s", task);
			for (int j = 0; j < (int) strlen(task); j++) {
				if (task[j] == '\n')
					continue;
				if (strchr("aeiouAEIOU", task[j]) != NULL)
					vocale++;
				if (strchr("aeiouAEIOU", task[j]) == NULL)
					consoane++;
			
			}
		}
		printf ("Proces[1] : v = %d; c = %d\n", vocale, consoane);
		MPI_Send(&vocale, 1, MPI_INT, 4, 0, MPI_COMM_WORLD);
		MPI_Send(&consoane, 1, MPI_INT, 5, 0, MPI_COMM_WORLD);
	}
	
	if (rank == 2) {
		MPI_Status status;
		MPI_Recv (&limita1, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		MPI_Recv (&limita2, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		for (int i = limita1 + 1; i <= limita2; i++) {
			MPI_Recv (task, sizeof(task), MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
			printf ("Proces[2] : %s", task);
			for (int j = 0; j < strlen(task); j++) {
				if (task[j] == '\n')
					continue;
				if (strchr("aeiouAEIOU", task[j]) != NULL)
					vocale++;
				if (strchr("aeiouAEIOU", task[j]) == NULL)
					consoane++;
			}
		}
		
		printf ("Proces[2] : v = %d; c = %d\n", vocale, consoane);
		MPI_Send(&vocale, 1, MPI_INT, 4, 0, MPI_COMM_WORLD);
		MPI_Send(&consoane, 1, MPI_INT, 5, 0, MPI_COMM_WORLD);
	}
	
	if (rank == 3) {
		MPI_Status status;
		MPI_Recv (&limita2, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		MPI_Recv (&limita3, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
		for (int i = limita2 + 1; i <= limita3; i++) {
			MPI_Recv (task, sizeof(task), MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
			printf ("Proces[3] : %s", task);
			for (int j = 0; j < strlen(task); j++) {
				if (task[j] == '\n')
					continue;
				if (strchr("aeiouAEIOU", task[j]) != NULL)
					vocale++;
				if (strchr("aeiouAEIOU", task[j]) == NULL)
					consoane++;
			}
		}
		
		printf ("Proces[3] : v = %d; c = %d\n", vocale, consoane);
		MPI_Send(&vocale, 1, MPI_INT, 4, 0, MPI_COMM_WORLD);
		MPI_Send(&consoane, 1, MPI_INT, 5, 0, MPI_COMM_WORLD);
	}
	
	if (rank == 4) {
		MPI_Status status;
		MPI_Recv (&vocale, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
		int received;
		MPI_Recv (&received, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, &status);
		vocale += received;
		MPI_Recv (&received, 1, MPI_INT, 3, 0, MPI_COMM_WORLD, &status);
		vocale += received;
		printf ("Vocale = %d\n", vocale);
	}
	
	if (rank == 5) {
		MPI_Status status;
		MPI_Recv (&consoane, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
		int received;
		MPI_Recv (&received, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, &status);
		consoane += received;
		MPI_Recv (&received, 1, MPI_INT, 3, 0, MPI_COMM_WORLD, &status);
		consoane += received;
		printf ("Consoane = %d\n", consoane);
	}
	
    	MPI_Finalize();
}
