//
//  iWillookDictionaryModelViewController.c
//  iWillookDictionary
//
//  Created by Mewlan Musajan on 10/25/17.
//  Copyright (c) 2017 Mewlan Musajan. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VOC 10
#define DICT_LL_LEN sizeof(struct DictionaryLL)
#define DICT_LEN sizeof(struct Dictionary)
#define HOME_LEN sizeof(struct HomeViewStruct)

typedef struct DictionaryLL
{
	char vocabulary[20];
	union {
		char latinDefinition[256];
		char cnDefinition[280];
	} definition;
	unsigned int yearofOrigin;
	struct DictionaryLL *next;
} DICT_LL;

typedef struct Dictionary
{
	char vocabulary[20];
	union {
		char latinDefinition[256];
		char cnDefinition[280];
	} definition;
	unsigned int yearofOrigin;
} DICT;

typedef struct HomeViewStruct
{
	char errorViewString[44];
	char dataInsertInstructionViewString[44];
	char dataUpdatedViewString[24];
	char searchViewString[24];
	char catalogViewString[20];
	char backViewString[24];
	char selectLanguageViewString[120];
	char functionDataInsertString[20];
	char functionsearchString[20];
	char functionCatalogString[20];
	char functionSelectLanguageString[20];

} HOME;

typedef enum Language
{
	english,
	chinese,
	uyghur
} LANG;

LANG CurrentLanguage;
unsigned short L;
unsigned short N;
char DictName[32];

/******Model******/

HOME * homeViewModel()
{
	FILE *languagePreferencefp;
	if ((languagePreferencefp = fopen("preference.dat", "r")) == NULL)
	{
		printf("can not open preference.dat\n");
		exit(0);
	}
	fscanf(languagePreferencefp, "%u", &L);
	fclose(languagePreferencefp);
	CurrentLanguage = L;
	FILE *homeViewfp;
	if ((homeViewfp = fopen("iWillookDictionaryViewString.dat", "rb")) == NULL)
	{
		printf("can not open iWillookDictionaryViewString.dat\n");
		exit(0);
	}
	switch (CurrentLanguage) {
		case english:rewind(homeViewfp);break;
		case chinese:fseek(homeViewfp, HOME_LEN, 0);break;
		case uyghur:fseek(homeViewfp, 2 * HOME_LEN, 0);break;
		default:break;
	}
	HOME homeView;
	if (fread(&homeView, HOME_LEN, 1, homeViewfp) != 1)
	{
		printf("homeView read error\n");
	}
	fclose(homeViewfp);
	HOME *homeViewPtr;
	homeViewPtr = &homeView;
	return(homeViewPtr);
}

DICT * dictionaryModel()
{
	FILE *dictionaryfp;
	if ((dictionaryfp = fopen("iWillookDictionary.dat", "rb")) == NULL)
	{
		printf("can not open iWillookDictionary.dat\n");
		exit(0);
	}
	switch (currentLanguage) {
		case english:rewind(dictionaryfp);break;
		case chinese:fseek(dictionaryfp, VOC * DICT_LEN, 0);break;
		case uyghur:fseek(dictionaryfp, 2 * VOC * DICT_LEN, 0);break;
		default:break;
	}
	DICT *dictionary;
	dictionary = (DICT*) calloc(VOC, DICT_LEN);
	for (int i = 0; i < VOC; ++i)
	{
		if (fread(&dictionary[i], DICT_LEN, 1, dictionaryfp) != 1)
		{
			printf("dictionary read error\n");
		}
	}
	fclose(dictionaryfp);
	return(dictionary);
}

/******View******/

void backView(HOME *homeViewPtr)
{
	printf("%s\n", (*homeViewPtr)->backViewString);
	char isBack;
	scanf("%c", &isBack);
	if (isBack == 'y')
	{
		void loadHomeView(HOME *homeViewPtr);
		loadHomeView(homeViewPtr);
	} else if (isBack == 'n')
	{
		exit(0);
	} else {
		void backViewErrorView(char aChar, HOME *homeViewPtr);
		backViewErrorView(isBack, homeViewPtr);
	}
}

void backViewErrorView(char aChar, HOME *homeViewPtr)
{
	printf("\'%c\' %s\n", aChar, (*homeViewPtr)->errorViewString);
	void backView(HOME *homeViewPtr);
	backView(homeViewPtr);
}

void selectFunctionErrorView(char aString[12], HOME *homeViewPtr)
{
	printf("\'%s\' %s\n", aString, (*homeViewPtr)->errorViewString);
	void selectFunctionController(HOME *homeViewPtr);
	selectFunctionController(homeViewPtr);
}

void selectLanguageErrorView(char aString[16])
{
	printf("\'%s\' is not a command.\n", aString);
}

void searchView(HOME *homeViewPtr)
{
	printf("%s\n", (*homeViewPtr)->searchViewString);
	DICT * dictionaryModel();
	void searchViewController(DICT *dictionaryPtr, HOME *homeViewPtr);
	searchViewController(dictionaryModel(), homeViewPtr);
}

void dataInsertInstructionView(HOME *homeViewPtr)
{
	printf("%s\n", (*homeViewPtr)->searchViewString);
	DICT_LL *head, *ptr1, *ptr2;
	ptr1 = ptr2 = (DICT_LL*) malloc(DICT_LL_LEN);
	dictModelUpdateViewController(dataInsertController(head, ptr1, ptr2));
	backView(homeViewPtr);
}

void catalogView(HOME *homeViewPtr)
{
	printf("%s\n", (*homeViewPtr)->catalogViewString);
	catalogViewController(dictionaryModel(), homeViewPtr);
	backView(homeViewPtr);
}

void selectLanguageView(HOME *homeViewPtr)
{
	printf("%s\n", (*homeViewPtr)->selectLanguageViewString);
	selectLanguageViewController();
}

void loadHomeView(HOME *homeViewPtr)
{
	printf("%s\n", (*homeViewPtr)->functionsearchString);
	printf("%s\n", (*homeViewPtr)->functionCatalogString);
	printf("%s\n", (*homeViewPtr)->functionDataInsertString);
	printf("%s\n", (*homeViewPtr)->functionSelectLanguageString);
	void selectFunctionController(HOME *homeViewPtr);
	selectFunctionController(homeViewPtr);
}

/******Controller******/

void selectFunctionController(HOME *homeViewPtr)
{
	char aString[32];
	scanf("%s", aString);
	switch (CurrentLanguage) {
		case english:
			if (strcmp(aString, "search") == 0)
			{
				void searchView(HOME *homeViewPtr);
				searchView(homeViewPtr);
			} else if (strcmp(aString, "CATALOG") == 0)
			{
				void catalogView(HOME *homeViewPtr);
				catalogView(homeViewPtr);
			} else if(strcmp(aString, "INSERT") == 0)
			{
				void dataInsertInstructionView(HOME *homeViewPtr);
				dataInsertInstructionView(homeViewPtr);
			} else if (strcmp(aString, "PREFERENCE" == 0))
			{
				void selectLanguageView(HOME *homeViewPtr);
				selectLanguageView(homeViewPtr);
			} else {
				void selectFunctionErrorView(char aString, HOME *homeViewPtr);
				selectFunctionErrorView(aString, homeViewPtr);
			};
		break;
		case chinese:
			if (strcmp(aString, "ËÑË÷") == 0)
			{
				void searchView(HOME *homeViewPtr);
				searchView(homeViewPtr);
			} else if (strcmp(aString, "Ä¿Â¼") == 0)
			{
				void catalogView(HOME *homeViewPtr);
				catalogView(homeViewPtr);
			} else if (strcmp(aString, "ÊäÈë") == 0)
			{
				void dataInsertInstructionView(HOME *homeViewPtr);
				dataInsertInstructionView(homeViewPtr);
			} else if (strcmp(aString, "ÉèÖÃ") == 0)
			{
				void selectLanguageView(HOME *homeViewPtr);
				selectLanguageView(homeViewPtr);
			} else {
				void selectFunctionErrorView(char aString, HOME *homeViewPtr);
				selectFunctionErrorView(aString, homeViewPtr);
			};
		break;
		case uyghur:
			if (strcmp(aString, "IZDE") == 0)
			{
				void searchView(HOME *homeViewPtr);
				searchView(homeViewPtr);
			} else if (strcmp(aString, "MUNDERIJE") == 0)
			{
				void catalogView(HOME *homeViewPtr);
				catalogView(homeViewPtr);
			} else if (strcmp(aString, "KIRGUZUSH") == 0)
			{
				void dataInsertInstructionView(HOME *homeViewPtr);
				dataInsertInstructionView(homeViewPtr);
			} else if (strcmp(aString, "TENGSHEK") == 0)
			{
				void selectLanguageView(HOME *homeViewPtr);
				selectLanguageView(homeViewPtr);
			} else {
				void selectFunctionErrorView(char aString, HOME *homeViewPtr);
				selectFunctionErrorView(aString, homeViewPtr);
			};
		break;
		default:break;
	}
}

void selectLanguageViewController()
{
	char aString[16];
	scanf("%s", aString);
	if (strcmp(aString, "ENGLISH") == 0)
	{
		L = CurrentLanguage = english;
		FILE *fp;
		if ((fp = fopen("preference.dat", "wb")) == NULL)
		{
			printf("can not open file.\n");
			return;
		}
		if (fwrite(&L, sizeof(unsigned short), 1, fp) != 1)
		{
			printf("file write error.\n");
		}
		fclose(fp);
	} else if (strcmp(aString, "CHINESE") == 0)
	{
		L = CurrentLanguage = chinese;
		FILE *fp;
		if ((fp = fopen("preference.dat", "wb")) == NULL)
		{
			printf("can not open file.\n");
			return;
		}
		if (fwrite(&L, sizeof(unsigned short), 1, fp) != 1)
		{
			printf("file write error.\n");
		}
		fclose(fp);
	} else if (strcmp(aString, "UYGHUR") == 0)
	{
		L = CurrentLanguage = chinese;
		FILE *fp;
		if ((fp = fopen("preference.dat", "wb")) == NULL)
		{
			printf("can not open file.\n");
			return;
		}
		if (fwrite(&L, sizeof(unsigned short), 1, fp) != 1)
		{
			printf("file write error.\n");
		}
		fclose(fp);
	} else {
		selectLanguageErrorView(aString);
		selectLanguageViewController();
	}
}

void catalogViewController(DICT *dictionaryPtr, HOME *homeViewPtr)
{
	for (int i = 0; i < VOC; ++i)
	{
		printf("%d. ", i + 1);
		printf("%s\n", *(dictionaryPtr + i)->vocabulary);
	}
}

void searchViewController(DICT *dictionaryPtr, HOME *homeViewPtr)
{
	char aString[32];
	scanf("%s", aString);
	for (int i = 0; i < VOC; ++i)
	{
		if (strcmp(aString, (*dictionaryPtr)[i]->vocabulary) == 0)
		{
			switch (CurrentLanguage) {
				case uyghur:
				case english:
					printf("%s\n", (*dictionaryPtr)[i]->definition.latinDefinition);
					printf("Year of Origin: %d\n", (*dictionaryPtr)[i]->yearofOrigin);
				break;
				case chinese:
					printf("%s\n", (*dictionaryPtr)[i]->definition.cnDefinition);
					printf("Year of Origin: %d\n", (*dictionaryPtr)[i]->yearofOrigin);
				break;
				default:break;
			}
		}
	}
	void backView(HOME *homeViewPtr);
	backView(homeViewPtr);
}

DICT_LL * dataInsertController(DICT_LL *head, DICT_LL *ptr1, DICT_LL *ptr2)
{
	scanf("%s", DictName);
	strcat(DictName, ".dat");
	switch (CurrentLanguage) {
		case uyghur:
		case english:
		scanf("%[^\n]%*c%[^\n]%*c%u", ptr1->vocabulary, ptr1->definition.latinDefinition, &ptr1->yearofOrigin);
		fflush(stdin);
		N = 0;
		head = NULL;
		while (ptr1->yearofOrigin != 2017) {
			++N;
			if (N == 1)
			{
				head = ptr1;
			} else {
				ptr2->next = ptr1;
			}
			ptr2 = ptr1;
			ptr1 = (DICT_LL*) malloc(DICT_LL_LEN);
			scanf("%[^\n]%*c%[^\n]%*c%u", ptr1->vocabulary, ptr1->definition.latinDefinition, &ptr1->yearofOrigin);
			fflush(stdin);
		}
		ptr2->next = NULL;
		return(head);
		break;
		case chinese:
		scanf("%[^\n]%*c%[^\n]%*c%u", ptr1->vocabulary, ptr1->definition.cnDefinition, &ptr1->yearofOrigin);
		fflush(stdin);
		N = 0;
		head = NULL;
		while (ptr1->yearofOrigin != 2017) {
			++N;
			if (N == 1)
			{
				head = ptr1;
			} else {
				ptr2->next = ptr1;
			}
			ptr2 = ptr1;
			ptr1 = (DICT_LL*) malloc(DICT_LL_LEN);
			scanf("%[^\n]%*c%[^\n]%*c%u", ptr1->vocabulary, ptr1->definition.cnDefinition, &ptr1->yearofOrigin);
			fflush(stdin);
		}
		ptr2->next = NULL;
		return(head);
		break;
		default:break;
	}
	return 0;
}

void dictModelUpdateViewController(DICT_LL *head)
{
	FILE *dictionaryfp;
	if ((dictionaryfp = fopen(DictName, "wb")) == NULL)
	{
		printf("can not open %s\n", DictName);
		exit(0);
	}
	switch (CurrentLanguage) {
		case english:rewind(dictionaryfp);break;
		case chinese:fseek(dictionaryfp, N * DICT_LEN, 0);break;
		case uyghur:fseek(dictionaryfp, 2 * N * DICT_LEN, 0);break;
		default:break;
	}
	DICT *dictionary;
	dictionary = (DICT*) calloc(N, DICT_LEN);
	DICT_LL *dictPtr = head;
	for (int i = 0; dictPtr->yearofOrigin != 2017; ++i, dictPtr = dictPtr->next)
	{
		*(dictionary + i)->vocabulary = dictPtr->vocabulary;
		switch (CurrentLanguage) {
			case uyghur:
			case english:
				*(dictionary + i)->definition.latinDefinition = dictPtr->definition.latinDefinition;
			break;
			case chinese:
				*(dictionary + i)->definition.cnDefinition = dictPtr->definition.cnDefinition;
			break;
			default:break;
		}
	}
	for (int i = 0; i < N; ++i)
	{
		if (fwrite(&dictionary[i], DICT_LEN, 1, dictionaryfp) != 1)
		{
			printf("dictionary write error\n");
		printf("%d. ", i + 1);
		switch (CurrentLanguage) {
			case uyghur:
			case english:
			printf("%s\n%s\n%u\n", dictionary[i].vocabulary, dictionary[i].definition.latinDefinition, dictionary[i].yearofOrigin);
			break;
			case chinese:
			printf("%s\n%s\n%u\n", dictionary[i].vocabulary, dictionary[i].definition.cnDefinition, dictionary[i].yearofOrigin);
			break;
			default:break;
		}
		}
	}
	free(dictionary);
	fclose(dictionaryfp);
}


int main(int argc, char const *argv[])
{
	HOME * homeViewModel();
	void loadHomeView(HOME *homeViewPtr);
	loadHomeView(homeViewModel());

	return 0;
}
