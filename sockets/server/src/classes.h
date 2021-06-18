#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct clase;
typedef struct clase Clase;
// int active_players[5] = {-1, -1, -1, -1, -1};

struct clase
{
    int type;  // 0: Cazador, 1: Médico, 2: Hacker, 3: GreatJagRuz, 4: Ruzalos, 5: Ruiz
    char* name; //nombre de usuario
    int initial_health;
    int current_health;
    int* prev_actions;  // Esto creo que hay que hacerlo de un tamaño definido
    // La idea es hacer un array con las últimas habilidades utilizadas por la clase
    int last_action;
    int bleeding; //puede ser de 0 a 3
    bool intoxicated;  // 0: no, 1: si
    int rounds_intoxicated;
    int fuerza_bruta; //cuando llega a 3 hace daño
    bool duplicated_attack; // 0: no, 1: si
    int rounds_duplicated; // cuando llega a 2 duplicated_attack se vuekve a 0
    bool distracted; // 0: no, 1: si
    Clase* target; //indica el cazador que debe ser atacado en el próximo turno si está distraido
};

Clase* clase_init(int type);

//jugadores
void estocada(Clase* attacker, Clase* enemy);
void corte_cruzado(Clase* attacker, Clase* enemy);
void distraer(Clase* attacker, Clase* enemy);

void curar(Clase* attacker, Clase* friend);
void destello_regenerador(Clase* attacker, Clase** friends, int num_friends, Clase* enemy);
void descarga_vital(Clase* attacker, Clase* enemy);

void inyeccion_sql(Clase* attacker, Clase* enemy);
void ataque_ddos(Clase* attacker, Clase* enemy);
void fuerza_bruta(Clase* attacker, Clase* enemy);

//Monstruos
void ruzgar(Clase* attacker, Clase* enemy);
void coletazo(Clase* attacker, Clase** enemies_array, int enemies_length);
void salto(Clase* attacker, Clase* enemy);
void espina_venenosa(Clase* attacker, Clase* enemy);
int introduce_player(int type);
bool active_class(int type);
void copia(Clase* attacker, Clase* enemy, Clase** friends, int num_friends);