constexpr auto max_block = 5;
constexpr auto char_size = 20;
constexpr auto hash_size = 4;
struct student {
	char name[char_size];
	char sur[char_size];
	char twoname[char_size];
	int id;
	int group;
	bool mod;
};
struct block {
	student s[5];
	bool mod;
};
struct bucket {
	int b[4][2];
	bool mod;
};
int hash(int h);
int findbucket(FILE*,bucket &b);
int search(FILE* DB,int& pb, int& pr,block &B);
void mycout();
int mycase(FILE*,bucket&,int m);
int clearfile(FILE* DB);