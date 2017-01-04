/* ****************************************************************** */
/*          COLORATION DE GRAPHES                                     */
/*                                                                    */
/*   Louis Becquey                                                    */
/*   Ambre Thomas                                                     */
/*                                 Bioinformatique et modelisation    */
/*                                           INSA Lyon                */
/* ****************************************************************** */

#ifndef __GRAPHEH__
#define __GRAPHEH__

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#define BUF_SIZE 4096
#define ORDER_MAX 30

typedef struct t_graphe graphe;
struct t_graphe {
	int* coloration;
	int order;
	int** adj_matrix;
};

char	*ouvrir_fichier(char *fichier);
void	ecrire_fichier_sortie(graphe g);
char	*ft_strnew(size_t size);
size_t	get_str_len(int n);
char	*ft_itoa(int n);
int		combinaisons(int n, int k);
graphe	init(graphe g);
graphe	graph_from_file(graphe g, const char *input_file);
void	bruteforce_search(graphe g);
void	display_graph(graphe g, int n);
void	display_vecteur(int *vecteur, int n);
void	printstr(const char* texte);
void	printnbr(int n);
void	glouton1(graphe g);
void	dsatur(graphe g);
void	glouton2(graphe g);
void	zykov(graphe g);


#endif
