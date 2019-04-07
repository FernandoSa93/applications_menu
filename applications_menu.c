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

/*********************************************************************************************************************************************/
//Variáveis globais
int input_value=0;
char statusWebBrowser[50]="Parado",
     statusTextEditor[50]="Parado",
     statusTerminal[50]="Parado",
     statusKillProcess[50]=" ";
pid_t pidWebBrowser,
      pidTextEditor,
      pidTerminal;
struct sigaction sinal;
sigset_t newmask, oldmask;

//Declaração dos métodos
void initiate_handler(struct sigaction* sign);
void treat_sign(int signum);
void home (void);
void exec_program(char *path, char *command, char status[50], char *URL);
void terminate_process(int process_pid, char status[50]);
int input_reading(void);

/*********************************************************************************************************************************************/
//Instanciador de sinais
//Recebe por parâmetro o sinal e inicia o tratador de sinais.
void initiate_handler(struct sigaction* sign){
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGINT);

  	memset(&sinal, 0, sizeof(sinal));
  	sinal.sa_handler = &treat_sign;
 
  	if(sigaction(SIGALRM, &sinal, NULL) != 0){
    		perror("Falha ao instalar tratador do sinal SIGALRM");
    		exit(-1);
  	}

	if(sigaction(SIGINT, &sinal, NULL) != 0){
    		perror("Falha ao instalar tratador do sinal SIGINT");
    		exit(-1);
  	}

	if(sigaction(SIGCHLD, &sinal, NULL) != 0){
    		perror("Falha ao instalar tratador do sinal SIGCHLD");
    		exit(-1);
  	}
}

/*********************************************************************************************************************************************/
//Tratador de sinais
//Recebe por parâmetro o sinal  
//Executa o tratamento adequado de acordo com o sinal recebido.
void treat_sign(int signum){
        pid_t pid = 0;
        int status;
	char *statusAbortado = "concluido";
	if(signum == SIGCHLD || signum == SIGALRM){
                pid = waitpid(-1, &status, WNOHANG);
                if(pid == pidWebBrowser && pid != 0 && ((strcmp(statusWebBrowser, statusAbortado)) != 0)){
                        pidWebBrowser = 0;
                        strcpy(statusWebBrowser, "Abortado");
                } else if(pid == pidTextEditor && pid != 0 && ((strcmp(statusTextEditor, statusAbortado)) != 0)){
                        pidTextEditor = 0;
                        strcpy(statusTextEditor, "Abortado");
                } else if(pid == pidTerminal && pid != 0 && ((strcmp(statusTerminal, statusAbortado)) != 0)){
                        pidTerminal = 0;
                        strcpy(statusTerminal, "Abortado");
                }
	}
	if (signum == SIGINT){
		printf("\nOpção inválida! CTRL+C pressionado.\n");	
	}		 
}

/*********************************************************************************************************************************************/
//Menu da aplicação
//Nele é exibido os status e PID de cada processo.
void home (void)
{
	system("clear\n\n");
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
//Inicia a execução de programas externos
//Recebe por parâmetro o caminho, comando, status e uma string de url
//O método valida os dados vindos por parâmetro e de acordo estes dados executa determinada ação.
void exec_program(char *path, char *command, char status[50], char *URL)
{
  pid_t pid;
  if((pid = fork()) < 0){
    perror("Erro no fork!");
  }
  else if(pid == 0){
    sigprocmask(SIG_BLOCK, &newmask, &oldmask);
    if(URL != NULL){
      execlp(path, command, URL, NULL);
    }
    else{
      execlp(path, command, NULL, NULL);
    }
  }
  else{
    switch(input_value){
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
//Método que finaliza o processo solicitado
//Recebe por parâmetros o pid e o status atual do processo a ser encerrado
//Após executar o kill ele atualiza o status do processo.
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
//Método que lê a entrada do usuário, válida se é uma entrada esperada e executa o comando adequado.
int input_reading(void)
{
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
	pause();
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
	pause();
        scanf("%d",&num_app);
        switch(num_app)
        {
          case 1:
	   if(pidWebBrowser != 0){
	       	terminate_process(pidWebBrowser, statusWebBrowser);
	   } 
	   else 
	   {
		printf("A aplicação não está executando, portanto, não é possível encerrá-la.");
	   }
           break;
          case 2:
	   if(pidTextEditor != 0){
	       	terminate_process(pidTextEditor, statusTextEditor);
	   } 
	   else 
	   {
		printf("A aplicação não está executando, portanto, não é possível encerrá-la.");
	   }
           break;
          case 3:
	   if(pidTerminal != 0){
             	terminate_process(pidTerminal, statusTerminal);
	   } 
	   else 
	   {
		printf("A aplicação não está executando, portanto, não é possível encerrá-la.");
	   }
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
	if ((!SIGCHLD && !SIGINT && !SIGALRM) && (input_value < 1 || input_value > 5)){
        	printf("Dado inválido! Selecione uma opção entre 1 e 5.\n");
    		break;
	}
  }
  printf("\n");
  sleep(1);

  return input_value;
}

/*********************************************************************************************************************************************/
//Método main da aplicação
//Dá início ao menu inicial e a leitura de entrada.

int main(void)
{

  initiate_handler(&sinal);   

  do
  {  	
    if (alarm(5) < 0){
      perror("Falha ao agendar alarme");
    }
    home();
    input_value=input_reading();
  } while (input_value!=5);

  printf("Até a próxima!\n\n");
  sleep(2);
  return 0;
}
