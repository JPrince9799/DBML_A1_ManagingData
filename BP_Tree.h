typedef struct Node bpTree;

bpTree bpCreate(void);
 
void bpDestroy();

int bpSearch(bpTree b, int key);

void bpInsert(bpTree b, int key);

void bpPrintKeys(bpTree t);