#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#define BUF_SIZE 4096
#define ORDER_MAX 30

typedef struct t_graphe graphe;
struct t_graphe {
	int order;
	int** adj_matrix;
};

int		taille_fichier(char *fichier){
	char buf[BUF_SIZE];
	int fd;
	int count;
	int ret;

	count = 0;
	fd = open(fichier, O_RDONLY);
	if (fd == -1)
		return (0);
	while ((ret = read(fd, buf, BUF_SIZE))){
		buf[ret] = '\0';
		count += ret;
	}
	return (count);
}

char*	ouvrir_fichier(char* fichier){
	char * buf;
	int ret;
	int fd;

	buf = (char*)malloc(sizeof(char) * (taille_fichier(fichier)+1));
	fd = open(fichier, O_RDONLY);
	if (fd == -1){
		printf("\nFichier invalide\n");
		buf = 0;
	}
	while ((ret = read(fd, buf, taille_fichier(fichier)))){
		buf[ret] = 0;
	}
	close(fd);
	return (buf);
}

graphe	init(graphe g){
	int i,j;
	for ( i=0; i<g.order; i++ ){
		for ( j=0; j<g.order; j++ ){
			g.adj_matrix[i][j] = 0;
		}
	}
	return (g);
}

graphe	graph_from_file(graphe g, const char* input_file){
	int i,j,k;
	int order = -1;
	i = 19;
	while (input_file[i] != '\n'){
		if ((input_file[i] != ',') && (input_file[i] != ' '))
			order++;
		i++;
	}
	i++;
	while (input_file[i] != '\n')
		i++;
	printf("\n%d espèces à cultiver.\n", order);
	g.order = order;
	for ( j=0; j<order; j++ ){
		for ( k=0; k<order; k++ ){
			g.adj_matrix[j][k] = 0;
		}
	}
	i++;
	while (input_file[i] && (input_file[i] != '.')){
		j = 0;
		k = 0;
		while ((input_file[i] != '\n') && (input_file[i] !='.')){
			if ((input_file[i] != ',') && (input_file[i] != ' ')){
				if (j==0){ j = input_file[i] - 64; }
				else {k = input_file[i] - 64; }
			}
			i++;
		}
		g.adj_matrix[k][j] = 1;
		g.adj_matrix[j][k] = 1;
		if (input_file[i] != '.') i++;
	}
	return (g);
}

void	display_graph(graphe g){
	int i,j;
	printf("\n\nDISPLAY graph:\n");
	for ( i=0; i<g.order; i++){
		printf("\n");
		for ( j=0; j<g.order; j++ ){
			printf(" %d", g.adj_matrix[i][j]);
		}
	}
	printf("\n");
}

int		remove_from_matrix(int** adj_matrix, int* clique, int from, int to, graphe g){
	int**	old_g_adj;
	int		i,j,k,to_keep;
	
	if ((old_g_adj = (int**)malloc(sizeof(int*)*from))==0)
		return (0);
	for ( k=0; k<from; k++)
		if ((old_g_adj[k] = (int*)malloc(sizeof(int)*from))==0)
			return (0);

	//retirer les lignes
	for ( j=0; j<from; j++ ){
		for ( k=0; k<from; k++ ){
			old_g_adj[j][k] = adj_matrix[j][k];
		}
	}
	k = -1;
	for ( i=0; i<from; i++ ){
		to_keep = 1;
		for ( j=0; j<from; j++ )
			if ( clique[j] == i+1 )
				to_keep = 0;
		if ( to_keep == 1){
			k++;
			for ( j=0; j<from; j++ )
				adj_matrix[k][j] = old_g_adj[i][j];
		}
	}
	//retirer les colonnes
	for ( j=0; j<from; j++ ){
		for ( k=0; k<from; k++ ){
			old_g_adj[j][k] = adj_matrix[j][k];
		}
	}
	k=-1;
	for ( i=0; i<from; i++ ){
		to_keep = 1;
		for ( j=0; j<from; j++ )
			if ( clique[j] == i+1 )
				to_keep = 0;
		if ( to_keep == 1){
			k++;
			for ( j=0; j<from; j++ )
				adj_matrix[j][k] = old_g_adj[j][i];
		}
	}

	for ( k=0; k<from; k++ )
		free(old_g_adj[k]);
	free(old_g_adj);
}

void	bruteforce_search(graphe g){
	int n = g.order;
	int couleurs = 0;
	int i,j,k;
	int coloration[n];
	int max_clique[n];  // peut contenir au max n noeuds
	int len_max_clique; // toujours <= n

	// init
	for ( i=0; i<n; i++){
		coloration[i] = 0;
		max_clique[i] = 0;
	}

	//boucle principale
	while (n>0){
		//=== recommencer la recherche de la plus grande clique ===
		printf("\n Encore %d noeuds à colorier.\n",n);
		len_max_clique = 0;
		k = n;
		while ( k > 0 && len_max_clique == 0){
			printf("==>Recherche d'une clique de taille %d...",k);
			//chercher si une clique de taille k existe
			//Si oui: chercher une clique de taille k
			k--;
		}

		//=== colorier la clique trouvée ===
		couleurs++;
		for ( i=0; i<len_max_clique; i++ )
			coloration[max_clique[i]] = couleurs;

		//=== soustraire la clique trouvée au graphe g ===

		n -= len_max_clique;
		printf(" J'en retire %d, on aura n=%d.\n",k,n);
		remove_from_matrix(g.adj_matrix, max_clique, g.order, n, g	);
		g.order = n;
	}
	display_graph(g);
}

int	main(int argc, const char* argv[]){

	int i,k;
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


	//======= recherche brute =====================
	bruteforce_search(g);

	
	printf("\nfin.\n");
	for ( k=0; k<ORDER_MAX; k++ )
		free(g.adj_matrix[k]);
	free(g.adj_matrix);
	return (0);
}
