/*********************************************************************************************************************************************
****Laborátorio de Sistemas Operacionais
****      Trabalho do Grau A (TGA)
****  Autores: Fábio, Fernando, Nadine
****      Turma: GR16031-00011 
*********************************************************************************************************************************************/
//includes
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>


/*********************************************************************************************************************************************/
//global variables
int input_value=0;
char statusWebBrowser[50]="Parado",
     statusTextEditor[50]="Parado",
     statusTerminal[50]="Parado",
     statusKillProcess[50]=" ";
pid_t pidWebBrowser,
      pidTextEditor,
      pidTerminal;
struct sigaction sinal;

void initiateSigAction(struct sigaction* p_conf_sinal);
void treatSignal(int signum);

/*********************************************************************************************************************************************/
//home menu (display default)
void home (void)
{
  system("clear\n\n");
  //      12345678901234567890123456789012345678901234567890123456789012
  printf("    **************************************************************\n"
         "    **                <<<< Aplications Menu >>>>                **\n"
         "    **************************************************************\n"
         "    **                              |                           **\n"
         "    **     1) Web Browser           | %-26s**\n"
         "    **     2) Text Editor           | %-26s**\n"
         "    **     3) Terminal              | %-26s**\n"
         "    **     4) Finalizar Processo    | %-26s**\n"
         "    **     5) Quit                  |                           **\n"
         "    **                              |                           **\n"
         "    **************************************************************\n"
         "    **            Copyright Fábio, Fernando e Nadine            **\n"
         "    **************************************************************\n"
         , statusWebBrowser, statusTextEditor, statusTerminal, statusKillProcess);
}

/*********************************************************************************************************************************************/
//execução de programas externos
void exec_program(char *path, char *command, char status[50], char *URL){
  pid_t pid;
  if((pid = fork()) < 0){
    perror("Erro no fork!");
  }
  else if(pid == 0){
    if(URL != NULL){
      execlp(path, command, URL, NULL);
    } 
      else {
       execlp(path, command, NULL, NULL);
      }
  }
  else{
    switch(input_value)
    {
    case 1:
      pidWebBrowser = pid;
      sprintf(status, "Executando, PID=%d", pidWebBrowser);
    case 2:
      pidTextEditor = pid;
      sprintf(status, "Executando, PID=%d", pidTextEditor);
    case 3:
      pidTerminal = pid;
      sprintf(status, "Executando, PID=%d", pidTerminal);
    }
  }
}

/*********************************************************************************************************************************************/
//Finalização do processo solicitado
void terminate_process(int process_pid, char status[50])
{
  int kill_status = 0;
  kill_status = kill(process_pid, SIGTERM);
  if (kill_status == 0)
  {
    strcpy(status, "concluido");
    strcpy(statusKillProcess, "concluido");
  }
   else if(kill_status == -1)
   {
     strcpy(status, "falhou");
     strcpy(statusKillProcess, "falhou");
   }
}
/*********************************************************************************************************************************************/
// Controle de exibição dos status
void initiateSigAction(struct sigaction* p_conf_sinal){
	memset(p_conf_sinal, 0, sizeof(&p_conf_sinal));
	sigemptyset(&p_conf_sinal->sa_mask);
	p_conf_sinal->sa_handler = treatSignal;
}

void treatSignal(int signum){
	pid_t pid;
	int status;
	if(signum == SIGCHLD){
		pid = waitpid(-1, &status, WNOHANG);
		if(pid == pidWebBrowser){
			pidWebBrowser = 0;
			if(statusWebBrowser == 0)
				strcpy(statusWebBrowser, "Abortado");
		} else if(pid == pidTextEditor){
			pidTextEditor = 0;
			if(statusTextEditor == 0)
				strcpy(statusTextEditor, "Abortado");
		} else if(pid == pidTerminal){
			pidTerminal = 0;
			if(statusTerminal = 0)
				strcpy(statusTerminal, "Abortado");
		}
	} else if(signum == SIGINT) {
	    	home();
	}
	  
}


/*********************************************************************************************************************************************/
//input reading
int input_reading(void)
{
  initiateSigAction(&sinal);
  sigaction(SIGINT, &sinal, NULL);
  sigaction(SIGCHLD, &sinal, NULL);
  char URL[200];
  int num_app = 0;
  input_value=0;
  printf("\n        Opção: ");
  scanf("%d",&input_value);
  printf("\n");
  switch(input_value)
  {
    case 1:
        printf("Digite a URL que deve abrir no Web Browser: \n");
  	scanf("%s",URL);
        exec_program("/usr/bin/firefox", "firefox", statusWebBrowser, URL);
        break;
    case 2:
        exec_program("/usr/bin/gedit", "gedit", statusTextEditor, NULL);
        break;
    case 3:
        exec_program("/usr/bin/xterm", "xterm", statusTerminal, NULL);
        break;
    case 4:
        printf("Digite o número da aplicação que deseja finalizar: ");
        scanf("%d",&num_app);
        switch(num_app)
        {
          case 1:
            terminate_process(pidWebBrowser, statusWebBrowser);
            break;
          case 2:
            terminate_process(pidTextEditor, statusTextEditor);
            break;
          case 3:
            terminate_process(pidTerminal, statusTerminal);
            break;
          default:
            printf("Número inválido! Digite um número entre 1 e 3.\n");
            sleep(2);
            break;
        }
        break;
    case 5:
        printf("Opção 'Quit' selecionada!");
        break;
    default:
        printf("Dado inválido! Selecione uma opção entre 1 e 5.\n");
//   ATENÇÃO!!!!
//        input_value=5;    (Acredito que aqui não seja necessário parar a execução...)
        break;
  }
  printf("\n");
  sleep(1);

  return input_value;
}

/*********************************************************************************************************************************************/
//main code
int main(void)//void or int?
{
//    ATENÇÃO!!!!
//   home();                        (home() e input_reading() estavam sendo chamados aqui e dentro do loop,
//   input_value=input_reading();   duplicando saídas como o quit, sendo necessário digitar 5 duas vezes para encerrar a execução...)

//Start home menu and input reading. It does while there isn't the value in the input.
  do
  {
    

    home();

    input_value=input_reading();

  } while (input_value!=5);

  printf("Até a próxima!\n\n");
  sleep(2);
//if int main
  return 0;
}
