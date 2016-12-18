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
	for ( i=1; i<=g.order; i++ ){
		for ( j=1; j<=g.order; j++ ){
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
	for ( j=1; j<=order; j++ ){
		for ( k=1; k<=order; k++ ){
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

void	dfs_rec(graphe g, unsigned int s, int* marque){
	unsigned int i;
	marque[s] = 1;
	for ( i=1; i<=g.order; i++ ){
		if ( g.adj_matrix[s][i]==1)
			if ( !marque[i] ){
				printf("\tRencontre préfixe de %d\n", i);
				dfs_rec(g, i, marque);
				printf("\tRencontre suffixe de %d\n", i);
			}
	}
}

void	depth_search(graphe g, unsigned int start_som){
	int* marque,i;
	marque = malloc(sizeof(int)*(g.order));
	for ( i=1; i<=g.order; i++ )
		marque[i] = 0;
	printf("\n\nDFS à partir du sommet %d\n",start_som);
	dfs_rec(g, start_som, marque);
	printf("\tRencontre suffixe de %d\n",start_som);
	free(marque);
	printf("\n");
}

void	breadth_search(graphe g, unsigned int s){
	int* list; //la pile fifo
	int* marque; //mémoriser les sommets déjà visités
	int i,deb,fin;
	int sommet_a_parcourir;
	list = (int *)malloc(sizeof(int)*(g.order + 2));
	marque = (int *)malloc(sizeof(int)*(g.order + 2 ));
	for ( i=1; i<=g.order; i++ )
		marque[i] = 0;
	//on empile le sommet de départ:
	deb = 1;
	fin = 2;
	list[deb] = s;
	marque[s] = 1; // déjà visité
	while (deb <= g.order ){
		sommet_a_parcourir = list[deb];
		deb++;
		printf("\n sommet %d", sommet_a_parcourir);
		//empiler les voisins du sommet à parcourir:
		for ( i=1; i<=g.order; i++ ){
			if ( g.adj_matrix[sommet_a_parcourir][i] && (marque[i]==0)){  //si voisin non marqué
				list[fin] = i;
				marque[i] = 1;
				fin++;
			}
		}
	}
}

//================= ALGORITHME GLOUTON ==========================

int*	copyYdansZ(int* destination, int* origine, graphe g){
	int i;
	for ( i=1; i<=g.order; i++ )
		destination[i] = origine[i];
	return (destination);
}

int		premier_sommet_de_Z(int* vecteur, graphe g){
	int i;
	for ( i=1; i<=g.order; i++ )
		if ( vecteur[i] == 0 )
			return (i);
	return (-1);
}

int*	retirer_s_et_ses_voisins(int s, int* vecteur, graphe g){
	int i;
	vecteur[s] = 1;
	vecteur[g.order + 1]++;
	for ( i=1; i<=g.order; i++)
		if ( (g.adj_matrix[s][i] > 0) && (vecteur[i] == 0)){
			vecteur[g.order + 1]++;
			vecteur[i] = 1;
		}
	return (vecteur);
}

int*	glouton(graphe g){
	int i,s;
	int* Y;
	int* Z;
	int cardY = 0;
	int cardZ = 0;
	int color = 0;

	printf("\n================= GLOUTON ==================\n");
	Y = (int *)malloc(sizeof(int)*(g.order+2));
	Z = (int *)malloc(sizeof(int)*(g.order+2));
	Z[g.order+1] = 0; //contient cardZ

	//init:
	for ( i=1; i<=g.order; i++ ){
		Y[i] = 0;
		Z[i] = 0;
	}
	while ( cardY < g.order ){
		Z = copyYdansZ(Z, Y, g);
		Z[g.order+1] = cardY;
		color++;
		printf("\n\tChangement de couleur à %d", color);
		while ( Z[g.order+1] < g.order ){
			s = premier_sommet_de_Z(Z,g);
			printf("\ncoloration de y=%d avec color=%d", s, color);
			Y[s] = color;
			cardY++;
			Z = retirer_s_et_ses_voisins(s,Z,g);
		}
	}
	return (Y);
}

//=============== ALGORITHME DSATUR ==========================

int dsat[ORDER_MAX];
int degre[ORDER_MAX];
int coloration_trouvee[ORDER_MAX];

void	init_dsat(graphe g){
	int i,j;
	for ( i=1; i<=g.order; i++ ){
		dsat[i] = 0;
		coloration_trouvee[i] = 0;
		for ( j=1; j<=g.order; j++ ){
			if ( g.adj_matrix[i][j] ) dsat[i]++;
		}
	}
}

int		sommet_non_colorie_de_dsat_max(graphe g){
	int v,i,dsat_max_local = 0;
	for ( i=1; i<=g.order; i++ ){
		if (coloration_trouvee[i] = 0){
			if (dsat[i] > dsat_max_local){
				v = i;
				dsat_max_local = dsat[i];
			}
		}
	}
	return (v);
}

int		coloration_avec_couleur_minimum(int t, graphe g){
	int i;
	int color_used[ORDER_MAX];
	//init:
	for ( i=1; i<=g.order; i++ )
		color_used[i] = 0;
	for ( i=1; i<=g.order; i++ ){
		if ( (g.adj_matrix[t][i]) && (coloration_trouvee[i] > 0)){
			color_used[coloration_trouvee[i]] = 1;
		}
	}
	//on colorie t par la plus petite couleur non utilisée au voisinnage:
	for ( i=1; i<=g.order; i++){
		if ( color_used[i] == 0 ){
			return (i);
		}
	}
}

int	 	nb_de_couleurs_differentes_autour_de(int t, graphe g){
	int i, nb_de_couleurs_utilisees_autour_de_t;
	int color_used[ORDER_MAX];
	//init:
	for ( i=1; i<=g.order; i++ )
		color_used[i] = 0;
	for ( i=1; i<=g.order; i++ ){
		if ( (g.adj_matrix[t][i]) && (coloration_trouvee[i]>0)){
			color_used[coloration_trouvee[i]] = 1;
		}
	}
	nb_de_couleurs_utilisees_autour_de_t = 0;
	for ( i=1; i<=g.order; i++ )
		if (color_used[i] == 1) nb_de_couleurs_utilisees_autour_de_t++;
	return (nb_de_couleurs_utilisees_autour_de_t);
}

void	maj_du_vecteur_dsat(int t, graphe g){
	int i;
	//synthèse pour que le sommet actuellement colorié soit
	//rapidement ecarté pour les autres choix
	dsat[t] = -1;
	for ( i=1; i<=g.order; i++ ){
		if ( (g.adj_matrix[t][i]==1) && (coloration_trouvee[i]==0)){
			dsat[i] = nb_de_couleurs_differentes_autour_de(i,g);
		}
	}
}

void	dsatur(graphe g){
	int i,j,s;
	printf("\n================== DSATUR ======================\n");
	init_dsat(g);
	for ( i=1; i<=g.order; i++ ){
		s = sommet_non_colorie_de_dsat_max(g);
		coloration_trouvee[s] = coloration_avec_couleur_minimum(s,g);
		printf("\n s=%d coloré par %d", s, coloration_trouvee[s]);
		maj_du_vecteur_dsat(s,g);
	}
}

//================================ UTILITAIRES ===========================

int		verif_color(graphe g){
	int i,j;
	for ( i=1; i<=g.order; i++ ){
		for ( j=1; j<=g.order; j++ ){
			if ( (g.adj_matrix[i][j]==1) && (coloration_trouvee[i]==coloration_trouvee[j])){
				printf("\nErreur: le sommet %d voisin de %d est colorié avec la même couleur %d", i, j, coloration_trouvee[i]);
				return (0);
			}
		}
	}
	return (1);
}

void	display_graph(graphe g){
	int i,j;
	printf("\n\nDISPLAY graph:\n");
	for ( i=1; i<=g.order; i++){
		printf("\n");
		for ( j=1; j<=g.order; j++ ){
			printf(" %d", g.adj_matrix[i][j]);
		}
	}
	printf("\n");
}

void	display_vecteur(int* vecteur, graphe g){
	int i;
	printf("\n\nDISPLAY couleurs (sommet: couleur):\n");
	for ( i=1; i<=g.order; i++ ){
		printf(" %d: %d\n", i, vecteur[i]);
	}
}

//=============================== MAIN =======================================

int	main(int argc, const char* argv[]){

	int i,k;
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

	printf("\nfin.\n");
	for ( k=0; k<ORDER_MAX; k++ )
		free(g.adj_matrix[k]);
	free(g.adj_matrix);
	return (0);
}
