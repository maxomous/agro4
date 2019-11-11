//trees.h


#ifndef 	__TREES_H__
#define 	__TREES_H__

typedef struct{
	
	point2D location;
	int speciesID;
	float age;
	float baseAge;
	float width;		// growthRate * age;
	float width_trunk;	// trunkgrowthRate * age;
	int isTooClose;		// if(L < r1+r2)
	int hasNitrogen;	// if(
	int hasMineral;
	
}tree_t;

// succession layers:
#define LAYER_HIGH_TREES	1
#define LAYER_SMALL_TREES	2
#define LAYER_SHRUBS		3
#define LAYER_PERENIALS		4
#define LAYER_GROUND_COVER	5
#define LAYER_ROOT_CROPS	6
#define LAYER_CLIMIBERS		7

typedef	struct {
	char name[MAX_STRING];
	float growthRate;
	float growthRate_trunk;
	// vertical growth rate
	float nitrogenMultiplier;
	int mineralAccum;
	float maxSize;
	float maxSize_trunk;
	float fullSizeAge;
	float mineralExtract;
	int successionLayer;
	
}species_t;


extern void drawTrees();
extern void addTree(tree_t t);
extern int checkOverlap(int X,int Y);
extern void moveTree(int id, int dx, int dy);
extern void moveAndUpdate(int currentID, int dx, int dy);
extern void updateAllTrees();
extern void trees_init();
void displayList_Tree();
void displayList_Trunk();
extern float totalMineral();
#endif
