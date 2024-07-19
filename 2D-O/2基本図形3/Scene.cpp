#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	//��ʃN���A
	color = { 0,0,0,1 };
	SHADER.m_spriteShader.DrawBox(0, 0, 360, 200, &color, true);

	if (GetAsyncKeyState('C') & 0x8000) {
		for (int i = 0; i < 50; i++) {
			color = { rand() / 32767.0f,rand() / 32767.0f,rand() / 32767.0f,1 };
			DrawCircleEx(rand() % 640 - 320, rand() % 360 - 180, rand() % 100, rand() % 100, &color);
		}
	}
	
	color = { 0,1,0,1 };
	DrawPolygon(0, 0, 100, 100, frame / 60 + 1, 90, &color, false); 

	color = { 1,1,0,1 };
	DrawPolygon(200, 50, size, size, 6, degree, &color, false);
	DrawPolygon(200, 50, size, size);

	DrawFireworks(-200,50);

	//������̓e�N�X�`���Ȃǂ�`�悵����ɏ�������
	// ������\��
	SHADER.m_spriteShader.DrawString(180, 170, "��{�}�`�R", Math::Vector4(0, 0, 0, 1));
	SHADER.m_spriteShader.DrawString(180+8, 170-5, "��{�}�`�R", Math::Vector4(1, 1, 0, frame / 180.0f));
}



void Scene::Update()
{
	//�㉺�L�[�Ŋg��k��
	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		size += 1;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		size -= 1;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		degree += 2;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		degree -= 2;
	}
	


	//�t���[�����𑝂₷
	frame++;
}

void Scene::Init()
{
	//�����̏�����
	srand(timeGetTime());

	size = 20;
	degree = 90;
	frame = 0;
}

void Scene::Release()
{
	
}

void Scene::DrawCircleEx(float cx, float cy, float radiusX, float radiusY, Math::Color* pColor)
{
	//�~�O��
	float a, b, deg;

	
	for (deg = 0; deg < 360; deg += 1)
	{
		a = cos(deg * 3.14f / 180) * radiusX;
		b = sin(deg * 3.14f / 180) * radiusY;
		SHADER.m_spriteShader.DrawPoint(cx + a, cy + b, pColor);
	}

}

void Scene::DrawPolygon(float cx, float cy, float radiusX, float radiusY, float vertexNum,float startDeg, Math::Color* pColor,bool paintFlg)
{
	//�~�O��
	float a, b, deg;				//���݂̓_�̏��
	float aNext, bNext, degNext;	//���̓_�̏��
	float intervalDeg;				//�p�x�̊Ԋu
	intervalDeg = 360 / vertexNum;	//���_���Ŋ���

	for (deg = startDeg; deg < 360 + startDeg; deg += intervalDeg)
	{
		//���݂̓_
		a = cos(DirectX::XMConvertToRadians(deg)) * radiusX;
		b = sin(DirectX::XMConvertToRadians(deg)) * radiusY;
		//SHADER.m_spriteShader.DrawPoint(cx + a, cy + b, pColor);
		//SHADER.m_spriteShader.DrawCircle(cx + a, cy + b, 10, pColor, false);

		//���̓_
		degNext = deg + intervalDeg;
		aNext = cos(DirectX::XMConvertToRadians(degNext)) * radiusX;
		bNext = sin(DirectX::XMConvertToRadians(degNext)) * radiusY;
		//SHADER.m_spriteShader.DrawPoint(cx + aNext, cy + bNext, pColor);
		//SHADER.m_spriteShader.DrawCircle(cx + aNext, cy + bNext, 20, pColor, false);

		if (!paintFlg) {
			//���݂̓_�Ǝ��̓_����Ō���
			SHADER.m_spriteShader.DrawLine(cx + a, cy + b, cx + aNext, cy + bNext, pColor);
		}
		else {
			//�h��Ԃ��̏ꍇ
			SHADER.m_spriteShader.DrawTriangle(cx, cy, cx + a, cy + b, cx + aNext, cy + bNext, pColor, true);
		}
	}

}

void Scene::DrawFireworks(float cx, float cy)
{
	float a, b, c = 100, deg;

	for (c = 0; c < 100; c += 15) {
		color = { rand() / 32767.0f,rand() / 32767.0f,rand() / 32767.0f,1};
		for (deg = 0; deg < 360; deg += 10)//�~1�����̃��[�v
		{
			a = cos(deg * 3.14f / 180) * c;
			b = sin(deg * 3.14f / 180) * c;
			SHADER.m_spriteShader.DrawCircle(cx + a, cy + b, 3, &color);
		}
	}
}

void Scene::ImGuiUpdate()
{
	return;

	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_Once);

	// �f�o�b�O�E�B���h�E
	if (ImGui::Begin("Debug Window"))
	{
		ImGui::Text("FPS : %d", APP.m_fps);
	}
	ImGui::End();
}
