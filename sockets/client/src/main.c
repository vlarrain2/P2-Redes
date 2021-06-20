#include <unistd.h>
#include <stdio.h>
#include "conection.h"
#include "comunication.h"


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
      char* option = get_input();
      // getchar(); //Para capturar el "enter" que queda en el buffer de entrada stdin
      // char* class;
      // if (option == 1){
      //   class = "Cazador";
      // }
      // else if (option == 2){
      //   class = "Médico";
      // }
      // else if (option == 3){
      //   class = "Hacker";
      // }
      client_send_message(server_socket, 1, option);
    }

    if (msg_code == 2) { //Recibimos un mensaje del servidor pidiendo nombre
      char * message = client_receive_payload(server_socket);
      printf("El servidor dice que soy de clase: %s\n", message);
      free(message);
      char* name;
      printf("Ingrese nombre de usuario:");
      name = get_input();
      client_send_message(server_socket, 2, name);

    }

    if (msg_code == 3) { //Mensaje lider para iniciar el juego
      char * message = client_receive_payload(server_socket);
      printf("El server dice: %s\n", message);
      free(message);

      printf("\n   1)Sí\n   2)No\n");
      char* option = get_input(); //Para capturar el "enter" que queda en el buffer de entrada stdin
      client_send_message(server_socket, 3, option);//confirmación inicio de juego
    }

    if (msg_code == 4){
      char * message = client_receive_payload(server_socket);
      printf("########################\n");
      printf("%s\n", message);
      printf("########################\n");
      free(message);
      //int option = getchar() - '0'; //Para capturar el "enter" que queda en el buffer de entrada stdin
      //getchar();

      char* option = get_input();

      //printf("Ingrese objetivo:\n");
      //char* objective = get_input();
      client_send_message(server_socket, 4, option);//confirmación inicio de juego

    }
    if (msg_code == 5){
      client_send_message(server_socket, 3, "hola");
    }
    if (msg_code == 6)
    {
      char * message = client_receive_payload(server_socket);
      printf("########################\n");
      printf("%s\n", message);
      printf("########################\n");
      free(message);

      char* obj = get_input();
      client_send_message(server_socket, 5, obj);
    }
    printf("------------------\n");
  }


  // Se cierra el socket
  close(server_socket);
  free(IP);

  return 0;
}
