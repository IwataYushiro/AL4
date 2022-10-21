#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

class Model
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X
	// ���_�f�[�^�\����
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT3 normal; // �@���x�N�g��
		XMFLOAT2 uv;  // uv���W
	};

	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferDataB0
	{
		//XMFLOAT4 color;	// �F (RGBA)
		XMMATRIX mat;	// �R�c�ϊ��s��
	};
	struct ConstBufferDataB1
	{
		XMFLOAT3 ambient;	// �A���r�G���g�W��
		float pad1;			// �p�f�B���O
		XMFLOAT3 diffuse;	// �f�B�t���[�Y�W��
		float pad2;			// �p�f�B���O
		XMFLOAT3 specular;	// �X�y�L�����[�W��
		float alpha;		// �A���t�@�l
	};
	//�}�e���A��
	struct Material
	{
		std::string name;				//�}�e���A����
		XMFLOAT3 ambient;				//�A���r�G���g�e���x
		XMFLOAT3 diffuse;				//�f�B�t���[�Y�e���x
		XMFLOAT3 specular;				//�X�y�L�����[�e���x
		float alpha;					//�A���t�@�l
		std::string textureFilename;	//�e�N�X�`���t�@�C����

		//�R���X�g���N�^
		Material() {
			ambient = { 0.3f,0.3f,0.3f };
			diffuse = { 0.0f,0.0f,0.0f };
			specular = { 0.0f,0.0f,0.0f };
			alpha = 1.0f;
		}
	};

private: // �萔
	static const int division = 50;					// ������
	static const float radius;				// ��ʂ̔��a
	static const float prizmHeight;			// ���̍���
	static const int planeCount = division * 2 + division * 2;		// �ʂ̐�
	static const int vertexCount = planeCount * 3;		// ���_��

public:
	Model();
	~Model();

private:
	// ���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	// �C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;
	// �e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texbuff;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// �V�F�[�_���\�[�X�r���[�̃n���h��(GPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	// ���_�o�b�t�@�r���[
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	// �C���f�b�N�X�o�b�t�@�r���[
	static D3D12_INDEX_BUFFER_VIEW ibView;
	// ���_�f�[�^�z��
	static std::vector<VertexPosNormalUv> vertices;
	// ���_�C���f�b�N�X�z��
	static std::vector<unsigned short> indices;
	//�}�e���A��
	static Material material;
};

Model::Model()
{
}

Model::~Model()
{
}