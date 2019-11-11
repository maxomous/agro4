//trees.h


#ifndef 	__TREES_H__
#define 	__TREES_H__

typedef struct{
	
	point2D location;
	int speciesID;
	float age;
	float baseAge;
	float width;		// growthRate * age;
	int isTooClose;		// if(L < r1+r2)
	int hasNitrogen;	// if(
	int hasMineral;
	
}tree_t;

typedef	struct {
	
	char name[MAX_STRING];
	float growthRate;
	// vertical growth rate
	float nitrogenMultiplier;
	int mineralAccum;
	float maxSize;
	float fullSizeAge;
	float mineralExtract;
	
}species_t;


extern void drawTrees();
extern void addTree(tree_t t);
extern int checkOverlap(int X,int Y);
extern void moveTree(int id, int dx, int dy);
extern void moveAndUpdate(int currentID, int dx, int dy);
extern void updateAllTrees();
extern void trees_init();
extern void displayList_Tree();
extern float totalMineral();
#endif
