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
	graphe g;
	char * input_file;
	char c[] = "cliquemax";
	char d[] = "dsatur";
	char g1[] = "glouton1";
	char g2[] = "glouton2";
	
	if (argc != 3)
	{
		printstr("Utilisation incorrecte, lisez le README.\n");
		return (0);
	}

	//========= construction du graphe: ================================

	input_file = ouvrir_fichier(argv[1]);
	if ((g.adj_matrix = (int**)malloc(sizeof(int*)*ORDER_MAX))==NULL)
		return (0);
	for ( k=0; k<ORDER_MAX; k++)
		if ((g.adj_matrix[k] = (int*)malloc(sizeof(int)*ORDER_MAX))==NULL)
			return (0);
	if ((g.coloration = (int*)malloc(sizeof(int)*ORDER_MAX))==NULL)
		return (0);
	g = graph_from_file(g, input_file);

	//======= affichage ================================================

	printstr(input_file);
	display_graph(g, g.order);

	//======= recherche ================================================

	if (strcmp(argv[2], c) == 0)
	{
		printstr("============== Recherche par clique max ===========\n");
		bruteforce_search(g);
	}
	else if (strcmp(argv[2], g1) == 0)
	{
		printstr("========== Recherche approchee gloutonne ==========\n");
		glouton1(g);
	}
	else if (strcmp(argv[2], g2) == 0)
	{
		printstr("========== Recherche approchee gloutonne ==========\n");
		glouton2(g);
	}
	else if (strcmp(argv[2], d) == 0)
	{
		printstr("========== Recherche approchee par DSATUR =========\n");
		dsatur(g);
	}
	else
	{
		printstr("================= Algorithme RLF ==================\n");
		zykov(g);
	}

	//====== Sortie ====================================================

	display_vecteur(g.coloration, g.order);
	ecrire_fichier_sortie(g);

	for ( k=0; k<ORDER_MAX; k++ )
		free(g.adj_matrix[k]);
	free(g.adj_matrix);
	free(g.coloration);
	return (0);
}
