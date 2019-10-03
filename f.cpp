#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "Header.h"
using namespace std;
int myhash(int h)
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

int search(FILE* DB, int& pb, int& pr, block& B, bucket& buc, int id, mode m) //0-not found
{
	switch (m)
	{
	case create:
		int h = myhash(id);
		if (buc.b[h][0] == NULL) {
			int max=0;
			for (int i = 0; i < 4; i++)
			{
				if (buc.b[i][1] > max)
					max = buc.b[i][1];
			}
			max++;
			fseek(DB, sizeof(bucket) + sizeof(block) * max, SEEK_SET);
			pb = max;
			buc.b[h][0] = max;
			buc.b[h][1] = max;
			pr = 0;
			return 0;
		}
		fseek(DB, sizeof(bucket) + sizeof(block) * buc.b[h][0], SEEK_SET);
		pb = buc.b[h][0];
		fread(&B, sizeof(block), 1, DB);
		while (true)
		{
			for (int i = 0; i < max_block; i++) {
				if (B.s[i].mod==0) {
					pr = i;
					return 0;
				}
			}
			if (B.next == NULL) {
				int max = 0;
				for (int i = 0; i < 4; i++)
				{
					if (buc.b[i][1] > max)
						max = buc.b[i][1];
				}
				max++;			
				pb = max;
				B.next = pb;
				buc.b[h][1] = max;
				fseek(DB, -sizeof(block), SEEK_CUR);
				fread(&B, sizeof(block), 1, DB);
				fseek(DB, sizeof(bucket) + sizeof(block) * max, SEEK_SET);
				pr = 0;
				return 0;
			} //Here just copy of line 30
			pb = B.next;
			fseek(DB, sizeof(bucket) + sizeof(block) * B.next, SEEK_SET);
			fread(&B, sizeof(block), 1, DB);
		}
		break;
	default:
		int h = myhash(id);
		if (buc.b[h][0] == NULL) return 1;
		fseek(DB, sizeof(bucket) + sizeof(block) * buc.b[h][0], SEEK_SET);
		fread(&B, sizeof(block), 1, DB);
		pb = buc.b[h][0];
		while(true)
		{
			for (int i = 0; i < max_block; i++) {
				if (B.s[i].id == id) {
					pr = i;
					switch (m)
					{
					case searc:
						return 0;
						break;
					case change:
						changedata(DB,pb,pr,B);
						break;
					case delet:
						del(DB, pb, pr, B, buc);
						break;
					}
				}
			}
			if (B.next == NULL) break;
			pb = B.next;
			fseek(DB, sizeof(bucket) + sizeof(block) * B.next, SEEK_SET);
			fread(&B, sizeof(block), 1, DB);
		}
		break;
	
	}
	return 1;
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
	case 1:
		newrecord(DB, pb, pr, B, buc);
		break;
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

void newrecord(FILE* DB, int& pb, int& pr, block& B, bucket& buc)
{
	reccout();
	student t;
	cin >> t.name >> t.sur >> t.pn >> t.id >> t.group;
	if (search(DB, pb, pr, B, buc, t.id, searc) == 0) {
		cout << "Data not unic";
		return;
	}//if found
	search(DB, pb, pr, B, buc, t.id, create);
	B.s[pr] = t;
	fread(&B, sizeof(block), 1, DB);
}
void showall(FILE* DB, bucket buc)
{
	for (int i = 0; i < 4; i++)
	{
		cout << "-----------------------------------" << endl;
		cout << "Block with hash " << i<< ":" << endl;
		for (int i = 0; i < length; i++)
		{

		}
	}
}
void reccout()
{
	cout << "Write N, S, P, ID, G" << endl;
}