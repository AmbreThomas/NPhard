#include "graphe.h"

//================================ UTILITAIRES ===========================

//~ int		verif_color(graphe g){
	//~ int i,j;
	//~ for ( i=1; i<=g.order; i++ ){
		//~ for ( j=1; j<=g.order; j++ ){
			//~ if ( (g.adj_matrix[i][j]==1) && (coloration_trouvee[i]==coloration_trouvee[j])){
				//~ printstr("\nErreur: le sommet ");
				//~ printnbr(i);
				//~ printstr(" voisin de ");
				//~ printnbr(j);
				//~ printstr(" est colorie avec la meme couleur "):
				//~ printnbr(coloration_trouvee[i]);
				//~ return (0);
			//~ }
		//~ }
	//~ }
	//~ return (1);
//~ }

void	display_graph(graphe g){
	int i,j;
	printstr("\ngraph:\n");
	for ( i=0; i<g.order; i++){
		printstr("\n");
		for ( j=0; j<g.order; j++ ){
			printstr(" ");
			printnbr(g.adj_matrix[i][j]);
		}
	}
	printstr("\n\n");
}

void	display_vecteur(int* vecteur, graphe g){
	int i;
	printstr("\n\ncouleurs (sommet: couleur):\n");
	for ( i=0; i<g.order; i++ ){
		printstr(" ");
		printnbr(i);
		printstr(": ");
		printnbr(vecteur[i]);
		printstr("\n");
	}
	printstr("\n");
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
	printstr("\n");
	printnbr(order);
	printstr(" especes a cultiver.\n");
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
				if (j==0){ j = input_file[i] - 65; }
				else {k = input_file[i] - 65; }
			}
			i++;
		}
		g.adj_matrix[k][j] = 1;
		g.adj_matrix[j][k] = 1;
		if (input_file[i] != '.') i++;
	}
	return (g);
}

//================= ALGORITHME ZYKOV ==============================



//================= ALGORITHME MAX_CLIQUE ==========================

void	remove_from_matrix(int** adj_matrix, int* clique, int len_max_c, int from){
	int**	old_g_adj;
	int		i,j,k,to_keep;
	
	if ((old_g_adj = (int**)malloc(sizeof(int*)*from))==0)
		return ;
	for ( k=0; k<from; k++)
		if ((old_g_adj[k] = (int*)malloc(sizeof(int)*from))==0)
			return ;

	//retirer les lignes
	for ( j=0; j<from; j++ ){
		for ( k=0; k<from; k++ ){
			old_g_adj[j][k] = adj_matrix[j][k];
		}
	}
	k = -1;
	for ( i=0; i<from; i++ ){
		to_keep = 1;
		for ( j=0; j<len_max_c; j++ )
			if ( clique[j] == i )
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
		for ( j=0; j<len_max_c; j++ )
			if ( clique[j] == i )
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

int		is_clique(graphe g, int* max_clique, int k, int* combin){
	int i,j;

	for( i=0; i<k; i++ )
		for ( j=0; j<k; j++ )
			if (combin[i]!=combin[j] && g.adj_matrix[max_clique[combin[i]]][max_clique[combin[j]]]==0) return(0);
	printstr("   clique de taille ");
	printnbr(k);
	printstr(" trouvee : {");
	for ( i=0; i<k; i++ ){
		printstr(" ");
		printnbr(max_clique[combin[i]]);
	}
	printstr(" }\n");
	return (1);
}

void	disp_combin(int** combin, int n_combi, int k){
	int i,j;

	i = 0;
	while ( i<n_combi ){
		printstr("   ");
		j = 0;
		while ( j<k ){
			printnbr(combin[i][j]);
			printstr(" ");
			j++;
		}
		printstr("\n");
		i++;
	}
}

int*	bruteforce_search(graphe g){
	int n = g.order;
	int couleurs = 0;
	int i,j,k,l,n_combin,croissant;
	int* coloration;
	int** combin;
	int max_clique[n];  // peut contenir au max n noeuds
	int len_max_c; // toujours <= n
	int degree[n];

	// init
	for (i=0; i<n; i++){
		for (j=0; j<n; j++)
			g.adj_matrix[i][j] = 1 - g.adj_matrix[i][j];
		g.adj_matrix[i][i] = 0;
	}
	if ((coloration = (int*)malloc(sizeof(int) * n)) == NULL)
		return (0);
	for ( i=0; i<n; i++)
		coloration[i++] = 0;

	//boucle principale
	while (n>0)
	{
		for (i=0; i<n; i++ ){
			k = 0;
			for ( j=0; j<n; j++ )
				k += g.adj_matrix[i][j];
			degree[i] = k;
		}
		printstr("\n==> Encore ");
		printnbr(n);
		printstr(" noeuds a colorier.\nDegres: {");
		for ( i=0; i<n; i++ ){
			printstr(" ");
			printnbr(degree[i]);
		}
		printstr(" }\n");
		//=== recherche de la plus grande clique ===
		len_max_c = 0;
		k = n;
		while ( k>0 && len_max_c == 0 ){
			printstr("Recherche d'une clique de taille ");
			printnbr(k);
			printstr("...\n");
			//chercher si une clique de taille k existe
			l = 0;
			for ( i=0; i<n; i++ ){
				if ( degree[i] >= k-1 ){
					max_clique[l] = i;
					l++;
				}
			}
			//On a l noeuds de degré >=(k-1), rangés dans max_clique
			//s'il y en a moins de k: pas de k-clique possible
			if ( l>=k ){
				printstr("   clique possible. verifions... ");


				//chercher les combinaisons de k noeuds parmi ces l
				n_combin = combinaisons(l, k);
				printnbr(n_combin);
				printstr(" possibilites identifiees\n");
				if ((combin = (int**)malloc(sizeof(int*)*n_combin))==0)
					return (0);
				for ( i=0; i<n_combin; i++)
					if ((combin[i] = (int*)malloc(sizeof(int)*k))==0)
						return (0);
				for (i=0; i<n_combin; i++)
					for (j=0; j<k; j++)
						combin[i][j] = 0;
				for ( j=0; j<k; j++ ){
					combin[n_combin-1][j] = j+l-k;
					combin[0][j] = j; // la première combinaison est 0,1,2,...,k-1
				}
				j = 0;
				while (j<(n_combin-1)) // dernière combinaison possible
				{
					i = 1;
					croissant = 1;
					while (i < k) //vérifie l'ordre croissant
					{
						if (combin[j][i - 1] >= combin[j][i])
							croissant = 0;
						i++;
					}
					//~ disp_combin(combin, n_combin, k);
					if (croissant && j==(n_combin-2)) break;
					if (croissant){
						j++;
						i = 0;
						while (i < k) {// recopie la précédente combinaison
							combin[j][i] = combin[j-1][i];
							i++;
						}
					}
					combin[j][k - 1]++; //incrémente la dernière case
					i = k;
					while (i) // si une case a atteint l, on passe à la case précédente
					{
						i--;
						if (combin[j][i] == l )
						{
							if (i)
								combin[j][i - 1]++;
							combin[j][i] = 0 ;
						}
					}
				}





				//pour chaque combinaison, vérifier si c'est une clique:
				for ( i=0; i<n_combin; i++ ){
					if (is_clique(g, max_clique, k, combin[i])){
						//si oui, récupérer les numéros des noeuds concernés
						for ( j=0; j<k; j++ )
							max_clique[j] = max_clique[combin[i][j]];
						len_max_c = k;
						break;
					}
				}
				
				for ( i=0; i<n_combin; i++ )
					free(combin[i]);
				free(combin);
				combin = 0;
			}
			k--;
		}
		k++;
		//=== colorier la clique trouvée ===
		couleurs++;
		i = 0;
		j = 0;
		l = 0;
		while ( i<len_max_c ){
			if (coloration[l] != 0)
				j++;
			if (coloration[l] == 0 && l == (max_clique[i]+j)){
				coloration[l] = couleurs;
				i++;
			}
			l++;
		}

		//=== soustraire la clique trouvée au graphe g ===
		n -= len_max_c;
		printstr("   je retire ");
		printnbr(k);
		printstr(" noeuds, on aura n=");
		printnbr(n);
		printstr(".\n");
		remove_from_matrix(g.adj_matrix, max_clique, len_max_c, g.order);
		g.order = n;
	}
	return (coloration);
}


//================= ALGORITHME GLOUTON ==========================

int*	copyYdansZ(int* destination, int* origine, graphe g){
	int i;
	for ( i=0; i<g.order; i++ )
		destination[i] = origine[i];
	return (destination);
}

int		premier_sommet_de_Z(int* vecteur, graphe g){
	int i;
	for ( i=0; i<g.order; i++ )
		if ( vecteur[i] == 0 )
			return (i);
	return (-1);
}

int*	retirer_s_et_ses_voisins(int s, int* vecteur, graphe g){
	int i;
	vecteur[s] = 1;
	vecteur[g.order + 1]++;
	for ( i=0; i<g.order; i++)
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
	int color = 0;

	Y = (int *)malloc(sizeof(int)*(g.order+1));
	Z = (int *)malloc(sizeof(int)*(g.order+1));
	Z[g.order+1] = 0; //contient cardZ

	//init:
	for ( i=0; i<g.order; i++ ){
		Y[i] = 0;
		Z[i] = 0;
	}
	while ( cardY < g.order ){
		Z = copyYdansZ(Z, Y, g);
		Z[g.order+1] = cardY;
		color++;
		printstr("\n\tChangement de couleur a ");
		printnbr(color);
		while ( Z[g.order+1] < g.order ){
			s = premier_sommet_de_Z(Z,g);
			printstr("\ncoloration de ");
			printnbr(s);
			printstr(" avec la couleur ");
			printnbr(color);
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
		if (coloration_trouvee[i] == 0){
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
	return (0);
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
	int i,s;
	printstr("\n================== DSATUR ======================\n");
	init_dsat(g);
	for ( i=1; i<=g.order; i++ ){
		s = sommet_non_colorie_de_dsat_max(g);
		coloration_trouvee[s] = coloration_avec_couleur_minimum(s,g);
		printstr("\n s=");
		printnbr(s);
		printstr(" coloré par ");
		printnbr(coloration_trouvee[s]);
		maj_du_vecteur_dsat(s,g);
	}
}

