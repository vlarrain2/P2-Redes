#include "classes.h"


Clase* clase_init(int type)
{
    Clase* clase = malloc(sizeof(Clase));
    if (type == 0)
    {
        clase -> type = type;
        clase -> initial_health = 5000;
        clase -> current_health = clase -> initial_health;
    }

    else if (type == 1)
    {
        clase -> type = type;
        clase -> initial_health = 3000;
        clase -> current_health = clase -> initial_health;
    }

    else if (type == 2)
    {
        clase -> type = type;
        clase -> initial_health = 2500;
        clase -> current_health = clase -> initial_health;
    }

    else if (type == 3)
    {
        clase -> type = type;
        clase -> initial_health = 10000;
        clase -> current_health = clase -> initial_health;
    }

    else if (type == 4)
    {
        clase -> type = type;
        clase -> initial_health = 20000;
        clase -> current_health = clase -> initial_health;
    }

    else if (type == 5)
    {
        clase -> type = type;
        clase -> initial_health = 25000;
        clase -> current_health = clase -> initial_health;
    }

    return clase;
}

void ruzgar(Clase* attacker, Clase* enemy)
{
    enemy -> current_health -= 1000;
    if (enemy -> current_health <= 0)
    {
        printf("El jugador murió\n");
    }
}

void coletazo(Clase* attacker, Clase** enemies_array, int enemies_length)
{
    for (int i = 0; i < enemies_length; i++)
    {
        enemies_array[0]->current_health -= 500;
        if (enemies_array[0]->current_health <= 0)
        {
            printf("El jugador murió\n");
        }
    }
}

void salto(Clase* attacker, Clase* enemy)
{
    if (attacker->last_action != 11)
    {
        enemy->current_health -= 1500;
        if (enemy->current_health <= 0)
        {
            printf("El jugador murió\n");
        }
    }
}

void espina_venenosa(Clase* attacker, Clase* enemy)
{
    if (enemy->intoxicated)
    {
        enemy->current_health -= 500;
        if (enemy->current_health <= 0)
        {
            printf("El jugador murió\n");
        }
    }
    else
    {
        // Hay que hacer que cada turno se le quite 450 de health
        enemy->intoxicated = 1;
        enemy->rounds_intoxicated = 3;
    }
}

void copia(Clase* attacker, Clase* enemy)
{
    // Hay que es
}