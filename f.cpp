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
	memset(&buc, '\0', sizeof(bucket));
	fread(&buc, sizeof(bucket), 1, DB);
	if (buc.mod == 0) { //If bucket not created
		buc.mod = 1;
		for (size_t i = 0; i < 4; i++)
			for (size_t j = 0; j < 2; j++)
				buc.b[i][j] = -1;
		fseek(DB, 0, SEEK_SET);
		fwrite(&buc, sizeof(bucket), 1, DB);
	}
	return 0;
}

int search(FILE* DB, int& pb, int& pr, block& B, bucket& buc, int id, mode m) //0-all ok 1-not ok
{
	switch (m)
	{
		int h;
	case create:
		h = myhash(id);
		if (buc.b[h][0] == -1) { //If first block
			int max=0;
			bool test=false;
			for (int i = 0; i < 4; i++)
			{
				if (buc.b[i][1] == -1) continue;
				if (buc.b[i][1] > max)
					max = buc.b[i][1];
				test = true;
			}
			if (test == true) max++;
			pb = max;
			buc.b[h][0] = max;
			buc.b[h][1] = max;
			pr = 0;
			memset(&B, '\0', sizeof(block));
			B.mod = true;
			B.next = -1;
			fseek(DB, 0, SEEK_SET);
			fwrite(&buc, sizeof(bucket), 1, DB);
			fseek(DB, sizeof(bucket) + sizeof(block) * max, SEEK_SET);
			return 0;
		}
		fseek(DB, sizeof(bucket) + sizeof(block) * buc.b[h][0], SEEK_SET);
		pb = buc.b[h][0];
		fread(&B, sizeof(block), 1, DB);
		while (true) //Search for free space
		{
			for (int i = 0; i < max_block; i++) {
				if (B.s[i].mod==false) {
					pr = i;
					fseek(DB, sizeof(bucket) + sizeof(block) * pb, SEEK_SET);
					return 0;
				}
			}
			if (B.next == -1) { //All current blocks are full
				int max = 0;
				for (int i = 0; i < 4; i++)
				{
					if (buc.b[i][1] > max)
						max = buc.b[i][1];
				}
				max++;	
				B.next = max;
				buc.b[h][1] = max;
				fseek(DB, sizeof(bucket)+ sizeof(block)*pb, SEEK_SET);
				fwrite(&B, sizeof(block), 1, DB);
				pb = max;
				fseek(DB, 0, SEEK_SET);
				fwrite(&buc, sizeof(bucket), 1, DB);
				fseek(DB, sizeof(bucket) + sizeof(block) * max, SEEK_SET);
				pr = 0;
				memset(&B, '\0', sizeof(block));
				B.mod = true;
				B.next = -1;
				return 0;
			} //Here just copy of line 30
			pb = B.next;
			fseek(DB, sizeof(bucket) + sizeof(block) * pb, SEEK_SET);
			fread(&B, sizeof(block), 1, DB);
		}
		break;
	default:
		h = myhash(id);
		if (buc.b[h][0] == -1) return 1;
		fseek(DB, sizeof(bucket) + sizeof(block) * buc.b[h][0], SEEK_SET);
		fread(&B, sizeof(block), 1, DB);
		pb = buc.b[h][0];
		while(true) //Search by ID
		{
			for (int i = 0; i < max_block && B.s[i].mod==true; i++) {
				if (B.s[i].id == id) {
					pr = i;
					switch (m)
					{
					case searc:
						return 0;
						break;
					case change:
						changedata(DB,pb,pr,B,buc);
						break;
					case delet:
						del(DB, pb, pr, B, buc);
						break;
					}
				}
			}
			if (B.next == -1) break;
			pb = B.next;
			fseek(DB, sizeof(bucket) + sizeof(block) * B.next, SEEK_SET);
			fread(&B, sizeof(block), 1, DB);
		}
		break;
	
	}
	return 1;
}

void changedata(FILE* DB, int& pb, int& pr, block& B,bucket &buc)
{
	cout << "Write new data" << endl;
	student t;
	cin >> t.name >> t.sur >> t.pn >> t.id >> t.group;
	if (t.id != B.s[pr].id) {
		if (search(DB, pb, pr, B, buc, t.id, searc) == 0) {
			cout << "Data not unic";
			return;
		}
	}
	t.mod = true;
	B.s[pr] = t;
	fseek(DB, sizeof(bucket) + sizeof(block) * pb, SEEK_SET);
	fwrite(&B, sizeof(block), 1, DB);

}

void del(FILE* DB, int& pb, int& pr, block& B, bucket& buc)
{
	//pr=0
	// pb not max
	//pr+1 mod==false
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

int mycase(FILE* DB, bucket &buc,block &B,int &pb,int &pr, int m)
{
	int d;
	switch (m)
	{
	case 1:
		newrecord(DB, pb, pr, B, buc);
		break;
	case 2:
		cout << "Write ID for search" << endl;
		cin >> d;
		if (search(DB, pb, pr, B, buc, d, searc) == 1)
			cout << "Record not found";
		else printrecord(B.s[pr]);
		break;
	case 3:
		showall(DB, buc);
		break;
	case 4:
		cout << "Write ID for search" << endl;
		cin >> d;
		if (search(DB, pb, pr, B, buc, d, change) == 1)
			cout << "Record not found";
		break;
	case 6:
		if (clearfile(DB))
			return 1;
		findbucket(DB, buc);
		break;
	case 9:
		mycout();
		break;
	case 0:
		return 0;
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
		cout << "Data not unic" << endl;
		return;
	}
	t.mod = true;//if found
	search(DB, pb, pr, B, buc, t.id, create);
	B.s[pr] = t;
	fwrite(&B, sizeof(block), 1, DB);
}
void showall(FILE* DB, bucket buc)
{
	block t;
	for (int i = 0; i < 4; i++)
	{	
		if (buc.b[i][0] == -1) {
			cout << "Bucket " << i << " is empty" << endl;
			continue;
		}
		cout << "Bucket " << i<< ":" << endl;
		fseek(DB, sizeof(bucket)+sizeof(block)*buc.b[i][0], SEEK_SET);
		fread(&t, sizeof(block), 1, DB);
		while(true) {
			cout << "-----------------------------------" << endl;
			for (int i = 0; i < 5 && t.s[i].mod; i++)
			{
				cout << t.s[i].name << " " << t.s[i].sur << " " <<
					t.s[i].pn << " " << t.s[i].id << " " << t.s[i].group << endl;
			}
			if (t.next == -1) {
				cout << "-----------------------------------" << endl;
				break;
			}
			fseek(DB, sizeof(bucket) +sizeof(block)* t.next, SEEK_SET);
			fread(&t, sizeof(block), 1, DB);
		}
	}
}
void printrecord(student s)
{
	cout << s.name << " " << s.sur << " " << s.pn << " " << s.id
		<< " " << s.group;
}
void deletefind(FILE* DB, block& B, bucket& buc, int& pb, int h)
{
	int max = 0;
	int hmax;
	for (int i = 0; i < 4; i++)
	{
		if (buc.b[i][1] > max) {
			max = buc.b[i][1];
			hmax = i;
		}
		if (buc.b[i][1] == pb) { //if need to delete last block
			deletelastblock(DB,B,buc,pb,h);
			return;
		}
	}
	deleteblock(DB,B,buc,pb,h,max,hmax);
}
void deletelastblock(FILE* DB, block& B, bucket& buc, int& pb, int h)
{
	block t;
	int curp;
	fseek(DB, sizeof(bucket) + sizeof(block) * buc.b[h][0], SEEK_SET);
	fread(&t, sizeof(block), 1, DB);
	curp = buc.b[h][0];
	while (t.next != pb) {
		fseek(DB, sizeof(bucket) + sizeof(block) * t.next, SEEK_SET);
		fread(&t, sizeof(block), 1, DB);
		curp = t.next;
	}
	t.next = -1;
	buc.b[h][1] = curp;
	fseek(DB, sizeof(bucket) + sizeof(block) * curp, SEEK_SET);
	fwrite(&t, sizeof(block), 1, DB);
	fseek(DB, 0, SEEK_SET);
	fwrite(&buc, sizeof(bucket), 1, DB);
	memset(&t, '\0', sizeof(block));
	fseek(DB, sizeof(bucket) + sizeof(block) * pb, SEEK_SET);
	fwrite(&t, sizeof(block), 1, DB);
}
void deleteblock(FILE* DB, block& B, bucket& buc, int& pb, int h, int max, int hmax)
{
	block t,del;
	int curp;
	fseek(DB, sizeof(bucket) + sizeof(block) * max, SEEK_SET);
	fread(&B, sizeof(block), 1, DB);
	fseek(DB, sizeof(bucket) + sizeof(block) * buc.b[h][0], SEEK_SET);
	fread(&t, sizeof(block), 1, DB);
	curp = buc.b[h][0];
	while (t.next != pb) {
		fseek(DB, sizeof(bucket) + sizeof(block) * t.next, SEEK_SET);
		fread(&t, sizeof(block), 1, DB);
		curp = t.next;
	}

}
void reccout()
{
	cout << "Write N, S, P, ID, G" << endl;
}