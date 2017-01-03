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
	int order;
	int** adj_matrix;
};

int		taille_fichier(char *fichier);
char	*ouvrir_fichier(char *fichier);
void	ecrire_fichier_sortie(int *coloration, graphe g);
char	*ft_strnew(size_t size);
size_t	get_str_len(int n);
char	*ft_itoa(int n);
graphe	init(graphe g);
graphe	graph_from_file(graphe g, const char *input_file);
void	dfs_rec(graphe g, unsigned int s, int *marque);
void	depth_search(graphe g, unsigned int start_som);
void	breadth_search(graphe g, unsigned int s);
int		*copyYdansZ(int *destination, int *origine, graphe g);
int		premier_sommet_de_Z(int *vecteur, graphe g);
int		*retirer_s_et_ses_voisins(int s, int *vecteur, graphe g);
int		*glouton(graphe g);
void	init_dsat(graphe g);
int		sommet_non_colorie_de_dsat_max(graphe g);
int*	bruteforce_search(graphe g);
int		combinaisons(int n, int k);
void	display_graph(graphe g);
void	display_vecteur(int *vecteur, graphe g);
