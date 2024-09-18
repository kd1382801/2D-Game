#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	
}

void Scene::Update()
{
	//SE�Đ�
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
		//���Đ�
		seInst->Play();//���[�v�Ȃ�
	}

	//���ʃ_�E��
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		//���ʂ�1%��������
		vol -= 0.01f;
		if (vol <= 0.0f) {
			vol = 0.0f;
		}
		seInst->SetVolume(vol);
		bgmInst->SetVolume(vol);
	}
	//���ʃA�b�v
	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		//���ʂ�1%���グ��
		vol += 0.01f;
		if (vol >= 1.0f) {
			vol = 1.0f;
		}
		seInst->SetVolume(vol);
		bgmInst->SetVolume(vol);
	}

	//Q�L�[�ŉ����~
	if (GetAsyncKeyState('Q') & 0x8000) {
		//IsPlay:�������Ă��邩?(true:���Ă��� false:���ĂȂ�)
		if (bgmInst->IsPlay() == true) {
			bgmInst->Stop();	//Stop():����~
		}
	}
	//W�L�[�ŉ����Đ�(�ŏ�����)
	if (GetAsyncKeyState('W') & 0x8000) {
		if (bgmInst->IsPlay() == false) {
			bgmInst->Play();	//�ŏ�����Đ�
		}
	}
	//Z�L�[�ŉ����ꎞ��~
	if (GetAsyncKeyState('Z') & 0x8000) {
		bgmInst->Pause();		//�ꎞ��~
	}
	//X�L�[�ŉ����Đ�(�ꎞ��~�ӏ�����)
	if (GetAsyncKeyState('X') & 0x8000) {
		//IsPose:�����ꎞ��~���Ă��邩?
		if (bgmInst->IsPause() == true) {
			bgmInst->Resume();	//�~�߂��Ƃ��납��Đ�
		}
	}


}

void Scene::Init()
{
	//���ʒ���
	vol = 0.2f;	//MAX 1.0

	//================================
	// �T�E���h�ǂݍ���
	//================================

	//�@�������̈�̊m��
	se = std::make_shared<KdSoundEffect>();

	//�A���f�[�^�̓ǂݍ���
	se->Load("Sound/Test.WAV");

	//�B�X�s�[�J�[�ɃA�h���X�Ɖ��f�[�^���Z�b�g
	seInst = se->CreateInstance(false);
	seInst->SetVolume(vol);	//�{�����[�����Z�b�^�[�œn��

	//BGM
	bgm = std::make_shared<KdSoundEffect>();
	bgm->Load("Sound/BGM.WAV");
	bgmInst = bgm->CreateInstance(false);
	bgmInst->SetVolume(vol);
	bgmInst->Play(true);	//���[�v�Đ�

}

void Scene::Release()
{

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
