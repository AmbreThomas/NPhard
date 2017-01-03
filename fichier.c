#include "graphe.h"

int		int_max(int *tab, unsigned int taille){
	unsigned int i;
	int max;

	max = tab[1];
	i = 1;
	while (i <= taille)
	{
		if (tab[i] > max)
			max = tab[i];
		i++;
	}
	return (max);
}

int		nb_occurences_int(int *tab, int taille, int n){
	int i;
	int j;

	j = 1;
	i = 0;
	while (j <= taille)
	{
		if (tab[j++] == n)
			i++;
	}
	return (i);
}

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

void	ecrire_fichier_sortie(int *coloration, graphe g){
	int i, k, max_boites, flag;
	char *stock_itoa;
	char **sortie_fichier;
	char premier[] = "We use ";
	char deuxieme[] = " petri dishes.\n";
	FILE* sortie = NULL;

	sortie = fopen("sortie", "w");
	max_boites = int_max(coloration, g.order);
	sortie_fichier = (char**)malloc(sizeof(char*) * (max_boites + 2));
	stock_itoa = ft_itoa(max_boites);
	sortie_fichier[0] = (char*)malloc(strlen("We use  petri dishes.\n") + strlen(stock_itoa) + 1);
	sortie_fichier[0] = strcat(strcat(premier, stock_itoa), deuxieme);
	i = 1;
	while (i <= max_boites)
	{
		sortie_fichier[i] = (char*)malloc(nb_occurences_int(coloration, g.order, i) + 1);
		i++;
	}
	sortie_fichier[i] = 0;
	i = 1;
	while (i <= max_boites)
	{
		k = 1;
		flag = 0;
		while (k <= g.order)
		{
			if (coloration[k] == i)
			{
				sortie_fichier[i][flag++] = k + 64;
				sortie_fichier[i][flag++] = ',';
			}
			k++;
		}
		sortie_fichier[i][--flag] = '.';
		sortie_fichier[i][++flag] = '\n';
		sortie_fichier[i][++flag] = 0;
		i++;
	}
	i = 0;
	while (sortie_fichier[i] && sortie != NULL)
	{
		printf("%s", sortie_fichier[i]);
		fputs(sortie_fichier[i++], sortie);
	}
	fclose(sortie);
}
