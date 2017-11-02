//
//  ViewController.h
//  iWillookDictionary
//
//  Created by Mewlan Musajan on 10/25/17.
//	Copyleft ( ) 2017 Mewlan Musajan. All Wrongs reserved.
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
	int yearofOrigin;
	struct DictionaryLL *next;
} DICT_LL;

typedef struct Dictionary
{
	char vocabulary[20];
	union {
		char latinDefinition[256];
		char cnDefinition[280];
	} definition;
	int yearofOrigin;
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
	english = 1,
	chinese,
	uyghur
} LANG;

LANG CurrentLanguage;
HOME *HomeViewPtr;
DICT *dictionary;
DICT *aDictionary;
unsigned short L;
unsigned short N;
char DictName[32];

/***Model***/

void homeViewModel();
void dictionaryModel();

/***ModelController***/

void dictModelUpdateViewController(DICT_LL *head);

/***View***/

void backView();
void searchViewbackView();
void backViewErrorView(char aChar);
void selectFunctionErrorView(char aString[12]);
void selectLanguageErrorView(char aString[16]);
void searchViewErrorView(char aString[32]);
void searchView();
void dataInsertInstructionView();
void catalogView();
void selectLanguageView();
void loadHomeView();

/***Controller***/

void selectFunctionController();
void selectLanguageViewController();
void catalogViewController();
void searchViewController();
DICT_LL * dataInsertController();
void selectionSort(DICT *aDict, unsigned short n);
