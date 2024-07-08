#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	//�L�����\��
	SHADER.m_spriteShader.SetMatrix(charaMat);
	SHADER.m_spriteShader.DrawTex(&charaTex, Math::Rectangle{ 0,0,64,64 }, 1.0f);
	

}

void Scene::Update()
{
	////�}�E�X���W�擾
	//GetCursorPos(&mouse);	//�f�B�X�v���C��̃}�E�X���W���擾(�f�B�X�v���C�̍��オ(0,0))
	//
	////�w��̃E�B���h�E��̃}�E�X���W�ɕϊ�(�E�B���h�E�̍��オ(0,0))
	//ScreenToClient(APP.m_window.GetWndHandle(), &mouse);
	//
	////�}�E�X�̍��W�n�����s�E�B���h�E�̍��W�n(���S��(0,0))�ɕ␳
	//mouse.x -= 640;
	//mouse.y -= 360;
	//mouse.y *= -1;

	//���N���b�N�ŃJ�[�\������
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		ShowCursor(true);
	}

	//mouse = GetMousePos();
	GetMousePos2(&mouse);

	//�L�����̍��W���X�V
	charaX = mouse.x;
	charaY = mouse.y;

	charaMat = Math::Matrix::CreateTranslation(charaX, charaY, 0);
}

void Scene::Init()
{
	//�J�[�\��������(false:��\�� true:�\��)
	ShowCursor(false);

	// �摜�̓ǂݍ��ݏ���
	charaTex.Load("Texture/player.png");
	charaX = 0;
	charaY = 0;

	//�}�E�X
	mouse.x = 0;
	mouse.y = 0;

}

void Scene::Release()
{
	// �摜�̉������
	charaTex.Release();
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

POINT Scene::GetMousePos()
{
	POINT mousePos;

	GetCursorPos(&mousePos);
	ScreenToClient(APP.m_window.GetWndHandle(), &mousePos);
	mousePos.x -= 640;
	mousePos.y -= 360;
	mousePos.y *= -1;
	return mousePos;
}

void Scene::GetMousePos2(POINT* mousePos)
{
	GetCursorPos(mousePos);
	ScreenToClient(APP.m_window.GetWndHandle(), mousePos);
	mousePos->x -= 640;
	mousePos->y -= 360;
	mousePos->y *= -1;
}
