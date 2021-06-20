#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct clase;
typedef struct clase Clase;
int rounds;
Clase* active_players[4];

struct clase
{
    int id;
    char* name; //nombre de usuario
    int type;  // 0: Cazador, 1: Médico, 2: Hacker, 3: GreatJagRuz, 4: Ruzalos, 5: Ruiz
    int initial_health;
    float current_health;
    int* prev_actions;  // Esto creo que hay que hacerlo de un tamaño definido
    // La idea es hacer un array con las últimas habilidades utilizadas por la clase
    int last_action;
    int bleeding; //puede ser de 0 a 3
    bool intoxicated;  // 0: no, 1: si
    int rounds_intoxicated;
    int fuerza_bruta; //cuando llega a 3 hace daño
    bool duplicated_attack; // 0: no, 1: si
    int rounds_duplicated; // cuando llega a 2 duplicated_attack se vuekve a 0
    bool jumped; // 0: no ha usado la habilidad "salto", 1: ya uso la habilidad "salto"
    bool distracted; // 0: no, 1: si
    bool reprobado;// 0: normal, 1: reprobado
    bool lider;
    Clase* target; //indica el cazador que debe ser atacado en el próximo turno si está distraido
    int habilidad;
};

int count_players();
void set_active_players();
int introduce_player(Clase* player);
Clase* clase_init(int type, int id);

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
void destello_regenerador_ruiz(Clase* attacker, Clase* enemy);
void ruzgar(Clase* attacker, Clase* enemy);
void coletazo(Clase* attacker);
void salto(Clase* attacker, Clase* enemy);
void espina_venenosa(Clase* attacker, Clase* enemy);
void reprobatron(Clase* attacker, Clase* enemy);
void sudo_rm_rf(Clase* attacker, Clase* enemy);
void reprobatron(Clase* attacker, Clase* enemy);
bool active_class(int type);
void game_statistics(Clase* enemy);
void great_jagruz_turn(Clase* attacker, Clase* enemy);
void ruzalos_turn(Clase* attacker, Clase* enemy);
void ruiz_turn(Clase* attacker, Clase* enemy);