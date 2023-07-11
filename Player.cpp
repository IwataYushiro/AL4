#include "Player.h"
#include "Input.h"
#include "DebugText.h"
#include "SphereCollider.h"
#include "ParticleManager.h"
#include "DirectXCommon.h"

using namespace DirectX;

Player* Player::Create(Model* model)
{
	Player* ins = new Player();
	if(ins==nullptr)return nullptr;

	//������
	if (!ins->Initialize())
	{
		delete ins;
		assert(0);
	}
	if (model)ins->SetModel(model);
	
	return ins;
}

bool Player::Initialize()
{
	if (!Object3d::Initialize())return false;

	ParticleManager::GetInstance()->Initialize();
	ParticleManager::GetInstance()->Update();
	//�R���C�_�[�ǉ�
	float radius = 0.6f;
	//���a�������������畂�������W�����̒��S�ɂ���
	SetCollider(new SphereCollider(XMVECTOR({ 0.0f,radius,0.0f,0.0f }), radius));
	return true;
}

void Player::Update()
{
	Input* input = Input::GetInstance();
	//����
	if (input->PushKey(DIK_A))rotation.y -= 2.0f;
	else if (input->PushKey(DIK_D))rotation.y += 2.0f;

	//�ړ��x�N�g����Y�����̊p�x�ŉ�]
	XMVECTOR move = { 0.0f,0.0f,0.1f,0.0f };
	XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rotation.y));
	move = XMVector3TransformNormal(move, matRot);

	//�����Ă�������Ɉړ�
	if (input->PushKey(DIK_S))
	{
		position.x -= move.m128_f32[0];
		position.y -= move.m128_f32[1];
		position.z -= move.m128_f32[2];
	}
	else if (input->PushKey(DIK_W))
	{
		position.x += move.m128_f32[0];
		position.y += move.m128_f32[1];
		position.z += move.m128_f32[2];
	}
	//�X�V
	Object3d::Update();
	ParticleManager::GetInstance()->Update();
}

void Player::DrawParticle(ID3D12GraphicsCommandList* cmdList)
{
	//�G�t�F�N�g�`��O����
	ParticleManager::GetInstance()->PreDraw((cmdList));

	//�G�t�F�N�g�`��
	ParticleManager::GetInstance()->Draw();
	//�G�t�F�N�g�`��㏈��
	ParticleManager::PostDraw();
}

void Player::OnCollision(const CollisionInfo& info)
{
	//DebugText::GetInstance()->Print("Collision Hit", 400.0f, 0.0f, 2.0f);

	for (int i = 0; i < 2; i++)
	{
		const float md_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;

		ParticleManager::GetInstance()->Add(10,
			XMFLOAT3(info.inter.m128_f32), vel, XMFLOAT3(), 0.0f, 1.0f);
		
	}
	
}
