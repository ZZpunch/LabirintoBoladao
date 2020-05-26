#include <GL/gl.h>       //biblioteca gl.h
#include <GL/glut.h>     //biblioteca glut - ferramentas adicionais
#include <math.h>
#include <stdio.h>

#define TAM_BLOCO 100
#define PASSO	10
#define NORTE  0
#define LESTE  1
#define SUL    2
#define OESTE  3

GLfloat corParede[] = { 0.2, 0.1, 0.2, 1.0 };
GLfloat corChao[] = { 1.0,1.0,1.0,1.0 };
GLfloat light_position[] = { 0.0, 500.0, 0.0, 1.0 };
GLfloat luz_branca[] = { 1.0,1.0,1.0,1.0 };
GLfloat lmodel_ambient[] = { 0.6,0.6,0.6,1.0 };
GLfloat mapaChegada[] = { 0.7, 0.1, 0.1, 1.0 };

GLfloat jog_x = TAM_BLOCO, jog_z = TAM_BLOCO;
GLfloat jog_y = 0;
GLfloat mov_x = 10, mov_y = 0, mov_z = 0;
GLint angle = 0;
GLint wire = 0;

/*labirinto */
GLint mapa[15][19] = 
{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
  1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
  1,0,0,0,0,0,1,1,1,1,1,0,1,1,1,1,1,1,1,
  1,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,1,
  1,1,0,1,0,0,0,1,0,1,1,0,1,1,1,1,1,0,1,
  1,0,0,0,0,1,0,1,0,1,1,0,1,0,0,0,0,0,1,
  1,0,1,1,0,1,0,1,0,1,1,0,1,0,1,1,0,1,1,
  1,0,0,0,0,0,0,1,0,1,1,0,1,0,1,1,0,1,1,
  1,0,1,1,0,1,1,1,0,0,0,0,1,0,1,1,0,0,1,
  1,0,1,0,0,1,0,0,0,1,1,1,1,0,1,0,1,0,1,
  1,0,1,0,1,1,0,1,0,0,0,0,0,0,1,0,1,0,1,
  1,0,1,0,0,0,0,1,1,1,1,1,1,0,1,0,0,0,1,
  1,0,1,0,1,1,0,1,0,0,0,0,0,0,1,1,0,1,1,
  1,0,0,0,0,0,0,1,0,0,1,1,1,1,0,0,0,1,1,
  1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1 };


float deltaAngle = 0.0f;
float anglesX = 0;
float anglesY = 0;
float nextMoveX =0;
float nextMoveZ =0;

int pode_mover(float pos_x, float pos_z, float mov_x, float mov_z)
{
	float mundo_x = pos_x + mov_x;
	float mundo_z = pos_z + mov_z;

	int ind_x = (int)((mundo_x + TAM_BLOCO / 2) / TAM_BLOCO) ;
	int ind_z = (int)((mundo_z + TAM_BLOCO / 2) / TAM_BLOCO) ;
	
	if (mapa[ind_x][ind_z]) return 0;
	else return 1;
}

void drawSnowMan() {
	glColor3f(1.0f, 1.0f, 1.0f);
// Draw Body
	glTranslatef(0.0f ,0.75f, 0.0f);
	glutSolidSphere(0.75f,20,20);


// Draw Head
	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.25f,20,20);

// Draw Eyes
	glPushMatrix();
	glColor3f(0.0f,0.0f,0.0f);
	glTranslatef(0.05f, 0.10f, 0.18f);
	glutSolidSphere(0.05f,10,10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.05f,10,10);
	glPopMatrix();

// Draw Nose
	glColor3f(1.0f, 0.5f , 0.5f);
	glutSolidCone(0.08f,0.5f,10,2);
}

void display(void)
{
	int x, y;
	int x_mun, y_mun;

	//limpa todos os pixels
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();


	gluLookAt(jog_x + mov_x, jog_y + mov_y, jog_z + mov_z, jog_x, jog_y , jog_z, 0, 1, 0);
	glPushMatrix();
		glTranslatef(jog_x, jog_y-2.5, jog_z);
		drawSnowMan();
	glPopMatrix();	

	glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, corChao);
		glBegin(GL_QUADS);
		glVertex3f(-10000, -TAM_BLOCO / 2, -10000);
		glVertex3f(-10000, -TAM_BLOCO / 2, 10000);
		glVertex3f(10000, -TAM_BLOCO / 2, 10000);
		glVertex3f(10000, -TAM_BLOCO / 2, -10000);
		glEnd();
	glPopMatrix();

	
	//Desenha mapa
	for (x = 0; x < 15; x++)
	{
		for (y = 0; y < 19; y++)
		{
			if (mapa[x][y])
			{
				x_mun = x * TAM_BLOCO;
				y_mun = y * TAM_BLOCO;

				glPushMatrix();
					glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, corParede);	
					glTranslatef(x_mun, 5, y_mun);
					if (wire) glutWireCube(TAM_BLOCO);
					else glutSolidCube(TAM_BLOCO);
				glPopMatrix();
			}
		}
	}
	glutSwapBuffers();
}


void Keyboard_Function(unsigned char key, int x, int y)
{
	nextMoveX = sinf(anglesX) * -cosf(anglesY) * PASSO;
	nextMoveZ = cosf(anglesX) * cosf(anglesY) * PASSO;
	switch (key)
	{
		case 27:
			exit(0); break;
		case 'q':
			jog_y += 10;
			break;
		case 'e':
			jog_y -= 10;
			break;
		case 'w':
			if (pode_mover(jog_x, jog_z, -nextMoveX, -nextMoveZ))
			{
				jog_x -= nextMoveX;
				jog_z -= nextMoveZ;	
			}
			break;
		case 's':
			if (pode_mover(jog_x, jog_z, nextMoveX, nextMoveZ))
			{
				jog_x += nextMoveX;
				jog_z += nextMoveZ;
			}
			break;
	}

}

void postRedisplay(void)
{
	glutPostRedisplay();
	
}

void mouseMove(int x, int y) { 	
	static int wrap = 0;

	if(!wrap) {
		int ww = glutGet(GLUT_WINDOW_WIDTH);
		int wh = glutGet(GLUT_WINDOW_HEIGHT);

		int dx = x - ww / 2;
		int dy = y - wh / 2;

		const float mousespeed = 0.001;

		anglesX += dx * mousespeed;
		anglesY += dy * mousespeed;

		if(anglesX < -M_PI)
			anglesX += M_PI * 2;
		else if(anglesX > M_PI)
			anglesX -= M_PI * 2;

		if(anglesY < -M_PI / 2)
			anglesY = -M_PI / 2;
		if(anglesY > M_PI / 2)
			anglesY = M_PI / 2;

		mov_x = sinf(anglesX) * -cosf(anglesY);
		mov_y = sinf(anglesY);
		mov_z = cosf(anglesX) * cosf(anglesY);
		
		wrap = 1;
		glutWarpPointer(ww / 2, wh / 2);
	} else {
		wrap = 0;
	}
}

void Inicializa(void)
{

	glShadeModel(GL_SMOOTH);

	glClearColor(0.3, 0.6, 0.8, 0.0);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luz_branca);
	glLightfv(GL_LIGHT0, GL_SPECULAR, luz_branca);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, 1, 0.1, 3000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_DEPTH_TEST);

}

void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Trabalho Computacao Grafica");
	glutFullScreen();

	Inicializa();

	glutDisplayFunc(display);
	glutKeyboardFunc(Keyboard_Function);

	glutMotionFunc(mouseMove);
	glutPassiveMotionFunc(mouseMove);	
	glutSetCursor(GLUT_CURSOR_NONE);

	glutIdleFunc(postRedisplay);

	glutMainLoop();
}