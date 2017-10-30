//
//  fwb.c
//  FileWriteBinary
//
//  Created by Mewlan Musajan on 10/29/17.
//  Copyright (c) 2017 Mewlan Musajan. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#define DICT_LEN sizeof(struct Dictionary)
#define COUNT 10

unsigned int L;

struct Dictionary
{
	char vocabulary[20];
	union {
		char latinDefinition[256];
		char cnDefinition[280];
	} definition;
	int yearofOrigin;
} Dict[COUNT];

typedef enum Language
{
	english,
	chinese,
	uyghur
} LANG;

LANG CurrentLanguage = 0;

/******Model******/

void save()
{
	FILE *dictionaryfp;
	if ((dictionaryfp = fopen("iWillookDictionary.dat", "ab")) == NULL)
	{
		printf("can not open file.\n");
		exit(0);
	}
	switch (CurrentLanguage) {
		case english:rewind(dictionaryfp);break;
		case chinese:fseek(dictionaryfp, DICT_LEN, 0);break;
		case uyghur:fseek(dictionaryfp, 2 * DICT_LEN, 0);break;
		default:break;
	}
	for (int i = 0; i < COUNT; ++i)
	{
		if (fwrite(&Dict[i], DICT_LEN, 1, dictionaryfp) != 1)
		{
			printf("dictionary write error\n");
		}
	}
	fclose(dictionaryfp);
}

/******Model******/


int main(int argc, char const *argv[])
{
	for (int i = 0; i < COUNT; ++i)
	{
		fflush(stdin);
		printf("vocabulary:, definition:\n");
		scanf("%[^\n]%*c%[^\n]%*c", Dict[i].vocabulary, Dict[i].definition.latinDefinition);
		fflush(stdin);
		printf("yearofOrigin:\n");
		scanf("%d", &Dict[i].yearofOrigin);
		fflush(stdin);
	}
	save();
	return 0;
}
