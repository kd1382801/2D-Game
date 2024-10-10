#pragma once

class C_Player {
public:

	C_Player(){}
	~C_Player(){}

	void Init();
	void Action();
	void Update();
	void Draw();

	void MapHitX(float posX, float moveX);
	void MapHitY(float posY, float moveY, bool jump);

	void SetTex(KdTexture* tex) { m_pTex = tex; }

	Math::Vector2 GetPos() { return m_pos; }
	Math::Vector2 GetFuturePos() { return m_pos + m_move; }
	float GetRadius() { return 32.0f; }

private:

	const float Gravity = 1.0f;		//重力
	const float JumpPower = 20.0f;	//ジャンプ力
	const float MovePower = 5.0f;	//移動力

	Math::Vector2 m_pos;
	Math::Vector2 m_move;
	KdTexture* m_pTex;
	Math::Matrix m_mat;
	Math::Matrix m_transMat;
	Math::Matrix m_scaleMat;

	bool m_alive;	//生存フラグ
	bool m_jump;	//ジャンプフラグ
	float m_scaleX;	//拡大率
};