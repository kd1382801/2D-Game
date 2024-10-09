#include "main.h"
#include "Scene.h"

//�e�N�X�`��(tmpTex)�ւ̕`�揈�� main.cpp 206 Draw2D�̑O�ɏ���
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

	//�m�C�Y����
	//x���̗h�ꕝ
	int x = 0;
	for (int i = 0; i < 720; i+=sliceNum) {
		//�m�C�Y�̐U�ꕝ-50~50
		x = rand() % (noiseLv + 1) - ((float)noiseLv / 2);

		//�؂��肵���摜����ʂ̈�ԏォ��\������
		back.mat = Math::Matrix::CreateTranslation(x, 360 - i - ((float)sliceNum / 2), 0);
		
		SHADER.m_spriteShader.SetMatrix(back.mat);
		SHADER.m_spriteShader.DrawTex(&tmpTex, Math::Rectangle(0, i, 1280, sliceNum), 1.0f);
	}

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

	//�m�C�Y
	//�X���C�X�̕�
	if (GetAsyncKeyState('S') & 0x8000) {
		sliceNum += 1;
		if (sliceNum > 50) {
			sliceNum = 50;
		}
	}
	if (GetAsyncKeyState('A') & 0x8000) {
		sliceNum -= 1;
		if (sliceNum < 1) {
			sliceNum = 1;
		}
	}
	//�m�C�Y�̋���
	if (GetAsyncKeyState('X') & 0x8000) {
		noiseLv++;
		if (noiseLv > 200) {
			noiseLv = 200;
		}
	}
	if (GetAsyncKeyState('Z') & 0x8000) {
		noiseLv--;
		if (noiseLv < 0) {
			noiseLv = 0;
		}
	}

	player.mat = Math::Matrix::CreateTranslation(player.x, player.y, 0);
	back.mat = Math::Matrix::CreateTranslation(back.x, back.y, 0);

	//���C�v�p
	wipe.transMat = Math::Matrix::CreateTranslation(wipe.x, wipe.y, 0);
	wipe.scaleMat = Math::Matrix::CreateScale(wipe.scaleX, wipe.scaleY, 1);
	wipe.mat = wipe.scaleMat * wipe.transMat;
}

void Scene::Init()
{
	srand(time(0));

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

	//�m�C�Y
	sliceNum = 1;
	noiseLv = 0;
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
