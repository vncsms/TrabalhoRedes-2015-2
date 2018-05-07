#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>



int main(int argc, char *argv[]){
	/***********************************VARIAVEIS*********************************/
	int s , tempo;
	struct sockaddr_in dest;
	FILE * auxx;
	struct timeval inicio, final;
	int tamanho = 0; // tamanho total transferido
	char entrada[2048] , saida[2048]; // Variaveis de entrada = entrada do write , saida saida do write
	/*****************************************************************************/

	if (argc != 4){
		printf("Erro no número de argumentos");
		return 0;
	}

	/*********************************** INICIALIZANDO ***************************/
	s = socket(AF_INET , SOCK_STREAM , 0); // criacao da estrutura socket
	bzero(&dest , sizeof(dest));
	dest.sin_family = AF_INET;
	dest.sin_port = htons(atoi(argv[2]));
	inet_aton(argv[1] , &dest.sin_addr.s_addr);
	/*****************************************************************************/

	/*********************************** ABERTURA ATIVA **************************/
	connect(s,(struct sockaddr*)&dest,sizeof(dest));
	/*****************************************************************************/

	/************************************ COMUNICANDO ****************************/
	strcpy(entrada , "CONECTAR");
	write(s , entrada , strlen(entrada) +1);
	tamanho += strlen(entrada);

	gettimeofday(&inicio, NULL);
	read(s,saida , 2048);

	if(!strcmp(saida , "CONECTAR ACK")){

		/*ENVIANDO HOST E PORTO*/
		strcpy(entrada , argv[1]);
		strcat(entrada , " ");
		strcat(entrada , argv[2]);
		
		write(s , entrada , strlen(entrada)+1);
		tamanho += strlen(entrada);
		
		read (s , saida   , 2048);
		if(!strcmp(saida , "ACK")){
			printf("Nome recebido\n");
		}
		/***********************/
		
		/*Capturando os arquivos do diretório*/


		char aux[2048];
		remove("temp.temp");
		auxx = fopen("temp.temp" , "a+");
		fclose(auxx);
		strcpy( aux , "ls "); 
		strcat( aux , argv[3]);
		strcat( aux , " > temp.temp ");
		system (aux);


		auxx = fopen("temp.temp" , "a+");
		char buf[2048];
		while( (fgets(buf, 2048, auxx))!=NULL ){
			strcpy( entrada , buf);
			write(s , entrada , strlen(entrada) +1);
			tamanho += strlen(entrada);
			read (s , saida  ,2048);
		}
		/*ENVIANDO MENSAGEM DE ENCERRAMENTO   */
		strcpy(entrada , "TCHAU");
		write(s , entrada , strlen(entrada) +1);
		tamanho += strlen(entrada);
		gettimeofday(&final, NULL);
		/**************************************/

	}
	/*******************************************************************************/

	tempo = (int) (1000 * (final.tv_sec - inicio.tv_sec) + (final.tv_usec - inicio.tv_usec) / 1000);
    	printf("tempo decorrido: %d milisegundos\n", tempo);
	printf("tamanho total de dados: %d bytes\n" , tamanho);
	printf("vazão: %f bytes/segundos\n" , ( (float)tamanho / (float) tempo )*1000);

	/*********************************** ENCERRANDO ********************************/
	close(s);
	fclose(auxx);
	remove("temp.temp");
	/*******************************************************************************/
}
