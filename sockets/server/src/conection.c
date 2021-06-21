#include "conection.h"

PlayersInfo * prepare_sockets_and_get_clients(char * IP, int port)
{
    int opt = TRUE;  
    int master_socket , addrlen , new_socket , client_socket[4] , max_clients = 4 , activity, i , valread , sd;  
    int max_sd;
    char * message;
    struct sockaddr_in address;  
    int id_received;
         
    char buffer[1025];  //data buffer of 1K 
         
    //set of socket descriptors 
    fd_set readfds;  
         
    //a message 
    PlayersInfo * sockets_clients = malloc(sizeof(PlayersInfo));
     
    //initialise all client_socket[] to 0 so not checked 
    for (i = 0; i < max_clients; i++)  
    {  
        client_socket[i] = 0;  
    }  
         
    //create a master socket 
    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) == 0)  
    {  
        perror("socket failed");  
        exit(EXIT_FAILURE);  
    }  
     
    //set master socket to allow multiple connections , 
    //this is just a good habit, it will work without this 
    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEPORT, (char *)&opt, sizeof(opt)) < 0 )  
    {  
        perror("setsockopt");  
        exit(EXIT_FAILURE);  
    }  
     
    //type of socket created 
    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    //address.sin_addr.s_addr = INADDR_ANY;
    inet_aton(IP, &address.sin_addr);
    address.sin_port = htons(port);
    

         
    //bind the socket to localhost port 8888 
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)  
    {  
        perror("bind failed");  
        exit(EXIT_FAILURE);  
    }  
    printf("Listener on port %d \n", port);  
         
    //try to specify maximum of 3 pending connections for the master socket 
    if (listen(master_socket, 3) < 0)  
    {  
        perror("listen");  
        exit(EXIT_FAILURE);  
    }  
         
    //accept the incoming connection 
    addrlen = sizeof(address);  
    puts("Waiting for connections ...");
    int n_jugador = 0;
    int launch_game = 0;
    int cant_jugadores = -1;
    char* names[4] = {NULL, NULL, NULL, NULL};
    int types[4] = {0, 0, 0, 0};
    bool mostro_electo = false;
    //set_active_players();
         
    while(1)  
    {

        if (launch_game == 1)
        {
            if (active_players[0]){sockets_clients -> socket_c1 = client_socket[0];}
            if (active_players[1]){sockets_clients -> socket_c2 = client_socket[1];}
            if (active_players[2]){sockets_clients -> socket_c3 = client_socket[2];}
            if (active_players[3]){sockets_clients -> socket_c4 = client_socket[3];}
            return sockets_clients;
        }

        //clear the socket set 
        FD_ZERO(&readfds);  
     
        //add master socket to set 
        FD_SET(master_socket, &readfds);  
        max_sd = master_socket;  
             
        //add child sockets to set 
        for ( i = 0 ; i < max_clients ; i++)  
        {  
            //socket descriptor 
            sd = client_socket[i];  
                 
            //if valid socket descriptor then add to read list 
            if(sd > 0)  
                FD_SET( sd , &readfds);  
                 
            //highest file descriptor number, need it for the select function 
            if(sd > max_sd)  
                max_sd = sd;  
        }  
     
        //wait for an activity on one of the sockets , timeout is NULL , 
        //so wait indefinitely 
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
       
        if ((activity < 0) && (errno!=EINTR))
        {
            printf("select error");
        }
             
        //If something happened on the master socket , 
        //then its an incoming connection 
        if (FD_ISSET(master_socket, &readfds))  
        {   
            if ((new_socket = accept(master_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)  //accept(server_socket, (struct sockaddr *)&client1_addr, &addr_size);
            {  
                perror("accept");  
                exit(EXIT_FAILURE);  
            }  
             
            //inform user of socket number - used in send and receive commands 
            printf("New connection , socket fd is %d , ip is : %s , port : %d \n" , new_socket , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));  
           
            //send new connection greeting message 

            n_jugador += 1;
            message = "Bienvenido a Monster Hunter: Ruiz\n ¿Cuál es tu nombre?\n";
            server_send_message(new_socket, 1, message);


                 
            //add new socket to array of sockets 
            for (i = 0; i < max_clients; i++)  
            {  
                //if position is empty 
                if( client_socket[i] == 0 )  
                {  
                    client_socket[i] = new_socket;  
                    printf("Adding to list of sockets as %d\n" , i);  
                    break;
                }  
            }  
        }  
             
        //else its some IO operation on some other socket
        for (i = 0; i < max_clients; i++)  
        { 
            sd = client_socket[i];  

            if (FD_ISSET( sd , &readfds))  
            {  
                id_received = server_receive_id(sd); //Recibe tipo de mensaje
                char * payload_received = server_receive_payload(sd);
                //Check if it was for closing , and also read the 
                //incoming message 
                if (id_received == 0)  
                {  
                    //Somebody disconnected , get his details and print 
                    getpeername(sd , (struct sockaddr*)&address , \
                        (socklen_t*)&addrlen);  
                    printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));  
                         
                    //Close the socket and mark as 0 in list for reuse 
                    close( sd );  
                    client_socket[i] = 0;  
                }
                else if (!names[i])
                {
                    message = "Elige la clase que quieres ocupar\n[1] Cazador\n[2] Médico\n[3] Hacker\n";
                    server_send_message(sd, 1, message);
                    names[i] = payload_received;
                }
                else if (!types[i])
                {
                    int type = atoi(payload_received); //cambiar
                    if ( type == 1)
                    {
                        types[i] = type;
                        Clase* jugador = clase_init(types[i]-1, names[i], i);
                        active_players[i] = jugador;
                        if (i != 0){
                        char buffer[60];
                        snprintf(buffer, sizeof(buffer), "SE HA UNIDO EL JUGADOR %d: (%s) => Cazador\n", n_jugador, names[i]);
                        server_send_message(client_socket[0], 0, buffer);
                        }

                    }
                    else if ( type == 2)
                    {
                        printf("JUGADOR %d: (%s) => Médico\n", n_jugador, names[i]);
                        types[i] = type;
                        Clase* jugador = clase_init(types[i]-1, names[i],i);
                        active_players[i] = jugador;
                        if (i != 0){
                        char buffer[60];
                        snprintf(buffer, sizeof(buffer), "SE HA UNIDO EL JUGADOR %d: (%s) => Medico\n", n_jugador, names[i]);
                        server_send_message(client_socket[0], 0, buffer);
                        }
                    }
                    else if ( type == 3)
                    {
                        printf("JUGADOR %d (%s) => Hacker\n", n_jugador, names[i]);
                        types[i] = type;
                        Clase* jugador = clase_init(types[i]-1, names[i],i);
                        active_players[i] = jugador;
                        if (i != 0){
                        char buffer[60];
                        snprintf(buffer, sizeof(buffer), "SE HA UNIDO EL JUGADOR %d: (%s) => Hacker\n", n_jugador, names[i]);
                        server_send_message(client_socket[0], 0, buffer);
                        }
                    }
                    else{message = "Elección inválida. \nElige la clase que quieres ocupar\n[1] Cazador\n[2] Médico\n[3] Hacker\n";server_send_message(sd, 1, message);}
                    if ((type == 1) || (type == 2) || (type == 3))
                    {
                        if (i == 0){
                            message = "Elige el monstruo contra el que quieres combatir\n[4] Great JagRuz\n[5] Ruzalos\n[6] Ruiz, el Gemelo Malvado del Profesor Ruz\n--SI QUIERES REVISAR SI SE HAN UNIDO NUEVOS JUGADORES, PRESIONA ENTER SIN ELEGIR OPCIÓN--\n";
                            server_send_message(sd, 1, message);
                            printf("%s es el líder de la party\n", names[i]);
                        }
                        else{
                            message = "Espera a que el líder elija un MOSTRO e inicie la partida.\n";
                            server_send_message(sd, 0, message);
                        }
                    }
                }
                else if ((i == 0) && (!mostro_electo))
                {
                    int mostro = atoi(payload_received);
                    if ( mostro == 4){printf("VAN A PELEAR CONTRA GREAT JAGRUZ\n");mostro_electo = true; enemy = clase_init(mostro - 1, "MOSTRO", 5);}
                    else if ( mostro == 5){printf("VAN A PELEAR CONTRA RUZALOS\n");mostro_electo = true; enemy = clase_init(mostro -1, "MOSTRO", 5);}
                    else if ( mostro == 6){printf("VAN A PELEAR CONTRA RUIZ, EL GEMELO MALVADO DEL PROFESOR RUZ\n");mostro_electo = true; enemy = clase_init(mostro - 1, "MOSTRO", 5);}
                    else if (mostro == 7)
                    {
                        printf("Eligiendo un monstruo aleatorio...\n");
                        mostro = (rand() % 3) + 4;
                        if ( mostro == 4){printf("VAN A PELEAR CONTRA GREAT JAGRUZ\n");mostro_electo = true; enemy = clase_init(mostro - 1, "MOSTRO", 5);}
                        else if ( mostro == 5){printf("VAN A PELEAR CONTRA RUZALOS\n");mostro_electo = true; enemy = clase_init(mostro - 1, "MOSTRO", 5);}
                        else if ( mostro == 6){printf("VAN A PELEAR CONTRA RUIZ, EL GEMELO MALVADO DEL PROFESOR RUZ\n");mostro_electo = true; enemy = clase_init(mostro - 1, "MOSTRO", 5);}
                    }
                    else{message = "\nElige el monstruo contra el que quieres combatir\n[4] Great JagRuz\n[5] Ruzalos\n[6] Ruiz, el Gemelo Malvado del Profesor Ruz\n[7] Monstruo Aleatorio\n--SI QUIERES REVISAR SI SE HAN UNIDO NUEVOS JUGADORES, PRESIONA ENTER SIN ELEGIR OPCIÓN--\n";server_send_message(sd, 1, message);}
                    if ((mostro == 4) || (mostro == 5) || (mostro == 6))
                    {
                        if (i == 0){
                            message = "SI ESTÁN TODOS LOS JUGADORES LISTOS, PRESIONA ENTER PARA INICIAR LA PARTIDA\n";
                            server_send_message(sd, 1, message);
                        }
                        else{
                            message = "Espera a que el líder elija un MOSTRO e inicie la partida.\n";
                            server_send_message(sd, 0, message);
                        }
                    }
                }
                else if (i == 0)
                {
                    //printf("cant_jugadores = %d     /     count_players() = %d\n", n_jugador, count_players());
                    if (n_jugador == count_players())
                    {
                        launch_game = 1;
                    }
                    else
                    {
                        message = "ASEGÚRATE DE QUE ESTÉN TODOS LISTOS E INTÉNTALO NUEVAMENTE\n";
                        server_send_message(sd, 1, message);
                    }
                }
                // int type = server_receive_id(new_socket);
                // if ( type == 1){printf("JUGADOR %d: %s => Cazador\n", n_jugador, payload_received);}
                // else if ( type == 2){printf("JUGADOR %d: %s => Médico\n", n_jugador, payload_received);}
                // else{printf("JUGADOR %d: %s => Hacker\n", n_jugador, payload_received);}
                // Clase* jugador = clase_init(type, payload_received);
                // if (new_socket == 4)
                // {
                //     jugador -> lider = TRUE;
                //     printf("%s es el líder de la party\n", payload_received);
                //     server_send_message(new_socket, 1, message);
                //     cant_jugadores = server_receive_id(new_socket);
                // }


                // if ((cant_jugadores == count_players()) && (i == 0))
                // {
                //     printf("cant_jugadores = %d     /     count_players() = %d\n", cant_jugadores, count_players());
                //     message = "Todos los jugadores listos, [1] para iniciar partida\n";
                //     server_send_message(client_socket[i], 1, message);
                //     if (server_receive_id(client_socket[i]) == 1)
                //     {
                //         if (n_jugador == count_players())
                //         {launch_game = 1;printf("PARTIDA INICIADA");}
                //     }
                // }

            }  
        }  
    }  
}