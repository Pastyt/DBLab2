#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "Header.h"
using namespace std;
//memset(&block, '\0', sizeof(block));
int main()
{
	block B;
	student temp;
	bucket buc;
	FILE* DB;
	DB = fopen("C:\\Games\\Data_Base.bin", "r+");
	cout << "OK" << endl;
	findbucket(DB,buc);
	mycout();
	int mcase=1;
	while (mcase != 0) {
		cin >> mcase;
		if (mycase(DB, buc, mcase)) return 1;
	}
	return 0;
	//fread(&bucket, sizeof(bucket), 1, DB);
}