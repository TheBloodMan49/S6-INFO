import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class ClientPlusOuMoins {
    public static void main(String[] args) {
        try {
            Socket s = new Socket("localhost", 9999);
            BufferedReader in = new BufferedReader(new InputStreamReader(s.getInputStream()));
            PrintWriter out = new PrintWriter(s.getOutputStream(), true);

            BufferedReader kb = new BufferedReader(new InputStreamReader(System.in));
            String input;
            while ((input = kb.readLine()) != null) {
                out.println(input);
                switch (in.readLine()) {
                    case "1":
                        System.out.println("Trop petit");
                        break;
                    case "-1":
                        System.out.println("Trop grand");
                        break;
                    case "0":
                        System.out.println("Gagné");
                        s.close();
                        return;
                    case "wtf":
                        System.out.println("Entrez un nombre valide");
                        break;
                }
            }

            s.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
