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

            while (1)
            {
              n_jugador += 1;
              message = "Bienvenido a Monster Hunter: Ruiz\n";
              server_send_message(new_socket, 1, message);
              id_received = server_receive_id(new_socket); //Recibe nombre del jugador
              if (id_received == 1)
              {
                char * name = server_receive_payload(new_socket);
                printf("-------------------\n");
                message = "Elige la clase que quieres ocupar\n[1] Cazador\n[2] Médico\n[3] Hacker\n";
                server_send_message(new_socket, 1, message);
                int type = server_receive_id(new_socket);
                if ( type == 1){printf("JUGADOR %d: %s => Cazador\n", n_jugador, name);}
                else if ( type == 2){printf("JUGADOR %d: %s => Médico\n", n_jugador, name);}
                else{printf("JUGADOR %d: %s => Hacker\n", n_jugador, name);}
                Clase* jugador = clase_init(type, name);
                if (new_socket == 4)
                {
                    jugador -> lider = TRUE;
                    printf("%s es el líder de la party\n", name);
                    message = "[1] para iniciar la partida\n";
                    server_send_message(new_socket, 1, message);
                }
                break;
              }
            }

                 
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
                if (i == 0)
                {
                    if (server_receive_id(client_socket[i]) == 1)
                    {
                        if (n_jugador == count_players())
                        {launch_game = 1;printf("PARTIDA INICIADA");}
                    }
                }
                //Check if it was for closing , and also read the 
                //incoming message 
                if ((valread = read( sd , buffer, 1024)) == 0)  
                {  
                    //Somebody disconnected , get his details and print 
                    getpeername(sd , (struct sockaddr*)&address , \
                        (socklen_t*)&addrlen);  
                    printf("Host disconnected , ip %s , port %d \n" , inet_ntoa(address.sin_addr) , ntohs(address.sin_port));  
                         
                    //Close the socket and mark as 0 in list for reuse 
                    close( sd );  
                    client_socket[i] = 0;  
                }  
                     
                //Echo back the message that came in 
                else 
                {  
                    //set the string terminating NULL byte on the end 
                    //of the data read 
                    buffer[valread] = '\0';  
                    send(sd , buffer , strlen(buffer) , 0 );  
                }

            }  
        }  
    }  
}