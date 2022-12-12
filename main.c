#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <time.h>

typedef enum irany { JOBB, BAL }irany; 

typedef struct node {
	int idx;
	struct node *left;
	struct node *right;
	int harcidx;
	int boltidx; 
}node;

typedef struct inventory {
	int kaja;
	int penz;
	int gomb;
	int bunda;
	int erme;
	int kc;
}inventory;

typedef struct karakter {
	char nev[21];
	int hp;
	int agi;
	inventory taska;
}karakter;

typedef struct creature {
	int idx;
	int hp;
	int agi;
}creature;

typedef struct story {
	int index;
	char szoveg[4000];

}story;

typedef struct valami{	
	int bal;
	int jobb;
	node *ptr;
}valami;	


node* newnode(int i, int harc, int bolt) {
	node* newnode;
	newnode = (node*)malloc(sizeof(node));
	newnode->idx = i;
	newnode->harcidx = harc;
	newnode->boltidx = bolt;
	newnode->left = NULL;
	newnode->right = NULL;
	return newnode;

}

node* lepes(node *root, irany k) {

	switch (k) {
	case BAL: if (root->left->idx != 0) { root = root->left; return root; }
			  return root;
	case JOBB: if (root->right->idx != 0) { root = root->right; return root; }
				return root;
	}
}

karakter penzes(karakter kalandor, int ar, int *d) {
	char ch;
	if (ar < 30)
	{
		printf("A - Ha igen, B- Ha nem.: ");
		scanf(" %c", &ch);
		if (ch == 'A' || ch == 'a') {
			if (kalandor.taska.penz - ar >= 0)
			{
				kalandor.taska.penz = kalandor.taska.penz - ar;
				switch (ar) {
				case 1: printf("Segitettel a raszorulonak!"); kalandor.taska.erme += 1; break;
				case 4:	printf("Sikeresen elelmiszerhez jutottal!"); kalandor.taska.kaja += 1; break;
				case 3: printf("Megvetted a tojast, majd el is dobtad. A tomeg tombol."); break;
				case 29: printf("Lefizetted az ort. Ne feledd: egyszeri bejutasra jogosit fel, ne hibazz!"); break;
				case 5: printf("Tipp: Az ordogkatlan fogadoban tartozkodik a hirhedt mafia!"); break;
				case 10: printf("Kifizeted a hangot, atnyujtod a resen a penzt"); kalandor.taska.penz -= 1; break;
				case 15: kalandor.agi += 1; printf("Ugyesseg pontod eggyel novekedett\n"); break;
				}
				*d = 1;
			}

			else
			{
				printf("Nincs eleg penz!"); *d = 2;
			}
		}
		else
		{
			*d = 2;
		}
	}
	else
	{
		switch (ar) { 
		case 31: kalandor.taska.penz = kalandor.taska.penz + 5; printf("+5 arany a taskaba"); break;
		case 32: kalandor.taska.bunda = kalandor.taska.bunda + 1; break;
		case 35: kalandor.taska.penz = kalandor.taska.penz + 10; printf("+10 arany a taskaba"); break;
		case 41:kalandor.taska.penz += 9; break;
		case 42: kalandor.taska.kaja += 1; break;
		case 45: kalandor.agi += 1; kalandor.taska.gomb += 1; break;
		case 46: kalandor.agi -= 1; break;
		}
		*d= 1;
	}
	return kalandor;
}

karakter csata(karakter kalandor, int idx, creature *k, int hossz, int *d) {
	srand(time(NULL));
	for(int i=0;i<hossz;i++)
		if (idx == k[i].idx)
		{
			int cagi, chp, kdob1, kdob2, cdob1, cdob2, kdobasok, cdobasok, crit;
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
				crit = rand() % 100 + 1;
				if (kdobasok > cdobasok)
				{
					if (crit != 100)
					{
						chp = chp - 2;
					}
					else
					{
						chp = chp - 4;
						printf("Kritikus talalat!");
					}

					printf("Megsebezted ellenfeled! Ellenfel hp: %d\n", chp);

					if (chp <= 0)
					{
						vege = true;
					}
				}
				else if (cdobasok > kdobasok)
				{
					if (crit != 100)
					{
						kalandor.hp = kalandor.hp - 2;
					}
					else
					{
						kalandor.hp = kalandor.hp - 4;
						printf("Kritikus talalat!");
					}

					printf("Megsebeztek! Jelenlegi hp: %d\n", kalandor.hp);

					if (kalandor.hp <= 0)
					{
						vege = true;
					}
				}
			} while (vege != true);

			if (kalandor.hp > 0)
			{
				printf("Gyozelem!\n"); *d = 1; kalandor.taska.kc += 1;  return kalandor;
			}
			else
			{
				printf("Vesztettel\n"); *d = 2; return kalandor;
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

karakter vizsgal(node *root, karakter kalandor, creature *k, int hossz, int *d) {
	int oldal=0;
	if ((root->boltidx)!= 0)
	{
		if ((root->boltidx)!= 30)
		{
			kalandor = penzes(kalandor, root->boltidx, &oldal);
			*d = oldal;
		}
		else
		{
			kalandor.taska.penz = 0;
		}
		
	}
	if ((root->harcidx)!= 0)
	{
		kalandor = csata(kalandor, root->harcidx, k, hossz, &oldal);
		*d = oldal;
	}
	
	return kalandor;
}

karakter kaja(karakter kalandor) {
	if ((kalandor.taska.kaja != 0) && (kalandor.hp < 20))
	{
		kalandor.taska.kaja = kalandor.taska.kaja - 1;
		kalandor.hp = kalandor.hp + 2;
		printf("2-vel nott az eleterod. Uj erteke: %d\n", kalandor.hp);
	}
	else
		printf("Nem vagy ehes/Nincs eleg ennivalod!\n");

	return kalandor;

}

void mivanbenne(karakter kalandor) {
	printf("Etel:%d\nPenz:%d\nVarazsgomb:%d\nBunda:%d\nErme:%d\n", kalandor.taska.kaja, kalandor.taska.penz, kalandor.taska.gomb, kalandor.taska.bunda, kalandor.taska.erme);
}				

node* fatolt() {
	FILE *fp = fopen("docs/map.txt", "r");
	if (fp == NULL)
	{
		return 0;
	}

	int n;
	fscanf(fp, "#%d", &n);
	valami *csucspontok;
	csucspontok = (valami*)malloc(n * sizeof(valami));
	int data, bal, jobb, harc, bolt;
	for (int i = 0; i < n; i++)
	{
		fscanf(fp, "%d\t%d\t%d\t%d\t%d", &data, &bal, &jobb, &harc, &bolt);
		csucspontok[i].ptr = newnode(data, harc, bolt);
		csucspontok[i].bal = bal;
		csucspontok[i].jobb = jobb;

	}
	fclose(fp);
	for (int i = 0; i < n; i++)
	{
		csucspontok[i].ptr->left = csucspontok[csucspontok[i].bal - 1].ptr;
		csucspontok[i].ptr->right = csucspontok[csucspontok[i].jobb - 1].ptr;
	}
	node *root = csucspontok[0].ptr;
	free(csucspontok);
	return root;
}

creature* creaturetolt(int *d) {
	FILE *fp = fopen("docs/creatures.txt", "r");
	if(fp == NULL)
	{
		return 0;
	}

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
	FILE *fp = fopen("docs/story.txt", "r");
	if (fp == NULL)
	{
		return 0;
	}
	int n;
	fscanf(fp, "#%d", &n);
	story *mesecske = (story*)malloc(n * sizeof(story));
	int index;
	char szoveg[4000];
	for (int i = 0; i < n; i++)
	{
		fscanf(fp, "%d", &index);
		fgets(szoveg, 3999, fp);
		mesecske[i].index = index;
		strcpy(mesecske[i].szoveg, szoveg);
	}
	fclose(fp);
	*d = n;
	return mesecske;
}

void bevezeto() {
	printf("\t\t\t~~~~~~~~~~~~~~~~~~~~BORALUSI KALANDOK~~~~~~~~~~~~~~~~~~~~\n");
	printf("\t\t\t~~~~~~~~~~~~~~~~~~~~ - Chapter 1 -~~~~~~~~~~~~~~~~~~~~\t\t\n");
	printf("\t\t\t[A teboly varosa]\n");
}

void stats(karakter kalandor) {
	FILE *out = fopen("stats.txt", "w");
	fprintf(out, "Legyozott ellenfelek: %d\n", kalandor.taska.kc);
	if (kalandor.taska.erme == 1)
	{
		fprintf(out, "Megszerezted a szegenyek ermejet");
	}
	if (kalandor.taska.gomb==1)
	{
		fprintf(out, "Megszerezted Dondulion hires varazsgombjet\n");
	}
	if (kalandor.taska.bunda==1)
	{
		fprintf(out, "Rajottel, hogy a verfarkasok allnak a maffia tenykedesei mogott.");
	}
	fclose(out);
	return;
}

void beolvas() {
	FILE *in = fopen("stats.txt", "r");
	if (in == NULL)
	{
		printf("kek");
		return;
	}

	printf("\t\t~~~~~~~~~~~~~DEEDS~~~~~~~~~~\n");
	char szoveg[100];
	while (fgets(szoveg, 61, in) !=NULL) {
		printf("%s", szoveg);
		}
	fclose(in);
	return;
}

int main()
{

	node* root = fatolt();
	creature* k;
	int khossz;
	k = creaturetolt(&khossz);
	story* s;
	int shossz;;
	s = storytolt(&shossz);
	karakter kalandor = { "Kalandor", 22, 11,{2,59, 0,0,0} };

	bevezeto();
	char choice;
	mese(root, s, shossz);
	int oldal;
	do {
		printf("Valasztasod: ");
		scanf(" %c", &choice);
		oldal = 0;
		switch (choice) {
		case 'j': root = lepes(root, JOBB); mese(root, s, shossz); kalandor = vizsgal(root, kalandor, k, khossz, &oldal);
			if (oldal == 1)
			{
				root = lepes(root, BAL); mese(root, s, shossz);
			}
			if(oldal == 2)
			{
				root = lepes(root, JOBB); mese(root, s, shossz);
			} break;
		case 'b': root= lepes(root, BAL); mese(root, s, shossz); kalandor = vizsgal(root, kalandor, k, khossz, &oldal);
			if (oldal == 1)
			{
				root = lepes(root, BAL); mese(root, s, shossz);
			}
			if (oldal == 2)
			{
				root = lepes(root, JOBB); mese(root, s, shossz);
			} break;
		case 'k': kalandor = kaja(kalandor); break;
		case 't': mivanbenne(kalandor); break;

		}
	} while (choice != 'x');

	stats(kalandor);
	beolvas();
	free(s);
	free(k);
	system("pause");
	return 0;
}