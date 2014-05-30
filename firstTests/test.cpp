#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdio>

/* Coordenadas do Objeto importado. */
struct coordinate {
	float x, y, z;
	coordinate(float a, float b, float c) : x(a),y(b),z(c) {};
};

/* Faces do Objeto importado. */
struct face {
	int facenum; /* Id da Face. */
	bool four; /* Face representada por quadrados (four) ou triangulos. */
	int faces[4]; /* Cada face pode possuir ate 4 coordenadas. */

	/* Construtor para representacao atraves de triangulo. */
	face(int num, int f1, int f2, int f3) : facenum(num) {
		faces[0] = f1;
		faces[1] = f2;
		faces[2] = f3;
		four=false;
	};
	/* Construtor para representacao atraves de quadrado. */
	face(int num, int f1, int f2, int f3, int f4) : facenum(num) {
		faces[0] = f1;
		faces[1] = f2;
		faces[2] = f3;
		faces[3] = f4;
		four=true;
	};
};

/* Funcao para importar objeto de um arquivo .obj */
int loadObject(const char* filename)
{
	/* Vetor dinamico para guardar todo os dados do arquivo como uma string. */
	std::vector<std::string*> coord;
	std::vector<coordinate*> vertex; /* Guardar os vertices e faces do objeto. */
	std::vector<face*> faces;
	/* Coordenadas dos vetores normais as superficies do objeto */
	std::vector<coordinate*> normals; 
	/* Lendo arquivo */
	std::ifstream in(filename);
	if(!in.is_open()) {
		std::cout << "Arquivo nao aberto!\n";
		return -1;
	}
	/* Buffer para guardar uma linha do arquivo. */
	char buffer[256];
	/* Inicializando vetor coord. */
	while(!in.eof()) {
		in.getline(c,256);
		coord.push_back(new std::string(c));
	}
	/* Lendo os vertices, os vetores normais e as faces de acordo com o arquivo .obj */
	for(int i=0;i<coord.size();++i) {
		if(*coord[i][0] == '#') /* Eh um Comentario */
			continue;
		else if(*coord[i][0]=='v' && *coord[i][1]==' ') /* Eh um vertice */{
			float tmpx,tmpy,tmpz;
			sscanf(coord[i]->c_str(),"v %f %f %f",&tmpx,&tmpy,&tmpz);
			vexter.push_back(new coordinate(tmpx,tmpy,tmpz));
		}else if(*coord[i][0]=='v' && *coord[i][1]==' ') /* Eh um normal */{
			float tmpx,tmpy,tmpz;
			sscanf(coord[i]->c_str(),"vn %f %f %f",&tmpx,&tmpy,&tmpz);
			normals.push_back(new coordinate(tmpx,tmpy,tmpz));
		}else if(*coord[i][0]='f') /* Eh uma face */{
			int a,b,c,d,e;
			if(count(coord[i]->begin(),coord[i]->end(), ' ')==4) /* Qtd de vertices */{
				sscanf(coord[i]->c_str())
			}
		}
	}

	/* Liberando memoria do vetor coord. */
	for(int i=0;i<coord.size();++i)
		delete coord[i];

}

int main() 
{
	//Start SDL 
	SDL_Init( SDL_INIT_EVERYTHING ); 
	//Starts up SDL and creates window 
	bool init(); 
	//Loads media 
	bool loadMedia(); 
	//Frees media and shuts down SDL 
	void close();
	//Quit SDL 
	SDL_Quit(); return 0; 
	return 0;
}