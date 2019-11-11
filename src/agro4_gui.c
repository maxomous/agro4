#include "standardShit.h"
#include "trees.h"

int currentID = -1;
int SCREEN_WIDTH = 500;
float ASPECTRATIO = 1;
int SCREEN_HEIGHT = 500;
float SCALE = 1;

float getScale() {
	return SCALE;
}

#define SCREEN_POSX 50
#define SCREEN_POSY 50



int DIM = 50;



void printv(va_list args, const char* format)
{
    char buf[8192];
    char* ch = buf;
    vsnprintf(buf,8192,format,args);
    while(*ch)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *ch++);
}

void printAt(float x, float y, const char* format, ...)
{
    va_list args;
    glRasterPos2f(x, y);
    va_start(args,format);
    printv(args,format);
    va_end(args);
}

void circle(point2D p, float r) 
{	
	glBegin(GL_LINE_LOOP);
		for (float th = 0; th < 2*PI; th = th + Deg2Rad(5)) {	
			float x1 = p.x + r*cos(th); 
			float y1 = p.y + r*-sin(th);
			glVertex2f(x1,y1);
		}
	glEnd();
}

void drawAxis() {
		
	glColor3f(1.0f, 0.1f, 0.1f);
	int i;
	
	for (i = 0; i <= DIM; i+=10)
		printAt(0, i, "%d", i);	
	
	for (i = 0; i <= DIM*ASPECTRATIO; i+=10)
		printAt(i, 0, "%d", i);
}

float ACCUMULATOR_POS_X = 10;
float ACCUMULATOR_POS_Y = 30;

void drawMineralAccum() {
	float w1 = 1;
	float w2 = 0.8;
	float h = 3;
	
	glTranslatef(ACCUMULATOR_POS_X, ACCUMULATOR_POS_Y, 0);
	
	glBegin(GL_QUADS);
	glColor3f(0.0f, 0.2f, 0.8f);
		glVertex2f( - w1/2,  - h/2);
		glVertex2f( - w1/2,  + h/2);
		glVertex2f( + w1/2,  + h/2);
		glVertex2f( + w1/2,  - h/2);
	glColor3f(1.0f, 0.0f, 0.0f);
		glVertex2f( - w2/2,  0);
		glVertex2f( - w2/2,  totalMineral());
		glVertex2f( + w2/2,  totalMineral());
		glVertex2f( + w2/2,  0);

	glEnd();
}

void drawAccumulators() {
	drawMineralAccum();
	}


void draw2D()
{
	drawAxis();
	drawTrees();
	drawAccumulators();
}



void display()
{
    // Clear the background
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    // Draw the 2D overlay
    draw2D();
    //Bring the back buffer to the front and vice-versa.
    glutSwapBuffers();
}

void onReshape(int w, int h)
{
	ASPECTRATIO = (h>0) ? (double)w/h : 1;
	SCREEN_WIDTH = w;
	SCREEN_HEIGHT = h;
    printf("w: %d\th: %d\n",w,h);
	SCALE = (float)DIM / (float)SCREEN_HEIGHT;
	 
	// set the drawable region of the window to the maximum
	// available window size
	glViewport(0,0,w,h);
	// set up the projection matrix
	glMatrixMode(GL_PROJECTION);
	// clear any previous transform and set to the identity matrix
	glLoadIdentity();
	// just use an orthographic projection
	glOrtho(0, DIM * ASPECTRATIO, 0, DIM, -1, 1); // Height is proportional to DIM
	//~ glOrtho(0, +DIM * ASPECTRATIO, 0, +DIM, DIM/4, DIM*4);// TODO NOTE: 100 - 50*DIM, DIM / 3); flipped over x & y axis!!! //   /*DIM*DISTANCE_MULTIPLIER2,*/ DIM/DISTANCE_MULTIPLIER);	// TODO swap -DIM, +DIM); with below
// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glutSwapBuffers();
}

typedef struct Mouse 
{
	int x;		/*	the x coordinate of the mouse cursor	*/
	int y;		/*	the y coordinate of the mouse cursor	*/
	int lmb;	/*	is the left button pressed?		*/
	int mmb;	/*	is the middle button pressed?	*/
	int rmb;	/*	is the right button pressed?	*/
	int xpress; /*	stores the x-coord of when the mousebutton was first pressed	*/
	int ypress; /*	stores the y-coord of when the mousebutton was first pressed	*/
	int doubleclick;
} Mouse;
// global mouse structure to hold the mouse information.
Mouse TheMouse = {0,0,0,0,0,0,0,0};
int keyModifiers = 0;
double lasttime = 0;	// for double clicks 

void gui_MouseButton(int button,int state,int x, int y)
{
    keyModifiers = glutGetModifiers(); // shift / ctrl
    // update the mouse position
    TheMouse.x = x;
    TheMouse.y = y;
  
    
    
    // Has the button been pressed or released?
    if (state == GLUT_DOWN) 
    {
	
		// This holds the location of the first mouse click
		if ( !(TheMouse.lmb || TheMouse.mmb || TheMouse.rmb) ) {
			TheMouse.xpress = x;
			TheMouse.ypress = y;
		}
				
	
				

		// Which button was pressed?
		switch(button) 
		{
			case GLUT_LEFT_BUTTON:
				//double click
				if (GetTimeMono() < lasttime)
				TheMouse.doubleclick = 1;
				lasttime = GetTimeMono() + 0.25;	 // + 250ms timer check for double click 
				
				TheMouse.lmb = 1;
				//~ printf("%d\n", checkOverlap(x,SCREEN_HEIGHT - y));
				
				currentID = checkOverlap(x, SCREEN_HEIGHT - y);
				
				
				break;
			case GLUT_MIDDLE_BUTTON:
				TheMouse.mmb = 1;
				break;
			case GLUT_RIGHT_BUTTON:
				TheMouse.rmb = 1;
				break;
			case 3:	// scroll up
				currentTime += 0.5;
				printf("Time: %g\n",currentTime);
				updateAllTrees();
				//~ gui_zoom(0);	// TODO only if x&y are on display??
				break;
			case 4: // scroll down
				currentTime -= 0.5;
				if(currentTime < 0)
					currentTime = 0;
				printf("Time: %g\n",currentTime);
				updateAllTrees();
				//~ gui_zoom(1);
				break;
		}
    }
    else if (state == GLUT_UP) 
    {	// Which button was released?
	
		switch(button) 
		{
			case GLUT_LEFT_BUTTON:
				TheMouse.doubleclick = 0;
				TheMouse.lmb = 0;
				currentID = -1;
				break;
			case GLUT_MIDDLE_BUTTON:
				TheMouse.mmb = 0;
				break;
			case GLUT_RIGHT_BUTTON:
				TheMouse.rmb = 0;
				break;
		}
		//~ glutSetCursor(GLUT_CURSOR_INHERIT);
    }

    glutPostRedisplay();
}

/*-----------------------------------------------------------------------------------------
 *	This function is called whenever the mouse cursor is moved AND A BUTTON IS HELD.
 ------------------------------------------------------------------------------------------*/
void gui_MouseMotion(int x, int y)
{
    // Calculate mouse movement when mouse button is held
    int dx = x - TheMouse.x;
    int dy = y - TheMouse.y;

    // Update the mouse position
    TheMouse.x = x;
    TheMouse.y = y;
	
	// if there is a tree selected
	if (currentID != -1)
	{
		
		moveAndUpdate(currentID, dx, dy);
		
		glutPostRedisplay();
	}
    // Update display
	//~ printf("%d %d\n", x, SCREEN_HEIGHT - y);
	//~ printf("%g %g\n", (float)x * SCALE, (float)(SCREEN_HEIGHT - y) * SCALE);
	//~ printf("SCALE = %g\n", SCALE);
}

/*---------------------------------------------------------------------------------------------
 *	This function is called whenever the mouse cursor is moved AND NO BUTTONS ARE HELD.
 --------------------------------------------------------------------------------------------*/
void gui_MousePassiveMotion(int x, int y)
{
    // Update the mouse position
    TheMouse.x = x;
    TheMouse.y = y;
   }
   

void gui_init(int argc, char **argv)
{
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA);// | GLUT_BORDERLESS|GLUT_CAPTIONLESS); // |GLUT_ALPHA
		glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
		glutInitWindowPosition(SCREEN_POSX, SCREEN_POSY);
		glutCreateWindow("Agroforestry");
		
		glutDisplayFunc(display);
		glutReshapeFunc(onReshape);
		//~ glutKeyboardFunc(gui_keyboard);
		//~ glutSpecialFunc(gui_keyboardSpecialDown);
		//~ glutSpecialUpFunc(gui_keyboardSpecialUp);
		glutMouseFunc(gui_MouseButton);
		glutMotionFunc(gui_MouseMotion);
		glutPassiveMotionFunc(gui_MousePassiveMotion);
		 
		// Set Background colour
		glClearColor(0.4f, 0.4f, 0.4f, 0.0); // background colour finder (see keyboard)
		
		trees_init();
		
		glutMainLoop();
}
