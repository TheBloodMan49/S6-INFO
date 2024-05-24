(: 1 :)

(:
for $f in doc("refbib.xml")/bib/livre
 where $f/titre="Data on the Web"
 return 
  for $a in $f//auteur
   return ($a/prenom,$a/nom)
:)

(: Result :
<?xml version="1.0" encoding="UTF-8"?>
<prenom>Serge</prenom>
<nom>Abiteboul</nom>
<prenom>Peter</prenom>
<nom>Buneman</nom>
<prenom>Dan</prenom>
<nom>Suciu</nom> 
:)


(: 2 :)

(:
for $f in doc("refbib.xml")/bib/livre
 where $f/titre="Data on the Web"
 return 
  <auteurs>{
  for $a in $f//auteur
   return $a/nom
  }</auteurs>
:)

(: 3 :)

(:
<biblio>{
for $f in doc("refbib.xml")/bib/livre
 where $f/edition="Addison-Wesley" and $f/@annee>1992
 return 
  <livre annee="{$f/@annee}">{$f/titre}</livre>
}</biblio>
:)

(: 4 :)

(:
<biblio>{
for $f in doc("refbib.xml")/bib/livre
 order by $f/titre
 return $f
}</biblio>
:)

(: 5 :)

(:
<auteurs>{
for $a in distinct-values(
  doc("refbib.xml")//auteur/nom
)
return
  <auteur>
  { $a }
  <livres>
  {
  for $f in doc("refbib.xml")/bib/livre
   where $f/auteur/nom=$a
   return $f/titre
  }
  </livres>
  </auteur>
}</auteurs>
:)

(: 6 :)

(:
<resultats>
{
  for $f in distinct-values(
    doc("refbib.xml")/bib/livre/titre
  )
  return
    <prixmin titre="{$f}">
    {
      min(
        for $p in doc("refbib.xml")/bib/livre
         where $p/titre=$f
         return $p/prix
      )
    }
    </prixmin>
}
</resultats>
:)

(: 7 :)

<livres>{
for $f in doc("refbib.xml")/bib/livre
  where count($f//auteur)>=1
  return
  <livre>
  {($f/titre, $f/auteur[1])}
  {
    if (exists($f/auteur[2]))
    then
      $f/auteur[2]
    else (
      if (exists($f/auteur[3]))
      then
        <et-al/>
      else ()
    )
  }
  </livre>
}</livres>
