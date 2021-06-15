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
    introduce_player(type);

    return clase;
}

int introduce_player(int type) //pobla la cantidad de jugadores activos cuando se crean, retorna 1 si no hay espacio
{
    for (int i = 0; i < 5; i++){
        if (active_players[i] != NULL){active_players[i] = type;}
        return 0;
    }
    return 1;
}

bool active_class(int type) //revisa si existe un jugador de clase type activo
{
    for (int i = 0; i < 5; i++)
    {
        if (active_players[i] == type){return true;}
    }
    return false;
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
    int classes = [0, 0, 0];
    for (int i = 0; i < 3; i++)
    {
        if (active_class(i)){classes[i] = 1;}
    }
    while (1)
    {
        int dice = rand() % 3;
        if (classes[dice] == 1 && dice == 0)
        {
            int spell_dice = rand() % 3;
            if (spell_dice == 0){return estocada(attacker, enemy);}
            else if (spell_dice == 1){return corte_cruzado(attacker, enemy);}
            else {return distraer(attacker, enemy);}//tiene que distraer a un jugador
        }
        else if (classes[dice] == 1 && dice == 1)
        {
            int spell_dice = rand() % 3;
            if (spell_dice == 0){return curar(attacker, attacker);} //se tiene que healear a si mismo
            else if (spell_dice == 1){return destello_regenerador(attacker, enemy);} // revisar como funciona destello_regenerador para hacer que el Ruz se healee a si mismo cuando copie esta habilidad
            else {return descarga_vital(attacker, enemy);}
        }
        else if (classes[dice] == 1 && dice == 3)
        {
            int spell_dice = rand() % 3;
            if (spell_dice == 0){return inyeccion_sql(attacker, attacker);}
            else if (spell_dice == 1){return ataque_ddos(attacker, enemy);} 
            else {return fuerza_bruta(attacker, enemy);}
        }
    }
}