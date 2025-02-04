#pragma once
#include <EngineBase/EngineMath.h>
#include <EngineCore/EngineVertexBuffer.h>
#include <sstream>

#define GRAVITY_ACC 9.8f

enum class ECircuit
{
	LUIGI_RACEWAY = 0,
	KOOPA_TROOPA_BEACH,
	MARIO_RACEWAY,
	WARIO_STADIUM,
	SHERBET_LAND,
	ROYAL_RACEWAY,
	BOWSERS_CASTLE,
	RAINBOW_ROAD,
	END
};

enum class ECharacter
{
	NONE = -1,
	MARIO = 0,
	LUIGI,
	PEACH,
	YOSHI,
	WARIO,
	BOWSER,
	END
};

enum class ENavType
{
	NONE = 0,
	ROAD,
	START_POINT,
	BORDER,
	FLATE_FASTER,
	FLATE_JUMP,
	WALL,
	END
};

enum class EItemType
{
	NONE = -1,
	GREEN_SHELL = 0,
	GREEN_SHELLS,
	RED_SHELL,
	RED_SHELLS,
	MUSHROOM,
	MUSHROOM2,
	MUSHROOM3,
	GOLD_MUSHROOM,
	BANANA,
	BANANAS,
	STAR,
	THUNDER,
	GHOST,
	FAKE_ITEMBOX,
	BOWSER_SHELL,
	SIZE
};

struct SMapPackage
{
	int CurIdx = 0;
	ECircuit Maps[4] = { ECircuit::END, ECircuit::END, ECircuit::END, ECircuit::END };
};

struct TrainData
{
	FVector InitLoc = FVector::ZERO;
	FVector Forward = FVector::ZERO;
	float NormX = 0.f;
	float NormZ = 0.f;
	float RotY = 0.f;
	int IsReverse = 0;
	float Velocity = 0.f;
	int DirV = 0;	// 0: None, 1: Forward, 2: Backward
	int DirH = 0;	// 0: None, 1: Left, 2: Right

	std::string ToString()
	{
		std::stringstream ss;
		ss << "{\"initLoc\":[" << InitLoc.X << "," << InitLoc.Y << "],"
			<< "\"forward\":[" << Forward.X << "," << Forward.Y << "," << Forward.Z << "],"
			<< "\"normX\":" << NormX << ","
			<< "\"normZ\":" << NormZ << ","
			<< "\"rotY\":" << RotY << ","
			<< "\"isReverse\":" << IsReverse << ","
			<< "\"velocity\":" << Velocity << ","
			<< "\"dirV\":" << DirV << ","
			<< "\"dirH\":" << DirH << "}";
		return ss.str();
	}
};

struct SMapSizeInfo
{
	FVector Min = FVector{ 99999.f, 99999.f, 99999.f};
	FVector Max = FVector::ZERO;
	FVector InitLoc = FVector::ZERO;
	FVector Scale = FVector::ZERO;
};

struct SPlayerInfo
{
	ECharacter Chracter = ECharacter::NONE;
	EItemType Item = EItemType::NONE;
	std::string Name = "";
	float4 MapLoc = { 0.f, 0.f, 0.f, 0.f };
	float4 Rotation = { 0.f, 0.f, 0.f, 0.f };
	float Time = 0.f;
	int Lap = 1;
	int Rank = 0;

	static std::string GetName(ECharacter _character)
	{
		std::string name = "";
		switch (_character)
		{
		case ECharacter::MARIO:
			name = "MARIO";
			break;
		case ECharacter::LUIGI:
			name = "LUIGI";
			break;
		case ECharacter::PEACH:
			name = "PEACH";
			break;
		case ECharacter::YOSHI:
			name = "YOSHI";
			break;
		case ECharacter::WARIO:
			name = "WARIO";
			break;
		case ECharacter::BOWSER:
			name = "BOWSER";
			break;
		}

		return name;
	}
};

struct FColor
{
	float4 Albedo;
};

struct FDebug
{
	float4 Index;
};

struct SVertexToNavData
{
	ENavType FloorType = ENavType::NONE;
	std::vector<FEngineVertex> Vertecies;
};

struct SNavData
{
	float4 Vertex[3] = { {0.f, 0.f, 0.f, 1.f}, {0.f, 0.f, 0.f, 1.f}, {0.f, 0.f, 0.f, 1.f} };
	int GroupIndex = -1;
	int Index = -1;
	ENavType FloorType = ENavType::NONE;
	std::vector<int> LinkData;

	SNavData()
	{
		LinkData.reserve(1000);	// temp
	}

	bool Intersects(const FVector& _loc, const FVector& _dir, const FMatrix& _scaleMat, const FMatrix& _rotationMat, const FMatrix& _translationMat, float& _refDist) const
	{
		_refDist = 0.f;
		DirectX::XMVECTOR v1 = (Vertex[0] * _scaleMat * _rotationMat * _translationMat).DirectVector;
		DirectX::XMVECTOR v2 = (Vertex[1] * _scaleMat * _rotationMat * _translationMat).DirectVector;
		DirectX::XMVECTOR v3 = (Vertex[2] * _scaleMat * _rotationMat * _translationMat).DirectVector;
		return DirectX::TriangleTests::Intersects(_loc.DirectVector, _dir.DirectVector, v1, v2, v3, _refDist);
	}

	bool IsSameF(float4 _v1, float4 _v2, float _eps = 1e-4)
	{
		// different sign
		if (_v1.X * _v2.X < 0 || _v1.Y * _v2.Y < 0 || _v1.Z * _v2.Z < 0)
		{
			return false;
		}

		bool isSame = std::fabs(_v1.X - _v2.X) < _eps;
		isSame &= std::fabs(_v1.Y - _v2.Y) < _eps;
		isSame &= std::fabs(_v1.Z - _v2.Z) < _eps;
		isSame &= std::fabs(_v1.W - _v2.W) < _eps;
		return isSame;
	}

	bool IsAttached(const SNavData& _other, float _eps = 1e-4)
	{
		bool isSame = false;
		for (int i=0; i<3; ++i)
		{
			isSame |= IsSameF(Vertex[i], _other.Vertex[0], _eps);
			isSame |= IsSameF(Vertex[i], _other.Vertex[1], _eps);
			isSame |= IsSameF(Vertex[i], _other.Vertex[2], _eps);
			if (isSame) break;
		}

		return isSame;
	}

	// It won't be duplicated.
	void LinkBoth(SNavData& _other)
	{
		LinkData.push_back(_other.Index);

		_other.Link(*this);
	}

	void Link(const SNavData& _other)
	{
		LinkData.push_back(_other.Index);
	}
};

struct SMapInfo;
struct SRenderInfo
{
	std::string Name = "";	// Vertex buffer, Pixel buffer, Mesh
	std::string MatName = "";	// Material
	std::string TexName = "";	// Texture
	float Z = 0.f;

	static std::map<ECircuit, SMapInfo> MapInfos;
};

struct SMapInfo
{
	SMapSizeInfo MapSizeInfo;
	std::vector<SRenderInfo> RenderInfos;
	std::vector<SNavData> NavInfos;
};

struct FPhysics
{
	/*
	* Vi : Initial velocity
	* Vf : Final velocity
	* Dx : Delta X
	* Acc : Acceleration
	* Time : Time
	*/
	static float GetVf(float _vi, float _acc, float _t)
	{
		return _vi + _acc * _t;
	}

	static float GetDeltaX(float _vi, float _acc, float _t)
	{
		return (_vi * _t) + (.5f * _acc * (_t * _t));
	}
};

struct SItemRoulette
{
	const int ALL_ITEM_SIZE = static_cast<int>(EItemType::SIZE);
	const float ACC_PICK_ITEM = -5.f;
	float VelocityPick = 0.f;
	float Distance = 0.f;

	void Reset()
	{
		VelocityPick = 20.f;
		srand(static_cast<unsigned int>(time(nullptr)));
		Distance = static_cast<float>(rand() % ALL_ITEM_SIZE);
		//OutputDebugStringA(("Distance: " + std::to_string(Distance) + "\n").)
	}

	int PickItem(float _deltaTime)
	{
		float dx = FPhysics::GetDeltaX(VelocityPick, ACC_PICK_ITEM, _deltaTime);
		int idx = static_cast<int>(Distance) % ALL_ITEM_SIZE;

		VelocityPick = FPhysics::GetVf(VelocityPick, ACC_PICK_ITEM, _deltaTime);
		Distance += dx;

		if (VelocityPick < 0)
		{
			return -1;
		}

		return idx;
	}
};