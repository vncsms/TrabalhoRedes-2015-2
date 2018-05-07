#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	
	/************************** VARIAVEIS ***************************/
	int s , client_s;
	struct sockaddr_in self , cliente; // estrutura do servidor e cliente
	int addrlen = sizeof(cliente);
	char entrada[2048] , saida[2048] , endereco[2048]; // Variaveis de entrada = entrada do write , saida saida do write
	FILE * P; // arquivo que serão gravados as arquivos e os diretorios
	/****************************************************************/

	/************************* INICIALIZANDO ************************/
	

	s = socket(AF_INET , SOCK_STREAM , 0); // inicializacao da estrutura

	bzero(&self , sizeof(self));
	self.sin_family = AF_INET;
	self.sin_port = htons(atoi(argv[1]));
	self.sin_addr.s_addr = INADDR_ANY;
	/****************************************************************/

	/********************** ABERTURA PASSIVA ************************/
	bind(s , (struct sockaddr*)&self , sizeof(self));
	listen(s,5);
	/****************************************************************/
	

	/********************* COMUNICANDO      *************************/
	
	client_s = accept (s,(struct sockaddr*)&cliente , &addrlen); // ABERTURA COMPLETA

	read(client_s , saida , 2048);   

	if(!strcmp(saida,"CONECTAR")){
		strcpy(entrada ,  "CONECTAR ACK");
		write(client_s , entrada , strlen(entrada)+1);
	}

	/* RECEBENDO HOST E DIRETORIO*/
	read(client_s , saida , 2048);


	strcpy(endereco , saida);
	strcpy(entrada , "ACK");
	write(client_s , entrada , strlen(entrada)+1);
	/*****************************/	
	remove(endereco);
	P = fopen(endereco , "a+");
	while(1){
		read(client_s , saida , 2048 ); /**Lendo arquivos **/
		if(!strcmp(saida,"TCHAU")){    /** comando tchau finaliza laço **/
			break;
		}
		else{
			fprintf(P,"%s",saida);
			write(client_s , "ACK" , strlen("ACK")+1);
		}
	}


	/****************** FECHANDO COMUNICACAO ************************/
	fclose(P);
	close(client_s);
	/****************************************************************/
	
	return 0;
}
