#include "graphe.h"

//================================ UTILITAIRES ===========================

//~ int		verif_color(graphe g){
	//~ int i,j;
	//~ for ( i=1; i<=g.order; i++ ){
		//~ for ( j=1; j<=g.order; j++ ){
			//~ if ( (g.adj_matrix[i][j]==1) && (coloration_trouvee[i]==coloration_trouvee[j])){
				//~ printf("\nErreur: le sommet %d voisin de %d est colorié avec la même couleur %d", i, j, coloration_trouvee[i]);
				//~ return (0);
			//~ }
		//~ }
	//~ }
	//~ return (1);
//~ }

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

graphe	init(graphe g){
	int i,j;
	for ( i=1; i<=g.order; i++ ){
		for ( j=1; j<=g.order; j++ ){
			g.adj_matrix[i][j] = 0;
		}
	}
	return (g);
}

graphe	graph_from_file(graphe g, const char *input_file){
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

//================= ALGORITHME EXACT ==========================

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

int*	 max_clique(graphe g, int* clique, int* voisinnage, int* sondable){
	
}

int		is_clique(graphe g, int* max_clique, int k, int* combin){
	int i,j;

	for( i=0; i<k; i++ ){
		for ( j=0; j<k; j++ ){
			if (combin[i]==j) break;
			if (g.adj_matrix[max_clique[combin[i]]][max_clique[combin[j]]]==0) return(0);
		}
	}
	printf("Clique de taille %d trouvée !\n",k);
	return (1);
}

void	bruteforce_search(graphe g){
	int n = g.order;
	int couleurs = 0;
	int h;
	int i;
	int j;
	int k;
	int l;
	int coloration[n];
	int max_clique[n];  // peut contenir au max n noeuds
	int len_max_clique; // toujours <= n
	int degree[n];

	// init
	for ( i=0; i<n; i++){
		coloration[i] = 0;
		k = 0;
		for ( j=0; j<n; j++ ){
			k += g.adj_matrix[i][j];
		}
		degree[i] = k;
	}

	//boucle principale
	while (n>0)
	{
		//=== recherche de la plus grande clique ===
		printf("Encore %d noeuds a colorier.\n",n);
		len_max_clique = 0;
		k = n;
		while ( k>0 && len_max_clique == 0 )
		{
			printf("==>Recherche d'une clique de taille %d...",k);
			//chercher si une clique de taille k existe
			l = 0;
			for ( i=0; i<n; i++ ){
				if ( degree[i] >= k-1 ){
					max_clique[l] = i;
					l++;
				}
			}
			//On a l noeuds de degré >=(k-1), rangés dans max_clique
			if ( l>=k ){ //si moins de k: pas de clique possible
				//chercher les combinaisons de k noeuds parmi ces l
				int** combin;
				int n_combin = combinaisons(l, k);
				if ((combin = (int**)malloc(sizeof(int*)*n_combin))==0)
					return;
				for ( i=0; i<n_combin; i++)
					if ((combin[i] = (int*)malloc(sizeof(int)*k))==0)
						return;


				for ( j=0; j<k; j++ )
					combin[0][j] = j; // la première combinaison est 0,1,2,...,k-1
				j = 0;
				while (combin[j][0] <= (l - k)) // dernière combinaison possible
				{
					j++;
					i = 0;
					while (i < k) // recopie la précédente combinaison
						combin[j][i] = combin[j-1][i++];

					combin[j][k - 1]++; //incrémente la dernière case
					i = k;
					while (i) // si une case a dépassé l, on passe à la case précédente
					{
						i--;
						if (combin[j][i] > l - 1)
						{
							if (i) combin[j][i - 1]++;
							combin[j][i] = 0;
						}
					}
				}


				//pour chaque combinaison, vérifier si c'est une clique:
				for ( i=0; i<n_combin; i++ ){
					if (is_clique(g, max_clique, k, combin[i])){
						//si oui, récupérer les numéros des noeuds concernés
						for ( j=0; j<k; j++ )
							max_clique[j] = max_clique[combin[i][j]];
						len_max_clique = k;
						break;
					}
				}
				for ( i=0; i<n_combin; i++ )
					free(combin[i]);
				free(combin);
			}
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
