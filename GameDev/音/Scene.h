#pragma once

class Scene
{
private:

	//SE�p
	std::shared_ptr<KdSoundEffect>   se;		//���f�[�^
	std::shared_ptr<KdSoundInstance> seInst;	//����炷�X�s�[�J�[
	//			   �^��              �ϐ���
	//std::shared_ptr<�^>:<>���̌^���|�C���^�Ő錾 

	//BGM�p
	std::shared_ptr<KdSoundEffect>	 bgm;
	std::shared_ptr<KdSoundInstance> bgmInst;

	//���ʒ���
	float vol;


public:

	// �����ݒ�
	void Init();

	// ���
	void Release();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw2D();

	// GUI����
	void ImGuiUpdate();

private:

	Scene() {}

public:
	static Scene& GetInstance()
	{
		static Scene instance;
		return instance;
	}
};

#define SCENE Scene::GetInstance()
