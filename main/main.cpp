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

typedef struct creature {
	char nev[21];
	int hp; //eletero
	int ero; //tamadas merteke
	int agi; // ugyesseg, a random faktor mellett egy kis segitseg
	inventory taska;
}creature;

node* createnode(int i) {
	node *newnode = (node*)malloc(sizeof(node));
	newnode->idx = i;
	newnode->left = NULL;
	newnode->right = NULL;
	return newnode;

}

node* insertleft(node *root, int i) {
	root->left = createnode(i);
	return root->left;
}

node* insertright(node *root, int i) {
	root->right = createnode(i);
	return root->right;
}

node* lepes(node *root, irany k) {
	FILE *fp;
	fp = fopen("fa.txt", "r");
	if (fp == NULL)
		return root;
	int data;
	char sbal[10], sjobb[10];
	do {
		fscanf(fp, "%d\t%s\t%s", &data, sbal, sjobb);
	} while (data != root->idx);

	switch (k) {
	case BAL: if (sbal[0] != 'N') { root = insertleft(root, atoi(sbal)); } return root;
	case JOBB: if (sjobb[0] != 'N') { root = insertright(root, atoi(sjobb)); } return root;
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

bool harc(node *root) {
	bool van_harc = false;
	FILE *fp = fopen("harc.txt", "r");
	if (fp == NULL)
		return;

	int data, bin;
	data = 0;
	do {
		fscanf(fp, "%d/t%d", &data, &bin);
	} while (data != root->idx);

	if (bin == 0)
	{
		return van_harc;
	}
	else
	{
		van_harc = true; return van_harc;
	}
}  // Ezt átkéne gondolni, mert így nincs értelme ha nem tudom hogy néz ki a file.

int main()
{
	node *root = createnode(1);
	char choice;
	mese(root);

	do {
		scanf("%c", &choice);

		switch (choice) {
		case 'j': root=lepes(root, JOBB); mese(root); break;
		case 'k': root=lepes(root, BAL); mese(root); break;
		}
	} while (choice != 'x');

	system("pause");
	return 0;
}