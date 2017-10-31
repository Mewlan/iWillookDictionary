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
			english,
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

		void homeViewModel()
		{
			printf("log: homeViewModel\n");
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
			static HOME homeView;
			if (fread(&homeView, HOME_LEN, 1, homeViewfp) != 1)
			{
				printf("homeView read error\n");
			}
			HomeViewPtr = &homeView;
			fclose(homeViewfp);
		}

		void dictionaryModel()
		{
			printf("log: dictionaryModel\n");
			FILE *dictionaryfp;
			if ((dictionaryfp = fopen("iWillookDictionary.dat", "rb")) == NULL)
			{
				printf("can not open iWillookDictionary.dat\n");
				exit(0);
			}
			switch (CurrentLanguage) {
				case english:rewind(dictionaryfp);break;
				case chinese:fseek(dictionaryfp, VOC * DICT_LEN, 0);break;
				case uyghur:fseek(dictionaryfp, (3 * VOC) * DICT_LEN, 0);break;
				default:break;
			}
			for (int i = 0; i < VOC; ++i)
			{
				if (fread(&dictionary[i], DICT_LEN, 1, dictionaryfp) != 1)
				{
					printf("dictionary read error\n");
				}
				printf("%s\n", dictionary[i].vocabulary);
			}
			fclose(dictionaryfp);
		}

		/***View***/

		void backView()
		{
			printf("log: backView\n");
			printf("%s\n", HomeViewPtr->backViewString);
			char isBack;
			isBack = getchar();
			if (isBack == 'y')
			{
				loadHomeView();
			} else if (isBack == 'n')
			{
				exit(0);
			} else {
				backViewErrorView(isBack);
			}
		}

		void backViewErrorView(char aChar)
		{
			printf("log: backViewErrorView\n");
			printf("\'%c\'\t%s\n", aChar, HomeViewPtr->errorViewString);
			backView();
		}

		void selectFunctionErrorView(char aString[12])
		{
			printf("log: selectFunctionErrorView\n");
			printf("\'%s\'\t%s\n", aString, HomeViewPtr->errorViewString);
			selectFunctionController();
		}

		void selectLanguageErrorView(char aString[16])
		{
			printf("log: selectLanguageErrorView\n");
			printf("\'%s\'\tis not a command.\n", aString);
		}

		void searchView()
		{
			printf("log: searchView\n");
			printf("%s\n", HomeViewPtr->searchViewString);
			dictionary = (DICT*) calloc(VOC, DICT_LEN);
			dictionaryModel();
			searchViewController();
			free(dictionary);
		}

		void dataInsertInstructionView()
		{
			printf("log: dataInsertInstructionView\n");
			printf(HomeViewPtr->dataInsertInstructionViewString, '\n', '\n', '\n');
			putchar('\n');
			DICT_LL * dataInsertController();
			aDictionary = (DICT*) calloc(N, DICT_LEN);
			dictModelUpdateViewController(dataInsertController());
			free(aDictionary);
			backView();
		}

		void catalogView()
		{
			printf("log: catalogView\n");
			printf("%s\n", HomeViewPtr->catalogViewString);
			dictionary = (DICT*) calloc(VOC, DICT_LEN);
			dictionaryModel();
			catalogViewController();
			free(dictionary);
			backView();
		}

		void selectLanguageView()
		{
			printf("log: selectLanguageView\n");
			printf(HomeViewPtr->selectLanguageViewString, '\t', '\n', '\t', '\n', '\t');
			putchar('\n');
			selectLanguageViewController();
		}

		void loadHomeView()
		{
			printf("log: loadHomeView\n");
			printf(HomeViewPtr->functionSearchString, '\t');
			putchar('\n');
			printf(HomeViewPtr->functionCatalogString, '\t');
			putchar('\n');
			printf(HomeViewPtr->functionDataInsertString, '\t');
			putchar('\n');
			printf(HomeViewPtr->functionSelectLanguageString, '\t');
			putchar('\n');
			selectFunctionController();
		}

		/***Controller***/

		void selectFunctionController()
		{
			printf("log: selectFunctionController\n");
			char aString[32];
			scanf("%s", aString);
			fflush(stdin);
			switch (CurrentLanguage) {
				case english:
					if (strcmp(aString, "SEARCH") == 0)
					{
						searchView();
					} else if (strcmp(aString, "CATALOG") == 0)
					{
						catalogView();
					} else if(strcmp(aString, "INSERT") == 0)
					{
						dataInsertInstructionView();
					} else if (strcmp(aString, "PREFERENCE") == 0)
					{
						selectLanguageView();
					} else {
						selectFunctionErrorView(aString);
					};
				break;
				case chinese:
					if (strcmp(aString, "ËÑË÷") == 0)
					{
						searchView();
					} else if (strcmp(aString, "Ä¿Â¼") == 0)
					{
						catalogView();
					} else if (strcmp(aString, "ÊäÈë") == 0)
					{
						dataInsertInstructionView();
					} else if (strcmp(aString, "ÉèÖÃ") == 0)
					{
						selectLanguageView();
					} else {
						selectFunctionErrorView(aString);
					};
				break;
				case uyghur:
					if (strcmp(aString, "IZDE") == 0)
					{
						searchView();
					} else if (strcmp(aString, "MUNDERIJE") == 0)
					{
						catalogView();
					} else if (strcmp(aString, "KIRGUZUSH") == 0)
					{
						dataInsertInstructionView();
					} else if (strcmp(aString, "TENGSHEK") == 0)
					{
						selectLanguageView();
					} else {
						selectFunctionErrorView(aString);
					};
				break;
				default:break;
			}
		}

		void selectLanguageViewController()
		{
			printf("log: selectLanguageViewController\n");
			char aString[16];
			scanf("%s", aString);
			fflush(stdin);
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
				homeViewModel();
				backView();
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
				homeViewModel();
				backView();
			} else if (strcmp(aString, "UYGHUR") == 0)
			{
				L = CurrentLanguage = uyghur;
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
				homeViewModel();
				backView();
			} else {
				selectLanguageErrorView(aString);
				selectLanguageViewController();
			}
		}

		void catalogViewController()
		{
			printf("log: catalogViewController\n");
			for (int i = 0; i < VOC; ++i)
			{
				printf("%d. ", i + 1);
				printf("%s\n", dictionary[i].vocabulary);
			}
		}

		void searchViewController()
		{
			printf("log: searchViewController\n");
			char aString[32];
			scanf("%s", aString);
			fflush(stdin);
			for (int i = 0; i < VOC; ++i)
			{
				if (strcmp(aString, dictionary[i].vocabulary) == 0)
				{
					switch (CurrentLanguage) {
						case english:
						case uyghur:
							printf("log: searchViewController:definition\n");
							printf("%s\n", dictionary[i].definition.latinDefinition);
							// printf("%d\n", *(dictionaryPtr + i)->yearofOrigin);
						break;
						case chinese:
							printf("%s\n", dictionary[i].definition.cnDefinition);
							// printf("%d\n", *(dictionaryPtr + i)->yearofOrigin);
						break;
						default:break;
					}
				}
			}
			backView();
		}

		DICT_LL * dataInsertController()
		{
			printf("log: dataInsertController\n");
			scanf("%s", DictName);
			fflush(stdin);
			strcat(DictName, ".dat");
			switch (CurrentLanguage) {
				case uyghur:
				case english:
                {
					DICT_LL *hp;
                    DICT_LL *p1, *p2;
					N = 0;
					p1 = p2 = (DICT_LL*) malloc(DICT_LL_LEN);
					scanf("%[^\n]%*c%[^\n]%*c%d", p1->vocabulary, p1->definition.latinDefinition, &p1->yearofOrigin);
					fflush(stdin);
					hp = NULL;
					while (p1->yearofOrigin != 2017) {
						fflush(stdin);
						++N;
						if (N == 1)
						{
							hp = p1;
						} else {
							p2->next = p1;
						}
						p2 = p1;
						p1 = (DICT_LL*) malloc(DICT_LL_LEN);
						scanf("%[^\n]%*c%[^\n]%*c%d", p1->vocabulary, p1->definition.latinDefinition, &p1->yearofOrigin);
						fflush(stdin);
					}
					p2->next = NULL;
					return(hp);
                }
				break;
				case chinese:
                {
					DICT_LL *hp;
                    DICT_LL *p1, *p2;
					N = 0;
					p1 = p2 = (DICT_LL*) malloc(DICT_LL_LEN);
					scanf("%[^\n]%*c%[^\n]%*c%d", p1->vocabulary, p1->definition.cnDefinition, &p1->yearofOrigin);
					fflush(stdin);
					hp = NULL;
					while (p1->yearofOrigin != 2017) {
						fflush(stdin);
						++N;
						if (N == 1)
						{
							hp = p1;
						} else {
							p2->next = p1;
						}
						p2 = p1;
						p1 = (DICT_LL*) malloc(DICT_LL_LEN);
						scanf("%[^\n]%*c%[^\n]%*c%d", p1->vocabulary, p1->definition.cnDefinition, &p1->yearofOrigin);
						fflush(stdin);
					}
					p2->next = NULL;
					return(hp);
                }
				break;
				default:break;
			}
			return 0;
		}

		void dictModelUpdateViewController(DICT_LL *head)
		{
			printf("log: dictModelUpdateViewController\n");
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
			DICT_LL *dictPtr = head;

			int i = 0;
			while (dictPtr != NULL) {
				strcpy(aDictionary[i].vocabulary, dictPtr->vocabulary);
				aDictionary[i].yearofOrigin = dictPtr->yearofOrigin;
				switch (CurrentLanguage) {
					case uyghur:
					case english:
					strcpy(aDictionary[i].definition.latinDefinition, dictPtr->definition.latinDefinition);
					break;
					case chinese:
						strcpy(aDictionary[i].definition.latinDefinition, dictPtr->definition.cnDefinition);
					break;
					default:break;
				}
				dictPtr = dictPtr->next;
				++i;
			}
			selectionSort(aDictionary, N);
			for (int i = 0; i < N; ++i)
			{
				printf("log: dictModelUpdateViewController: fwrite\n");
				if (fwrite(&aDictionary[i], DICT_LEN, 1, dictionaryfp) != 1)
				{
					printf("dictionary write error\n");
				}
				switch (CurrentLanguage) {
					case uyghur:
					case english:
					printf("%d. %s\n\t%s\n\t%d\n", i + 1, aDictionary[i].vocabulary, aDictionary[i].definition.latinDefinition, aDictionary[i].yearofOrigin);
					break;
					case chinese:
					printf("%d. %s\n\t%s\n\t%d\n", i + 1, aDictionary[i].vocabulary, aDictionary[i].definition.cnDefinition, aDictionary[i].yearofOrigin);
					break;
					default:break;
				}
			}
			fclose(dictionaryfp);
		}

		void selectionSort(DICT *aDict, unsigned short n)
		{
			printf("log: selectionSort()\n");
			int min;
			for (int i = 0; i < n - 1; ++i)
			{
				min = i;
				for (int j = i + 1; j < n; ++j)
				{
					if (strcmp(aDict[min].vocabulary, aDict[j].vocabulary) > 0)
					{
						min = j;
					}
				}
				if (min != i)
				{

					DICT temp = aDict[min];
					aDict[min] = aDict[i];
					aDict[i] = temp;
				}
			}
			for (int i = 0; i < n; ++i)
			{
				printf("%d. %s\n\t%s\n\t%d\n", i + 1, aDict[i].vocabulary, aDict[i].definition.cnDefinition, aDict[i].yearofOrigin);
			}

		}


		int main(int argc, char const *argv[])
		{
			homeViewModel();
			loadHomeView();
			return 0;
		}
