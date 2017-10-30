//
//  frb.c
//  FileReadBinary
//
//  Created by Mewlan Musajan on 10/29/17.
//  Copyright (c) 2017 Mewlan Musajan. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#define DICT_LEN sizeof(struct Dictionary)
#define VOC 10

unsigned int L;

struct Dictionary
{
	char vocabulary[20];
	union {
		char latinDefinition[256];
		char cnDefinition[280];
	} definition;
	unsigned int yearofOrigin;
} Dict[VOC];

typedef enum Language
{
	english,
	chinese,
	uyghur
} LANG;

LANG CurrentLanguage;

/******Model******/

void read()
{
	FILE *fp;
	if ((fp = fopen("iWillookDictionary.dat", "rb")) == NULL)
	{
		printf("can not open file.\n");
		exit(0);
	}
	switch (CurrentLanguage) {
		case english:rewind(fp);break;
		case chinese:fseek(fp, VOC * DICT_LEN, 0);break;
		case uyghur:fseek(fp, 2 * VOC * DICT_LEN, 0);break;
		default:break;
	}
	for (int i = 0; i < VOC; ++i)
	{
		if (fread(&Dict[i], DICT_LEN, 1, fp) != 1)
		{
			printf("struct write error\n");
		}
		printf("%s\n", Dict[i].vocabulary);
		printf("%s\n", Dict[i].definition.latinDefinition);
		printf("%u\n", Dict[i].yearofOrigin);
	}
	fclose(fp);
}

/******Model******/


int main(int argc, char const *argv[])
{
	CurrentLanguage = english;
	read();
	return 0;
}