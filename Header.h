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
void changedata(FILE* DB, int& pb, int& pr, block& B); //Not realized
void del(FILE* DB, int& pb, int& pr, block& B, bucket& buc); //Not realized
void mycout();
void reccout();
int mycase(FILE*,bucket&,int m);
int clearfile(FILE* DB);
void newrecord(FILE* DB, int& pb, int& pr, block& B, bucket& buc); //Not realized
void showall(FILE* DB, bucket buc);