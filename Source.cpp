#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "Header.h"
using namespace std;
//memset(&block, '\0', sizeof(block));
int main()
{
	block B;
	bucket buc;
	int pr, pb;
	FILE* DB;
	DB = fopen("Data_Base.bin", "r+");
	findbucket(DB,buc);
	mycout();
	int mcase=1;
	while (mcase != 0) {
		cin >> mcase;
		if (mycase(DB, buc,B,pb,pr,mcase)) return 1;
	}
	fclose(DB);
	return 0;
	//fread(&bucket, sizeof(bucket), 1, DB);
}