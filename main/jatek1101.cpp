#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

typedef enum irany { JOBB, BAL }irany; // Ket irany, a faba a leszarmazottakhoz

typedef struct node {
	int idx;
	node *left;
	node *right;
	int harcidx; // Amennyiben ez 0, akkor nincsen harc; ha meg nem 0, akkor az a leny indexet mutatja.
	int boltidx; // Amennyiben ez 0, akkor nincsen bolt; ha meg nem 0, akkor a "fizet" inputra, ennyi levonodik a penzbol
	//, amennyiben nem lenne az kisebb 0-nal. Így ez meg deedre vagy inventoryba kerul. Hozzateszem hogy a bolt != bolttal, azaz itt nem feltetlen vasarlas tortenik
}node;  // Struktura egy csomoponthoz, index-e, illetve a két leszarmazottjahoz mutato pointer

typedef struct inventory {
	int kaja;
	int penz; // Elkepzelheto hogy ennek nem lesz ertelme

}inventory;

typedef struct karakter {
	char nev[21];
	int hp; //eletero
	int agi; // ugyesseg, a random faktor mellett egy kis segitseg
	inventory taska;
}karakter;

typedef struct creature {
	int idx;
	int hp;
	int agi;
}creature;

typedef struct story {
	int index;
	char szoveg[2000];

}story;

node* newnode(int i) {
	node* newnode;
	newnode = (node*)malloc(sizeof(node));
	newnode->idx = i;
	newnode->left = NULL;
	newnode->right = NULL;
	return newnode;

}

node* insertleft(node *root, int i) {
	root->left = newnode(i);
	return root->left;
}

node* insertright(node *root, int i) {
	root->right = newnode(i);
	return root->right;
}

node* lepes(node *root, irany k, node *p, int hossz) {

	switch (k) {
	case BAL: if (root->left != 0)
	{
		for (int i = 0; i < hossz; i++)
			if (p[i].idx == root->left->idx)
			{
				*root = p[i]; return root;
			}
	}
			  return root;
	case JOBB: if (root->right != 0)
	{
		for (int i = 0; i < hossz; i++)
			if (p[i].idx == root->right->idx)
			{
				*root = p[i]; return root;
			}
	}
			   return root;
	default: return root;
	}
}

karakter penzes(karakter kalandor, int ar) {
	char ch;
	printf("A - Ha igen, B- Ha nem.: ");
	scanf("%c", &ch);
	if (ch == 'A' || ch == 'a')
		if (kalandor.taska.penz - ar >= 0)
		{
			kalandor.taska.penz = kalandor.taska.penz - ar;
		}
		else
		{
			printf("Nincs eleg penz!");
		}
	else
		return kalandor;

	return kalandor;
}
karakter csata(karakter kalandor, int idx, creature *k, int hossz) {
	for(int i=0;i<hossz;i++)
		if (idx == k[i].idx)
		{
			int cagi, chp, kdob1, kdob2, cdob1, cdob2, kdobasok, cdobasok;
			cagi=k[i].agi;
			chp=k[i].hp;
			bool vege = false;
			do {
				kdob1 = rand() % 6 + 1;
				kdob2 = rand() % 6 + 1;
				kdobasok = kdob1 + kdob2+kalandor.agi;
				printf("Kalandor: 1. dobas: %d  2. dobas:%d  Tamadoertek: %d\n", kdob1, kdob2, kdobasok);
				cdob1 = rand() % 6 + 1;
				cdob2 = rand() % 6 + 1;
				cdobasok = cdob1 + cdob2+cagi;
				printf("Ellenfel: 1. dobas: %d  2. dobas:%d  Tamadoertek: %d\n", cdob1, cdob2, cdobasok);
				if (kdobasok > cdobasok)
				{
					chp = chp - 2;
					printf("Megsebezted ellenfeled! Ellenfel hp: %d\n", chp);
					if (chp <= 0)
					{
						vege = true;
					}
				}
				else if (cdobasok > kdobasok)
				{
					kalandor.hp = kalandor.hp - 2;
					printf("Megsebeztek! Jelenlegi hp: %d\n", kalandor.hp);
					if (kalandor.hp <= 0)
					{
						vege = true;
					}
				}
			} while (vege != true);

			if (kalandor.hp > 0)
			{
				printf("Gyozelem!\n"); return kalandor;
			}
			else
			{
				printf("Kalandod veget ert!\n"); return kalandor;
			}
		}
	return kalandor;
}

void mese(node *root, story *s, int a) {

	for (int i = 0; i < a; i++)
	{
		if ((root->idx) == s[i].index)
			printf("%s\n", s[i].szoveg);
	}
	
	return;
}

karakter vizsgal(node *root, karakter kalandor, creature *k, int hossz) {
	if (root->boltidx!= 0)
	{
		kalandor=penzes(kalandor, root->boltidx);
		
	}
	if (root->harcidx!= 0)
	{
		kalandor = csata(kalandor, root->harcidx, k, hossz);
	}
	return kalandor;
}

node* fatolt(int *d) {
	FILE *fp = fopen("fa.txt", "r"); // FILE MEGNYITASA
	if (fp == NULL)
	{
		return 0;
	}

	int n;
	fscanf(fp, "#%d", &n);
	node *csucspontok;
	csucspontok = (node*)malloc(n * sizeof(node));
	int data, bal, jobb, harc, bolt;
	for (int i = 0; i < n; i++)
	{
		fscanf(fp, "%d\t%d\t%d\t%d\t%d", &data, &bal, &jobb, &harc, &bolt);
		csucspontok[i] = *newnode(data);
		node *k;
		k = &csucspontok[i];
		csucspontok[i].left = insertleft(k, bal);
		csucspontok[i].right = insertright(k, jobb);
		csucspontok[i].harcidx = harc;
		csucspontok[i].boltidx = bolt;

	} // Ennel a pontnal van egy node tipusu tombom, ami tartalmazza az osszes csomopontot azon belul annak indexét, leszármazottait, bolt illetve harciindexét.
	fclose(fp);
	*d = n;
	return csucspontok;
}

creature* creaturetolt(int *d) {
	FILE *fp = fopen("creatures.txt", "r");
	int n;
	fscanf(fp, "#%d", &n);
	creature *lenyek = (creature*)malloc(n * sizeof(creature));
	int cdata, chp, cagi;
	for (int i = 0; i < n; i++)
	{
		fscanf(fp, "%d\t%d\t%d", &cdata, &chp, &cagi);
		lenyek[i].idx = cdata;
		lenyek[i].hp = chp;
		lenyek[i].agi = cagi;
	}
	fclose(fp);
	*d = n;
	return lenyek;
}

story* storytolt(int *d) {
	FILE *fp = fopen("story.txt", "r");
	int n;
	fscanf(fp, "#%d", &n);
	story *mesecske = (story*)malloc(n * sizeof(story));
	int index;
	char szoveg[2000];
	for (int i = 0; i < n; i++)
	{
		fscanf(fp, "%d", &index);
		fgets(szoveg, 1999, fp);
		mesecske[i].index = index;
		//mesecske[i].szoveg = szoveg;
		strcpy(mesecske[i].szoveg, szoveg);
	}
	*d = n;
	return mesecske;
}

void bevezeto() {
	printf("[Insert udvozlo szoveg/jatek mukodese/hasznos parancsok/mire lehet szamitani/hattertortenet]\n");
}

int main()
{
	/****************************************************************************************************************************/
	node* p;
	int phossz;
	p = fatolt(&phossz);

	/************************************************************************************************************************/
	creature* k;
	int khossz; //ez itt koszos HAHA
	k = creaturetolt(&khossz);

	/************************************************************************************************************************/
	story* s;
	int shossz;;
	s = storytolt(&shossz);

	/************************************************************************************************************************/

	karakter kalandor = { "Kalandor", 22, 11,{2,5} };
	bevezeto();
	char choice;
	node *root;
	root = &p[0];
	mese(root, s, shossz);

	do {
		scanf("%c", &choice);

		switch (choice) {
		case 'j': root = lepes(root, JOBB, p, phossz); mese(root, s, shossz); kalandor=vizsgal(root, kalandor, k, khossz); break;
		case 'b': root= lepes(root, BAL, p, phossz); mese(root, s, shossz); kalandor = vizsgal(root, kalandor, k, khossz); break;
		case 'k': break;
		case 't': break;

		}
	} while (choice != 'x');

	system("pause");
	return 0;
}
