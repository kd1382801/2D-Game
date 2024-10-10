#pragma once

class C_Map {
public:
	
	C_Map(){}
	~C_Map(){}

	void LoadMap();
	void Init();
	void Update();
	void Draw();

	void SetTex(KdTexture* mTex) { m_pTex = mTex; }

	const int GetHeight() { return MapHeight; }
	const int GetWidth() { return MapWidth; }
	const Math::Vector2 GetPos(int y, int x) { m_pos[y][x]; }
	const int GetData(int y, int x) { return m_data[y][x]; }
	const float GetRadius() { return 32.0f; }

private:
	
	static const int MapHeight = 12;
	static const int MapWidth = 32;
	
	int m_data[MapHeight][MapWidth];
	Math::Vector2 m_pos[MapHeight][MapWidth];
	Math::Matrix m_mat[MapHeight][MapWidth];
	KdTexture* m_pTex;

};