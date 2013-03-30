#define NB_CASES 30
#define BUFSZ 1024

struct request {
	long type;
	char content[1024];
};
