#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
//#include <DirectXMath.h>
#include <vector>
#include <string>
#include <math.h>
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include "Input.h"
#include <wrl.h> // 7-1
#include <d3dx12.h>//7-3
#include<xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include<fstream>

#include "Object3d.h"

#include "Win.h"
#include "DirectXCommon.h"
#include "CollisionPrimitive.h"
#include "Collision.h"

#include "Sprite.h"
#include "SpriteCommon.h"
#include <algorithm>

#include "DebugText.h"

#include "Audio.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
//#pragma comment(lib, "DirectXTex.lib")

using namespace Microsoft::WRL;

//#include <d2d1_1.h>
//#pragma comment(lib, "d2d1.lib")

//#include <dsound.h>
//#pragma comment(lib, "dsound.lib")
//#pragma comment(lib, "dxguid.lib")
//#pragma comment(lib, "winmm.lib")

using namespace DirectX;

//スプライト用テクスチャ枚数
//const int spriteSRVCount = 524;


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{


// WindowsAPI初期化処理
	

	Win* win = nullptr;
	Input* input = nullptr;
	DirectXCommon* dxcommon = nullptr;
	Audio* audio = nullptr;

	win = new Win();
	win->WinCreate();


	dxcommon = new DirectXCommon();
	dxcommon->Init(win);

	SpriteCommon* spriteCommon = new SpriteCommon;
	spriteCommon->Init(dxcommon->GetDev(), dxcommon->GetCmdlist(), win->window_width, win->window_height);

	DebugText* debugtext_minute = nullptr;
	debugtext_minute = new DebugText();

	Object3d::StaticInitialize(dxcommon->GetDev(), win->window_width, win->window_height);

	audio = new Audio();
	audio->Init();

	input = new Input();
	input->Initialize(win);

	// DirectX初期化処理　ここまで

	//描画初期化処理　ここから
	spriteCommon->LoadTexture(0, L"Resource/GE3_2.png");

	Sprite *sprite = Sprite::Create(spriteCommon,0);

	const int debugTextTexNumber3 = 4;

	spriteCommon->LoadTexture(debugTextTexNumber3, L"Resource/ASC_White.png");

	debugtext_minute->debugTextInit(spriteCommon, debugTextTexNumber3);

	DebugText* debugtext_minute2 = nullptr;
	debugtext_minute2 = new DebugText();

	const int debugTextTexNumber4 = 5;

	spriteCommon->LoadTexture(debugTextTexNumber4, L"Resource/ASC_White.png");
	debugtext_minute2->debugTextInit(spriteCommon, debugTextTexNumber4);


	//音声読み込み
	audio->LoadWave("powerdown07.wav");



#pragma region model
	

	Object3d* object3d_camera = nullptr;


	Model* model4 = Model::LoadFromOBJ("ene-0");
	Model* model_taihou = Model::LoadFromOBJ("taihou");
	

	Object3d* object3d_bullet = object3d_bullet->Create();

	object3d_bullet->SetModel(model4);

	object3d_bullet->SetPosition({ -92,12,+60 });
	object3d_bullet->scale.x = 3.8f;
	object3d_bullet->scale.y = 3.8f;
	object3d_bullet->scale.z = 3.8f;

	Object3d* object3d_freeFall = object3d_freeFall->Create();

	object3d_freeFall->SetModel(model4);
	object3d_freeFall->SetPosition({ -92,-50,+60 });

	Object3d* taihou = taihou->Create();
	taihou->SetModel(model_taihou);
	taihou->rotation.y= 135;
	taihou->SetPosition({ -100,0,+50 });
	//当たり判定　球
	Sphere sphere;

	//当たり判定　三角形
	Triangle triangle_ene2;

	//球の初期値を設定
	sphere.center = XMVectorSet(0, -10, -5, 1);//中心点座標
	sphere.radius = 10.0f;//半径

#pragma endregion
	//描画初期化処理　ここまで



#pragma region シェーダーの色,初期化


	float xmfloat4_r = 1.0f;
	float xmfloat4_b = 1.0f;
	float xmfloat4_g = 1.0f;
	float xmfloat4_a = 1.0f;


	char moji[64];
	char moji2[64];
	char moji3[64];

	

	int gameScene = 1;
	bool shootflag = false;

	//重力加速度
	float gravity = 9.8f / 60.0f;
	//加速度
	float accela_x = 5.0f;//大砲
	float accela_y = -3.0f;//大砲

	float vel_yB = 0.0f;
	
	//
	float vel_y = -accela_y;
	//
	float k = 1.0f;

	float resi = 0.0f;

	resi = (k * vel_y) / 60;
	

	bool ecall = false;

	float accelaM_x = 4.0f;//摩擦

	//動摩擦係数
	float moveFric_ = 0.04f;
	//動摩擦力
	float moveFric = 0.0f;
	//質量
	float weight = 1.0f;

	moveFric = moveFric_ * weight;

	sprite->SetPosition({ 1040.0f,130.0f,0.0f });
	sprite->SetTexsize({440.0f,250.0f });

	sprite->TransVertexBuffer();
	
#pragma endregion


	while (true)  // ゲームループ
	{
		//// メッセージがある？
		

		//// ?ボタンで終了メッセージが来たらゲームループを抜ける

		if (win->ProcessMessage()) {
			break;
		}

		// DirectX毎フレーム処理　ここから

		//キーボード情報の取得開始
		//全キーの入力状態を取得する
		input->Update();

		sprite->Update();

		
		if (gameScene == 1)
		{
//ここから課題			
			if (input->isKeyTrigger(DIK_SPACE) && shootflag == false)
			{
				shootflag = true;
			}
			if (shootflag == true)
			{
				//球が飛んでいく
				
				accela_y += gravity;//重力加速度の加算
				
				if (accela_y >= resi  && ecall == false)//落ち始めるときに抵抗力を計算始める
				{
					vel_y = -accela_y;
					resi = (k * vel_y) / 60;

					//accela_y -= accela_y + resi;//抵抗力を減算

					if (gravity <= resi )//重力加速度　と　抵抗力が同じなら　抵抗力を　減算させない
					{
						ecall = true;	
					}

				}
				if (ecall == true)
				{
					//accela_y += accela_y + resi;
				}
				vel_yB = accela_y;
				object3d_bullet->position.x += accela_x;
				object3d_bullet->position.y -= vel_yB;



				//摩擦
				accelaM_x -= moveFric;
				object3d_freeFall->position.x += accelaM_x;

				if (accelaM_x <= 0.0f + moveFric)
				{
					moveFric = 0;
					accelaM_x = 0;
				}
			}
			//if (object3d_bullet->position.x >= 1280.0f)
			//{
			//	//球を戻す
			//	object3d_bullet->position.x = -92;
			//	object3d_bullet->position.y = 12;
			//	accela_x = 10.0f;
			//	accela_y = -3.0f;
			//	shootflag = false;
			//}

	
		}

		object3d_freeFall->Update();
		object3d_bullet->Update();
		taihou->Update();
	
		dxcommon->BeginDraw();

		// ４．描画コマンドここから


		Object3d::PreDraw(dxcommon->GetCmdlist());
		
	
		object3d_freeFall->Draw();
		object3d_bullet->Draw();
		taihou->Draw();


		Object3d::PostDraw();

		////スプライト共通コマンド
		spriteCommon->PreDraw();

		////スプライト描画
		//sprite->Draw();


		/*debugtext_minute->DrawAll();
		debugtext_minute2->DrawAll();*/
		// ４．描画コマンドここまで
		// 
		// ５．リソースバリアを戻す

		dxcommon->EndDraw();
		
	}
	// ウィンドウクラスを登録解除
	win->WinFinalize();


	delete object3d_bullet;
	delete taihou;
	delete object3d_freeFall;

	//Audio解放
	audio->Finalize();
	delete audio;

	//デバッグテキスト解放
	debugtext_minute->Finalize();
	delete debugtext_minute;


	delete sprite;
	delete spriteCommon;
	delete dxcommon;
	delete input;
	
	
	
	return 0;
}