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

//�X�v���C�g�p�e�N�X�`������
//const int spriteSRVCount = 524;


// Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{


// WindowsAPI����������
	

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

	// DirectX�����������@�����܂�

	//�`�揉���������@��������
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


	//�����ǂݍ���
	audio->LoadWave("powerdown07.wav");



#pragma region model
	

	Object3d* object3d_camera = nullptr;


	Model* model4 = Model::LoadFromOBJ("ene-0");
	Model* model_taihou = Model::LoadFromOBJ("taihou");
	

	Object3d* object3d_ene = object3d_ene->Create();

	object3d_ene->SetModel(model4);

	object3d_ene->SetPosition({ -92,12,+60 });
	object3d_ene->scale.x = 3.8f;
	object3d_ene->scale.y = 3.8f;
	object3d_ene->scale.z = 3.8f;

	Object3d* taihou = taihou->Create();
	taihou->SetModel(model_taihou);
	taihou->rotation.y= 135;
	taihou->SetPosition({ -100,0,+50 });
	//�����蔻��@��
	Sphere sphere;

	//�����蔻��@�O�p�`
	Triangle triangle_ene2;

	//���̏����l��ݒ�
	sphere.center = XMVectorSet(0, -10, -5, 1);//���S�_���W
	sphere.radius = 10.0f;//���a

#pragma endregion
	//�`�揉���������@�����܂�



#pragma region �V�F�[�_�[�̐F,������


	float xmfloat4_r = 1.0f;
	float xmfloat4_b = 1.0f;
	float xmfloat4_g = 1.0f;
	float xmfloat4_a = 1.0f;


	char moji[64];
	char moji2[64];
	char moji3[64];

	

	int gameScene = 1;
	bool shootflag = false;

	float gravity = 9.8f / 60.0f;
	float accela_x = 10.0f;
	float accela_y = -3.0f;

	sprite->SetPosition({ 1040.0f,130.0f,0.0f });
	sprite->SetTexsize({440.0f,250.0f });

	sprite->TransVertexBuffer();
	
#pragma endregion


	while (true)  // �Q�[�����[�v
	{
		//// ���b�Z�[�W������H
		

		//// ?�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���

		if (win->ProcessMessage()) {
			break;
		}

		// DirectX���t���[�������@��������

		//�L�[�{�[�h���̎擾�J�n
		//�S�L�[�̓��͏�Ԃ��擾����
		input->Update();

		sprite->Update();

		
		if (gameScene == 1)
		{
//��������ۑ�̃v���O����
			if (input->isKeyTrigger(DIK_SPACE) && shootflag == false)
			{
				shootflag = true;//�����o���t���O
			}
			if (shootflag == true)
			{
				//�������ł���
				accela_x += gravity;
				accela_y += gravity;

				object3d_ene->position.x += accela_x;
				object3d_ene->position.y -= accela_y;
			}
			if (object3d_ene->position.x >= 1280.0f)
			{
				//����߂�
				object3d_ene->position.x = -92;
				object3d_ene->position.y = 12;
				accela_x = 10.0f;
				accela_y = -3.0f;
				shootflag = false;
			}
	
		}

		object3d_ene->Update();
		taihou->Update();
	
		dxcommon->BeginDraw();

		// �S�D�`��R�}���h��������


		Object3d::PreDraw(dxcommon->GetCmdlist());
		
	

		object3d_ene->Draw();
		taihou->Draw();


		Object3d::PostDraw();

		////�X�v���C�g���ʃR�}���h
		spriteCommon->PreDraw();

		////�X�v���C�g�`��
		//sprite->Draw();


		/*debugtext_minute->DrawAll();
		debugtext_minute2->DrawAll();*/
		// �S�D�`��R�}���h�����܂�
		// 
		// �T�D���\�[�X�o���A��߂�

		dxcommon->EndDraw();
		
	}
	// �E�B���h�E�N���X��o�^����
	win->WinFinalize();


	delete object3d_ene;
	delete taihou;


	//Audio���
	audio->Finalize();
	delete audio;

	//�f�o�b�O�e�L�X�g���
	debugtext_minute->Finalize();
	delete debugtext_minute;


	delete sprite;
	delete spriteCommon;
	delete dxcommon;
	delete input;
	
	
	
	return 0;
}