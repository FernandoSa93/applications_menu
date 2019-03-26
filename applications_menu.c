/*********************************************************************************************************************************************
****Laborátorio de Sistemas Operacionais
****      Trabalho do Grau A (TGA)
****  Autores: Fábio, Fernando, Nadine
****      Turma: GR16031-00011 
*********************************************************************************************************************************************/
//includes
#include <stdio.h>
#include <stdlib.h>
//#include <sys/types.h>
#include <unistd.h>
#include <string.h>

/*********************************************************************************************************************************************/
//global variables
int input_value=0;

/*********************************************************************************************************************************************/
//home menu (display default)
void home (void)
{
  system("clear\n\n");
  //      12345678901234567890123456789012345678901234567890123456789012
  printf("    **************************************************************\n");
  printf("    **                <<<< Aplications Menu >>>>                **\n");
  printf("    **************************************************************\n");
  printf("    **                              |                           **\n");
  printf("    **     1) Web Browser           |                           **\n");
  printf("    **     2) Text Editor           |                           **\n");
  printf("    **     3) Terminal              |                           **\n");
  printf("    **     4) Finalizar Processo    |                           **\n");
  printf("    **     5) Quit                  |                           **\n");
  printf("    **                              |                           **\n");
  printf("    **************************************************************\n");
  printf("    **            Copyright Fábio, Fernando e Nadine            **\n");
  printf("    **************************************************************\n");
}

/*********************************************************************************************************************************************/
//menu refresh (update display)

/*********************************************************************************************************************************************/
//input reading
int input_reading(void)
{
  input_value=0;
  printf("\n        Opção: ");
  scanf("%d",&input_value);
  printf("\n");
  switch(input_value)
  {
    case 1:
        printf("=1");
        break;
    case 5:
        printf("Opção 'Quit' selecionado!");
        break;
    default:
        printf("Dado inválido!");
        input_value=5; 
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
   home();
   input_value=input_reading();
//Start home menu and input reading. It does while there isn't the value in the input.
  do
  {
    home();
    input_value=input_reading();
  } while (input_value!=5);

  printf("Até a próxima!\n\n");
  sleep(1);
//if int main
  return 0;
}
