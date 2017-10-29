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
	char errorViewString[52];
	char dataInsertInstructionViewString[80];
	char dataUpdatedViewString[32];
	char searchViewString[32];
	char catalogViewString[32];
	char backViewString[32];
	char selectLanguageViewString[120];
	char functionDataInsertString[32];
	char functionSearchString[32];
	char functionCatalogString[32];
	char functionSelectLanguageString[32];

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
	printf("homeViewModel\n");
	FILE *languagePreferencefp;
	if ((languagePreferencefp = fopen("preference.dat", "rb")) == NULL)
	{
		printf("can not open preference.dat\n");
		exit(0);
	}
	if (fread(&L, sizeof(unsigned short), 1, languagePreferencefp) != 1)
	{
		printf("preference read error.\n");
	}
	fclose(languagePreferencefp);
	CurrentLanguage = L;
	FILE *homeViewfp;
	if ((homeViewfp = fopen("iWillDictViewString.dat", "rb")) == NULL)
	{
		printf("can not open iWillDictViewString.dat\n");
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
	HOME *homeViewPtr;
	homeViewPtr = &homeView;
	printf(homeViewPtr->errorViewString);
	putchar('\n');
	printf(homeViewPtr->dataInsertInstructionViewString, '\n', '\n', '\n');
	putchar('\n');
	printf(homeViewPtr->dataUpdatedViewString, '\0');
	putchar('\n');
	printf(homeViewPtr->searchViewString);
	putchar('\n');
	printf(homeViewPtr->catalogViewString);
	putchar('\n');
	printf(homeViewPtr->backViewString);
	putchar('\n');
	printf(homeViewPtr->selectLanguageViewString, '\t', '\n', '\t', '\n', '\t');
	putchar('\n');
	printf(homeViewPtr->functionDataInsertString, '\t');
	putchar('\n');
	printf(homeViewPtr->functionSearchString, '\t');
	putchar('\n');
	printf(homeViewPtr->functionCatalogString, '\t');
	putchar('\n');
	printf(homeViewPtr->functionSelectLanguageString, '\t');
	putchar('\n');
	fclose(homeViewfp);
	return(homeViewPtr);
}

DICT * dictionaryModel()
{
	printf("dictionaryModel\n");
	FILE *dictionaryfp;
	if ((dictionaryfp = fopen("iWillookDictionary.dat", "rb")) == NULL)
	{
		printf("can not open iWillookDictionary.dat\n");
		exit(0);
	}
	switch (CurrentLanguage) {
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
	printf("backView\n");
	printf("%s\n", homeViewPtr->backViewString);
	char isBack;
	scanf("%c", &isBack);
	if (isBack == 'y')
	{
		loadHomeView(homeViewPtr);
	} else if (isBack == 'n')
	{
		exit(0);
	} else {
		backViewErrorView(isBack, homeViewPtr);
	}
}

void backViewErrorView(char aChar, HOME *homeViewPtr)
{
	printf("backViewErrorView\n");
	printf("\'%c\'\t%s\n", aChar, homeViewPtr->errorViewString);
	backView(homeViewPtr);
}

void selectFunctionErrorView(char aString[12], HOME *homeViewPtr)
{
	printf("selectFunctionErrorView\n");
	printf("\'%s\'\t%s\n", aString, homeViewPtr->errorViewString);
	selectFunctionController(homeViewPtr);
}

void selectLanguageErrorView(char aString[16])
{
	printf("selectLanguageErrorView\n");
	printf("\'%s\'\tis not a command.\n", aString);
}

void searchView(HOME *homeViewPtr)
{
	printf("searchView\n");
	printf("%s\n", homeViewPtr->searchViewString);
	DICT * dictionaryModel();
	searchViewController(dictionaryModel(), homeViewPtr);
}

void dataInsertInstructionView(HOME *homeViewPtr)
{
	printf("dataInsertInstructionView\n");
	printf(homeViewPtr->dataInsertInstructionViewString, '\n', '\n', '\n');
	putchar('\n');
	DICT_LL * dataInsertController(DICT_LL *head, DICT_LL *ptr1, DICT_LL *ptr2);
	DICT_LL *head, *ptr1, *ptr2;
	ptr1 = ptr2 = (DICT_LL*) malloc(DICT_LL_LEN);
	dictModelUpdateViewController(dataInsertController(head, ptr1, ptr2));
	backView(homeViewPtr);
}

void catalogView(HOME *homeViewPtr)
{
	printf("catalogView\n");
	printf("%s\n", homeViewPtr->catalogViewString);
	catalogViewController(dictionaryModel(), homeViewPtr);
	backView(homeViewPtr);
}

void selectLanguageView(HOME *homeViewPtr)
{
	printf("selectLanguageView\n");
	printf(homeViewPtr->selectLanguageViewString, '\t', '\n', '\t', '\n', '\t');
	putchar('\n');
	selectLanguageViewController();
}

void loadHomeView(HOME *homeViewPtr)
{
	printf("loadHomeView\n");
	printf(homeViewPtr->functionSearchString, '\t');
	putchar('\n');
	printf(homeViewPtr->functionCatalogString, '\t');
	putchar('\n');
	printf(homeViewPtr->functionDataInsertString, '\t');
	putchar('\n');
	printf(homeViewPtr->functionSelectLanguageString, '\t');
	putchar('\n');
	selectFunctionController(homeViewPtr);
}

/******Controller******/

void selectFunctionController(HOME *homeViewPtr)
{
	printf("selectFunctionController\n");
	char aString[32];
	scanf("%s", aString);
	switch (CurrentLanguage) {
		case english:
			if (strcmp(aString, "SEARCH") == 0)
			{
				searchView(homeViewPtr);
			} else if (strcmp(aString, "CATALOG") == 0)
			{
				catalogView(homeViewPtr);
			} else if(strcmp(aString, "INSERT") == 0)
			{
				dataInsertInstructionView(homeViewPtr);
			} else if (strcmp(aString, "PREFERENCE" == 0))
			{
				selectLanguageView(homeViewPtr);
			} else {
				selectFunctionErrorView(aString, homeViewPtr);
			};
		break;
		case chinese:
			if (strcmp(aString, "����") == 0)
			{
				searchView(homeViewPtr);
			} else if (strcmp(aString, "Ŀ¼") == 0)
			{
				catalogView(homeViewPtr);
			} else if (strcmp(aString, "����") == 0)
			{
				dataInsertInstructionView(homeViewPtr);
			} else if (strcmp(aString, "����") == 0)
			{
				selectLanguageView(homeViewPtr);
			} else {
				selectFunctionErrorView(aString, homeViewPtr);
			};
		break;
		case uyghur:
			if (strcmp(aString, "IZDE") == 0)
			{
				searchView(homeViewPtr);
			} else if (strcmp(aString, "MUNDERIJE") == 0)
			{
				catalogView(homeViewPtr);
			} else if (strcmp(aString, "KIRGUZUSH") == 0)
			{
				dataInsertInstructionView(homeViewPtr);
			} else if (strcmp(aString, "TENGSHEK") == 0)
			{
				selectLanguageView(homeViewPtr);
			} else {
				selectFunctionErrorView(aString, homeViewPtr);
			};
		break;
		default:break;
	}
}

void selectLanguageViewController()
{
	printf("selectLanguageViewController\n");
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
	printf("catalogViewController\n");
	for (int i = 0; i < VOC; ++i)
	{
		printf("%d. ", i + 1);
		printf("%s\n", *(dictionaryPtr + i)->vocabulary);
	}
}

void searchViewController(DICT *dictionaryPtr, HOME *homeViewPtr)
{
	printf("searchViewController\n");
	char aString[32];
	scanf("%s", aString);
	for (int i = 0; i < VOC; ++i)
	{
		if (strcmp(aString, *(dictionaryPtr + i)->vocabulary) == 0)
		{
			switch (CurrentLanguage) {
				case uyghur:
				case english:
					printf("%s\n", *(dictionaryPtr + i)->definition.latinDefinition);
					// printf("%u\n", *(dictionaryPtr + i)->yearofOrigin);
				break;
				case chinese:
					printf("%s\n", *(dictionaryPtr + i)->definition.cnDefinition);
					// printf("%u\n", *(dictionaryPtr + i)->yearofOrigin);
				break;
				default:break;
			}
		}
	}
	backView(homeViewPtr);
}

DICT_LL * dataInsertController(DICT_LL *head, DICT_LL *ptr1, DICT_LL *ptr2)
{
	printf("dataInsertController\n");
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
	printf("dictModelUpdateViewController\n");
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