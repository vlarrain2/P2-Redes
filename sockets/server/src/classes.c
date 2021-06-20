#include "classes.h"

int introduce_player(Clase* player) //pobla la cantidad de jugadores activos cuando se crean, retorna 1 si no hay espacio
{
    for (int i = 0; i < 4; i++){
        if (!active_players[i]){active_players[i] = player;}
        return 0;
    }
    return 1;
}

int count_players()
{
    int n = 0;
    for (int i = 0; i < 4; i++)
    {if (active_players[i]){n += 1;}}
    return n;
}

Clase* clase_init(int type, int id)
{
    Clase* clase = malloc(sizeof(Clase));
    clase -> id = id;
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
        clase -> name = "Great JagRuz";
    }

    else if (type == 4)
    {
        clase -> type = type;
        clase -> initial_health = 20000;
        clase -> current_health = clase -> initial_health;
        clase -> name = "Ruzalos";
    }

    else if (type == 5)
    {
        clase -> type = type;
        clase -> initial_health = 25000;
        clase -> current_health = clase -> initial_health;
        clase -> name = "Ruiz, el Gemelo Malvado del Profesor Ruz";
    }
    clase -> bleeding = 0; //puede ser de 0 a 3
    clase -> intoxicated = 0;  // 0: no, 1: si
    clase -> rounds_intoxicated = 0;
    clase -> fuerza_bruta = 0; //cuando llega a 3 hace daño
    clase -> duplicated_attack = 0; // 0: no, 1: si
    clase -> rounds_duplicated = 0; // cuando llega a 2 duplicated_attack se vuekve a 0
    clase -> jumped = 0; // 0: no ha usado la habilidad "salto", 1: ya uso la habilidad "salto"
    clase -> distracted = 0; // 0: no, 1: si
    clase -> reprobado = 0;// 0: normal, 1: reprobado
    clase -> lider = 0;
    //clase -> name = name;
    int aver = introduce_player(clase);

    return clase;
}


bool active_class(int type) //revisa si existe un jugador de clase type activo
{
    for (int i = 0; i < 4; i++)
    {
        if (active_players[i]->type == type){return true;}
    }
    return false;
}
// Clases

void estocada(Clase* attacker, Clase* enemy)
{
    enemy -> current_health -= 1000;
    if (enemy->current_health <= 0)
    {
        printf("El jugador %s murió\n", enemy -> name);
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
        printf("El jugador %s murió\n", enemy -> name);
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

void destello_regenerador_ruiz(Clase* attacker, Clase* enemy)
{
    int num = (rand() % (2000 - 750 + 1)) + 750;  // numero random
    int regeneracion = (int)(num/2);  // redondeo hacia arriba
    // no se puede tener más que la vida inicial
    if ((attacker -> current_health + regeneracion) >= attacker->initial_health)
    {
        attacker -> current_health = attacker -> initial_health;
    }
    else
    {
        attacker -> current_health += regeneracion;
    }

    enemy -> current_health -= num;
    if (enemy->current_health <= 0)
    {
        printf("El jugador %s murió\n", enemy -> name);
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
    if (enemy->current_health <= 0)
    {
        printf("El jugador %s murió\n", enemy -> name);
    }
}

void descarga_vital(Clase* attacker, Clase* enemy)
{
    int dano = 2 * (attacker -> initial_health - attacker -> current_health);
    enemy -> current_health -= dano;
    if (enemy->current_health <= 0)
    {
        printf("El jugador %s murió\n", enemy -> name);
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
        printf("El jugador %s murió\n", enemy -> name);
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
            enemy->current_health = 0;
            printf("El jugador %s murió\n", enemy -> name);
        }
        attacker -> fuerza_bruta = 0;
    }
    
}

// Monstruos
void ruzgar(Clase* attacker, Clase* enemy)
{
    if (enemy -> reprobado){enemy -> current_health -= 1500;}
    else {enemy -> current_health -= 1000;}
    if (enemy->current_health <= 0)
    {
        enemy->current_health = 0;
        printf("El jugador %s murió\n", enemy -> name);
    }
}

void coletazo(Clase* attacker)
{
    for (int i = 0; i < 4; i++)
    {
        if (active_players[i] != NULL)
        {
            if (active_players[i] -> reprobado){active_players[i] -> current_health -= 750;}
            else {active_players[i] -> current_health -= 500;}
            if (active_players[i] -> current_health <= 0)
            {
                active_players[i] -> current_health = 0;
                printf("El jugador %s murió\n", active_players[i] -> name);
            }
        }
    }
}

void salto(Clase* attacker, Clase* enemy)
{
    if (enemy -> reprobado == 1){enemy->current_health -= 1.5 * 1500;}
    else{enemy->current_health -= 1500;}
    
    if (enemy->current_health <= 0)
    {
        enemy->current_health = 0;
        printf("El jugador %s murió\n", enemy->name);
    }
}

void espina_venenosa(Clase* attacker, Clase* enemy)
{
    if (enemy->intoxicated)
    {
        if (enemy -> reprobado){enemy->current_health -= 750;}
        else{enemy->current_health -= 500;}
        if (enemy->current_health <= 0)
        {
            enemy -> current_health = 0;
            printf("El jugador %s murió\n", enemy->name);
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
    int classes[3] = {0, 0, 0};
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
            else if (spell_dice == 1){return destello_regenerador_ruiz(attacker, enemy);} // revisar como funciona destello_regenerador para hacer que el Ruz se healee a si mismo cuando copie esta habilidad
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

void reprobatron(Clase* attacker, Clase* enemy)
{
    enemy -> reprobado = 1;
}

void sudo_rm_rf(Clase* attacker, Clase* enemy)
{
    for (int i = 0; i < 4; i++)
    {
        if (active_players[i] != NULL)
        {
            if (active_players[i] -> reprobado == 1)
            {
                if(active_players[i] -> current_health >= 1.5 * 100 * rounds){enemy->current_health -= 1.5 * 100 * rounds;}
                else{active_players[i] -> current_health = 0;}
            }
            else
            {
                if(active_players[i] -> current_health >= 100 * rounds){enemy->current_health -= 100 * rounds;}
                else{active_players[i] -> current_health = 0;}
            }
        }
    }
    rounds = 0;
}

void great_jagruz_turn(Clase* attacker, Clase* enemy)
{
    int dice = rand() % 2;
    if (dice == 0){ruzgar(attacker, enemy);}
    else {coletazo(attacker);}
}

void ruzalos_turn(Clase* attacker, Clase* enemy)
{
    if (attacker -> jumped == 1)
    {
        espina_venenosa(attacker, enemy);
        attacker -> jumped = 0;
    }
    else
    {
        int dice = rand() % 5;
        if (dice < 2)
        {
            salto(attacker, enemy);
            attacker -> jumped = 1;
        }
        else {espina_venenosa(attacker, enemy);}
    }
}

void ruiz_turn(Clase* attacker, Clase* enemy)
{
    int dice = rand() % 5;
    if (dice < 2){copia(attacker, enemy);}
    else if (dice == 2){reprobatron(attacker, enemy);}
    else{sudo_rm_rf(attacker, enemy);}
}

void game_statistics(Clase* enemy)
{
    //3: GreatJagRuz, 4: Ruzalos, 5: Ruiz
    char* type[6];
    type[0] = "Cazador";
    type[1] = "Médico";
    type[2] = "Hacker";
    type[3] = "GreatJagRuz";
    type[4] = "Ruzalos";
    type[5] = "Ruiz";
    printf("______ESTE ES EL ESTADO ACTUAL DEL JUEGO______:\n");
    for (int i = 0; i < 4; i++)
    {
        if (active_players[i] != NULL)
        {printf("%s : %s  -> VIDA ACTUAL = %f / %d [%f] \n", active_players[i]->name, type[active_players[i] -> type], active_players[i] -> current_health, active_players[i] -> initial_health, (active_players[i] -> current_health / active_players[i] -> initial_health));}
    }
    printf("%s  -> VIDA ACTUAL = %f / %d [%f]", type[enemy -> type], enemy -> current_health, enemy -> initial_health, (enemy -> current_health / enemy -> initial_health));
    printf("_____________________________\n");
}
