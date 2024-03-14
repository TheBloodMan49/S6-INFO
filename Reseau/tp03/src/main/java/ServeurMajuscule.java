import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class ServeurMajuscule {
    public static void main(String[] args) throws IOException {
            ServerSocket server = new ServerSocket(9999);

            while (true) {
                try {
                    Socket s = server.accept();
                    BufferedReader in = new BufferedReader(new InputStreamReader(s.getInputStream()));
                    PrintWriter out = new PrintWriter(s.getOutputStream(), true);

                    out.println("Bienvenue sur ce serveur de mise en majuscule.\n" +
                            "Entrez un texte ligne par ligne, il sera converti.\n" +
                            "Tapez '.' pour terminer.");

                    String line = in.readLine();
                    while (!line.equals(".")) {
                        out.println(line.toUpperCase());
                        line = in.readLine();
                    }
                    s.close();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
    }
}
