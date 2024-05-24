(: 1 :)

for $pers in doc("personnes.xml")/personnes/perso_tuple,
    $obj in doc("objets.xml")/objets/objet_tuple
  where $pers/idperso = $obj/propose_par
  and $obj/prix_de_reserve > 1000
  and $pers/confiance > "C"
  return 
    <suspect>
      <nom>{data($pers/nom)}</nom>
      <confiance>{data($pers/confiance)}</confiance>
      <description_objet>{data($obj/description)}</description_objet>
      <prix_de_reserve>{data($obj/prix_de_reserve)}</prix_de_reserve>
    </suspect>


