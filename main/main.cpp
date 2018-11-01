#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef enum irany { JOBB, BAL }irany; // Ket irany, a faba a leszarmazottakhoz

typedef struct node {
	int idx;
	node *left;
	node *right;
}node;  // Struktura egy csomoponthoz, index-e, illetve a két leszarmazottjahoz mutato pinter

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

node* newnode(int i) {
	node *newnode = (node*)malloc(sizeof(node));
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

node* lepes(node *root, irany k) {
	FILE *fp;
	fp = fopen("fa.txt", "r");
	if (fp == NULL)
		return root;
	int data;
	char sbal[10], sjobb[10], bolt[10], harc[10];
	do {
		fscanf(fp, "%d\t%s\t%s\t%s\t%s", &data, sbal, sjobb, bolt, harc);
	} while (data != root->idx);

	switch (k) {
	case BAL: if (sbal[0] != 'N') { root = insertleft(root, atoi(sbal)); } return root;
	case JOBB: if (sjobb[0] != 'N') { root = insertright(root, atoi(sjobb)); } return root;
	}
}

karakter penzes(karakter kalandor, int ar) {

}
karakter csata(karakter kalandor, int idx) {
	FILE *fp;
	fp = fopen("creatures.txt", "r");
	if (fp == NULL)
		return kalandor;
	int data, chp, cagi;
	do {
		fscanf(fp, "%d\t%d\t%d", &data, &chp, &cagi);
	} while (data != idx);

	bool vege = false;
	int dobas1, dobas2, cobas1, cobas2;
	while (!(vege)) {
		
	}
}

void mese(node *root) {
	FILE *fp;
	fp = fopen("story.txt", "r");
	if (fp == NULL)
	{
		return;
	}
	int data = 0;
	char szoveg[1000];
	do {
		fgets(szoveg, 1000, fp);
		data = atoi(&szoveg[0]);
	} while (data != root->idx);
	printf("%s", szoveg);
	
	
	return;
}

karakter vizsgal(node *root, karakter kalandor) {
	FILE *fp;
	fp = fopen("fa.txt", "r");
	if (fp == NULL)
		return kalandor;
	int data;
	char sbal[10], sjobb[10], bolt[10], harc[10];
	do {
		fscanf(fp, "%d\t%s\t%s\t%s\t%s", &data, sbal, sjobb, bolt, harc);
	} while (data != root->idx);
	if (bolt[0] != 'N')
	{
		kalandor=penzes(kalandor, atoi(bolt));
		
	}
	if (harc[0] != 'N')
	{
		kalandor = csata(kalandor, atoi(harc));
	}
	return kalandor;
}

void bevezeto() {
	printf("Udvozlegy blabla");
}
int main()
{
	karakter kalandor = { "Kalandor", 22, 11,{2,5} };
	bevezeto();
	node *root = newnode(1);
	char choice;
	mese(root);

	do {
		scanf("%c", &choice);

		switch (choice) {
		case 'j': root = lepes(root, JOBB); mese(root); kalandor=vizsgal(root, kalandor); break;
		case 'k': root=lepes(root, BAL); mese(root); break;
		}
	} while (choice != 'x');

	system("pause");
	return 0;
}