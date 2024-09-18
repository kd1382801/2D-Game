#pragma once

class Scene
{
private:

	//SE用
	std::shared_ptr<KdSoundEffect>   se;		//音データ
	std::shared_ptr<KdSoundInstance> seInst;	//音を鳴らすスピーカー
	//			   型名              変数名
	//std::shared_ptr<型>:<>内の型をポインタで宣言 

	//BGM用
	std::shared_ptr<KdSoundEffect>	 bgm;
	std::shared_ptr<KdSoundInstance> bgmInst;

	//音量調整
	float vol;


public:

	// 初期設定
	void Init();

	// 解放
	void Release();

	// 更新処理
	void Update();

	// 描画処理
	void Draw2D();

	// GUI処理
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
