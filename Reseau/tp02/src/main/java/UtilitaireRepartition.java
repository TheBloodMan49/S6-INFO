import java.awt.*;

class UtilitaireRepartition
{
    static  void ajouter(Container composant,
                         Component sousComposant,
                         int x, int y, int largeur, int hauteur,
                         int typeElargissement,
                         int typePosition,
                         int etalementHorizontal,int etalementVertical,
                         int espacementHaut, int espacementBas,
                         int espacementGauche , int espacementDroite,
                         double poidsHorizontal, double poidsVertical)
    {
        GridBagConstraints contraintes = new GridBagConstraints();

        contraintes.gridx = x;
        contraintes.gridy = y;
        contraintes.gridwidth = largeur;
        contraintes.gridheight = hauteur;
        contraintes.fill = typeElargissement;
        contraintes.anchor = typePosition;
        contraintes.ipadx = etalementHorizontal;
        contraintes.ipady = etalementVertical;
        if (espacementHaut + espacementBas +
                espacementGauche + espacementDroite>0)
            contraintes.insets = new Insets(espacementHaut, espacementBas,
                    espacementGauche, espacementDroite);
        contraintes.weightx = poidsHorizontal;
        contraintes.weighty = poidsVertical;
        ((GridBagLayout)composant.getLayout()).
                setConstraints(sousComposant,contraintes);
        composant.add(sousComposant);
    }

    static void ajouter(Container composant,Component sousComposant,
                        int x, int y, int largeur, int hauteur,
                        double poidsHorizontal, double poidsVertical)
    {
        ajouter(composant, sousComposant, x, y, largeur, hauteur,
                GridBagConstraints.BOTH,
                GridBagConstraints.NORTHWEST, 0, 0, 0, 0, 0, 0,
                poidsHorizontal,poidsVertical);
    }
}
