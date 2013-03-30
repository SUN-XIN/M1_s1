#define SVID_SOURCE 1 

void* Producteur(void* arg);
void* Consommmateur(void* arg);

void Push(int c,int num);
int Pop(int num);
