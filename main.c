/* ****************************************************************** */
/*          COLORATION DE GRAPHES                                     */
/*                                                                    */
/*   Louis Becquey                                                    */
/*   Ambre Thomas                                                     */
/*                                 Bioinformatique et modelisation    */
/*                                           INSA Lyon                */
/* ****************************************************************** */

#include "graphe.h"

int	main(int argc, char** argv){

	int k;
	int * coloration;
	graphe g;
	char * input_file;
	char c[] = "cliquemax";
	char d[] = "dsatur";
	char r[] = "rlf";
	char g1[] = "glouton1";
	char g2[] = "glouton2";
	
	if (argc != 3)
	{
	       printstr("Utilisation incorrecte, lisez le README.\n");
	       return (0);
	}

	//========= construction du graphe: ===========
	input_file = ouvrir_fichier(argv[1]);
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

        if (strcmp(argv[2], c) == 0)
        {
	       printstr("========== Algorithme par clique max ===========\n");
	       coloration = bruteforce_search(g);
	}

	//====== Glouton1 ==============================
        if (strcmp(argv[2], g1) == 0)
        {
	       printstr("========== Recherche approchee gloutonne ===========\n");
	       coloration = glouton1(g);
	}

	//====== Glouton2 ==============================
        if (strcmp(argv[2], g2) == 0)
        {
	       printstr("========== Recherche approchee gloutonne ===========\n");
	       coloration = glouton2(g);
	}
	
	//====== DSATUR ==============================
        if (strcmp(argv[2], d) == 0)
        {
	       printstr("========== Recherche approchee par DSATUR ===========\n");
	       coloration = dsatur(g);
	}

        if (strcmp(argv[2], r) == 0)
        {
	       printstr("========== Algorithme RLF ===========\n");
	       coloration = zykov(g);
	}
	
	display_vecteur(coloration, g.order);

	//====== Sortie ===============================
	ecrire_fichier_sortie(coloration, g);
	
	if (verif_color(g, coloration)){
		printstr("\n Bonne coloration\n");
	} else {
		printstr("\n Erreur dans la coloration.\n");
	}
	
	for ( k=0; k<ORDER_MAX; k++ )
		free(g.adj_matrix[k]);
	free(g.adj_matrix);
	free(coloration);
	return (0);
}
