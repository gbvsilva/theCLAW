#ifndef MAIN_H
#define MAIN_H

// ROBOT ARM CONTROLS
GLfloat baseSpin = 0.0;        
GLfloat shoulderAng = -22.5;  
GLfloat elbowAng = -22.5;   
GLfloat wristAng = -22.5;
GLfloat wristRot = 0.0;
GLfloat fingerAng = 45.0;
GLfloat clawScale = 0.8; // Variação de escala da garra

// ROBOT COLORS
GLfloat red[] = {1.0, 0, 0};
GLfloat green[] = {0, 1.0, 0};
GLfloat weakGreen[] = {0, 0.2, 0};
GLfloat blue[] = {0, 0, 1.0};
GLfloat yellow[] = {1.0, 1.0, 0};
GLfloat darkYellow[] = {0.25, 0.25, 0};
GLfloat purple[] = {1.0, 0, 1.0};
GLfloat white[] = {1.0, 1.0, 1.0};
GLfloat grey[] = {0.5, 0.5, 0.5};
GLfloat lightGrey[] = {0.75, 0.75, 0.75};
GLfloat darkGrey[] = {0.25, 0.25, 0.25};
GLfloat black[] = {0.0, 0.0, 0.0};
GLfloat blueLightGrey[] = {0.7, 0.75, 0.85};
GLfloat blueDarkGrey[] = {0.2, 0.25, 0.35};

// CAMERA CONTROLS
GLfloat eye[] = {0.0, 6.0, 6.0};
GLfloat center[] = {0.0, 1.0, 0.0};
GLfloat up[] = {0.0, 1.0, 0.0};
GLfloat camRot[] = {0.0, 0.0}; // Rotação da câmera nos eixos Y e X
GLfloat shake[] = {0.0, 0.0, 0.0}; // Vetor do screenshake
bool autocam = false; //Switch da câmera em terceira pessoa
bool autorotation = false; //Switch da autorotação
bool lighttest = false; //Switch dos postes de teste

// Frustum attributes
GLfloat fovy = 30.0;
GLfloat aspect = 512.0/512.0;
GLfloat zNear = 0.01;
GLfloat zFar = 50.0;
bool culling = false; //Switch do frustum

// PHYSICS
GLfloat* catchPtDir = (GLfloat*)malloc(sizeof(GLint)*16);
GLfloat* catchPtEsq = (GLfloat*)malloc(sizeof(GLint)*16);
GLfloat* catchPtCenter = (GLfloat*)malloc(sizeof(GLint)*16);
GLfloat* catchPtWrist = (GLfloat*)malloc(sizeof(GLint)*16);
bool clawSwitch = false; //Switch da captura da garra
bool caught = false; //Se algum objeto foi capturado
bool collision = false; //Se houve colisão

// CLAW AREA
GLfloat floorSize = 2.5; // Largura do piso
GLfloat gridSize = 0.5; // Tamanho de cada grid do chão. Não utilizado
GLfloat wallSize = 1; // Tamanho da parede
GLfloat collectArea[] = {0.6, 1.2, -0.3, 0.3}; // Área de coleta de blocos. {p1x, p2x, p1y, p2y}

// LIGHT
// Switches de luz
bool light0 = false;
bool light1 = false;
bool light2 = false;
bool light3 = false;
// Luz 0: Difusa, acima do campo
GLfloat light0Pos[] = {0.0, 2.0, 0.0, 1.0};
GLfloat light0Intensity[] = {1.0, 1.0, 1.0, 1.0};
GLfloat light0Specular[] = {0.0, 0.0, 0.0, 1.0};
// Luz 1: Ambiente
GLfloat light1Pos[] = {0.0, 0.0, 0.0, 0.0};
GLfloat light1Intensity[] = {0.4, 0.4, 0.4, 1.0};
// Luz 2: Especular, câmera
GLfloat light2Pos[] = {0, floorSize, floorSize, 1.0};
GLfloat light2Intensity[] = {1.0, 1.0, 1.0, 1.0};
// Luz 3: Spot difusa, acoplada à garra. Parte difusa não funcionando
GLfloat light3Pos[] = {0.0, 0.0, 0.0, 1.0};
GLfloat light3Intensity[] = {1.0, 1.0, 1.0, 1.0};
GLfloat light3Dir[] = {0.0, -1.0, 0.0};

// TEXTURE
GLuint wallTexture; // Identificador da textura atualmente em uso
unsigned int width, height; // Altura x Largura da imagem de textura
unsigned char * data; // Dados da imagem de textura

// OBJECT LIST
Cube** cubeArray; // Array com os cubos
int totalCube = 1; // Quantidade total de cubos daquele nível
int cubeLeft = 1; // Quantidade restante de cubos
int maxLevel = 2; // Quantidade máxima de níveis
int cubeIncrease = 2; // Quantidade de cubos adicionados por nível
time_t start; // Tempo de início
time_t end; // Tempo de término

// PROTÓTIPOS
GLuint loadBMP(const char*);
GLfloat ambLight(GLfloat);
void lightConfig();
void visible(float x, float y, float z, float w, char code[6]);
void setMaterial(GLfloat[3], GLfloat);
void drawUnitCylinder(int);
void drawUnitCube();
void drawUnitSphere(int);
void drawFloor();
void drawWalls();
void drawArmAxle();
void drawArm(GLfloat);
void drawFinger();
void drawAim();
void drawTHEClaw();
void drawCube(Cube*);
void drawCubeArray();
void drawPost(GLfloat);
void drawPostGroup();
void drawCollectBox();
void drawCatchSphere();
void updateCatchPt();
void display();
void reshape();
void idle();
void keyInput();
void lightConfig();
void createCatchPt();
void createCubeArray();
void visible(float, float, float, char[6]);
int init(int, char**);
int main(int, char**);

#endif
