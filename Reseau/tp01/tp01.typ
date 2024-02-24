#import "insa-template/template.typ": *

#show: compte-rendu.with(
  titre: "Réseau",
  auteurs: (
    "Paul Gasnier",
    "Léo Breidenstein"
  ),
  numéro: 1,
  date: "15/02/2024"
)

= Environnement IP et Ethernet

== Question 1
Hostname: Zeus
Adresse IP: `10.9.25.85`

== Question 2
Il y a 4 interfaces réseau sur la machine:
- `lo`: MTU 65536 Bytes
- `eno2`: MTU 1500 Bytes
- `wlan0`: MTU 1500 Bytes
- `docker0`: MTU 1500 Bytes

== Question 3
Les adresses `172.21.X.X` routent vers docker0.
Le reste route vers wlan0.

== Question 4
Après avoir ssh sur un VPS distant, on peut voir que ces informations diffèrent selon la machine.

= Analyse ARP et ICMP

== Question 5
`
Address                  HWtype  HWaddress           Flags Mask            Iface
10.9.30.198              ether   90:e8:68:3e:c1:2d   C                     wlan0
_gateway                 ether   00:09:0f:09:00:1a   C                     wlan0
10.9.25.250              ether   02:74:78:6a:62:72   C                     wlan0
10.9.31.88               ether   82:ce:6b:4f:a9:15   C                     wlan0
10.9.27.171              ether   e0:0a:f6:70:7b:65   C                     wlan0
10.9.26.216              ether   60:ab:67:c2:c8:07   C                     wlan0
`

== Question 6
Je suis ma machine donc wireshark est disponible peu importe l'heure !

== Question 10
Le message ARP est broadcast sur le réseau:\
Il contient l'addresse à résoudre et l'addresse de la machine qui envoie le message.\
La réponse contient l'addresse MAC de la machine qui a l'addresse IP demandée.

== Question 11
`
1212	9.142983206	Intel_d5:45:8f	Broadcast	ARP	42	Who has 10.9.30.142? Tell 10.9.25.85
1213  9.146701271	Intel_61:93:5b	Intel_d5:45:8f	ARP	42	10.9.30.142 is at dc:21:48:61:93:5b
`

== Question 12
Contenu du ping:
- 64 octets de données
- Le type de requête (request ou reply)
- Un checksum
- Un identifiant
- Un numéro de séquence
- Un timestamp

== Question 13
La requête ping est envoyée à l'adresse IP de la machine cible
avec 64 octets de données. La réponse contient les mêmes données.

= IP fragmentation

== Questions 14 et 15
`
811	5.823528754	10.9.25.85	10.9.30.142	IPv4	1514	Fragmented IP protocol (proto=ICMP 1, off=0, ID=2071) [Reassembled in #812]
812	5.823559559	10.9.25.85	10.9.30.142	ICMP	562	Echo (ping) request  id=0x0008, seq=1/256, ttl=64 (reply in 857)

850	6.054728582	10.9.30.142	10.9.25.85	IPv4	1514	Fragmented IP protocol (proto=ICMP 1, off=0, ID=a2ed) [Reassembled in #857]
857	6.055436792	10.9.30.142	10.9.25.85	ICMP	562	Echo (ping) reply    id=0x0008, seq=1/256, ttl=64 (request in 812)
`

Le paquet IP spécifie qu'il est le premier (offset=0) d'un groupe fragmenté (More fragments=1).\
Sa taille est de 1500 octets (=MTU) + 14 octets d'entête Ethernet.

Le paquet ICMP a un offset de 1480 octets (=MTU - 20 octets d'entête IP) donc ce n'est pas le premier.
More fragments=0 indique que c'est le dernier fragment.

== Question 16
Les flags et l'offset sont différents entre les deux paquets.

= Nommage

== Question 17
Le Japon et les Etats Unis sont loins.
L'addresse `www.kyoto-u.ac.jp` est très courte à joindre donc elle est probablement hébergée en Europe.

== Question 19
- `www.free.fr` -> 212.27.48.10
- `www.insa-rennes.fr` -> 10.5.1.3 (Sur INSA Wifi)

= TCP et HTTP

== Question 20
Le serveur HTTP est sur le port 80.
C'est un port well-known.

== Question 21
L'entête TCP fait 40 octets.

== Question 22
Le segment est vide car ce paquet est le premier d'un handshake.

== Question 23
Le numéro de séquence est 142776364.

La taille de la fenêtre est 32120 octets.\
Cela veut dire que le client peut recevoir 32120 octets de données avant que le serveur ne doive attendre un acquittement.

== Question 24

==== Premier paquet (SYN), headers Ethernet
Source: `00:09:0f:09:00:1a`\
Destination: `f0:77:c3:d5:45:8f`\
Type: `IPv4`

==== Deuxième paquet (SYN-ACK), headers IP
Source Address: `34.223.124.45`\
Destination Address: `10.9.25.85`\
Protocol: `TCP (6)`

==== Deuxième paquet (SYN-ACK), headers TCP
Acknowledgment number (raw): `142776365`\
Acknowledgment number (relative): `1`

TCP Segment Len: `0`

Sequence Number (raw): `2344971599`\
Sequence number (relative): `1`

La taille de la fenêtre est 26847 octets.

== Question 25
La MSS (Maximum Segment Size) est de 1460 octets.

== Question 26

==== Troisième paquet (ACK), headers TCP
Acknowledgment number (raw): `2344971600`\
Acknowledgment number (relative): `1`

Sequence Number (raw): `142776365`\
Sequence number (relative): `1`

La taille de la fenêtre est 32128 octets.

== Question 27
Les numéros de séquence et d'acquittement sont incrémentés de la taille des données envoyées.

== Question 28
Le header HTTP signale 2238 octets de données.

== Question 29
On peut penser que le paquet va être fragmenté à la couche IP car la taille des données est supérieure à la MTU.
Selon Wireshark, la fragmentation des données s'effectue à la couche TCP: on observe deux paquets TCP rassemblés en un paquet HTTP.
