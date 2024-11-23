#pragma warning(disable:4996)

#include "CTerreno.h"



CTerreno::CTerreno()
{
	this->displayListId = 0;
}


CTerreno::~CTerreno()
{
	if (glIsList(this->displayListId))
		glDeleteLists(this->displayListId, 1);
}

// Fun��o que retorna a altura dentro do heightmap
int CTerreno::Height(int X, int Y)
{

	// Isto � usado como index para nosso array heightmap
	// Para ter certeza que n�o ir� exceder o tamanho do array, usa-se %.
	// Desse modo X e Y ser�o calculados com (MAX_SIZE - 1)

	int x = X % MAP_SIZE;	// Checar erros para o valor de X
	int y = Y % MAP_SIZE;	// Checar erros para o valor de Y

	if (!pHeightMap) return 0;		// Se houver erros...CAI FORA!!!

	// Abaixo n�s precisamos tratar o array simples como um array 2D
	// Pode-se usar a equa��o: index = (x + (y * arrayWidth)
	// Isso assume que estamos usando um array[x][y]
	// Agora que temos o index corrente, podemos retornar a algura daquele index

	return pHeightMap[x + (y * MAP_SIZE)] * HEIGHT_RATIO;	// Indexa em nosso array de altura e retorna a altura

}


// Seta uma cor para cada index em particular, dependendo da altura do index
void CTerreno::SetVertexColor(int x, int y)
{

	if (!pHeightMap) return;		// Ter certeza que que nossos dados de altura s�o v�lidos

	// Aqui n�s setamos a cor para o vertice baseado no indedice de altura
	// Para fazer escuro come�amos com -0.15f. Tamb�m pegamos o ratio da cor
	// para 0 e 1.0 pela divis�o da altura por 256.0f
	float fColor = -0.15f + (Height(x, y) / 256.0f);

	// Associamos a sombra verde para o v�rtice corrente
	glColor3f(fColor, fColor, fColor);

}




// Carrega o arquivo .RAW dentro de um array de bytes.
// Cada valor � um valor de altura
void CTerreno::LoadRawFile(LPSTR strName)
{

	FILE* pFile = NULL;

	// Abrindo o arquivo no modo Read/Binary
	pFile = fopen(strName, "rb");

	// Checar se o arquivo foi encontrado e p�de ser aberto
	if (pFile == NULL)
	{
		// Mostra mensagem de erro e termina o programa
		MessageBox(NULL, "Arquivo n�o encontrado!", "Erro carregando textura", MB_OK);
		return;
	}

	// Aqui n�s carregamos nosso .RAW dentro de nosso array de dados pHeightMap
	fread(pHeightMap, 1, MAP_SIZE * MAP_SIZE, pFile);

	// Depois que os dados foram lidos, � bom checar se tudo foi lido corretamente
	int result = ferror(pFile);

	// Checando se recebemos algum erro
	if (result)
	{
		MessageBox(NULL, "Dados da textura n�o recebidos!", "Erro na integridade dos dados", MB_OK);
	}

	// Fechamos o arquivo de textura
	fclose(pFile);

}


// Renderiza o heightmap com QUADS (poligonos)
void CTerreno::RenderHeightMap(bool isTexTile)
{
	int X = 0, Y = 0;		// Vari�veis para percorrer o array
	int x, y, z;			// Variaveis para readability
	float fColor = 0.0f;	// Vari�vel para guardar a cor de nosso pol�gono

	if (!pHeightMap) return;	// Ter certeza que que nossos dados de altura s�o v�lidos

	glBegin(GL_TRIANGLES);

	int index = 0;
	for (X = 0; X < MAP_SIZE; X += STEP_SIZE)
	{

		for (Y = 0; Y < MAP_SIZE; Y += STEP_SIZE)
		{
			float s = 0.0f;
			float t = 0.0f;

			x = X;
			y = Height(X, Y);
			z = Y;
			glm::vec3 v1 = glm::vec3(x, y, z);

			x = X;
			y = Height(X, Y + STEP_SIZE);
			z = Y + STEP_SIZE;
			glm::vec3 v2 = glm::vec3(x, y, z);

			x = X + STEP_SIZE;
			y = Height(X + STEP_SIZE, Y + STEP_SIZE);
			z = Y + STEP_SIZE;
			glm::vec3 v3 = glm::vec3(x, y, z);

			glm::vec3 n1 =  CalculateTriangleNormalVector(v1, v2, v3);
			glNormal3f(n1.x, n1.y, n1.z);

			if (isTexTile)
				glTexCoord2f(0.0f, 0.0f);
			else
			{
				s = (float)(v1.x * (1.0f / MAP_SIZE));
				t = (float)(v1.z * (1.0f / MAP_SIZE));
				glTexCoord2f(s, t);
			}
			glVertex3i(v1.x, v1.y, v1.z);
			
			if (isTexTile)
				glTexCoord2f(1.0f, 0.0f);
			else
			{
				s = (float)(v2.x * (1.0f / MAP_SIZE));
				t = (float)(v2.z * (1.0f / MAP_SIZE));
				glTexCoord2f(s, t);
			}
			glVertex3i(v2.x, v2.y, v2.z);
			
			if (isTexTile)
				glTexCoord2f(1.0f, 1.0f);
			else
			{
				s = (float)(v3.x * (1.0f / MAP_SIZE));
				t = (float)(v3.z * (1.0f / MAP_SIZE));
				glTexCoord2f(s, t);
			}
			glVertex3i(v3.x, v3.y, v3.z);





			x = X;
			y = Height(X, Y);
			z = Y;
			glm::vec3 v4 = glm::vec3(x, y, z);

			x = X + STEP_SIZE;
			y = Height(X + STEP_SIZE, Y + STEP_SIZE);
			z = Y + STEP_SIZE;
			glm::vec3 v5 = glm::vec3(x, y, z);

			x = X + STEP_SIZE;
			y = Height(X + STEP_SIZE, Y);
			z = Y;
			glm::vec3 v6 = glm::vec3(x, y, z);

			glm::vec3 n2 = CalculateTriangleNormalVector(v4, v5, v6);
			glNormal3f(n2.x, n2.y, n2.z);

			if (isTexTile)
				glTexCoord2f(0.0f, 0.0f);
			else
			{
				s = (float)(v4.x * (1.0f / MAP_SIZE));
				t = (float)(v4.z * (1.0f / MAP_SIZE));
				glTexCoord2f(s, t);
			}
			glVertex3i(v4.x, v4.y, v4.z);			

			if (isTexTile)
				glTexCoord2f(1.0f, 1.0f);
			else
			{
				s = (float)(v5.x * (1.0f / MAP_SIZE));
				t = (float)(v5.z * (1.0f / MAP_SIZE));
				glTexCoord2f(s, t);
			}
			glVertex3i(v5.x, v5.y, v5.z);			

			if (isTexTile)
				glTexCoord2f(0.0f, 1.0f);
			else
			{
				s = (float)(v6.x * (1.0f / MAP_SIZE));
				t = (float)(v6.z * (1.0f / MAP_SIZE));
				glTexCoord2f(s, t);
			}
			glVertex3i(v6.x, v6.y, v6.z);
		}
	}

	glEnd();
}

void CTerreno::RenderHeightMapMultiTex(bool isTexTile)
{
	int X = 0, Y = 0;		// Vari�veis para percorrer o array
	int x, y, z;			// Variaveis para readability
	float fColor = 0.0f;	// Vari�vel para guardar a cor de nosso pol�gono

	if (!pHeightMap) return;	// Ter certeza que que nossos dados de altura s�o v�lidos

	glBegin(GL_TRIANGLES);

	int index = 0;
	for (X = 0; X < MAP_SIZE; X += STEP_SIZE)
	{

		for (Y = 0; Y < MAP_SIZE; Y += STEP_SIZE)
		{
			float s = 0.0f;
			float t = 0.0f;

			x = X;
			y = Height(X, Y);
			z = Y;
			glm::vec3 v1 = glm::vec3(x, y, z);

			x = X;
			y = Height(X, Y + STEP_SIZE);
			z = Y + STEP_SIZE;
			glm::vec3 v2 = glm::vec3(x, y, z);

			x = X + STEP_SIZE;
			y = Height(X + STEP_SIZE, Y + STEP_SIZE);
			z = Y + STEP_SIZE;
			glm::vec3 v3 = glm::vec3(x, y, z);

			glm::vec3 n1 = CalculateTriangleNormalVector(v1, v2, v3);
			glNormal3f(n1.x, n1.y, n1.z);

			if (isTexTile)
			{
				s = (float)(v1.x * (1.0f / MAP_SIZE));
				t = (float)(v1.z * (1.0f / MAP_SIZE));
				glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
				glMultiTexCoord2f(GL_TEXTURE1, s, t);
				glMultiTexCoord2f(GL_TEXTURE2, 0.0f, 0.0f);
			}
			else
			{
				s = (float)(v1.x * (1.0f / MAP_SIZE));
				t = (float)(v1.z * (1.0f / MAP_SIZE));
				glMultiTexCoord2f(GL_TEXTURE0, s, t);
				glMultiTexCoord2f(GL_TEXTURE1, s, t);
				glMultiTexCoord2f(GL_TEXTURE2, s, t);
			}
			glVertex3i(v1.x, v1.y, v1.z);

			if (isTexTile)
			{
				s = (float)(v2.x * (1.0f / MAP_SIZE));
				t = (float)(v2.z * (1.0f / MAP_SIZE));
				glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 0.0f);
				glMultiTexCoord2f(GL_TEXTURE1, s, t);
				glMultiTexCoord2f(GL_TEXTURE2, 1.0f, 0.0f);
			}
			else
			{
				s = (float)(v2.x * (1.0f / MAP_SIZE));
				t = (float)(v2.z * (1.0f / MAP_SIZE));
				glMultiTexCoord2f(GL_TEXTURE0, s, t);
				glMultiTexCoord2f(GL_TEXTURE1, s, t);
				glMultiTexCoord2f(GL_TEXTURE2, s, t);
			}
			glVertex3i(v2.x, v2.y, v2.z);

			if (isTexTile)
			{
				s = (float)(v3.x * (1.0f / MAP_SIZE));
				t = (float)(v3.z * (1.0f / MAP_SIZE));
				glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 1.0f);
				glMultiTexCoord2f(GL_TEXTURE1, s, t);
				glMultiTexCoord2f(GL_TEXTURE2, 1.0f, 1.0f);
			}
			else
			{
				s = (float)(v3.x * (1.0f / MAP_SIZE));
				t = (float)(v3.z * (1.0f / MAP_SIZE));
				glMultiTexCoord2f(GL_TEXTURE0, s, t);
				glMultiTexCoord2f(GL_TEXTURE1, s, t);
				glMultiTexCoord2f(GL_TEXTURE2, s, t);
			}
			glVertex3i(v3.x, v3.y, v3.z);





			x = X;
			y = Height(X, Y);
			z = Y;
			glm::vec3 v4 = glm::vec3(x, y, z);

			x = X + STEP_SIZE;
			y = Height(X + STEP_SIZE, Y + STEP_SIZE);
			z = Y + STEP_SIZE;
			glm::vec3 v5 = glm::vec3(x, y, z);

			x = X + STEP_SIZE;
			y = Height(X + STEP_SIZE, Y);
			z = Y;
			glm::vec3 v6 = glm::vec3(x, y, z);

			glm::vec3 n2 = CalculateTriangleNormalVector(v4, v5, v6);
			glNormal3f(n2.x, n2.y, n2.z);

			if (isTexTile)
			{
				s = (float)(v4.x * (1.0f / MAP_SIZE));
				t = (float)(v4.z * (1.0f / MAP_SIZE));
				glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 0.0f);
				glMultiTexCoord2f(GL_TEXTURE1, s, t);
				glMultiTexCoord2f(GL_TEXTURE2, 0.0f, 0.0f);
			}
			else
			{
				s = (float)(v4.x * (1.0f / MAP_SIZE));
				t = (float)(v4.z * (1.0f / MAP_SIZE));
				glMultiTexCoord2f(GL_TEXTURE0, s, t);
				glMultiTexCoord2f(GL_TEXTURE1, s, t);
				glMultiTexCoord2f(GL_TEXTURE2, s, t);
			}
			glVertex3i(v4.x, v4.y, v4.z);

			if (isTexTile)
			{
				s = (float)(v5.x * (1.0f / MAP_SIZE));
				t = (float)(v5.z * (1.0f / MAP_SIZE));
				glMultiTexCoord2f(GL_TEXTURE0, 1.0f, 1.0f);
				glMultiTexCoord2f(GL_TEXTURE1, s, t);
				glMultiTexCoord2f(GL_TEXTURE2, 1.0f, 1.0f);
			}
			else
			{
				s = (float)(v5.x * (1.0f / MAP_SIZE));
				t = (float)(v5.z * (1.0f / MAP_SIZE));
				glMultiTexCoord2f(GL_TEXTURE0, s, t);
				glMultiTexCoord2f(GL_TEXTURE1, s, t);
				glMultiTexCoord2f(GL_TEXTURE2, s, t);
			}
			glVertex3i(v5.x, v5.y, v5.z);

			if (isTexTile)
			{
				s = (float)(v6.x * (1.0f / MAP_SIZE));
				t = (float)(v6.z * (1.0f / MAP_SIZE));
				glMultiTexCoord2f(GL_TEXTURE0, 0.0f, 1.0f);
				glMultiTexCoord2f(GL_TEXTURE1, s, t);
				glMultiTexCoord2f(GL_TEXTURE2, 0.0f, 1.0f);
			}
			else
			{
				s = (float)(v6.x * (1.0f / MAP_SIZE));
				t = (float)(v6.z * (1.0f / MAP_SIZE));
				glMultiTexCoord2f(GL_TEXTURE0, s, t);
				glMultiTexCoord2f(GL_TEXTURE1, s, t);
				glMultiTexCoord2f(GL_TEXTURE2, s, t);
			}
			glVertex3i(v6.x, v6.y, v6.z);
		}
	}

	glEnd();
}



void CTerreno::CreateHeightMapDisplayList(bool isTexTile, bool isMultiTex)
{
	displayListId = glGenLists(1);
	glNewList(displayListId, GL_COMPILE);

	if (isMultiTex)
		RenderHeightMapMultiTex(isTexTile);
	else
		RenderHeightMap(isTexTile);

	glEndList();
}



void CTerreno::RenderHeightMapDisplayList()
{
	glCallList(displayListId);
}




glm::vec3 CTerreno::CalculateQuadNormalVector(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4)
{
	/*
		   v4      v3
			+------+
			|      |
			|      |
			+------+
		   v1      v2
	*/

	glm::vec3 A, B;
	A.x = v2.x - v1.x;
	A.y = v2.y - v1.y;
	A.z = v2.z - v1.z;

	B.x = v4.x - v1.x;
	B.y = v4.y - v1.y;
	B.z = v4.z - v1.z;

	// Calcula o Cross Product
	glm::vec3 normal;
	normal.x = A.y * B.z - A.z * B.y;
	normal.y = A.z * B.x - A.x * B.z;
	normal.z = A.x * B.y - A.y * B.x;

	// Calcula a magnitude do vetor normal
	double magnitude = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);

	// Normaliza o vetor normal
	glm::vec3 normalizedVector;
	normalizedVector.x = (normal.x / magnitude);
	normalizedVector.y = (normal.y / magnitude);
	normalizedVector.z = (normal.z / magnitude);

	return normalizedVector;
}



glm::vec3 CTerreno::CalculateTriangleNormalVector(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3)
{
	/*
			   v3
			   /\
			  /  \
			 /    \
			/      \
			+------+
		   v1      v2
	*/

	glm::vec3 A, B;
	A.x = v2.x - v1.x;
	A.y = v2.y - v1.y;
	A.z = v2.z - v1.z;

	B.x = v3.x - v1.x;
	B.y = v3.y - v1.y;
	B.z = v3.z - v1.z;

	// Calcula o Cross Product
	glm::vec3 normal;
	normal.x = A.y * B.z - A.z * B.y;
	normal.y = A.z * B.x - A.x * B.z;
	normal.z = A.x * B.y - A.y * B.x;

	// Calcula a magnitude do vetor normal
	double magnitude = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);

	// Normaliza o vetor normal
	glm::vec3 normalizedVector;
	normalizedVector.x = (normal.x / magnitude);
	normalizedVector.y = (normal.y / magnitude);
	normalizedVector.z = (normal.z / magnitude);

	return normalizedVector;
}
