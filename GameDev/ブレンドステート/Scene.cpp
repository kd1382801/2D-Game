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
		SHADER.m_spriteShader.SetMatrix(sun[s].mat);
		SHADER.m_spriteShader.DrawTex(&sunTex, Math::Rectangle{ 0,0,128,128 }, 1.0f);
	}
}

void Scene::Update()
{
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

	for (int s = 0; s < sunNum; s++) {
		sun[s].x += sun[s].moveX;
		sun[s].y += sun[s].moveY;

		//��ʒ[����
		if (sun[s].x <=  (-640 + 64)) {
			sun[s].x = (-640 + 64);
			sun[s].moveX *= -1;
		}
		if (sun[s].x >= (640 - 64)) {
			sun[s].x = (640 - 64);
			sun[s].moveX *= -1;
		}
		if (sun[s].y <= (-360 + 64)) {
			sun[s].y = (-360 + 64);
			sun[s].moveY *= -1;
		}
		if (sun[s].y >= (360 - 64)) {
			sun[s].y = (360 - 64);
			sun[s].moveY *= -1;
		}
		
		sun[s].mat = Math::Matrix::CreateTranslation(sun[s].x, sun[s].y, 0);
	}

	backMat = Math::Matrix::CreateTranslation(0, 0, 0);
}

void Scene::Init()
{
	srand(time(0));

	// �摜�̓ǂݍ��ݏ���
	backTex.Load("Texture/back.png");
	sunTex.Load("Texture/Sun.png");

	//���z
	for (int s = 0; s < SUN_MAX; s++) {
		sun[s].x = rand() % (1280 - 128 + 1) - (640 - 64);
		sun[s].y = rand() % (720 - 128 + 1) - (360 - 64);
		sun[s].moveX = rand() % 2 * 10 - 5;	//2�̂��܂��0��1 �� 0*10=0 �� 1*10=10 
		sun[s].moveY = rand() % 2 * 10 - 5;	//��0-5=-5 �� 10-5=5�����łȂ�
	}
	//���z�̏�����
	sunNum = 1;
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
