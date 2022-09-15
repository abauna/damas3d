#include <GL/glut.h>
#include<math.h>
#include<stdio.h>

// ALUNOS CI73A

// Arthur Lobo - 106839
// Daniel Braz - 104995
// Jackson Nikolay - 108952
// Lucas Muniz - 106990
// Miguel Awad - 097750

// O jogo conta com ambiente 3d, divisão de turnos, rotação do mouse, controle de zoom no mouse, som ao comer outra peça,
// animação da peça deslocando, a seleção de peças se dá com as setas, a seleção com o botão de 'home' e
// a movimentação das peças após selecionadas é com as setas de direita e esquerda


#define SOLID CLOSED CYLINDER(QUAD, BASE, TOP, HEIGHT, SLICES, STACKS)\
gluCylinder(QUAD, BASE, TOP, HEIGHT, SLICES, STACKS);\
glRotatef(180, 1, 0, 0);\
gluDisk(QUAD, 0.0 f, BASE, SLICES, 1);\
glRotatef(180, 1, 0, 0);\
glTranslatef(0.0 f, 0.0 f, HEIGHT);\
gluDisk(QUAD, 0.0 f, TOP, SLICES, 1);\
glTranslatef(0.0 f, 0.0 f, -HEIGHT);

GLfloat angle, fAspect;

float yPos = -0;
float xPos = 0;
int selected = 0;

typedef struct {
    float x = 0, y = 0, z = 0;
} coord;

typedef struct {
    int spinning = 0;
    coord begin, end;
    float tBegin = 0;
    float duration = 0;
} Animation;

Animation animation;

struct Piece {
    int x = 0;
    int y = 0;
    coord pos;
    int side = 0;
    int alive = 0;
    int cor = 0;
};

int viewAngle = 0;
Piece piece[25];

coord moveTo(coord start, coord end, float tStart, float tActual, float duration) {
    coord result;
    float t = (tActual - tStart) / duration;
    result.x = ((end.x - start.x) * t) + start.x;
    result.y = ((end.z - start.y) * t) + start.y;

    return result;

}

void visualizationParameters();

int last = 0;
int aux = 1;

void solidPiece(GLUquadric *QUAD, float radius, float HEIGHT, int SLICES, int STACKS, int cor) {
    switch (cor) {
        case 0:
            glColor3f(139, 0, 139);
            break;
        case 1:
            glColor3f(0, 0, 0);
            break;
        case 2:
            glColor3f(1, 1, 1);
            break;
        case 4:
            glColor3f(0, 255, 0);
            break;
    }
    gluCylinder(QUAD, radius, radius, HEIGHT, SLICES, STACKS);
    glPushMatrix();
    glCullFace(GL_FRONT);
    glTranslated(0, 0, HEIGHT * 0.8);
    gluCylinder(QUAD, radius * 0.6, radius * 0.75, HEIGHT * 0.2, 64, 9);
    glCullFace(GL_BACK);
    glPopMatrix();
    glPushMatrix();
    glTranslated(0, 0, HEIGHT * 0.8);
    gluDisk(QUAD, 0, radius * 0, 6 * 64, 8);
    glPopMatrix();
    glPushMatrix();
    glTranslated(0, 0, HEIGHT);
    gluDisk(QUAD, 0, radius * 0.75, 64, 8);
    glPopMatrix();
    glPushMatrix();
    glRotated(180, 1, 0, 0);
    gluDisk(QUAD, 0, radius, 64, 8);
    glPopMatrix();
}

void line1() {
    for (int i = 0; i < 4; i++) {
        glColor3f(1.0f, 1.0f, 1.0f);
        glPushMatrix();
        glTranslatef(20.0f, 0.0f, 10.0f);
        glutSolidCube(20.0f);
        glPopMatrix();
        glTranslatef(-20.0f, 0.0f, 0.0f);
        glColor3f(0, 0, 255);
        glPushMatrix();
        glTranslatef(20.0f, 0.0f, 10.0f);
        glutSolidCube(20.0f);
        glPopMatrix();
        glTranslatef(-20.0f, 0.0f, 0.0f);
    }
}


void line2() {
    for (int i = 0; i < 4; i++) {
        glColor3f(0, 0, 255);
        glPushMatrix();
        glTranslatef(20.0f, 0.0f, 10.0f);
        glutSolidCube(20.0f);
        glPopMatrix();
        glTranslatef(-20.0f, 0.0f, 0.0f);
        glColor3f(1.0f, 1.0f, 1.0f);
        glPushMatrix();
        glTranslatef(20.0f, 0.0f, 10.0f);
        glutSolidCube(20.0f);
        glPopMatrix();
        glTranslatef(-20.0f, 0.0f, 0.0f);
    }
}

void generatePiece(Piece p, GLUquadric *quadric) {
    if (p.alive) {
        float y = ((p.pos.y - yPos) * 20);
        float x = ((p.pos.x - xPos) * 20);
        yPos = p.pos.y;
        xPos = p.pos.x;

        glTranslatef(x, y, 0);
        solidPiece(quadric, 10, 20, 100, 100, p.cor);
    }
}

void draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;

    if (animation.spinning == 1) {
        if (t - animation.tBegin >= animation.duration) {
            piece[aux].pos.x = animation.end.x;
            piece[aux].pos.y = animation.end.z;
            animation.spinning = 0;
        } else {
            piece[aux].pos = moveTo(animation.begin, animation.end, animation.tBegin, t, animation.duration);
        }
    }

    gluLookAt(cos((float) viewAngle / 10000) * 200, 80, sin((float) viewAngle / 10000) * 200, 0, 0, 0, 0, 1, 0);

    glTranslatef(80.0f, -80.0f, 80.0f);
    xPos = 0;
    yPos = 0;
    glTranslatef(-60.0f, 0.0f, -150.0f);
    line1();
    glTranslatef(160.0f, 0.0f, 20.0f);
    line2();
    glTranslatef(160.0f, 0.0f, 20.0f);
    line1();
    glTranslatef(160.0f, 0.0f, 20.0f);
    line2();
    glTranslatef(160.0f, 0.0f, 20.0f);
    line1();
    glTranslatef(160.0f, 0.0f, 20.0f);
    line2();
    glTranslatef(160.0f, 0.0f, 20.0f);
    line1();
    glTranslatef(160.0f, 0.0f, 20.0f);
    line2();
    GLUquadric *quadric = gluNewQuadric();
    glTranslatef(40.0f, 20.0f, -130.0f);
    glRotatef(90, 1, 0, 0);

    for (int i = 0; i <= 24; i++) {
        generatePiece(piece[i], quadric);
    }

    glutSwapBuffers();
}

int verifyMovement(int x, int y, int side) {
    if ((y > -1) && (y < 8) && (x > -1) && (x < 8)) {
        if (last != side) {
            last = side;
            for (int i = 0; i < 24; i++) {
                if (piece[i].pos.x == x) {
                    if (piece[i].pos.y == y) {
                        if (piece[i].side == side) {
                            return 1;
                        } else {
                            piece[i].alive = 0;
                            piece[i].pos.x = 0;
                            piece[i].pos.y = 0;
                            return 2;
                        }
                    }
                }
            }
        } else {
            return 1;
        }
    } else {
        return 1;
    }
    return 0;
}

void specialKeys(int key, int x, int y) {
    int n_c;
    if (aux <= 11) {
        n_c = 1;
    } else {
        n_c = 2;
    }
    if (selected) {
        int xt = 0;
        int yt = 0;
        if (piece[aux].side) {
            if (key == GLUT_KEY_RIGHT) {
                xt = 1;
                yt = -1;
                selected = 0;
            }
            if (key == GLUT_KEY_LEFT) {
                xt = 1;
                yt = 1;
                selected = 0;
            }
        } else {
            if (key == GLUT_KEY_RIGHT) {
                xt = -1;
                yt = -1;
                selected = 0;
            }
            if (key == GLUT_KEY_LEFT) {
                xt = -1;
                yt = 1;
                selected = 0;
            }
        }
        const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
        int nx = round(xt + piece[aux].pos.x);
        int ny = round(yt + piece[aux].pos.y);

        switch (verifyMovement(nx, ny, piece[aux].side)) {
            case 0:
                if (animation.spinning == 0) {
                    animation.begin = piece[aux].pos;
                    animation.end.x = xt + piece[aux].pos.x;
                    animation.end.y = 15;
                    animation.end.z = yt + piece[aux].pos.y;
                    animation.tBegin = t;
                    animation.duration = 2;
                    animation.spinning = 1;
                }
                break;

            case 1:
                break;

            case 2:
                printf("\a");
                animation.begin = piece[aux].pos;
                animation.end.x = (2 * xt) + piece[aux].pos.x;
                animation.end.y = 15;
                animation.end.z = (2 * yt) + piece[aux].pos.y;
                animation.tBegin = t;
                animation.duration = 2;
                animation.spinning = 1;
                break;
        }
        selected = 0;
    } else {
        printf(" \n piece: %d side: %d alive: %d x=%f y=%f \n", aux, piece[aux].side, piece[aux].alive,
               piece[aux].pos.x, piece[aux].pos.y);

        if (key == GLUT_KEY_UP) {
            piece[aux].cor = n_c;
            aux = aux - 4;
            piece[aux].cor = 0;
        }

        if (key == GLUT_KEY_DOWN) {
            piece[aux].cor = n_c;
            aux = aux + 4;
            piece[aux].cor = 0;
        }

        if (key == GLUT_KEY_LEFT) {
            piece[aux].cor = n_c;
            aux = aux + 1;
            piece[aux].cor = 0;
        }

        if (key == GLUT_KEY_RIGHT) {
            piece[aux].cor = n_c;
            aux = aux - 1;
            piece[aux].cor = 0;
        }

        if (key == GLUT_KEY_HOME) {
            selected = 1;
            piece[aux].cor = 4;
        }

    }

    visualizationParameters();
    glFlush();
    glutPostRedisplay();
}


void initialize() {
    GLfloat ambientLight[4] = {
            0.2,
            0.2,
            0.2,
            1.0
    };
    GLfloat diffusedLight[4] = {
            0.7,
            0.7,
            0.7,
            1.0
    }; // "cor"
    GLfloat specularLight[4] = {
            1.0,
            1.0,
            1.0,
            1.0
    }; // "brilho"
    GLfloat lightPosition[4] = {
            0.0,
            50.0,
            50.0,
            1.0
    };

    // Capacidade de brilho do material
    GLfloat specularity[4] = {
            1.0,
            1.0,
            1.0,
            1.0
    };
    GLint materialSpecularity = 60;

    glClearColor(0.0f, 0.1f, 0.0f, 0.0f);
    glShadeModel(GL_SMOOTH);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularity);
    glMateriali(GL_FRONT, GL_SHININESS, materialSpecularity);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffusedLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    angle = 60;
}

void visualizationParameters() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(angle, fAspect, 0.4, 500);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0, 90, 100, 10, 10, 10, 0, 1, 0);
}

void dealWindowSize(GLsizei w, GLsizei h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    fAspect = (GLfloat) w / (GLfloat) h;
    visualizationParameters();
}

static void idle() {
    glutPostRedisplay();
}

void dealMouseMove(int x, int y) {
    viewAngle += x;
}

void dealMouseAngle(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON)
        if (state == GLUT_DOWN) {  // Zoom-in
            if (angle >= 10) angle -= 5;
        }
    if (button == GLUT_RIGHT_BUTTON)
        if (state == GLUT_DOWN) {  // Zoom-out
            if (angle <= 130) angle += 5;
        }
    visualizationParameters();
    glutPostRedisplay();
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    for (int i = 0; i < 24; i++) {
        if (i <= 11) {
            piece[i].x = i / 4;
            piece[i].pos.x = i / 4;
            piece[i].y = (piece[i].x % 2) + (i % 4) * 2;
            piece[i].pos.y = (piece[i].x % 2) + (i % 4) * 2;
            piece[i].cor = 1;
            piece[i].side = 1;
            piece[i].alive = 1;
        } else {
            piece[i].x = (i / 4) + 2;
            piece[i].y = ((piece[i].x % 2) + (i % 4) * 2);
            piece[i].pos.x = (i / 4) + 2;
            piece[i].pos.y = ((piece[i].x % 2) + (i % 4) * 2);
            piece[i].cor = 2;
            piece[i].side = 0;
            piece[i].alive = 1;
        }
    }
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 450);
    glutCreateWindow("DAMA 3D");
    glutDisplayFunc(draw);
    glutIdleFunc(idle);
    glutReshapeFunc(dealWindowSize);
    initialize();

    glutMouseFunc(dealMouseAngle);
    glutMotionFunc(dealMouseMove);
    glutSpecialFunc(specialKeys);

    glutMainLoop();
}