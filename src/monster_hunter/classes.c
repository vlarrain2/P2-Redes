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

// Clases

void estocada(Clase* attacker, Clase* enemy)
{
    enemy -> current_health -= 1000;
    if (enemy->current_health <= 0)
    {
        printf("El jugador murió\n");
    }
    if (enemy -> bleeding < 3) {
        enemy -> bleeding++;
    }
}

void corte_cruzado(Clase* attacker, Clase* enemy)
{
    enemy -> current_health -= 3000;
    if (enemy->current_health <= 0)
    {
        printf("El jugador murió\n");
    }
}

void distraer(Clase* attacker, Clase* enemy)
{
    //el enemy en el proximo turno debe atacar al último
    //cazador en ocupar distraer  
    enemy -> distracted = 1;
    enemy -> target = attacker;
}

void curar(Clase* attacker, Clase* friend)
{
    //no se puede tener más que la vida inicial
    if ((friend -> current_health + 2000) >= friend->initial_health)
    {
        friend -> current_health = friend -> initial_health;
    }
    else
    {
        friend -> current_health += 2000;
    }
}

void destello_regenerador(Clase* attacker, Clase** friends, int num_friends, Clase* enemy)
{
    int num = (rand() % (2000 - 750 + 1)) + 750; //numero random
    int regeneracion = (int)(num/2); // redondeo hacia arriba
    
    int num_friend = rand() % (num_friends + 1);
    Clase* friend = friends[num_friend];
    //no se puede tener más que la vida inicial
    if ((friend -> current_health + regeneracion) >= friend->initial_health)
    {
        friend -> current_health = friend -> initial_health;
    }
    else
    {
        friend -> current_health += regeneracion;
    }

    enemy -> current_health -= num;
}

void descarga_vital(Clase* attacker, Clase* enemy)
{
    int dano = 2 * (attacker -> initial_health - attacker -> current_health);
    enemy -> current_health -= dano;
    if (enemy->current_health <= 0)
    {
        printf("El jugador murió\n");
    }
}

void inyeccion_sql(Clase* attacker, Clase* friend)
{
    // bool de inyeccion y numero que dure 2 turnos? para duplicar mientras bool sea true
    friend -> duplicated_attack = 1;
    friend -> rounds_duplicated = 0;
}

void ataque_ddos(Clase* attacker, Clase* enemy)
{
    enemy -> current_health -= 1500;
    if (enemy->current_health <= 0)
    {
        printf("El jugador murió\n");
    }
}


void fuerza_bruta(Clase* attacker, Clase* enemy)
{
    attacker -> fuerza_bruta ++;
    if (attacker -> fuerza_bruta == 3)
    {
        enemy->current_health -= 10000;
        if (enemy->current_health <= 0)
        {
            printf("El jugador murió\n");
        }
        attacker -> fuerza_bruta = 0;
    }
    
}

// Monstruos

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