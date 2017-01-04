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
int		combinaisons(int n, int k);
graphe	init(graphe g);
graphe	graph_from_file(graphe g, const char *input_file);
int*	bruteforce_search(graphe g);
void	display_graph(graphe g, int n);
void	display_vecteur(int *vecteur, int n);
void	printstr(const char* texte);
void	printnbr(int n);
int*	glouton1(graphe g);
int*	dsatur(graphe g);
int		verif_color(graphe g, int* coloration_trouvee);
int     *glouton2(graphe g);
int     comptage(int *boite);
int     compatible(graphe g, int j, int *boite);
int*	zykov(graphe g);
