import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.net.*;

//////////////////////////////////////////////////
//                 Client_UDP_Graph_ST
//////////////////////////////////////////////////

public class Client_UDP extends Frame implements ActionListener
{

    /**
     *
     */
    private static final long serialVersionUID = 1L;

    GridBagLayout Disposition=new GridBagLayout();

    public Label Lab_Mess_recu  = new Label("(Tjrs rien ................................ )", Label.CENTER);
    Label Lab_Port       = new Label("Quel port interroger  ? ", Label.LEFT);
    Label Lab_Hote       = new Label("Quelle machine  ? ", Label.LEFT);
    Label l1             = new Label("Message recu :", Label. LEFT);
    Button Bouton_1      = new Button("Envoi message");
    Button Bouton_2      = new Button("Arret des clients UDP");
    TextField Champ_Hote = new TextField(24);
    TextField Champ_Port = new TextField(6);

    Client_UDP()
    {
        setLayout(Disposition);

        //
        // Mise en forme de l'interface graphique
        //
        Lab_Port.setBackground(Color.yellow);
        UtilitaireRepartition.ajouter(this, Lab_Hote, 0, 5, 1, 1,
                GridBagConstraints.NONE,
                GridBagConstraints.WEST, 0,0 ,10, 10,
                10, 10, 0, 0);

        UtilitaireRepartition.ajouter(this, Champ_Hote, 2, 5, 1, 1,
                GridBagConstraints.NONE,
                GridBagConstraints.WEST, 0,0 ,10, 10,
                10, 10, 0, 0);

        Lab_Hote.setBackground(Color.yellow);
        UtilitaireRepartition.ajouter(this, Lab_Port, 0, 10, 1, 1,
                GridBagConstraints.NONE,
                GridBagConstraints.WEST, 0,0 ,10, 10,
                10, 10, 0, 0);

        UtilitaireRepartition.ajouter(this, Champ_Port, 2, 10, 1, 1,
                GridBagConstraints.NONE,
                GridBagConstraints.WEST, 0,0 ,10, 10,
                10, 10, 0, 0);

        UtilitaireRepartition.ajouter(this, l1, 0, 15, 1, 1,
                GridBagConstraints.NONE,
                GridBagConstraints.WEST, 0,0 ,10, 10,
                10, 10, 0, 0);

        Lab_Mess_recu.setBackground(Color.white);
        UtilitaireRepartition.ajouter(this, Lab_Mess_recu, 2, 15, 1, 1,
                GridBagConstraints.NONE,
                GridBagConstraints.WEST, 0,0 ,10, 10,
                10, 10, 0, 0);

        Bouton_1.setBackground(Color.yellow);
        UtilitaireRepartition.ajouter(this, Bouton_1, 0, 20, 1, 1,
                GridBagConstraints.NONE,
                GridBagConstraints.WEST, 0,0 ,10, 10,
                10, 10, 0, 0);

        Bouton_2.setBackground(Color.pink);
        UtilitaireRepartition.ajouter(this, Bouton_2, 2, 20, 1, 1,
                GridBagConstraints.NONE,
                GridBagConstraints.WEST, 0,0 ,10, 10,
                10, 10, 0, 0);
        //
        // Attacher des actions aux boutons et aux textes
        //
        Champ_Hote.addActionListener(this);
        Champ_Port.addActionListener(this);
        Bouton_1.addActionListener(this);
        Bouton_2.addActionListener(this);

        addWindowListener(new WindowAdapter() {
            public void windowClosing(WindowEvent we) {
                dispose();
            }
        });
    }


//////////////////////////////////////////////////
//            actionPerformed :
//    Definitions des actions
//////////////////////////////////////////////////

    public void actionPerformed(ActionEvent event)
    {

        Object source = event.getSource();
        int Port = 0;
        String Hote;

        if (source == Bouton_1)
        {
            try
            {
                Port = Integer.parseInt(Champ_Port.getText());
                Hote = Champ_Hote.getText();
                Comm_UDP_1  Trans_UDP = new Comm_UDP_1(Port, this, Hote);
                Trans_UDP.run();  // ----
            }
            catch  (NumberFormatException e)
            {
                System.out.println("Erreur port ou thread");
                e.printStackTrace();
                return;
            }
        }
        //
        // Arret de tous les threads gerant une comm. UDP
        //
        if (source == Bouton_2)
        {
            Thread Tab_Threads[] = new Thread [Thread.activeCount()];
            int Nbre_Threads = 0;
            int i ;

            System.out.println(" Avant arret -> Nbre de threads : "
                    + Thread.activeCount() );
            Nbre_Threads = Thread.enumerate(Tab_Threads);

            for (i=0; i < Nbre_Threads ; i++)
            {
                if ( Tab_Threads[i].getName().startsWith("UDP") )
                {
                    //Tab_Threads[i].stop();
                    System.out.println("Arret de " +Tab_Threads[i].getName());
                }
            }
            Lab_Mess_recu.setText(" ... RAZ faite ...");

            System.out.println(" Apres arret Nbre de threads : "
                    + Thread.activeCount() );


        }


    }

//////////////////////////////////////////////////
//                main
//////////////////////////////////////////////////

    public static void main(String[] argv)
    {
        Client_UDP Mon_Client_UDP = new Client_UDP();
        Mon_Client_UDP.setSize(400,350);
        Mon_Client_UDP.setVisible(true);
    }
}

//////////////////////////////////////////////////
//                Comm_UDP_1
//////////////////////////////////////////////////

class Comm_UDP_1
{
    int Port;
    String hote;
    Client_UDP client;

    public Comm_UDP_1 (int Un_Port, Client_UDP c, String Un_Hote )
    {

        this.Port  = Un_Port;
        this.client = c;
        this.hote  = Un_Hote;
    }

    public void run()
    {
        DatagramPacket Message;
        byte[] sendBuf = new byte[256];
        DatagramSocket Socket_UDP;
        InetAddress    Adresse_IP = null;

        // Donner au thread un nom qui commence par UDP
        // ---- this.setName("UDP"+Thread.currentThread().getName());

        try
        {
            Adresse_IP = InetAddress.getByName(hote);
        }
        catch (UnknownHostException e)
        {
            System.out.println("Erreur sur Adresse_IP");
            System.exit (1);
        }

        // Ouvrir un socket UDP
        try
        {
            Socket_UDP = new DatagramSocket();
            Socket_UDP.setSoTimeout(2000);
        }
        catch  (IOException e)
        {
            System.out.println("Erreur sur DatagramSocket");
            return;
        }
        // Envoyer un message sur ce port
        String m = "Hello!";
        byte[] recBuf = m.getBytes();
        Message = new DatagramPacket(recBuf, recBuf.length, Adresse_IP, Port);
        try
        {
            Socket_UDP.send(Message);
        }
        catch  (IOException e)
        {
            System.out.println("Emission ratee ...");
            e.printStackTrace();
            return;
        }
        System.out.println
                (Thread.currentThread().getName()
                        + " : Emission vers " + Adresse_IP
                        + ", port " + Port);

        // Attendre la reponse emise par le serveur
        Message = new DatagramPacket(sendBuf, 256);
        try
        {
            Socket_UDP.receive(Message);
        }
        catch  (IOException e)
        {
            System.out.println("Erreur Socket_UDP.receive :");
            e.printStackTrace();
            return;
        }

        // Visualisation du message recu
        String received = new String(Message.getData());
        System.out.println
                (Thread.currentThread().getName()
                        +" recu : "
                        + received);

        client.Lab_Mess_recu.setText(received);
    }


}

