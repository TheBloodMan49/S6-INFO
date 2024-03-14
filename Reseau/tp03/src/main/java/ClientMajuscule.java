import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class ClientMajuscule {
    public static void main(String[] args) {
        try {
            Socket s = new Socket("localhost", 9999);
            BufferedReader in = new BufferedReader(new InputStreamReader(s.getInputStream()));
            PrintWriter out = new PrintWriter(s.getOutputStream(), true);
            for (int i = 0; i < 3; i++) {
                System.out.println(in.readLine());
            }
            for (String arg : args) {
                out.println(arg);
                System.out.println(in.readLine());
            }

            out.println(".");

            s.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
