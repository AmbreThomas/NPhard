#NPhard

##Utilisation des algorithmes de coloration de graphes
Taper dans un terminal :

`make`
`./petriboxes ENTREE ALGORITHME`

avec ENTREE, le nom d'un fichier formaté comme suit :

Bacteria Species :A,B,C,D,E.  
Incompatibility List :  
A, B  
A, C  
C, D.  

avec ALGORITHME qui peut être, au choix, remplacé par :
* glouton
* glouton2
* cliquemax
* rlf
* dsatur

---

L'algorithme expliquera dans la console ses étapes majeures et un
fichier appelé "sortie" se trouvera dans le dossier où a été
executé l'algorithme. Ce fichier contiendra le nombre de boites de
Petri qu'il faut pour cultiver les bactéries données en entrée ainsi
que les groupements de bactéries qu'il est possible de cultiver ensemble

**contacts :**  
ambre.thomas@insa-lyon.fr  
louis.becquey@insa-lyon.fr
