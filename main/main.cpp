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
	int gomb;
}inventory;		//Inventory, igazából a megszerezhetõ tárgyakat tartalmazza

typedef struct karakter {
	char nev[21];
	int hp; //eletero
	int agi; // ugyesseg, a random faktor mellett egy kis segitseg
	inventory taska;
}karakter;	   //Kalandorunk strukturaja

typedef struct creature {
	int idx;
	int hp;
	int agi;
}creature;	   //Történet során különbözõ lények strukturaja

typedef struct story {
	int index;
	char szoveg[4000];

}story;			//A könnyebb kezelés érdekében, a történetet indexxel megáldva struktruaban tarolom

node* newnode(int i) {
	node* newnode;
	newnode = (node*)malloc(sizeof(node));
	newnode->idx = i;
	newnode->left = NULL;
	newnode->right = NULL;
	return newnode;

}//A (bináris) fában egy új csomópont létrejöttéért felelõs függvény			

node* insertleft(node *root, int i) {
	root->left = newnode(i);
	return root->left;
} //A fában, ezzel a függvénnyel hozzuk létre egy adott gyökérponthoz a bal oldali leszármazottját.  

node* insertright(node *root, int i) {
	root->right = newnode(i);
	return root->right;
} //A fában ezzel a függvénnyel hozzuk meg a gyökérnek a jobboldali leszármazottját

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
			 
	}
} //Ezzel a függvénnyel történik meg a léptetés, iránytól függõen

karakter penzes(karakter kalandor, int ar, int *d) {
	char ch;
	if (ar < 30)
	{
		printf("A - Ha igen, B- Ha nem.: ");
		scanf("%c", &ch);
		if (ch == 'A' || ch == 'a')
			if (kalandor.taska.penz - ar >= 0)
			{
				kalandor.taska.penz = kalandor.taska.penz - ar;
				switch (ar) { //vasarlas
				case 1: printf("Segitettel a raszorulonak!"); break;
				case 2:	printf("Sikeresen elelmiszerhez jutottal!"); break;
				case 29: printf("Lefizetted az ort. Ne feledd: egyszeri bejutasra jogosit fel, ne hibazz!"); break;
				case 5: printf("Tipp: Az ordogkatlan fogadoban tartozkodik a hirhedt mafia!"); break;
				}
				*d = 1;
			}
			else
			{
				printf("Nincs eleg penz!"); *d = 2;
			}
		else
		{
			*d = 2;
		}
	}
	else
	{
		switch (ar) { //++zseton
		case 31: kalandor.taska.penz = kalandor.taska.penz + 5; printf("+5 arany a taskaba"); break;
		}
		*d= 1;
	}
	return kalandor;
}

karakter csata(karakter kalandor, int idx, creature *k, int hossz, int *d) {
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
				printf("Gyozelem!\n"); *d = 1; return kalandor;
			}
			else
			{
				printf("Kalandod veget ert!\n"); *d = 2; return kalandor;
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
		printf("2-vel nott az eleterod. Uj erteke: %d", kalandor.hp);
	}
	else
		printf("Nem vagy ehes/Nincs eleg ennivalod!");

	return kalandor;

}

void mivanbenne(karakter kalandor) {
	printf("Etel:%d, Penz:%d, Varazsgomb:%d", kalandor.taska.kaja, kalandor.taska.penz, kalandor.taska.gomb);
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
	char szoveg[4000];
	for (int i = 0; i < n; i++)
	{
		fscanf(fp, "%d", &index);
		fgets(szoveg, 3999, fp);
		mesecske[i].index = index;
		//mesecske[i].szoveg = szoveg;
		strcpy(mesecske[i].szoveg, szoveg);
	}
	*d = n;
	return mesecske;
}

void bevezeto() {
	printf("[\tA teboly varosa\t]\n"); 
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

	karakter kalandor = { "Kalandor", 22, 11,{2,59, 0} };
	bevezeto();
	char choice;
	node *root;
	root = &p[0];
	mese(root, s, shossz);
	int oldal;
	do {
		scanf("%c", &choice);
		oldal = 0; //Ha a vizsgalbol 1-eskent jon vissza, azt jelenti, hogy a harcban gyozott/boltnal vasarolt(Automatikusan balra lep)
					//Ha a vizsgalatbol 2-eskent jon vissza, azt jelenti, hogy a harcban vesztett/boltnal nem vasarolt(vagy nem tudott(Automatikusan jobbra lep)
		switch (choice) {
		case 'j': root = lepes(root, JOBB, p, phossz); mese(root, s, shossz); kalandor = vizsgal(root, kalandor, k, khossz, &oldal);
			if (oldal == 1)
			{
				root = lepes(root, BAL, p, phossz); mese(root, s, shossz);
			}
			if(oldal == 2)
			{
				root = lepes(root, JOBB, p, phossz); mese(root, s, shossz);
			} break;
		case 'b': root= lepes(root, BAL, p, phossz); mese(root, s, shossz); kalandor = vizsgal(root, kalandor, k, khossz, &oldal);
			if (oldal == 1)
			{
				root = lepes(root, BAL, p, phossz); mese(root, s, shossz);
			}
			if (oldal == 2)
			{
				root = lepes(root, JOBB, p, phossz); mese(root, s, shossz);
			} break;
		case 'k': kalandor = kaja(kalandor); break;
		case 't': mivanbenne(kalandor); break;

		}
	} while (choice != 'x');

	system("pause");
	return 0;
}