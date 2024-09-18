#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	////�w�i�@�����Ȃ�
	//D3D.SetBlendState(BlendMode::NoBlend);	//�F�̃u�����h���@�̐ݒ� NoBlend:�摜�����̂܂ܕ\������(�����x���ς��Ȃ�)

	////�w�i�@����������
	D3D.SetBlendState(BlendMode::Alpha);	//�F�̃u�����h���@�̐ݒ� Alpha:����������

	//�w�i
	SHADER.m_spriteShader.SetMatrix(backMat);
	SHADER.m_spriteShader.DrawTex(&backTex, Math::Rectangle{ 0,0,1280,720 }, 0.5f);

	if (addFlg) {
		////���z�@���Z����
		D3D.SetBlendState(BlendMode::Add);	//�F�̃u�����h���@�̐ݒ� Add:���Z����
	}

	//���z
	for (int s = 0; s < sunNum; s++) {
		if (sun[s].flg) {
			SHADER.m_spriteShader.SetMatrix(sun[s].mat);
			SHADER.m_spriteShader.DrawTex(&sunTex, Math::Rectangle{ 0,0,128,128 }, 1.0f);
		}
	}
}

void Scene::Update()
{
	//�}�E�X���W�擾
	GetCursorPos(&mousePos);
	ScreenToClient(APP.m_window.GetWndHandle(), &mousePos);
	mousePos.x -= 640;
	mousePos.y -= 360;
	mousePos.y *= -1;

	//�u�����h���[�h�ؑ�
	if (GetAsyncKeyState('A') & 0x8000) {
		addFlg = true;
	}
	if (GetAsyncKeyState('S') & 0x8000) {
		addFlg = false;
	}

	//�\��������
	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		sunNum++;
		if (sunNum >= SUN_MAX) {
			sunNum = SUN_MAX;
		}
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		sunNum--;
		if (sunNum <= 1) {
			sunNum = 1;
		}
	}

	//���[�h�؂芷��
	if (GetAsyncKeyState('Z') & 0x8000) {
		mode = 1;
		SunInit();
	}
	if (GetAsyncKeyState('X') & 0x8000) {
		mode = 2;
		SunInit();
	}
	if (GetAsyncKeyState('C') & 0x8000) {
		mode = 3;
		SunInit();
	}

	//==============================
	// �ǔ����[�h
	//==============================
	if (mode == 1) {
		float dec = 0;	//������

		//speed���ő�30�A�ŏ�3�ɂȂ�悤�Ȍ����ʂ����߂�i�ő�l�@-�@�ŏ��l�j/�i�v�f���@-�@1�j
		if (sunNum > 1) {
			dec = (float)27 / (sunNum - 1);
		}

		for (int s = 0; s < sunNum; s++) {
			//���̑��z�ق�speed�̒l������������
			//sun[s].speed = 30 - (s * 1);
			sun[s].speed = 30 - (s * dec);

			//�ړ��ʁi�}�E�X�Ɍ����킹�鏈���j
			float angle;
			if (s == 0) {
				angle = atan2(mousePos.y - sun[s].y, mousePos.x - sun[s].x);
			}
			else {
				angle = atan2(sun[s - 1].y - sun[s].y, sun[s - 1].x - sun[s].x);
			}

			sun[s].moveX = cos(angle) * sun[s].speed;
			sun[s].moveY = sin(angle) * sun[s].speed;

			sun[s].x += sun[s].moveX;
			sun[s].y += sun[s].moveY;
		}
	}

	//==============================
	// �X�}�z�Q�[�G�t�F�N�g��
	//==============================
	if (mode == 2) {
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
			//flg��false�̑��z��true��(1����)
			for (int s = 0; s < sunNum; s++) {
				if (!sun[s].flg) {
					sun[s].flg = true;
					sun[s].x = mousePos.x;
					sun[s].y = mousePos.y;
					sun[s].moveX = rand() % 2 * 2 - 1;
					sun[s].moveY = rand() % 2 * 2 - 1;
					sun[s].timer = rand() % (30 + 1) + 10;
					break;
				}
			}
			//flg��true�̑��z�͐������Ԃ�0�ɂȂ�܂ňړ��ʂ����Z����
			for (int s = 0; s < sunNum; s++) {
				if (sun[s].flg) {
					//�^�C�}�[���f�N�������g
					sun[s].timer--;
					if (sun[s].timer <= 0) {
						sun[s].flg = false;
					}
					//���W�Ɉړ��ʂ����Z
					sun[s].x += sun[s].moveX;
					sun[s].y += sun[s].moveY;
				}
			}
		}
		else {
			for (int s = 0; s < sunNum; s++) {
				sun[s].flg = false;
			}
		}
	}


	for (int s = 0; s < sunNum; s++) {
		sun[s].mat = Math::Matrix::CreateTranslation(sun[s].x, sun[s].y, 0);
	}
	
	backMat = Math::Matrix::CreateTranslation(0, 0, 0);
}

void Scene::SunInit()
{
	//���z
	for (int s = 0; s < SUN_MAX; s++) {
		sun[s].x = 0;
		sun[s].y = 0;
		sun[s].moveX = 0; 
		sun[s].moveY = 0;
		sun[s].speed = 0;	
		sun[s].flg = true;	
		sun[s].timer = 0;	
	}
	//���z�̏�����
	sunNum = 20;
}

void Scene::Init()
{
	srand(time(0));

	// �摜�̓ǂݍ��ݏ���
	backTex.Load("Texture/back.png");
	sunTex.Load("Texture/Sun.png");

	SunInit();

	mode = 1;
}

void Scene::Release()
{
	// �摜�̉������
	backTex.Release();
	sunTex.Release();
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
