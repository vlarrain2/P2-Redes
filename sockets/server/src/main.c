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
    message = "¿Qué habilidad escoge?\n   1)Estocada\n   2)Corte Cruzado\n   3)Distraer\n   4)Me rindo :(\n";
  }
  else if (type == 1){
    message = "¿Qué habilidad escoge?\n   1)Curar\n   2)Destello regenerador\n   3)Descarga vital\n   4)Me rindo :(\n";
  }
  else if (type == 2){
    message = "¿Qué habilidad escoge?\n   1)Inyección SQL\n   2)Ataque DDOS\n   3)Fuerza Bruta\n   4)Me rindo :(\n";
  }
  return message;
}

void die(int my_attention, int* sockets_array)
{
  free(players[my_attention]);
  for (int i = my_attention; i < (num_of_players - 1); i++)
  {
    //free(players[i]);
    players[i] = players[i + 1];
    active_players[i] = active_players[i + 1];
  }
  for (int i = my_attention; i < (num_of_players - 2); i++)
  {
    sockets_array[i] = sockets_array[i + 1];
  }
  players[num_of_players - 1] = NULL;
  active_players[num_of_players - 1] = NULL;
  num_of_players--;
}

int main(int argc, char *argv[]){
  // Se define una IP y un puerto
  char * IP = argv[2];
  int PORT = atoi(argv[4]);

  // Se crea el servidor y se obtienen los sockets de ambos clientes.
  PlayersInfo * players_info = prepare_sockets_and_get_clients(IP, PORT);
  num_of_players = count_players() + 1;
  for (int i=0;i<(num_of_players - 1);i++){
    players[i] = active_players[i];
  }
  players[num_of_players -1] = enemy;

  // Le enviamos al primer cliente un mensaje de bienvenida
  //char * welcome = "Bienvenido Cliente 1!!";
  //server_send_message(players_info->socket_c1, 1, welcome);

  // Guardaremos los sockets en un arreglo e iremos alternando a quién escuchar.
  int sockets_array[4] = {players_info->socket_c1, players_info->socket_c2, players_info->socket_c3, players_info->socket_c4};
  int my_attention = 0;
  char * hability_msg = habilities(players[0]->type);
  server_send_message(sockets_array[0], 4, hability_msg);
  while (1)
  {
    // Se obtiene el paquete del cliente 1
    int msg_code = server_receive_id(sockets_array[my_attention]);

    if (msg_code == 1) //El cliente me envió un mensaje a mi (servidor)
    {
      // char* client_class = server_receive_payload(sockets_array[my_attention]);
      // int class = atoi(client_class);
      // class --;
      // players[my_attention] = clase_init(class, my_attention);
      // num_of_players++;
      // printf("El cliente %d es de clase %d", my_attention + 1, players[my_attention]->type);
      
      // // Le enviamos la respuesta
      // server_send_message(sockets_array[my_attention], 2, client_class);
    }
    
    else if (msg_code == 2){ //El cliente le envía un mensaje al otro cliente
      // char * client_message = server_receive_payload(sockets_array[my_attention]);
      // players[my_attention] -> name = client_message;
      // printf("El cliente %d tiene clase %d y nombre %s\n", my_attention + 1, players[my_attention] -> type, players[my_attention] -> name);
      // //printf("Cliente %d ingresado correctamente:\n    nombre: %s\n", my_attention + 1, client_message);
      // printf("Servidor dando la bienvenida a jugador %d\n", my_attention+2);
      // if (my_attention == 3){
      //   //mensaje para líder, para iniciar el juego
      //   my_attention = 0;
      //   server_send_message(sockets_array[0], 3, "¿Quieres iniciar el juego?"); 
      // }
      // else{
      //   // Mi atención cambia al otro socket
      //   my_attention++;
      //   server_send_message(sockets_array[my_attention], 1, "Bienvenido!");
      // }
    }
    if (msg_code == 3){
      //eleccion de monstruo y se inicia el primer turno
      // char * message = server_receive_payload(sockets_array[my_attention]);
      // printf("Cliente %d dice %s\n", my_attention + 1, message);

      // int monster = atoi(message);
      // if (monster != 4){
      //   players[num_of_players] = clase_init(monster + 2, num_of_players);
      //   num_of_players++;
      // }
      // else{
      //   //monstruo aleatorio
      //   int num = (rand() % (5 - 3 + 1)) + 3;  // se elige un random entre 3 y 5
      //   players[num_of_players] = clase_init(num, num_of_players);
      //   num_of_players++;
      // }

      // char * hability_msg = habilities(players[my_attention]->type);
      // server_send_message(sockets_array[my_attention], 4, hability_msg);

    }
    if (msg_code == 4)
    {
      char* habilidad = server_receive_payload(sockets_array[my_attention]);
      players[my_attention] -> habilidad = atoi(habilidad);
      free(habilidad);
      if (players[my_attention] -> habilidad == 4){
        server_send_message(sockets_array[my_attention], 8, "Te rendiste perdedor\n");
        die(my_attention, sockets_array);
        if (num_of_players == 1)
        {
          printf("Ganó %s\n", players[0] -> name);
          break;
        }
        
        char * hability_msg = habilities(players[my_attention]->type);
        server_send_message(sockets_array[my_attention], 4, hability_msg);
      } else
      {
        char message[1024] = "";
        char player[1024] = "";
        for (int i = 0; i < num_of_players; i++)
        {
          sprintf(player, "%d) %s\n", i + 1, players[i] -> name);
          strcat(message, player);
        }
        server_send_message(sockets_array[my_attention], 6, message);
      }
    }

    if (msg_code == 5)
    {
      //dependiendo de la clase, s manejan en orden las funciones, 
      //esto según el ataque que se elija.
      char * objective = server_receive_payload(sockets_array[my_attention]);
      int obj = atoi(objective) - 1;
      free(objective);
      int class = players[my_attention] -> type;
      printf("Habilidad de %s: %d\n", players[my_attention] -> name, players[my_attention] -> habilidad);

      // printf("______ESTE ES EL ESTADO ACTUAL DEL JUEGO______:\n");
      // for (int i = 0; i < num_of_players; i++)
      // {
      //   printf("%s : %d  -> VIDA ACTUAL = %f / %d [%f] \n", players[i]->name, players[i] -> type, players[i] -> current_health, players[i] -> initial_health, (players[i] -> current_health / players[i] -> initial_health));
      // }
      // printf("_____________________________\n");

      // printf("%s atacando a %s\n", players[my_attention] -> name, players[obj] -> name);
      game_statistics(players[num_of_players-1]);

      //actualizar bleeding
      players[my_attention]->current_health -= (players[my_attention]->bleeding)*500;

      //actualizar en caso de intoxicación
      if ((players[my_attention]->intoxicated == 1) && (players[my_attention]->rounds_intoxicated>0))
      {
        players[my_attention]->current_health -= 450;
        players[my_attention]->rounds_intoxicated --;
      }

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

      int current_player_id = players[my_attention] -> id;
      int dead[5];
      int j = 0;
      for (int i = 0; i < num_of_players; i++)
      {
        if (players[i] -> current_health <= 0)
        {
          dead[j] = i;
          j++;
        }
      }
      for (int i = 0; i < j; i++)
      {
        if (players[dead[i]] -> type < 3)
        {
          server_send_message(sockets_array[dead[i]], 8, "Moriste\n");
        }
        die(dead[i], sockets_array);
      }
      if (num_of_players == 1)
      {
        printf("Ganó %s\n", players[0] -> name);
        break;
      }
      if (players[num_of_players - 1] -> type < 3)
      {
        //si el ultimo jugador no es de tipo monstruo, entonces el monstruo murió y se acaba el juego
        printf("Muerte del monstruo, fin del juego\n");
        for (int i = 0; i < num_of_players; i++)
        {
          server_send_message(sockets_array[i], 8, "El monstruo murió, fin del juego\n");
        }
        break;
      }

      for (int i = 0; i < num_of_players; i++)
      {
        if (players[i] -> id == current_player_id)
        {
          my_attention = i;
          break;
        }
      }


      my_attention = (my_attention + 1) % (num_of_players - 1);

  
      if (my_attention == 0)
      { 
        //antes de volver al jugador 1, el monstruo debe jugar
        int mons_obj = (rand()%(num_of_players - 1)); //elige uno de los 4 jugadores
        int mons_class = players[num_of_players - 1]->type;
        printf("clase del monstruo: %d\n", mons_class);
        printf("Monstruo atacando a %s\n", players[mons_obj] -> name);

        if (mons_class == 3)
        {
          printf("jagruz\n");
          great_jagruz_turn(players[num_of_players], players[mons_obj]);
          printf("jagruz_end\n");
        }
        else if (mons_class == 4)
        {
          printf("ruzalos\n");
          ruzalos_turn(players[num_of_players], players[mons_obj]);
          printf("ruzalos end\n");
        }
        else if (mons_class == 5)
        {
          printf("ruiz\n");
          ruiz_turn(players[num_of_players], players[mons_obj]);
          printf("ruiz end\n");
        }
        printf("Usuario atacado\n");

        if (num_of_players == 1)
        {
          printf("Ganó %s\n", players[0] -> name);
          break;
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
      free(response);
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


  //FREES HECHOS PARA VER EL VALGRIND DEL LOBBY, 0 LEAKS
  free(players_info);
  for (int i = 0; i < 4; i++)
  {
    if (active_players[i])
    {
      //free(players[i]);
      free(active_players[i]);
    }
  }
  free(enemy);

  return 0;
}
