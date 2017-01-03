#include "graphe.h"

int	main(void){

	int k;
	int * coloration;
	graphe g;
	char * input_file;

	//========= construction du graphe: ===========
	input_file = ouvrir_fichier("ENTREE");
	if ((g.adj_matrix = (int**)malloc(sizeof(int*)*ORDER_MAX))==NULL)
		return (0);
	for ( k=0; k<ORDER_MAX; k++)
		if ((g.adj_matrix[k] = (int*)malloc(sizeof(int)*ORDER_MAX))==NULL)
			return (0);
	g = graph_from_file(g, input_file);

	//======= affichage ===========================
	printstr(input_file);
	display_graph(g);

	//====== Glouton ==============================
	//~ printf("========== Recherche approchée ===========\n");
	//~ coloration = glouton(g);
	//~ display_vecteur(coloration, g);

	//====== DSATUR ===============================
	//~ int verification_de_la_coloration;
	//~ dsatur(g);
	//~ if (verif_color(g)==1){
		//~ printf("\n Bonne coloration approuvée");
	//~ } else {
		//~ printf("\n Erreur dans la coloration.");
	//~ }

	//======= recherche brute =====================
	printstr("========== Algorithme de Zykov ===========\n");

	display_vecteur(coloration, g);


	//====== Sortie ===============================
	ecrire_fichier_sortie(coloration, g);
	for ( k=0; k<ORDER_MAX; k++ )
		free(g.adj_matrix[k]);
	free(g.adj_matrix);
	free(coloration);
	return (0);
}
