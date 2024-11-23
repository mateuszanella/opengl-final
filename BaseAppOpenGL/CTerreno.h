#ifndef __TERRENO_H__
#define __TERRENO_H__



#include <windows.h>	// Header File For Windows
#include <fstream>

#include <glew.h>
#include <gl.h>										// Header File For The OpenGL32 Library
#include <glut.h>
#include <glext.h>
#include <glm/glm.hpp>


using namespace std;

#define MAP_SIZE		1025	// Este � o tamanho do arquivo .RAW
#define STEP_SIZE		20.0f	// Esta � a largura e altura de cada QUAD(quadrado do terreno)
#define	HEIGHT_RATIO	0.4f 

class CTerreno
{
public:
	CTerreno();
	~CTerreno();

	

	// Fun��o que retorna a altura (0 a 255) para o heightmap pegando X e Y
	int Height(int X, int Y);

	// Seta uma cor para cada index em particular, dependendo da altura do index
	void SetVertexColor(int x, int y);

	// Fun��o que carrega arquivo .RAW com o tamanho certo do arquivo
	void LoadRawFile(LPSTR strName);

	// Fun��o que p�e os dados do heightmap nas primitivas e desenha na tela
	void RenderHeightMap(bool isTexTile);
	void RenderHeightMapMultiTex(bool isTexTile);

	void CreateHeightMapDisplayList(bool isTexTile = false, bool isMultiTex = false);
	void RenderHeightMapDisplayList();


	glm::vec3 CalculateQuadNormalVector(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4);
	glm::vec3 CalculateTriangleNormalVector(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3);


private:
	BYTE pHeightMap[MAP_SIZE * MAP_SIZE];
	GLuint displayListId;
};


#endif
