#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

typedef enum irany { JOBB, BAL }irany;

typedef struct node {
	int idx;
	node *left;
	node *right;
}node;

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

void lepes(node *root, irany k) {
	FILE *fp;
	fp = fopen("fa.txt", "r");
	if (fp == NULL)
		return;
	int data;
	char sbal[10], sjobb[10];
	do {
		fscanf(fp, "%d\t%s\t%s", &data, sbal, sjobb);
	} while (data != root->idx);

	switch (k) {
	case BAL: if (sbal[0] != 'N') { root = insertleft(root, atoi(sbal)); } return;
	case JOBB: if (sjobb[0] != 'N') { root = insertright(root, atoi(sjobb)); } return;
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

int main()
{
	node *root = createnode(1);
	char choice;
	mese(root);
	do {
		scanf("%c", &choice);

		switch (choice) {
		case 'j': lepes(root, JOBB); mese(root); break;
		case 'k': lepes(root, BAL); mese(root); break;
		}
	} while (choice != 'x');

	system("pause");
	return 0;
}
