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
	display_graph(g, g.order);

	//======= recherche brute =====================
	//~ printstr("========== Algorithme par clique max ===========\n");
	//~ coloration = bruteforce_search(g);
	//~ display_vecteur(coloration, g);
	//~ free(coloration);
	//~ for ( k=0; k<ORDER_MAX; k++ )
		//~ free(g.adj_matrix[k]);
	//~ free(g.adj_matrix);
	//~ g = graph_from_file(g, input_file);

	printstr("========== Algorithme de Zykov ===========\n");
	coloration = zykov(g);
	display_vecteur(coloration, g.order);

	//====== Glouton ==============================
	//~ printf("========== Recherche approchée gloutonne ===========\n");
	//~ coloration = glouton1(g);
	//~ display_vecteur(coloration, g);

	//~ printf("========== Recherche approchée par DSATUR ===========\n");
	//~ coloration = dsatur(g);
	//~ display_vecteur(coloration, g);

	//====== Sortie ===============================
	ecrire_fichier_sortie(coloration, g);
	if (verif_color(g, coloration)==1){
		printstr("\n Bonne coloration\n");
	} else {
		printstr("\n Erreur dans la coloration.\n");
	}
	for ( k=0; k<ORDER_MAX; k++ )
		free(g.adj_matrix[k]);
	free(g.adj_matrix);
	return (0);
}
