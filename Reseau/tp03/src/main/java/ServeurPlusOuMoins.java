import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class ServeurPlusOuMoins {
    public static void main(String[] args) throws IOException {
        ServerSocket server = new ServerSocket(9999);

        while (true) {
            try {
                Socket s = server.accept();
                System.out.println("Nouvelle connexion");
                BufferedReader in = new BufferedReader(new InputStreamReader(s.getInputStream()));
                PrintWriter out = new PrintWriter(s.getOutputStream(), true);

                int randNb = (int) Math.ceil((Math.random() * 100));
                while (true) {
                    try {
                        int guess = Integer.parseInt(in.readLine());
                        if (guess < randNb) {
                            out.println("1");
                        } else if (guess > randNb) {
                            out.println("-1");
                        } else {
                            out.println("0");
                            s.close();
                            break;
                        }
                    } catch (NumberFormatException e) {
                        out.println("wtf");
                    } catch (IOException e) {
                        s.close();
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
}
