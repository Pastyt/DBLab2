#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "Header.h"
using namespace std;
int hash(int h)
{
	return h % 4;
}

int findbucket(FILE* DB, bucket &buc)
{
	fread(&buc, sizeof(bucket), 1, DB);
	if (buc.mod == 0) { //If bucket not created
		buc.mod = 1;
		for (size_t i = 0; i < 4; i++)
			for (size_t j = 0; j < 2; j++)
				buc.b[i][j] = NULL;
		fseek(DB, 0, SEEK_SET);
		fwrite(&buc, sizeof(bucket), 1, DB);
	}
	return 0;
}

int search(FILE* DB, int& pb, int& pr, block& B)
{
	fseek(DB, 0, SEEK_SET);
	return 0;
}

void mycout()
{
	cout << "1. New data" << endl;
	cout << "2. Search data" << endl;
	cout << "3. Show all" << endl;
	cout << "4. Change data" << endl;
	cout << "5. Delete data" << endl;
	cout << "6. Delete database" << endl;
	cout << "0. Exit" << endl;
}

int mycase(FILE* DB, bucket &buc, int m)
{
	switch (m)
	{
	case 6:
		if (clearfile(DB))
			return 1;
		findbucket(DB, buc);
		break;
	case 9:
		mycout();
		break;
	}
	return 0;
}

int clearfile(FILE* DB)
{
	if (fclose(DB)) 
		return 1;
	DB = fopen("Data_Base.bin", "w+");
	return 0;
}