#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "comunication.h"
#include "conection.h"
#include "classes.h"

Clase* players[4];

char * revert(char * message){
  //Se invierte el mensaje

  int len = strlen(message) + 1;
  char * response = malloc(len);

  for (int i = 0; i < len-1; i++)
  {
    response[i] = message[len-2-i];
  }
  response[len-1] = '\0';
  return response;
}

int main(int argc, char *argv[]){
  // Se define una IP y un puerto
  char * IP = "0.0.0.0";
  int PORT = 8080;

  // Se crea el servidor y se obtienen los sockets de ambos clientes.
  PlayersInfo * players_info = prepare_sockets_and_get_clients(IP, PORT);

  // Le enviamos al primer cliente un mensaje de bienvenida
  char * welcome = "Bienvenido Cliente 1!!";
  server_send_message(players_info->socket_c1, 1, welcome);

  // Guardaremos los sockets en un arreglo e iremos alternando a quién escuchar.
  int sockets_array[4] = {players_info->socket_c1, players_info->socket_c2, players_info->socket_c3, players_info->socket_c4};
  int my_attention = 0;
  while (1)
  {
    // Se obtiene el paquete del cliente 1
    int msg_code = server_receive_id(sockets_array[my_attention]);

    if (msg_code == 1) //El cliente me envió un mensaje a mi (servidor)
    {
      char* client_class = server_receive_payload(sockets_array[my_attention]);
      int class = atoi(client_class);
      class --;
      players[my_attention] = clase_init(class);
      printf("El cliente %d es de clase %d", my_attention + 1, players[my_attention]->type);
      
      // Le enviamos la respuesta
      server_send_message(sockets_array[my_attention], 2, client_class);
    }
    // else if (msg_code ==2) //El cliente me envió un mensaje a mi (servidor)
    // {
    //   char* name = server_receive_payload(sockets_array[my_attention]);
    //   players[my_attention] -> name = name;
    //   printf("El cliente %d tiene clase %d y nombre %s", my_attention + 1, players[my_attention] -> type, players[my_attention] -> name);
    //   server_send_message(sockets_array[my_attention], 2, name);
    // }
    else if (msg_code == 2){ //El cliente le envía un mensaje al otro cliente
      char * client_message = server_receive_payload(sockets_array[my_attention]);
      players[my_attention] -> name = client_message;
      printf("El cliente %d tiene clase %d y nombre %s", my_attention + 1, players[my_attention] -> type, players[my_attention] -> name);
      //printf("Cliente %d ingresado correctamente:\n    nombre: %s\n", my_attention + 1, client_message);
      printf("Servidor dando la bienvenida a jugador %d\n", my_attention+2);
      if (my_attention == 3){
        //mensaje para líder, para iniciar el juego
        server_send_message(sockets_array[0], 3, "¿Quieres iniciar el juego?"); 
      }
      else{
        // Mi atención cambia al otro socket
        my_attention++;
        server_send_message(sockets_array[my_attention], 1, "Bienvenido!");
      }
    }
    if (msg_code == 3){
      //manejo de rondas
      char * message;
      if (players[my_attention]->type == 0){
        message = "¿Qué habilidad escoge?\n   1)Estocada\n   2)Corte Cruzado\n   3)Distraer\n";
      }
      else if (players[my_attention]->type == 1){
        message = "¿Qué habilidad escoge?\n   1)Curar\n   2)Destello regenerador\n   3)Descarga vital\n";
      }
      else if (players[my_attention]->type == 2){
        message = "¿Qué habilidad escoge?\n   1)Inyección SQL\n   2)Ataque DDOS\n   3)Fuerza Bruta\n";
      }
      server_send_message(sockets_array[my_attention], 1, message);
      //my_attention = (my_attention + 1) % 4;

    }
    //dependiendo de la clase, s manejan en orden las funciones, 
    //esto según el ataque que se elija.
    if (msg_code == 4){
      //manejo funcion 1
      char * objective = server_receive_payload(sockets_array[my_attention]);
      int obj = atoi(objective);
      int class = players[my_attention] -> type;
      if (class == 0){
        estocada(players[my_attention], players[obj]);
      }

      else if (class == 1){
        curar(players[my_attention], players[obj]);
      }

      else if (class == 2){
        inyeccion_sql(players[my_attention], players[obj]);
      }
    }
    if (msg_code == 5){
      //manejo funcion 2
      char * objective = server_receive_payload(sockets_array[my_attention]);
      int obj = atoi(objective);
      int class = players[my_attention] -> type;
      if (class == 0){
        corte_cruzado(players[my_attention], players[obj]);
      }

      else if (class == 1){
        //destello_regenerador(players[my_attention], players[obj]);
      }

      else if (class == 2){
        ataque_ddos(players[my_attention], players[obj]);
      }

    }
    if (msg_code == 6){
      //manejo funcion 3
      char * objective = server_receive_payload(sockets_array[my_attention]);
      int obj = atoi(objective);
      int class = players[my_attention] -> type;
      if (class == 0){
        distraer(players[my_attention], players[obj]);
      }

      else if (class == 1){
        descarga_vital(players[my_attention], players[obj]);
      }

      else if (class == 2){
        fuerza_bruta(players[my_attention], players[obj]);
      }
    }
    
    printf("------------------\n");
  }

  return 0;
}
