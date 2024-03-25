cmake_minimum_required(VERSION 3.28)
project(tp04 C)

set(CMAKE_C_STANDARD 11)

add_executable(Client_TCP Client_TCP.c
        Client_plusoumoins.c
        Serveur_plusoumoins.c)
add_executable(Serveur_TCP Serveur_TCP.c)
add_executable(Client_UDP_V2 Client_UDP_V2.c)
add_executable(Serveur_UDP_V2 Serveur_UDP_V2.c)
add_executable(Client_plusoumoins Client_plusoumoins.c)
add_executable(Serveur_plusoumoins Serveur_plusoumoins.c)
