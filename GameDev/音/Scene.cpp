#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	
}

void Scene::Update()
{
	//SE再生
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
		//音再生
		seInst->Play();//ループなし
	}

	//音量ダウン
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		//音量を1%ずつ下げる
		vol -= 0.01f;
		if (vol <= 0.0f) {
			vol = 0.0f;
		}
		seInst->SetVolume(vol);
		bgmInst->SetVolume(vol);
	}
	//音量アップ
	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		//音量を1%ずつ上げる
		vol += 0.01f;
		if (vol >= 1.0f) {
			vol = 1.0f;
		}
		seInst->SetVolume(vol);
		bgmInst->SetVolume(vol);
	}

	//Qキーで音を停止
	if (GetAsyncKeyState('Q') & 0x8000) {
		//IsPlay:音が鳴っているか?(true:鳴っている false:鳴ってない)
		if (bgmInst->IsPlay() == true) {
			bgmInst->Stop();	//Stop():音停止
		}
	}
	//Wキーで音を再生(最初から)
	if (GetAsyncKeyState('W') & 0x8000) {
		if (bgmInst->IsPlay() == false) {
			bgmInst->Play();	//最初から再生
		}
	}
	//Zキーで音を一時停止
	if (GetAsyncKeyState('Z') & 0x8000) {
		bgmInst->Pause();		//一時停止
	}
	//Xキーで音を再生(一時停止箇所から)
	if (GetAsyncKeyState('X') & 0x8000) {
		//IsPose:音が一時停止しているか?
		if (bgmInst->IsPause() == true) {
			bgmInst->Resume();	//止めたところから再生
		}
	}


}

void Scene::Init()
{
	//音量調整
	vol = 0.2f;	//MAX 1.0

	//================================
	// サウンド読み込み
	//================================

	//①メモリ領域の確保
	se = std::make_shared<KdSoundEffect>();

	//②音データの読み込み
	se->Load("Sound/Test.WAV");

	//③スピーカーにアドレスと音データをセット
	seInst = se->CreateInstance(false);
	seInst->SetVolume(vol);	//ボリュームをセッターで渡す

	//BGM
	bgm = std::make_shared<KdSoundEffect>();
	bgm->Load("Sound/BGM.WAV");
	bgmInst = bgm->CreateInstance(false);
	bgmInst->SetVolume(vol);
	bgmInst->Play(true);	//ループ再生

}

void Scene::Release()
{

}

void Scene::ImGuiUpdate()
{
	return;

	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_Once);

	// デバッグウィンドウ
	if (ImGui::Begin("Debug Window"))
	{
		ImGui::Text("FPS : %d", APP.m_fps);
	}
	ImGui::End();
}
