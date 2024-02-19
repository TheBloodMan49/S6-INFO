import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.UnknownHostException;

public class Serveur_UDP {


    public static void main(String[] args)
    {
        DatagramSocket Socket_UDP;
        byte[] sendBuf = new byte[256];

        // Donner au thread un nom qui commence par UDP
        // ---- this.setName("UDP"+Thread.currentThread().getName());

        // Ouvrir un socket UDP
        try
        {
            Socket_UDP = new DatagramSocket(55555);
        }
        catch  (IOException e)
        {
            System.out.println("Erreur sur DatagramSocket");
            return;
        }

        while (true) {
            // Attendre le message émis par le client
            DatagramPacket Message = new DatagramPacket(sendBuf, 256);
            try {
                Socket_UDP.receive(Message);
            } catch (IOException e) {
                System.out.println("Erreur Socket_UDP.receive :");
                e.printStackTrace();
                return;
            }

            // Envoyer un message sur ce port
            byte[] recBuf = Message.getData();

            System.out.println("Message recu : " + new String(recBuf));
            Message = new DatagramPacket(recBuf, recBuf.length, Message.getAddress(), Message.getPort());
            try {
                Socket_UDP.send(Message);
            } catch (IOException e) {
                System.out.println("Emission ratee ...");
                e.printStackTrace();
                return;
            }
            System.out.println
                    (Thread.currentThread().getName()
                            + " : Emission vers " + Message.getAddress()
                            + ", port " + Message.getPort());
        }

    }


}
