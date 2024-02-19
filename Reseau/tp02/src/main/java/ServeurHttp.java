import java.net.*;
import java.io.*;
import java.util.*;

public class ServeurHttp
{
    static String nomServeur ="-- Serveur HTTP Java --";
    static String entete ="";
    static String enteteReponse="";
    static int port = 8888;  // port par défaut
    static String racine = "assets/";  // sous répertoire racine du serveur

    static void usage(){
        message("Usage :\n java ServeurHttp [port]\n");
    }

    static void message(String msg){
        System.err.println(msg);
    }

    static void erreur(String msg){
        message("Erreur: "+msg);
    }

    static void erreur(){
        erreur(null);
    }

    static String date(){
        // ou System.currentTimeMillis()
        Date d= new Date();

        return d.toString();
    }

    static String erreur400() {
        String msg=null;

        msg += "HTTP/1.0 400 Bad Request\n";
        msg += "Date: "+date()+"\n";
        msg += "Server: "+entete+"\n";
        msg += "Content-type: text/html\n\n";
        msg += "<HEAD><TITLE>Mauvaise requête</TITLE></HEAD>\n";
        msg += "<BODY><H1>Mauvaise requête</H1>\n";
        msg += "Votre butineur a envoyé une requête que ce serveur ne peut pas (encore) traiter.<P>\n</BODY>\n";
        return msg;
    }

    static String typeMime(String nom)
    {
        if (nom.matches(".*\\.html$"))
            return "text/html";
        if (nom.matches(".*\\.gz$"))
            return "application/gzip";
        else return "text/plain";
    }

    public static void main (String[] argv) {
        ServerSocket socket = null;
        PrintWriter out = null;
        BufferedReader in = null;

        if (argv.length == 1 ){
            port = Integer.parseInt(argv[0]);
        } else if ( argv.length >= 1 )
            usage();

        // Création de la socket
        try {socket = new ServerSocket(port);} catch (IOException e){
            erreur("Impossible d'ouvrir le port "+port+":"+e);}


        while( true ) { // Attente de connexion
            Socket s = null;
            try {s=socket.accept();} catch (IOException e){
                erreur("accept "+e);
            }

            // Journalisation
            System.out.println("["+date()+"] Connexion :"
                    +s.getInetAddress().getHostName()
                    +":"+s.getPort()+" ("
                    +s.getInetAddress().getHostAddress()+") ");
            // Traitement de la requête
            try {
                out =new PrintWriter(s.getOutputStream());
                //		out.println(entete); out.flush();
            } catch (IOException e){
                erreur("Ecriture socket "+e);
            }

            try {
                in = new BufferedReader(new
                        InputStreamReader(s.getInputStream()));

            } catch (IOException e){
                erreur("Lecture socket "+e);
            }

            String requete= null;
            File   fichier= null;

            try { requete = in.readLine();} catch (IOException e){
                erreur("lecture "+e);
            }
            System.out.println(requete);
            // découpage HTTP 1.0
            // GET url HTTP/1.0
            String reqHTTP[]= requete.split("\\s");
            // En première approximation, on ne regarde que le 2ème élt
            String reponse= null;
            if (reqHTTP.length != 3) {
                reponse = erreur400();
                out.println(reponse); out.flush();
                //  Rajouter statistique erreur 400
            } else {
                fichier= new File(racine+reqHTTP[1]);
                if (!fichier.exists()) {
                    reponse =  "HTTP/1.0 404 Not Found\n";
                    reponse += "Date: "+date()+"\n";
                    reponse += "Server: "+nomServeur+"\n";
                    reponse += "Content-type: text/html\n\n";
                    reponse += "<HEAD><TITLE>Fichier Non Trouvé</TITLE></HEAD>\n\n";
                    reponse += "<BODY><H1>Fichier Non Trouvé</H1>\n";
                    reponse += "La ressource <i>"+reqHTTP[1]+"</i> n'est pas présente sur ce serveur.<P>\n\n</BODY>\n";
                    // Réponse au client
                    out.println(reponse); out.flush();

                    //  Rajouter statistique erreur 404

                } else {
                    //  Rajouter statistique accès à ce fichier reqHTTP[1]

                    reponse = "HTTP/1.0 200 OK\r\n";
                    reponse += "Date: "+date()+"\r\n";
                    reponse += "Server: "+nomServeur+"\r\n";
                    // Déterminer le type mime d'après l'extension
                    reponse += "Content-type: "+typeMime(reqHTTP[1])+" \r\n\r\n";
                    // Réponse (partielle) au client
                    out.println(reponse); out.flush();
                    // Transfert du fichier (éventuellement binaire)
                    FileInputStream f = null;
                    try{f= new FileInputStream(fichier);}
                    catch(IOException e){
                        erreur("lecture ressource 1"+e);
                        try{ s.close(); } catch (IOException e2) {
                            System.err.println("Impossible fermer la socket"+e2);
                        }
                        continue;
                    }
                    int lu = -1;

                    try{lu= f.read();} catch(IOException e){
                        erreur("lecture ressource 2 "+e);
                        try{ s.close(); } catch (IOException e2) {
                            System.err.println("Impossible fermer la socket");
                        }
                        continue;
                    }
                    while ( lu != -1  ) {
                        out.write(lu);
                        try{lu= f.read();} catch(IOException e){
                            erreur("lecture ressource 3 "+e);
                        }
                    }
                    out.flush();
                }
            }

            try{ s.close(); } catch (IOException e) {
                System.err.println("Impossible fermer la socket "+e);
            }
        }
    }
}

