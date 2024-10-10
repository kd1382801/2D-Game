#include "Map.h"

void C_Map::LoadMap()
{
	for (int h = 0; h < MapHeight; h++) {
		for (int w = 0; w < MapWidth; w++) {
			m_data[h][w] = 0;
		}
	}

	FILE* fp;
	if ((fp = fopen("Texture/Map/Stage.txt", "r")) == nullptr)return;
	
	char ch;
	int h = 0;
	int w = 0;

	while ((ch = fgetc(fp)) != EOF) {
		if (ch == '\n') {
			h++;
			w = 0;
		}
		else {
			m_data[h][w] = atoi(&ch);
			w++;
		}
	}
	fclose(fp);
}

void C_Map::Init()
{
	for (int h = 0; h < MapHeight; h++) {
		for (int w = 0; w < MapWidth; w++) {
			const float x = -640 + (w * 64);
			const float y = 360 - (h * 64);

			m_pos[h][w] = Math::Vector2(x, y);


		}
	}
}

void C_Map::Update()
{
	for (int h = 0; h < MapHeight; h++) {
		for (int w = 0; w < MapWidth; w++) {
			m_mat[h][w] = Math::Matrix::CreateTranslation(m_pos[h][w].x, m_pos[h][w].y, 0);
		}
	}
}

void C_Map::Draw()
{
	Math::Rectangle rect;

	for (int h = 0; h < MapHeight; h++) {
		for (int w = 0; w < MapWidth; w++) {
			switch (m_data[h][w])
			{
			case 0:
				rect = { 0,0,64,64 };
				break;
			case 1:
				rect = { 64,0,64,64 };
				break;
			default:
				break;
			}
			SHADER.m_spriteShader.SetMatrix(m_mat[h][w]);
			SHADER.m_spriteShader.DrawTex(m_pTex, rect, 1.0f);
		}
	}
}
