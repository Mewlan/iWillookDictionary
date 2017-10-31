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
			backView();
		}

		void catalogView()
		{
			printf("log: catalogView\n");
			printf("%s\n", HomeViewPtr->catalogViewString);
			dictionary = (DICT*) calloc(VOC, DICT_LEN);
			dictionaryModel();
			catalogViewController();
			backView();
			free(dictionary);
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
				case english:{
					DICT_LL *hp;
                    DICT_LL *p1, *p2;
					N = 0;
					p1 = p2 = (DICT_LL*) malloc(sizeof(DICT_LL*));
					scanf("%[^\n]%*c%[^\n]%*c%d", p1->vocabulary, p1->definition.latinDefinition, &p1->yearofOrigin);
					fflush(stdin);
					hp = NULL;
					while (p1->yearofOrigin != 2017) {
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
					// fflush(stdin);
					hp = NULL;
					while (p1->yearofOrigin != 2017) {
						fflush(stdin);
						++N;
						if (N == 1)
						{
							printf("log: N == 1\n");
							hp = p1;
							printf("log: head = 0x%x \n", hp);
						} else {
							printf("log: N != 1\n");
							p2->next = p1;
							printf("log: ptr2->next = 0x%x, ptr2 =  0x%x\n", p2->next, p2);
						}
						if (N == 1)
						{
							printf("log: N == 1, after initialized.\n");
						}
						p2 = p1;
						printf("log: ptr2 = 0x%x \n", p2);
						p1 = (DICT_LL*) malloc(DICT_LL_LEN);
						printf("log: ptr1 = 0x%x \n", p1);
						scanf("%[^\n]%*c%[^\n]%*c%d", p1->vocabulary, p1->definition.cnDefinition, &p1->yearofOrigin);
					}
					p2->next = NULL;
					printf("log: ptr2 = 0x%x, ptr2->next = 0x%x\n", p2, p2->next);
					int i = 0;
					printf("log: hp = 0x%x, hp->next = 0x%x\n", hp, hp->next);
					while (hp != NULL) {
						++i;
						if (i == 1)
						{
							printf("log: head = head.\n");
						}
						printf("%s\n%s\n%d\n", hp->vocabulary, hp->definition.cnDefinition, hp->yearofOrigin);
						hp = hp->next;
					} exit(0);
					return(hp);
                }
				break;
				default:break;
			}
			printf("log: return 0.\n");
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
			while (dictPtr->next != NULL) {
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
				printf("llog: %s\nllog: %s\nllog: %d\n", dictPtr->vocabulary, dictPtr->definition.cnDefinition, dictPtr->yearofOrigin);
				dictPtr = dictPtr->next;
				++i;
			}
			for (int i = 0; i < N; ++i)
			{
				printf("log: dictModelUpdateViewController: fwrite\n");
				if (fwrite(&aDictionary[i], DICT_LEN, 1, dictionaryfp) != 1)
				{
					printf("dictionary write error\n");
				}
				printf("%d. ", i + 1);
				switch (CurrentLanguage) {
					case uyghur:
					case english:
					printf("%s\n%s\n%d\n", aDictionary[i].vocabulary, aDictionary[i].definition.latinDefinition, aDictionary[i].yearofOrigin);
					break;
					case chinese:
					printf("%s\n%s\n%d\n", aDictionary[i].vocabulary, aDictionary[i].definition.cnDefinition, aDictionary[i].yearofOrigin);
					break;
					default:break;
				}
			}
			free(dictionary);
			fclose(dictionaryfp);
		}


		int main(int argc, char const *argv[])
		{
			homeViewModel();
			loadHomeView();
			return 0;
		}
