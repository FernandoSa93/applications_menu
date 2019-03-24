#include <stdio.h>
#include <stdlib.h>
//#include <sys/types.h>
#include <unistd.h>
#include <string.h>

//Esboço do menu
int main(){
  int opcao = 0;
  char statusWebBrowser[50]="Parado",
       statusTextEditor[50]="Parado",
       statusTerminal[50]="Parado", 
       statusFinalizarProcesso[50]=" ";
  
  //Funções
  void executaTextEditor(){
    system("gedit&");
    sleep(1);
    strcpy(statusTextEditor, "Executando, PID=");
  }

  void quit(){
    sleep(1);
    printf("Bye\n\n");
  }
  
  while (opcao != 5){
    system("clear");
    printf("<<<< Applications Menu >>>\n"
           "      1) Web Browser         (%s)\n"
           "      2) Text Editor         (%s)\n"
           "      3) Terminal            (%s)\n"
           "      4) Finalizar Processo  (%s)\n"
           "      5) Quit\n"
           "Opção: ", statusWebBrowser, statusTextEditor, statusTerminal, statusFinalizarProcesso);
    scanf("%d", &opcao);

    if (opcao>5 || opcao<1){
      printf("Comando inválido! Selecione uma opção entre 1 e 5.\n");
      sleep(2);
      continue;
    }

    switch(opcao){
      case 2: executaTextEditor();
      case 5: quit();
    }
  }
}
