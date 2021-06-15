#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct clase;
typedef struct clase Clase;
int active_players = [NULL, NULL, NULL, NULL, NULL];

struct clase
{
    int type;  // 0: Cazador, 1: Médico, 2: Hacker, 3: GreatJagRuz, 4: Ruzalos, 5: Ruiz
    int initial_health;
    int current_health;
    int* prev_actions;  // Esto creo que hay que hacerlo de un tamaño definido
    // La idea es hacer un array con las últimas habilidades utilizadas por la clase
    int last_action;
    bool intoxicated;  // 0: no, 1: si
    int rounds_intoxicated;
};

Clase* clase_init(int type);

void ruzgar(Clase* attacker, Clase* enemy);
void coletazo(Clase* attacker, Clase** enemies_array, int enemies_length);
void salto(Clase* attacker, Clase* enemy);
void espina_venenosa(Clase* attacker, Clase* enemy);
int introduce_player(int type);
bool active_class(int type);