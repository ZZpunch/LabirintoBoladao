//Aluno Lucas Heitor

// #include <windows.h>     //biblioteca do windows...
#include <GL/gl.h>       //biblioteca gl.h
#include <GL/glut.h>     //biblioteca glut - ferramentas adicionais
#include <math.h>

//DEFINES--------------------------------------------

#define TAM_BLOCO 100
#define PASSO	10

#define NORTE  0
#define LESTE  1
#define SUL    2
#define OESTE  3

/* ESTRUTURAS------------ */

typedef struct _esf
{
	GLint  x_pos, z_pos;
	GLint dir;

} ESFERA;


//GLOBALS--------------------------------------------

GLfloat corParede[] = { 0.2, 0.1, 0.2, 1.0 };
GLfloat corChao[] = { 1.0,1.0,1.0,1.0 };
GLfloat light_position[] = { 0.0, 500.0, 0.0, 1.0 };
GLfloat luz_branca[] = { 1.0,1.0,1.0,1.0 };
GLfloat lmodel_ambient[] = { 0.6,0.6,0.6,1.0 };
GLfloat mapaChegada[] = { 0.7, 0.1, 0.1, 1.0 };



GLfloat jog_x = TAM_BLOCO, jog_z = TAM_BLOCO;
GLfloat jog_y = 25;
GLfloat mov_x = PASSO, mov_z = 0;
GLint angulo = 0;
GLint wire = 0;


ESFERA g_esfera;


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
  1,0,1,0,1,1,0,1,0,0,0,0,1,0,1,0,1,0,1,
  1,0,1,0,0,0,0,1,1,1,1,1,1,0,1,0,0,0,1,
  1,0,1,0,1,1,0,1,0,0,0,0,0,0,1,1,0,1,1,
  1,0,0,0,0,0,0,1,0,0,1,1,1,1,0,0,0,1,1,
  1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1 };

//FUNCOES--------------------------------------------
int pode_mover(float pos_x, float pos_z, float vet_x, float vet_z)
{
	float mundo_x = pos_x + vet_x;
	float mundo_z = pos_z + vet_z;

	int ind_x = (int)((mundo_x + TAM_BLOCO / 2) / TAM_BLOCO);
	int ind_z = (int)((mundo_z + TAM_BLOCO / 2) / TAM_BLOCO);

	if (mapa[ind_x][ind_z]) return 0;
	else return 1;
}

//---------------------------------------------

void display(void)
{
	int x, y;
	int x_mun, y_mun;


	//limpa todos os pixels
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	gluLookAt(jog_x, jog_y, jog_z, jog_x + mov_x, jog_y, jog_z + mov_z, 0, 1, 0);


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

				if (x == 13 && y == 8) {
					glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mapaChegada);
				}
				else {
					glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, corParede);	
				}

				glTranslatef(x_mun, 5, y_mun);

				if (wire) glutWireCube(TAM_BLOCO);
				else glutSolidCube(TAM_BLOCO);

				glPopMatrix();

			}
		}
	}

	//Desenho uma parede vermelha para indicar o fim do labirinto.

	glutSwapBuffers();
}

//-----------------------------------------------------------------

void Keyboard_Function(unsigned char key, int x, int y)
{
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
	}

}

//-----------------------------------------------------------------

void Special_Function(int key, int x, int y)
{
	float rad;

	switch (key)
	{
	case GLUT_KEY_DOWN:

		if (pode_mover(jog_x, jog_z, -mov_x, -mov_z))
		{
			jog_x -= mov_x;
			jog_z -= mov_z;	
		}
		break;


	case GLUT_KEY_UP:

		if (pode_mover(jog_x, jog_z, mov_x, mov_z))
		{
			jog_x += mov_x;
			jog_z += mov_z;
		}
		break;

	case GLUT_KEY_LEFT:

		angulo -= 10;

		if (angulo < 0) angulo += 360;


		rad = (float)(3.14159 * angulo / 180.0f);

		mov_x = cos(rad) * PASSO;
		mov_z = sin(rad) * PASSO;
		break;


	case GLUT_KEY_RIGHT:

		angulo += 10;

		if (angulo >= 360) angulo -= 360;

		rad = (float)(3.14159 * angulo / 180.0f);

		mov_x = cos(rad) * PASSO;
		mov_z = sin(rad) * PASSO;

		break;
	}

}

//-----------------------------------------------------------------
void postRedisplay(void)
{
	glutPostRedisplay();
	
}
//-----------------------------------------------------------------

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

//--------------------------------------------------------------

void main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Trabalho Computacao Grafica - Professor Chaua");
	glutFullScreen();

	Inicializa();

	glutDisplayFunc(display);
	glutKeyboardFunc(Keyboard_Function);
	glutSpecialFunc(Special_Function);
	glutIdleFunc(postRedisplay);

	glutMainLoop();

}


