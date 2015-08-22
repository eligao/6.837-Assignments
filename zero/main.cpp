#include "GL/freeglut.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>
#include "vecmath.h"
#include "util.h"
using namespace std;

// Macros
#define MAX_BUFFER_SIZE 256

// Globals

// This is the list of points (3D vectors)
vector<Vector3f> vecv;

// This is the list of normals (also 3D vectors)
vector<Vector3f> vecn;

// This is the list of faces (indices into vecv and vecn)
vector<vector<unsigned> > vecf;


// You will need more global variables to implement color and position changes

// index of color
int iColor = 0;

// the gllookat matrix
GLdouble eyex = 0.0, eyey = 0.0, eyez = 5.0,
centerx = 0.0, centery = 0.0, centerz = 0.0,
upx = 0.0, upy = 1.0, upz = 0.0;
//the light0 coord 
GLfloat lightx=1.0,lighty=1.0,lightz=5.0;


// These are convenience functions which allow us to call OpenGL 
// methods on Vec3d objects
inline void glVertex(const Vector3f &a) 
{ glVertex3fv(a); }

inline void glNormal(const Vector3f &a) 
{ glNormal3fv(a); }


// This function is called whenever a "Normal" key press is received.
void keyboardFunc( unsigned char key, int x, int y )
{
    switch ( key )
    {
    case 27: // Escape key
        exit(0);
        break;
    case 'c':
        // add code to change color here
		iColor = (iColor + 1) % 4;
		cout << "Key C pressed, color changed to index " << iColor << "." << endl; 
        break;
    default:
        cout << "Unhandled key press " << key << "." << endl;        
    }

	// this will refresh the screen so that the user sees the color change
    glutPostRedisplay();
}

// This function is called whenever a "Special" key press is received.
// Right now, it's handling the arrow keys.
void specialFunc( int key, int x, int y )
{
    switch ( key )
    {
    case GLUT_KEY_UP:
        // add code to change light position
		lighty += 0.5f;
		cout << "Key press: up arrow. Light.Y changed to"<< lighty << endl;
		break;
    case GLUT_KEY_DOWN:
        // add code to change light position
		lighty -= 0.5f;
		cout << "Key press: down arrow. Light.Y changed to" << lighty << endl;
		break;
    case GLUT_KEY_LEFT:
        // add code to change light position
		lightx -= 0.5f;
		cout << "Key press: left arrow. Light.X changed to" << lightx << endl;
		break;
    case GLUT_KEY_RIGHT:
        // add code to change light position
		lightx += 0.5f;
		cout << "Key press: right arrow. Light.X changed to" << lightx << endl;
		break;
    }

	// this will refresh the screen so that the user sees the light position
    glutPostRedisplay();
}

// This function is responsible for displaying the object.
void drawScene(void)
{

    // Clear the rendering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Rotate the image
    glMatrixMode( GL_MODELVIEW );  // Current matrix affects objects positions
    glLoadIdentity();              // Initialize to the identity

    // Position the camera at [0,0,5], looking at [0,0,0],
    // with [0,1,0] as the up direction.
	gluLookAt(
		eyex, eyey, eyez,
		centerx, centery, centerz,
		upx, upy, upz);

    // Set material properties of object

	// Here are some colors you might use - feel free to add more
    GLfloat diffColors[4][4] = { {0.5, 0.5, 0.9, 1.0},
                                 {0.9, 0.5, 0.5, 1.0},
                                 {0.5, 0.9, 0.3, 1.0},
                                 {0.3, 0.8, 0.9, 1.0} };
    
	// Here we use the first color entry as the diffuse color
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, diffColors[iColor]);

	// Define specular color and shininess
    GLfloat specColor[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat shininess[] = {100.0};

	// Note that the specular color and shininess can stay constant
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
  
    // Set light properties

    // Light color (RGBA)
    GLfloat Lt0diff[] = {1.0,1.0,1.0,1.0};
    // Light position
	GLfloat Lt0pos[] = {lightx, lighty, lightz, 1.0f};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
    glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);

	// This GLUT method draws a teapot.  You should replace
	// it with code which draws the object you loaded.
// 	glutSolidTeapot(1.0);
	for (vector<unsigned> face : vecf)
	{
		const unsigned
			&a = face[0], &b = face[1], &c = face[2],
			&d = face[3], &e = face[4], &f = face[5],
			&g = face[6], &h = face[7], &i = face[8];
		glBegin(GL_TRIANGLES);
		glNormal3d(vecn[c - 1][0], vecn[c - 1][1], vecn[c - 1][2]);
		glVertex3d(vecv[a - 1][0], vecv[a - 1][1], vecv[a - 1][2]);
		glNormal3d(vecn[f - 1][0], vecn[f - 1][1], vecn[f - 1][2]);
		glVertex3d(vecv[d - 1][0], vecv[d - 1][1], vecv[d - 1][2]);
		glNormal3d(vecn[i - 1][0], vecn[i - 1][1], vecn[i - 1][2]);
		glVertex3d(vecv[g - 1][0], vecv[g - 1][1], vecv[g - 1][2]);
		glEnd();
	}

    
    // Dump the image to the screen.
    glutSwapBuffers();


}

// Initialize OpenGL's rendering modes
void initRendering()
{
    glEnable(GL_DEPTH_TEST);   // Depth testing must be turned on
    glEnable(GL_LIGHTING);     // Enable lighting calculations
    glEnable(GL_LIGHT0);       // Turn on light #0.
}

// Called when the window is resized
// w, h - width and height of the window in pixels.
void reshapeFunc(int w, int h)
{
    // Always use the largest square viewport possible
    if (w > h) {
        glViewport((w - h) / 2, 0, h, h);
    } else {
        glViewport(0, (h - w) / 2, w, w);
    }

    // Set up a perspective view, with square aspect ratio
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // 50 degree fov, uniform aspect ratio, near = 1, far = 100
    gluPerspective(50.0, 1.0, 1.0, 100.0);
}

void loadInput()
{
	// load the OBJ file here
// 	vecf;
// 	vecn;
// 	vecv;
	char buffer[MAX_BUFFER_SIZE];
	while (cin.getline(buffer, MAX_BUFFER_SIZE))
	{
		if (buffer[0]=='#')//skip commentary lines.
		{
			continue;
		}
		stringstream ss(buffer);
		string entry_type;
		ss >> entry_type;

		if (entry_type=="v")//vector
		{
			//v -0.523721 0.453991 -0.720840
			Vector3f v3;
			ss >> v3.x() >> v3.y() >> v3.z();
			vecv.push_back(v3);
		}
		else if (entry_type=="vn")//vector normal
		{
			//vn 0.309017 0.000000 -0.951057
			Vector3f v3;
			ss >> v3.x() >> v3.y() >> v3.z();
			vecn.push_back(v3);
		}
		else if (entry_type=="f")//face
		{
			//f 191/200/191 190/199/190 171/179/171
			vector<unsigned> face_index;
			string slashed_data;
			while (ss>>slashed_data)//in forms of "191/200/191"
			{
				vector<string> parts;
				split(slashed_data, parts, "/");//split into 191 200 191
				int i=0;
				for (string part :parts)//load parts into vector
				{
					face_index.push_back(atoi((part.c_str())));
					i++;
				}
				while (i<2)//fill in the trailing data in case of omit
				{
					face_index.push_back(0);
					i++;
				}
			}
			vecf.push_back(face_index);
		}
		else//omit other types
		{
			continue;
		}
	}
}

// Main routine.
// Set up OpenGL, define the callbacks and start the main loop
int main( int argc, char** argv )
{
    loadInput();

    glutInit(&argc,argv);

    // We're going to animate it, so double buffer 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

    // Initial parameters for window position and size
    glutInitWindowPosition( 60, 60 );
    glutInitWindowSize( 360, 360 );
    glutCreateWindow("Assignment 0");

    // Initialize OpenGL parameters.
    initRendering();

    // Set up callback functions for key presses
    glutKeyboardFunc(keyboardFunc); // Handles "normal" ascii symbols
    glutSpecialFunc(specialFunc);   // Handles "special" keyboard keys

     // Set up the callback function for resizing windows
    glutReshapeFunc( reshapeFunc );

    // Call this whenever window needs redrawing
    glutDisplayFunc( drawScene );

    // Start the main loop.  glutMainLoop never returns.
    glutMainLoop( );

    return 0;	// This line is never reached.
}
