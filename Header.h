constexpr auto max_block = 5;
constexpr auto char_size = 20;
constexpr auto hash_size = 4;
enum mode {create,searc,change,delet};
struct student {
	char name[char_size];
	char sur[char_size];
	char pn[char_size];
	int id;
	int group;
	bool mod;
};
struct block {
	student s[5];
	int next;
	bool mod;
};
struct bucket {
	int b[4][2];
	bool mod;
};
int myhash(int h);
int findbucket(FILE*,bucket &b);
int search(FILE* DB,int& pb, int& pr,block &B,bucket &buc,int id,mode m);
void changedata(FILE* DB, int& pb, int& pr, block& B,bucket &buc);
void del(FILE* DB, int& pb, int& pr, block& B, bucket& buc);
void mycout();
void reccout();
int mycase(FILE*,bucket&,block&,int&,int&,int m);
int clearfile(FILE* DB);
void newrecord(FILE* DB, int& pb, int& pr, block& B, bucket& buc);
void showall(FILE* DB, bucket buc);
void printrecord(student s);
void deleteblockfind(FILE* DB, block& B, bucket& buc,int h);
//void deletelastblock(FILE* DB, block& B, bucket& buc, int& pb, int h);
//void deleteblock(FILE* DB, block& B, bucket& buc, int& pb, int h, int max, int hmax);
void deleterec(FILE* DB, block& B, bucket& buc, int& pb, int& pr,int h);