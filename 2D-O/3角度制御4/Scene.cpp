#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	DrawPlayer();
	//DrawSight();
	DrawBullet();
	DrawEnemy();

	//文字列はテクスチャなどを描画した後に書くこと
	// 文字列表示
	SHADER.m_spriteShader.DrawString(230, 350, "角度制御３", Math::Vector4(0, 0, 0, 1));
	SHADER.m_spriteShader.DrawString(230+8, 350-5, "角度制御３", Math::Vector4(1, 1, 0, frame / 180.0f));

	char str[50];
	sprintf_s(str, sizeof(str), "deg:%.2f", deg);
	SHADER.m_spriteShader.DrawString(240, 300, str, Math::Vector4(0, 1, 0, 1));
	sprintf_s(str, sizeof(str), "c:%.2f", c);
	SHADER.m_spriteShader.DrawString(240, 260, str, Math::Vector4(0, 1, 0, 1));

}

void Scene::Update()
{
	UpdatePlayer();
	UpdateSight();
	UpdateBullet();
	UpdateEnemy();
	
	//フレーム数を増やす
	frame++;
}

void Scene::Init()
{
	srand(timeGetTime());

	// 画像の読み込み処理
	player.m_texture.Load("Texture/smile_transparent.png");
	sight.m_texture.Load("Texture/sight_transparent.png");
	charaTex.Load("Texture/bullet_transparent.png");
	enemy.m_texture.Load("Texture/smile_transparent.png");

	//読み込んだテクスチャ(charaTex)のアドレスをm_pTextureへ入れる
	for (int b = 0; b < bulletNum; b++) {
		bullet[b].m_pTexture = &charaTex;
	}

	InitPlayer();
	InitSight();
	InitBullet();
	InitEnemy();

	debugFlg = false;
	keyCount = 0;
	frame = 0;
}

void Scene::Release()
{
	// 画像の解放処理
	player.m_texture.Release();
	sight.m_texture.Release();
	charaTex.Release();
	enemy.m_texture.Release();
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

void Scene::InitPlayer()
{
	//座標の初期化
	player.m_posX = 0;
	player.m_posY = 0;

}

void Scene::UpdatePlayer()
{
	//座標更新
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		player.m_posX += 8;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		player.m_posX -= 8;
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		player.m_posY += 8;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		player.m_posY -= 8;
	}

	//ZとXでdegを増減する
	if (GetAsyncKeyState('Z') & 0x8000) 
	{
		deg += 2;
	}
	if (GetAsyncKeyState('X') & 0x8000) 
	{
		deg -= 2;
	}

	//AとSでcを増減する
	if (GetAsyncKeyState('A') & 0x8000) {
		c -= 2;
	}
	if (GetAsyncKeyState('S') & 0x8000) {
		c += 2;
	}

	keyCount++;
	if (keyCount >= shotInterval) {
		keyCount = 0;
		//スペースキーで弾発射
		if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
			ShotBullet();
		}

		//左右弾
		if (GetAsyncKeyState('Q') & 0x8000) {
			ShotBulletDS(180, 15);
			ShotBulletDS(0, 15);
		}

		//3WAYショット
		if (GetAsyncKeyState('W') & 0x8000) {
			ShotBulletDS(90, 15);
			ShotBulletDS(60, 15);
			ShotBulletDS(120, 15);
		}

		//8方向ショット
		if (GetAsyncKeyState('E') & 0x8000) {
			for (int d = 0; d < 360; d += 45) {
				ShotBulletDS(d, 10);
			}
		}

		//ランダムショット
		if (GetAsyncKeyState('R') & 0x8000) {
			ShotBulletDS(rand() % 360, 15);
		}

		//オリジナルショット
		if (GetAsyncKeyState('T') & 0x8000) {
			ShotBulletDS(45, 10);
			ShotBulletDS(135, 10);
			ShotBulletDS(225, 10);
			ShotBulletDS(315, 10);
		}

		//Dキーでデバッグモード切り換え(トグル動作)
		if (GetAsyncKeyState('D') & 0x8000) {
			debugFlg = !debugFlg;
			//敵をワープさせる
			enemy.m_posX = -500;
			enemy.m_matrix = Math::Matrix::CreateTranslation(enemy.m_posX, enemy.m_posY, 0);
		}
	}


	//行列の作成
	player.m_matrix = Math::Matrix::CreateTranslation(player.m_posX, player.m_posY, 0);

}

void Scene::DrawPlayer()
{
	//行列をセット
	SHADER.m_spriteShader.SetMatrix(player.m_matrix);

	//描画
	Math::Rectangle srcRect{ 0,0,64,64 };			//テクスチャ座標
	Math::Color color = { 1.0f,1.0f,1.0f,1.0f };	//色RGBA
	SHADER.m_spriteShader.DrawTex(&player.m_texture, 0, 0, &srcRect, &color);
}

void Scene::InitSight()
{
	sight.m_posX = 0;
	sight.m_posY = 0;

	c = 200;
	deg = 30;
}

void Scene::UpdateSight()
{

	//deg += 1;

	a = cos(DirectX::XMConvertToRadians(deg)) * c;
	b = sin(DirectX::XMConvertToRadians(deg)) * c;

	sight.m_posX = player.m_posX + a;
	sight.m_posY = player.m_posY + b;

	sight.m_matrix = Math::Matrix::CreateTranslation(sight.m_posX, sight.m_posY, 0);
}

void Scene::DrawSight()
{
	SHADER.m_spriteShader.SetMatrix(sight.m_matrix);

	Math::Rectangle srcRect{ 0,0,32,32 };			//テクスチャ座標
	Math::Color color = { 1.0f,1.0f,1.0f,1.0f };	//色RGBA
	SHADER.m_spriteShader.DrawTex(&sight.m_texture, 0, 0, &srcRect, &color);
}

void Scene::InitEnemy()
{
	enemy.m_posX = 0;
	enemy.m_posY = 300;
	enemy.m_moveX = 4;
	enemy.m_moveY = 0;
	enemy.m_radius = 32;
}

void Scene::UpdateEnemy()
{
	if (debugFlg) {
		return;
	}

	enemy.m_posX += enemy.m_moveX;
	enemy.m_posY += enemy.m_moveY;

	if (enemy.m_posX < SCREEN_LEFT + enemy.m_radius || enemy.m_posX > SCREEN_RIGHT - enemy.m_radius) {
		enemy.m_moveX *= -1;
	}

	enemy.m_matrix = Math::Matrix::CreateTranslation(enemy.m_posX, enemy.m_posY, 0);
}

void Scene::DrawEnemy()
{
	SHADER.m_spriteShader.SetMatrix(enemy.m_matrix);
	Math::Rectangle srcRect = { 0,0,64,64 };
	Math::Color color = { 1.0f,0.0f,0.0f,0.5f };
	SHADER.m_spriteShader.DrawTex(&enemy.m_texture,0,0, &srcRect, &color);
}

void Scene::InitBullet()
{
	for (int b = 0; b < bulletNum; b++) {
		bullet[b].m_posX = 0;
		bullet[b].m_posY = 0;
		bullet[b].m_bActive = false;
	}
}

void Scene::UpdateBullet()
{
	for (int b = 0; b < bulletNum; b++) {
		if (bullet[b].m_bActive) {

			bullet[b].m_count++;
			if (bullet[b].m_count > 120) {
				bullet[b].m_bHoming = false;
			}

			////弾の動きにストーリー性をを持たせない場合
			//if (bullet[b].m_count < 20) {
			//	bullet[b].m_bHoming = false;
			//}
			//else if (bullet[b].m_count < 100) {
			//	bullet[b].m_moveX = 0;
			//	bullet[b].m_moveY = 0;
			//}
			//else if (bullet[b].m_count == 180) {
			//	bullet[b].m_bHoming = true;
			//}

			//ホーミングフラグがtrueのとき
			if (bullet[b].m_bHoming) {
				//角度を求める
				deg = GetAngleDeg(bullet[b].m_posX, bullet[b].m_posY, enemy.m_posX, enemy.m_posY);
				//移動量を再計算する
				bullet[b].m_moveX = cos(DirectX::XMConvertToRadians(deg)) * bullet[b].m_speed;
				bullet[b].m_moveY = sin(DirectX::XMConvertToRadians(deg)) * bullet[b].m_speed;
			}

			//座標更新
			bullet[b].m_count = 0;
			bullet[b].m_posX += bullet[b].m_moveX;
			bullet[b].m_posY += bullet[b].m_moveY;

			//弾と敵の当たり判定
			if (GetDistance(bullet[b].m_posX, bullet[b].m_posY, enemy.m_posX, enemy.m_posY) < enemy.m_radius) {
				bullet[b].m_bActive = false;
			}

			//画面端判定
			if (bullet[b].m_posX < SCREEN_LEFT ||
			    bullet[b].m_posX > SCREEN_RIGHT ||
			    bullet[b].m_posY > SCREEN_TOP ||
			    bullet[b].m_posY < SCREEN_BOTTOM) {
				bullet[b].m_bActive = false;
			}
			bullet[b].m_matrix = Math::Matrix::CreateTranslation(bullet[b].m_posX, bullet[b].m_posY, 0);
		}
	}
}

void Scene::DrawBullet()
{
	for (int b = 0; b < bulletNum; b++) {
		if (bullet[b].m_bActive) {
			SHADER.m_spriteShader.SetMatrix(bullet[b].m_matrix);

			SHADER.m_spriteShader.DrawTex(bullet[b].m_pTexture, 0, 0, &bullet[b].m_srcRect, &bullet[b].m_color);
		}
	}
}

void Scene::ShotBullet()
{
	//弾フラグの空きを探索する
	for (int b = 0; b < bulletNum; b++) {
		if (!bullet[b].m_bActive) {
			bullet[b].m_posX = player.m_posX;
			bullet[b].m_posY = player.m_posY;

			/*bullet[b].m_moveX = rand() % 11 - 5;
			bullet[b].m_moveY = rand() % 11 - 5;*/

			bullet[b].m_speed = 10.0f;

			deg = GetAngleDeg(player.m_posX, player.m_posY, enemy.m_posX, enemy.m_posY);

			bullet[b].m_moveX = cos(DirectX::XMConvertToRadians(deg)) * bullet[b].m_speed;
			bullet[b].m_moveY = sin(DirectX::XMConvertToRadians(deg)) * bullet[b].m_speed;

			bullet[b].m_srcRect = { 0,0,32,32 };
			bullet[b].m_color = { rand() / (float)RAND_MAX + 0.5f,rand() / 32767.0f + 0.5f,rand() / 32767.0f + 0.5f,1 };
			
			bullet[b].m_bHoming = true;
			bullet[b].m_bActive = true;
			break;
		}
	}
}

void Scene::ShotBulletDS(float a_degree, float a_speed)
{
	//弾フラグの空きを探索する
	for (int b = 0; b < bulletNum; b++) {
		if (!bullet[b].m_bActive) {
			bullet[b].m_posX = player.m_posX;
			bullet[b].m_posY = player.m_posY;

			bullet[b].m_speed = a_speed;
			bullet[b].m_moveX = cos(DirectX::XMConvertToRadians(a_degree)) * bullet[b].m_speed;
			bullet[b].m_moveY = sin(DirectX::XMConvertToRadians(a_degree)) * bullet[b].m_speed;

			bullet[b].m_srcRect = { 0,0,32,32 };
			bullet[b].m_color = { rand() / (float)RAND_MAX + 0.2f,rand() / 32767.0f + 0.2f,1,1 };

			bullet[b].m_count = 0;
			bullet[b].m_bHoming = false;
			bullet[b].m_bActive = true;
			break;
		}
	}
}

float Scene::GetAngleRad(float srcX, float srcY, float destX, float destY)
{
	float a, b, rad;

	a = destX - srcX;
	b = destY - srcY;
	rad = atan2(b, a);
	return rad;
}

float Scene::GetAngleDeg(float srcX, float srcY, float destX, float destY)
{
	float a, b, rad, deg;

	a = destX - srcX;
	b = destY - srcY;
	rad = atan2(b, a);
	//deg = rad * 180 / 3.141592;
	deg = DirectX::XMConvertToDegrees(rad);

	//0~359度内に補正する
	if (deg < 0) {
		deg += 360;
	}

	return deg;
}

float Scene::GetDistance(float srcX, float srcY, float destX, float destY)
{
	float a, b, c;

	a = destX - srcX;
	b = destY - srcY;
	c = sqrt(a * a + b * b);
	return c;
}
