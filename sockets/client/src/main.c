#include <unistd.h>
#include <stdio.h>
#include "conection.h"
#include "comunication.h"
//#include "../../classes.h"

char * get_input(){
  char * response = malloc(20);
  int pos=0;
  while (1){
    char c = getchar();
    if (c == '\n') break;
    response[pos] = c;
    pos++;
  }
  response[pos] = '\0';
  return response;
}


int main (int argc, char *argv[]){
  //Se obtiene la ip y el puerto donde está escuchando el servidor (la ip y puerto de este cliente da igual)
  char * IP = "0.0.0.0";
  int PORT = 8080;

  // Se prepara el socket
  int server_socket = prepare_socket(IP, PORT);

  // Se inicializa un loop para recibir todo tipo de paquetes y tomar una acción al respecto
  while (1){
    int msg_code = client_receive_id(server_socket);

    if (msg_code == 1) { //Recibimos un mensaje del servidor
      char * message = client_receive_payload(server_socket);
      printf("%s\n", message);
      free(message);

      printf("####################\n¿Qué clase desea?\n   1)Cazador\n   2)Médico\n   3)Hacker\n####################\n");
      int option = getchar() - '0';
      getchar(); //Para capturar el "enter" que queda en el buffer de entrada stdin
      char* class;
      if (option == 1){
        class = "Cazador";
      }
      else if (option == 2){
        class = "Médico";
      }
      else if (option == 3){
        class = "Hacker";
      }
      client_send_message(server_socket, 1, class);
    }

    if (msg_code == 2) { //Recibimos un mensaje del servidor pidiendo nombre
      char * message = client_receive_payload(server_socket);
      printf("El servidor dice que soy de clase: %s\n", message);
      free(message);

      int sure = 0;
      char* name;
      printf("Ingrese nombre de usuario:");
      name = get_input();
      // while (sure == 0){
      //   printf("Ingrese nombre de usuario:");
      //   char * name = get_input();
      //   printf("Tu nombre es %s, ¿estas seguro? (y/n)\n", name);
      //   sure = getchar() - '0';
      //   getchar();
      //   while (sure != 0 && sure != 1){
      //     printf("INSTRUCCION INVALIDA. Tu nombre es %s, ¿estas seguro? (1:si, 0: no))\n", name);
      //     sure = getchar() - '0';
      //     getchar();
      //   }
      // }

      client_send_message(server_socket, 3, name);

    }

    if (msg_code == 3) { //Recibimos un mensaje que proviene del otro cliente
      char * message = client_receive_payload(server_socket);
      printf("El otro cliente dice: %s\n", message);
      free(message);

      printf("¿Qué desea hacer?\n   1)Enviar mensaje al servidor\n   2)Enviar mensaje al otro cliente\n");
      int option = getchar() - '0';
      getchar(); //Para capturar el "enter" que queda en el buffer de entrada stdin

      printf("Ingrese su mensaje: ");
      char * response = get_input();

      client_send_message(server_socket, option, response);
    }
    printf("------------------\n");
  }

  // Se cierra el socket
  close(server_socket);
  free(IP);

  return 0;
}
