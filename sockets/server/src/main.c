#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comunication.h"
#include "conection.h"
#include "classes.h"

Clase* players[5];
int num_of_players = 0;

char * habilities(int type){
  char * message;
  if (type == 0){
    message = "¿Qué habilidad escoge?\n   1)Estocada\n   2)Corte Cruzado\n   3)Distraer\n";
  }
  else if (type == 1){
    message = "¿Qué habilidad escoge?\n   1)Curar\n   2)Destello regenerador\n   3)Descarga vital\n";
  }
  else if (type == 2){
    message = "¿Qué habilidad escoge?\n   1)Inyección SQL\n   2)Ataque DDOS\n   3)Fuerza Bruta\n";
  }
  return message;
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
      num_of_players++;
      printf("El cliente %d es de clase %d", my_attention + 1, players[my_attention]->type);
      
      // Le enviamos la respuesta
      server_send_message(sockets_array[my_attention], 2, client_class);
    }
    
    else if (msg_code == 2){ //El cliente le envía un mensaje al otro cliente
      char * client_message = server_receive_payload(sockets_array[my_attention]);
      players[my_attention] -> name = client_message;
      printf("El cliente %d tiene clase %d y nombre %s\n", my_attention + 1, players[my_attention] -> type, players[my_attention] -> name);
      //printf("Cliente %d ingresado correctamente:\n    nombre: %s\n", my_attention + 1, client_message);
      printf("Servidor dando la bienvenida a jugador %d\n", my_attention+2);
      if (my_attention == 3){
        //mensaje para líder, para iniciar el juego
        my_attention = 0;
        server_send_message(sockets_array[0], 3, "¿Quieres iniciar el juego?"); 
      }
      else{
        // Mi atención cambia al otro socket
        my_attention++;
        server_send_message(sockets_array[my_attention], 1, "Bienvenido!");
      }
    }
    if (msg_code == 3){
      //eleccion de monstruo y se inicia el primer turno
      char * message = server_receive_payload(sockets_array[my_attention]);
      printf("Cliente %d dice %s\n", my_attention + 1, message);

      int monster = atoi(message);
      if (monster != 4){
        players[4] = clase_init(monster + 2);
        num_of_players++;
      }
      else{
        //monstruo aleatorio
        int num = (rand() % (5 - 3 + 1)) + 3;  // se elige un random entre 3 y 5
        players[4] = clase_init(num);
        num_of_players++;
      }

      char * hability_msg = habilities(players[my_attention]->type);
      server_send_message(sockets_array[my_attention], 4, hability_msg);

    }
    if (msg_code == 4)
    {
      char* habilidad = server_receive_payload(sockets_array[my_attention]);
      players[my_attention] -> habilidad = atoi(habilidad);
      char message[1024] = "";
      char player[1024] = "";
      for (int i = 0; i < num_of_players; i++)
      {
        sprintf(player, "%d) %s\n", i + 1, players[i] -> name);
        strcat(message, player);
      }
      server_send_message(sockets_array[my_attention], 6, message);
    }

    if (msg_code == 5)
    {
      //dependiendo de la clase, s manejan en orden las funciones, 
      //esto según el ataque que se elija.
      char * objective = server_receive_payload(sockets_array[my_attention]);
      int obj = atoi(objective) - 1;
      int class = players[my_attention] -> type;
      printf("Habilidad de %s: %d\n", players[my_attention] -> name, players[my_attention] -> habilidad);

      printf("______ESTE ES EL ESTADO ACTUAL DEL JUEGO______:\n");
      for (int i = 0; i < 5; i++)
      {
        printf("%s : %d  -> VIDA ACTUAL = %f / %d [%f] \n", players[i]->name, players[i] -> type, players[i] -> current_health, players[i] -> initial_health, (players[i] -> current_health / players[i] -> initial_health));
      }
      printf("_____________________________\n");

      if (players[my_attention] -> habilidad == 1){
        //manejo funcion 1
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
      if (players[my_attention] -> habilidad == 2){
        //manejo funcion 2
        if (class == 0){
          corte_cruzado(players[my_attention], players[obj]);
        }

        else if (class == 1){
          destello_regenerador(players[my_attention], players, num_of_players - 1, players[obj]);
        }

        else if (class == 2){
          ataque_ddos(players[my_attention], players[obj]);
        }
      }
      if (players[my_attention] -> habilidad == 3){
        //manejo funcion 3
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
      my_attention = (my_attention + 1) % 4;

      if (my_attention == 0)
      { 
        //antes de volver al jugador 1, el monstruo debe jugar
        int mons_obj = (rand()%4); //elige uno de los 4 jugadores
        int mons_class = players[4]->type;
        printf("Monstruo atacando a %s\n", players[mons_obj] -> name);

        if (mons_class == 3)
        {
          great_jagruz_turn(players[5], players[mons_obj]);
        }
        else if (mons_class == 4)
        {
          ruzalos_turn(players[5], players[mons_obj]);
        }
        else if (mons_class == 5)
        {
          ruiz_turn(players[5], players[mons_obj]);
        }

      }


      char * hability_msg = habilities(players[my_attention]->type);
      server_send_message(sockets_array[my_attention], 4, hability_msg);
    }

    if (msg_code == 6)
    {
      //elegir monstruo
      //Great JagRuz - Ruzalos -  Ruiz, el Gemelo Malvado del Profesor Ruz - aleatorio
      char* response = server_receive_payload(sockets_array[my_attention]);
      int message = atoi(response);
      if (message == 2)
      {
        server_send_message(sockets_array[my_attention], 3, "¿Quieres iniciar el juego?");
      } 
      
      else if (message == 1)
      {
        printf("Se inicia el juego\n");
        
        char* monsters = "¿Qué monstruo eliges?\n   1)Great JagRuz\n   2)Ruzalos\n   3)Ruiz, el Gemelo Malvado del Profesor Ruz\n   4)Aleatorio\n";
        server_send_message(sockets_array[my_attention], 7, monsters);
      }
    }

    
    printf("------------------\n");
  }

  return 0;
}
