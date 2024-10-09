#include "main.h"
#include "Scene.h"

//�e�N�X�`��(tmptex)�ւ̕`�揈�� main.cpp 206 Draw2D�̑O�ɏ���
void Scene::DynamicDraw2D()
{
	//�`���̃e�N�X�`���̃N���A(�����F)
	tmpTex.ClearRenerTarget(Math::Color(0.0f, 0.0f, 0.0f, 0.0f));

	//�`�����e�N�X�`���֐؂�ւ�
	tmpTex.SetRenderTarget();

	//�w�i
	SHADER.m_spriteShader.SetMatrix(back.mat);
	SHADER.m_spriteShader.DrawTex(&backTex, Math::Rectangle(0, 0, 1280, 720), 1.0f);

	//���@
	SHADER.m_spriteShader.SetMatrix(player.mat);
	SHADER.m_spriteShader.DrawTex(&playerTex, Math::Rectangle(0, 0, 64, 64), 1.0f);

}

//�o�b�N�o�b�t�@(���s���)�ւ̕`�揈��
void Scene::Draw2D()
{
	//�`�����o�b�N�o�b�t�@�֐؂�ւ�(�ȍ~�̕`�揈���̓o�b�N�o�b�t�@��)
	D3D.SetBackBuffer();

	//DynamicDraw2D()�ō쐬�����e�N�X�`����`��

	//������
	SHADER.m_spriteShader.SetMatrix(leftMat);
	SHADER.m_spriteShader.DrawTex(&tmpTex, Math::Rectangle(0, 0, 640, 720), 1.0f);
	
	//�E����
	SHADER.m_spriteShader.SetMatrix(rightMat);
	SHADER.m_spriteShader.DrawTex(&tmpTex, Math::Rectangle(640, 0, 640, 720), 1.0f);

	//���C�v�\��(������xtemTex���E��ɏ������\��)
	SHADER.m_spriteShader.SetMatrix(wipe.mat);
	SHADER.m_spriteShader.DrawTex(&tmpTex, Math::Rectangle(0, 0, 1280, 720), 1.0f);



}

void Scene::Update()
{
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		player.x -= 5;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		player.x += 5;
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		player.y += 5;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		player.y -= 5;
	}

	//��ʕ���
	if (GetAsyncKeyState('Z') & 0x8000) {
		cutX += 10;
	}
	if (GetAsyncKeyState('X') & 0x8000) {
		cutX -= 10;
	}

	player.mat = Math::Matrix::CreateTranslation(player.x, player.y, 0);
	back.mat = Math::Matrix::CreateTranslation(back.x, back.y, 0);

	//���C�v�p
	wipe.transMat = Math::Matrix::CreateTranslation(wipe.x, wipe.y, 0);
	wipe.scaleMat = Math::Matrix::CreateScale(wipe.scaleX, wipe.scaleY, 1);
	wipe.mat = wipe.scaleMat * wipe.transMat;

	//��ʕ���
	leftMat = Math::Matrix::CreateTranslation(-cutX, 0, 0);
	rightMat = Math::Matrix::CreateTranslation(cutX, 0, 0);
}

void Scene::Init()
{
	playerTex.Load("Texture/player.png");
	backTex.Load("Texture/Back_02.png");

	//�����_�[�^�[�Q�b�g�p�e�N�X�`���쐬
	tmpTex.CreateRenderTarget(1280, 720);


	back.x = 0.0f;
	back.y = 0.0f;

	player.x = 0.0f;
	player.y = 0.0f;

	wipe.x = 400.0f;
	wipe.y = 200.0f;
	wipe.scaleX = 0.25f;
	wipe.scaleY = 0.25f;

	cutX = 320;
}

void Scene::Release()
{
	// �摜�̉������
	playerTex.Release();
	backTex.Release();
	tmpTex.Release();
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
