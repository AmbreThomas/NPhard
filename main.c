#include "dsatur.h"

int	main(int argc, const char* argv[]){

	int i, k;
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
	printf("%s",input_file);
	display_graph(g);
	coloration = glouton(g);
	display_vecteur(coloration, g);

	/*
	int verification_de_la_coloration;
	dsatur(g);
	if (verif_color(g)==1){
		printf("\n Bonne coloration approuvée");
	} else {
		printf("\n Erreur dans la coloration.");
	}
	*/

	ecrire_fichier_sortie(coloration, g);
	printf("\nfin.\n");
	for ( k=0; k<ORDER_MAX; k++ )
		free(g.adj_matrix[k]);
	free(g.adj_matrix);
	return (0);
}
