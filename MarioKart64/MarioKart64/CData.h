#pragma once
#include <EngineBase/EngineMath.h>

#define GRAVITY_ACC 9.8f

struct FColor
{
	float4 Albedo;
};

struct NavData
{
	float4 Vertex[3] = { {0.f, 0.f, 0.f, 1.f}, {0.f, 0.f, 0.f, 1.f}, {0.f, 0.f, 0.f, 1.f} };
	int Index = -1;
	std::vector<int> LinkData;

	NavData()
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

	bool IsAttached(const NavData& _other, float _eps = 1e-4)
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
	void LinkBoth(NavData& _other)
	{
		LinkData.push_back(_other.Index);

		_other.Link(*this);
	}

	void Link(const NavData& _other)
	{
		LinkData.push_back(_other.Index);
	}
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