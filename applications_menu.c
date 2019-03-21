#include <stdio.h>
//Esboço do menu
int main() {
  int opcao = 0;
  while (opcao != 5) {
    printf("<<<< Aplications Menu >>>\n"
           "      1) Web Browser\n"
           "      2) Text Editor\n"
           "      3) Terminal\n"
           "      4) Finalizar Processo\n"
           "      5) Quit\n"
           "Opção: ");
    scanf("%d", &opcao);
  }
}
