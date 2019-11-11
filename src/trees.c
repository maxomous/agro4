//trees.c
#include "standardShit.h"
#include "agro4_gui.h"
#include "trees.h"

int gitHubCheck = TRUE;
int noTrees = 0;
#define MAX_TREES 20
tree_t trees[MAX_TREES];

#define MAX_SPECIES 20
species_t species[MAX_SPECIES];


void addTree(tree_t t) {

	if (noTrees >= MAX_TREES) { 
			printf("NOPE! \n too many trees!");
			exit(1);
		}
	//trees = malloc((noTrees + 1)*sizeof (tree_t));
	trees[noTrees] = t;
	noTrees++;
	

}

float getNitrogenMultiplier(int id)
{
	return species[trees[id].speciesID].nitrogenMultiplier;
}
float getNitrogenWidth(int id)
{
	return trees[id].width * getNitrogenMultiplier(id);
}


void updateTrees(int treeID1, int treeID2) {
	
	if (treeID1 == treeID2)
		return;

	//~ float dx = (trees[treeID2].location.x - trees[treeID1].location.x);
	//~ float dy = (trees[treeID2].location.y - trees[treeID1].location.y);
	//~ float length = sqrtf(dx*dx + dy*dy);
	float length = Distance(trees[treeID1].location, trees[treeID2].location);
	
	if (length < (trees[treeID1].width + trees[treeID2].width)/2) {
		trees[treeID1].isTooClose |= TRUE;
		trees[treeID2].isTooClose |= TRUE;
	}

	float nitrogenWidth = getNitrogenWidth(treeID1);
	if( (nitrogenWidth)  &&  (length < (nitrogenWidth + trees[treeID2].width)/2) )
		trees[treeID2].hasNitrogen |= TRUE;
		
		
}	



void updateAllTrees() {
	// clear values
	tree_t* t;
	for (int i = 0; i < noTrees; i++)
	{	
		t = &trees[i];
		t->age = t->baseAge + currentTime;
		
			t->width = t->age * species[t->speciesID].growthRate;
		if (t->width > species[t->speciesID].maxSize)
			t->width = species[t->speciesID].maxSize;
			
		//t->width = t->age * species[t->speciesID].growthRate;
		if(t->width < 0.3)
			t->width = 0.3;
		t->isTooClose = FALSE;
		t->hasNitrogen = FALSE;
	}
		
		
	for (int ID1 = 0; ID1 < noTrees; ID1++)
	{
		for (int ID2 = 0; ID2 < noTrees; ID2++)
		{
			
			//~ printf("%d&%d\n", ID1, ID2);
			updateTrees(ID1, ID2);
			
		}
	}

	printf("There is this much stuff: %g\n", totalMineral());
}

void addSpecies() 
{
	strncpy(species[0].name, "Willow", MAX_STRING);
	species[0].mineralAccum = TRUE;
	species[0].nitrogenMultiplier = 2;
	species[0].growthRate = 1.5;	// metres wide / yr
	species[0].fullSizeAge = 5;
	species[0].maxSize = species[0].growthRate * species[0].fullSizeAge;
	species[0].mineralExtract = 0.4;
	
	strncpy(species[1].name, "Birch", MAX_STRING);
	species[1].mineralAccum = FALSE;
	species[1].nitrogenMultiplier = 0;
	species[1].growthRate = 1.7;
	species[1].fullSizeAge = 10;
	species[1].maxSize = species[1].growthRate * species[1].fullSizeAge;
	species[1].mineralExtract = -0.3;
}

float totalMineral() {
	float mineralAmount = 0;
	for (int i = 0; i < noTrees; i++)
	{
		mineralAmount += (species[trees[i].speciesID].mineralExtract * trees[i].age);
	}
	return mineralAmount;
}

void sowSeeds() 
{
	point2D p = (point2D){.x = 1, .y = 1};
	tree_t t = (tree_t){.location = p, .baseAge = 0, .speciesID = 0};
	addTree(t);
	
	p = (point2D){.x = 5, .y = 5};
	t = (tree_t){.location = p, .baseAge = 0, .speciesID = 1};
	addTree(t);

	p = (point2D){.x = 10, . y = 10};
	t = (tree_t){.location = p, .baseAge = 0, .speciesID = 1};
	addTree(t);
	
	updateAllTrees();
}

void trees_init() 
{
	displayList_Tree();	// create the tree graphics
	addSpecies();
	sowSeeds();
}
		
		

void moveTree(int id, int dx, int dy) {
	trees[id].location.x += dx * getScale();
	trees[id].location.y -= dy * getScale();
}

void moveAndUpdate(int currentID, int dx, int dy) {
	
	moveTree(currentID, dx, dy);
	updateAllTrees();
}
   
   

// This takes in screen position
int checkOverlap(int X,int Y) {
	
	float x = getScale() * (float)X;
	float y = getScale() * (float)Y;
	
	for (int id = 0; id < noTrees; id++)
	{
		float r = trees[id].width/2;
		float x1 = trees[id].location.x;
		float y1 = trees[id].location.y;
		
		if (((x >= x1 - r)  &&  (x <= x1 + r))  &&  ((y >= y1 - r)  &&  (y <= y1 + r)))
		{
			printf("id: %d\n", id);
			return id;
		}
		
	}
	return -1;	
}

#define MAX_TREE_TYPES 20
GLint index_Tree[MAX_TREE_TYPES];

void displayList_Tree() 
{	
	for (int i = 0; i < MAX_TREE_TYPES; i++)
	{
		
		int n = i+1;	// no of leaves
		float r1 = 1;	// now controlled by glScale
		float r2 = r1/1.7;
		
		double anglePerArc = (2*PI / n);
		double anglePerHalfArc = anglePerArc / 2;
		double startAngle = anglePerHalfArc;
		
		
		index_Tree[i] = glGenLists(1);
		glNewList(index_Tree[i], GL_COMPILE);		//GL_COMPILE or GL_COMPILE_AND_EXECUTE
			//~ glColor3f(0.75f, 0.75f, 0.75f);
			glLineWidth(1.0);


			glBegin(GL_LINE_LOOP);
				
				for (int i = 0; i < n; i++)
				{
					double startAngleNeg = CleanUpAngle(startAngle - anglePerHalfArc);
					double startAnglePos = CleanUpAngle(startAngle + anglePerHalfArc);
				
					point2D c = PointFromPolar((point2D){.x = 0, .y = 0}, r2, startAngle);
					point2D start = PointFromPolar((point2D){.x = 0, .y = 0}, r1, startAngleNeg);
					point2D end = PointFromPolar((point2D){.x = 0, .y = 0}, r1, startAnglePos);
					
					double ang1 = AngleFrom2Points(c, start);
					double ang2 = AngleFrom2Points(c, end);
					
					CleanUpAngles(&ang1, &ang2, 1);
					
					if (ang1 > ang2)
					{
						printf("DAFUQ you doing\n");
						exit(1);
					}
					for (double th = ang1; th < ang2; th+= Deg2Rad(1))
					{
						point2D out = PointFromPolar(c, Distance(c, start), th);
						glVertex2f(out.x, out.y);
					}
					startAngle = CleanUpAngle(startAngle + anglePerArc);
					
				}
				
			glEnd();
		glEndList();
	}
}

void treeDrawing(point2D p, float r, uint noLeaves) 
{
	if(noLeaves == 0) {
		printf("Leaves can't be 0\n");
		return;
	}
	size_t n = (noLeaves > MAX_TREE_TYPES) ? 19 : noLeaves-1;
	if(n<2)
		n = 2;
	glPushMatrix();
		glTranslatef(p.x,p.y,0);
		glScalef(r, r, r);
		//~ glRotatef((GLfloat)Rad2Deg(20), 0,0,1);	// rotate about z
		glCallList(index_Tree[n]);
	glPopMatrix();
    
}


void drawTree(int id) {
	
	tree_t t = trees[id];	// use pointer for any modifications
	if (t.isTooClose)
		glColor3f(1.0f, 0.0f, 0.0f);
	else
		glColor3f(0.0f, 0.8f, 0.0f);
	
	float numLeaves = t.age;
	if (numLeaves > species[t.speciesID].fullSizeAge)
	{
		numLeaves = species[t.speciesID].fullSizeAge;
	}
	treeDrawing(t.location, t.width/2, numLeaves+2);
	float nitrogenWidth = getNitrogenWidth(id);
	if(nitrogenWidth){
		glColor3f(0.5f, 0.5f, 0.5f);
		circle(t.location, nitrogenWidth/2);
	}
	
	 //~ printf("N: %d\ttooClose: %d\n", t.hasNitrogen, t.isTooClose);
	
	if (t.hasNitrogen == TRUE)
	{
		glColor3f(0.1f, 0.1f, 0.1f);
		glRasterPos2f(t.location.x, t.location.y);
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, 'N');
	}
}

void drawTrees() {
	
	for (int i = 0; i < noTrees; i++)
	{
		drawTree(i);	
	}
	
}

