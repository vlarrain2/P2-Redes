#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct clase;
typedef struct clase Clase;

struct clase
{
    int type;  // 0: Cazador, 1: Médico, 2: Hacker, 3: GreatJagRuz, 4: Ruzalos, 5: Ruiz
    int initial_health;
    int current_health;
    int* prev_actions;  // Esto creo que hay que hacerlo de un tamaño definido
    // La idea es hacer un array con las últimas habilidades utilizadas por la clase
    int last_action;
    int bleeding; //puede ser de 0 a 3, luego de 3 se reinicia y blood pasa a 0.
    bool blood; // 0: no, 1: si
    bool intoxicated;  // 0: no, 1: si
    int rounds_intoxicated;
    int fuerza_bruta; //cuando llega a 3 hace daño
};

Clase* clase_init(int type);

//jugadores
void estocada(Clase* attacker, Clase* enemy);
void corte_cruzado(Clase* attacker, Clase* enemy);
void distraer(Clase* attacker, Clase* enemy);

void curar(Clase* attacker, Clase* friend);
void destello_regenerador(Clase* attacker, Clase* friend);
void descarga_vital(Clase* attacker, Clase* enemy);

void inyeccion_sql(Clase* attacker, Clase* enemy);
void ataque_ddos(Clase* attacker, Clase* enemy);
void fuerza_bruta(Clase* attacker, Clase* enemy);

//Monstruos
void ruzgar(Clase* attacker, Clase* enemy);
void coletazo(Clase* attacker, Clase** enemies_array, int enemies_length);
void salto(Clase* attacker, Clase* enemy);
void espina_venenosa(Clase* attacker, Clase* enemy);