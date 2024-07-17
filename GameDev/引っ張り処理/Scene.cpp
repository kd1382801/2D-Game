#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	//�L�����\��
	SHADER.m_spriteShader.SetMatrix(charaMat);
	SHADER.m_spriteShader.DrawTex(&charaTex, Math::Rectangle{ 0,0,64,64 }, 1.0f);
	
	//�G�\��
	SHADER.m_spriteShader.SetMatrix(enemyMat);
	SHADER.m_spriteShader.DrawTex(&enemyTex, Math::Rectangle{ 0,0,64,64 }, 1.0f);

}

void Scene::Update()
{
	//�}�E�X���W���擾
	mouse = GetMousePos();


	//���N���b�N
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		if (clickFlg == false)
		{
			clickFlg = true;
			ShowCursor(true);

			//�N���b�N�����u�Ԃ̃}�E�X���W��ۑ�
			clickPos = mouse;
			system("cls");//��ʃN���A
			printf("clickPos.x = %d\nclickPos.y = %d\n\n", clickPos.x, clickPos.y);
		}
		
	}
	else 
	{
		//�N���b�N����Ă��Ԃ��痣���ꂽ��
		if (clickFlg) 
		{
			//�������u�Ԃ̃}�E�X���W��ۑ�
			releasePos = mouse;
			printf("releasePos.x = %d\nreleasePos.y = %d\n\n", releasePos.x, releasePos.y);


			//�A�[�N�^���W�F���g���g�p���Ċp�x�����߂�  ��atan2(����, ���)
			angle = atan2(clickPos.y - releasePos.y, clickPos.x - releasePos.x);

			//�Z�o�����p�x�ɑ΂��Ċe���̈ړ��ʂ��m��
			moveX = cos(angle) * speed;	//cos�ŋ��߂������̒l*���x
			moveY = sin(angle) * speed;	//sin�ŋ��߂������̒l*���x
		}
		clickFlg = false;
	}

	//�o�E���h����(�㉺���E)
	if (charaY > HEIGHT / 2 - 32) {
		moveY *= -1;
		charaY = HEIGHT / 2 - 32;//��ʒ[�ŌŒ�
	}
	if (charaY < -HEIGHT / 2 + 32) {
		moveY *= -1;
		charaY = -HEIGHT / 2 + 32;
	}
	if (charaX < -WIDTH / 2 + 32) {
		moveX *= -1;
		charaX = -WIDTH / 2 + 32;
	}
	if (charaX > WIDTH / 2 - 32) {
		moveX *= -1;
		charaX = WIDTH / 2 - 32;
	}

	//�ړ��ʌ���
	moveX *= 0.98f;
	moveY *= 0.98f;


	//�G�ƃL�����̓����蔻��(�~����)
	float a = enemyX - charaX;
	float b = enemyY - charaY;
	float c = sqrt(a * a + b * b);
	if (c < 64)
	{
		//�����_���Ȉʒu�ɍďo��
		enemyX = rand() % (WIDTH - 64) + 1 - (WIDTH / 2 - 32);
		enemyY = rand() % (HEIGHT - 64) + 1 - (HEIGHT / 2 - 32);

	}

	//�L�������W�X�V
	charaX += moveX;
	charaY += moveY;

	charaMat = Math::Matrix::CreateTranslation(charaX, charaY, 0);
	enemyMat = Math::Matrix::CreateTranslation(enemyX, enemyY, 0);
}

void Scene::Init()
{
	//�R���\�[��
	AllocConsole();
	freopen_s(&fp_c, "CONOUT$", "w", stdout);

	srand(time(0));
	//�J�[�\��������(false:��\�� true:�\��)
	ShowCursor(false);

	// �摜�̓ǂݍ��ݏ���
	charaTex.Load("Texture/Chara.png");
	enemyTex.Load("Texture/enemy.png");

	//�L����
	charaX = 0;
	charaY = 0;
	moveX = 0;
	moveY = 0;
	angle = 0;
	speed = 100;

	//�G
	enemyX = 100;
	enemyY = 200;

	//�}�E�X
	mouse.x = 0;
	mouse.y = 0;
	clickPos = { 0,0 };//{x, y}
	releasePos = { 0,0 };
	clickFlg = false;

}

void Scene::Release()
{
	// �摜�̉������
	charaTex.Release();
	enemyTex.Release();
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
	mousePos.x -= WIDTH / 2;
	mousePos.y -= HEIGHT / 2;
	mousePos.y *= -1;
	return mousePos;
}

void Scene::GetMousePos2(POINT* mousePos)
{
	GetCursorPos(mousePos);
	ScreenToClient(APP.m_window.GetWndHandle(), mousePos);
	mousePos->x -= WIDTH / 2;
	mousePos->y -= HEIGHT / 2;
	mousePos->y *= -1;
}
