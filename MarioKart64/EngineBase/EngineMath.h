#pragma once
#include <Windows.h>
#include <string>
#include <functional>

#include <DirectXMath.h>
#include <DirectXCollision.h>

#include "EngineDefine.h"

class ENGINEAPI UEngineMath
{
public:
	static inline const double DPI = 3.14159265358979323846264338327950288419716939937510;
	static inline const double DPI2 = DPI * 2.0;

	static inline const float PI = 3.14159265358979323846264f;
	static inline const float PI2 = PI * 2.0f;

	static inline const float D2R = UEngineMath::PI / 180.0f;
	static inline const float R2D = 180.0f / UEngineMath::PI;

	static float Sqrt(float _Value)
	{
		return ::sqrtf(_Value);
	}

	template <typename DataType>
	DataType ClampMax(DataType value, DataType maxValue)
	{
		return (value > maxValue) ? maxValue : value;
	}

	template <typename DataType>
	DataType ClampMin(DataType value, DataType minValue)
	{
		return (value < minValue) ? minValue : value;
	}

	template <typename DataType>
	static DataType Clamp(DataType value, DataType minValue, DataType maxValue)
	{
		if (value < minValue)
			return minValue;
		else if (value > maxValue)
			return maxValue;
		else
			return value;
	}

	template <typename DataType>
	static DataType Lerp(DataType A, DataType B, DataType Alpha)
	{
		return A * (1 - Alpha) + B * Alpha;
	}
};

template<typename ValueType>
class TVector
{
public:
	static const TVector NONE;
	static const TVector ZERO;
	static const TVector LEFT;
	static const TVector RIGHT;
	static const TVector UP;
	static const TVector DOWN;
	static const TVector FORWARD;
	static const TVector BACK;

	static const TVector WHITE;
	static const TVector BLACK;
	static const TVector RED;
	static const TVector BLUE;
	static const TVector GREEN;


public:
	union
	{
		struct
		{
			ValueType X;
			ValueType Y;
			ValueType Z;
			ValueType W;
		};

		ValueType Arr2D[1][4];
		ValueType Arr1D[4];
		DirectX::XMFLOAT3 DirectFloat3;
		DirectX::XMFLOAT4 DirectFloat4;
		DirectX::XMVECTOR DirectVector;
	};


	ENGINEAPI TVector()
		: X(0.0f), Y(0.0f), Z(0.0f), W(1.0f)
	{

	}

	ENGINEAPI TVector(DirectX::XMVECTOR _DirectVector) : DirectVector(_DirectVector)
	{

	}


	ENGINEAPI TVector(float _X, float _Y) : X(_X), Y(_Y), Z(0.0f), W(1.0f)
	{

	}

	ENGINEAPI TVector(float _X, float _Y, float _Z) : X(_X), Y(_Y), Z(_Z), W(1.0f)
	{

	}

	TVector(float _X, float _Y, float _Z, float _W) : X(_X), Y(_Y), Z(_Z), W(_W)
	{

	}


	TVector(int _X, int _Y) : X(static_cast<float>(_X)), Y(static_cast<float>(_Y)), Z(0.0f), W(1.0f)
	{

	}

	TVector(long _X, long _Y) : X(static_cast<float>(_X)), Y(static_cast<float>(_Y)), Z(0.0f), W(1.0f)
	{

	}

	static float GetVectorAngleDeg(const TVector& _Left, const TVector& _Right)
	{
		return GetVectorAngleRad(_Left, _Right) * UEngineMath::R2D;
	}

	static float GetVectorAngleRad(const TVector& _Left, const TVector& _Right)
	{
		TVector LCopy = _Left;
		TVector RCopy = _Right;
		LCopy.Normalize();
		RCopy.Normalize();

		float CosRad = Dot(LCopy, RCopy);

		return acos(CosRad);
	}

	static TVector Cross(const TVector& _Left, const TVector& _Right)
	{
		TVector Result;
		Result.X = _Left.Y * _Right.Z - _Left.Z * _Right.Y;
		Result.Y = _Left.Z * _Right.X - _Left.X * _Right.Z;
		Result.Z = _Left.X * _Right.Y - _Left.Y * _Right.X;
		return Result;
	}

	static float Dot(const TVector& _Left, const TVector& _Right)
	{
		float LeftLen = _Left.Length();
		float RightLen = _Right.Length();

		return _Left.X * _Right.X + _Left.Y * _Right.Y + _Left.Z * _Right.Z;
	}

	static TVector Normalize(TVector _Value)
	{
		_Value.Normalize();
		return _Value;
	}

	static TVector AngleToVectorDeg(float _Angle)
	{
		return AngleToVectorRad(_Angle * UEngineMath::D2R);
	}


	static TVector Lerp(TVector _A, TVector _B, float _Alpha)
	{
		TVector Result;
		_Alpha = UEngineMath::Clamp(_Alpha, 0.0f, 1.0f);
		Result.X = UEngineMath::Lerp(_A.X, _B.X, _Alpha);
		Result.Y = UEngineMath::Lerp(_A.Y, _B.Y, _Alpha);
		return Result;
	}

	static TVector AngleToVectorRad(float _Angle)
	{
		return { cosf(_Angle), sinf(_Angle) };
	}

	static TVector Transform(const TVector& _Vector, const class FMatrix& _Matrix);

	// with translation
	static TVector TransformCoord(const TVector& _Vector, const class FMatrix& _Matrix);

	// without translation
	static TVector TransformNormal(const TVector& _Vector, const class FMatrix& _Matrix);

	int iX() const
	{
		return static_cast<int>(X);
	}

	int iY() const
	{
		return static_cast<int>(Y);
	}

	int iZ() const
	{
		return static_cast<int>(Z);
	}

	float hX() const
	{
		return X * 0.5f;
	}

	float hY() const
	{
		return Y * 0.5f;
	}

	bool IsZeroed() const
	{
		return X == 0.0f || Y == 0.0f || Z == 0.0f;
	}

	TVector Half() const
	{
		return { X * 0.5f, Y * 0.5f };
	}

	float Length() const
	{
		return UEngineMath::Sqrt(X * X + Y * Y + Z * Z);
	}

	POINT ConvertWindowPOINT()
	{
		return { iX(), iY() };
	}

	ENGINEAPI class FIntPoint ConvertToPoint() const;

	void Normalize()
	{
		float Len = Length();
		if (0.0f < Len && false == isnan(Len))
		{
			X = X / Len;
			Y = Y / Len;
			Z = Z / Len;
		}
	}

	TVector NormalizeReturn() const
	{
		TVector Result = *this;
		Result.Normalize();
		return Result;
	}

	void RotationXDeg(float _Angle)
	{
		RotationXRad(_Angle * UEngineMath::D2R);
	}

	void RotationXRad(float _Angle)
	{
		TVector Copy = *this;
		Z = (Copy.Z * cosf(_Angle)) - (Copy.Y * sinf(_Angle));
		Y = (Copy.Z * sinf(_Angle)) + (Copy.Y * cosf(_Angle));
	}

	TVector RotationXDegReturn(float _Angle)
	{
		return RotationXRadReturn(_Angle * UEngineMath::D2R);
	}

	TVector RotationXRadReturn(float _Angle)
	{
		TVector Result = *this;
		Result.Z = (Z * cosf(_Angle)) - (Y * sinf(_Angle));
		Result.Y = (Z * sinf(_Angle)) + (Y * cosf(_Angle));
		return Result;
	}

	void RotationYDeg(float _Angle)
	{
		RotationYRad(_Angle * UEngineMath::D2R);
	}

	void RotationYRad(float _Angle)
	{
		TVector Copy = *this;
		X = (Copy.X * cosf(_Angle)) - (Copy.Z * sinf(_Angle));
		Z = (Copy.X * sinf(_Angle)) + (Copy.Z * cosf(_Angle));
	}

	TVector RotationYDegReturn(float _Angle)
	{
		return RotationYRadReturn(_Angle * UEngineMath::D2R);
	}

	TVector RotationYRadReturn(float _Angle)
	{
		TVector Result = *this;
		Result.X = (X * cosf(_Angle)) - (Z * sinf(_Angle));
		Result.Z = (X * sinf(_Angle)) + (Z * cosf(_Angle));
		return Result;
	}

	TVector ABSVectorReturn()
	{
		return DirectX::XMVectorAbs(DirectVector);
	}

	void RotationZDeg(float _Angle)
	{
		RotationZRad(_Angle * UEngineMath::D2R);
	}

	void RotationZRad(float _Angle)
	{
		TVector Copy = *this;
		X = (Copy.X * cosf(_Angle)) - (Copy.Y * sinf(_Angle));
		Y = (Copy.X * sinf(_Angle)) + (Copy.Y * cosf(_Angle));
	}

	TVector RotationZDegReturn(float _Angle)
	{
		return RotationZRadReturn(_Angle * UEngineMath::D2R);
	}

	TVector RotationZRadReturn(float _Angle)
	{
		TVector Result = *this;
		Result.X = (X * cosf(_Angle)) - (Y * sinf(_Angle));
		Result.Y = (X * sinf(_Angle)) + (Y * cosf(_Angle));
		return Result;
	}

	float Dot(const TVector& other) const
	{
		return X * other.X + Y * other.Y;
	}

	TVector operator*(float _Value) const
	{
		TVector Result;
		Result.X = X * _Value;
		Result.Y = Y * _Value;
		Result.Z = Z * _Value;
		return Result;
	}

	TVector operator+(const TVector& _Other) const
	{
		TVector Result;
		Result.X = X + _Other.X;
		Result.Y = Y + _Other.Y;
		Result.Z = Z + _Other.Z;
		return Result;
	}

	TVector& operator+=(const TVector& _Other) const
	{
		X += _Other.X;
		Y += _Other.Y;
		Z += _Other.Z;
		return *this;
	}

	ENGINEAPI TVector operator*(const class FMatrix& _Matrix) const;
	ENGINEAPI TVector& operator*=(const class FMatrix& _Matrix);

	TVector& operator-=(const TVector& _Other)
	{
		X -= _Other.X;
		Y -= _Other.Y;
		Z -= _Other.Z;
		return *this;
	}


	TVector operator-(const TVector& _Other) const
	{
		TVector Result;
		Result.X = X - _Other.X;
		Result.Y = Y - _Other.Y;
		Result.Z = Z - _Other.Z;
		return Result;
	}

	TVector operator-() const
	{
		TVector Result;
		Result.X = -X;
		Result.Y = -Y;
		Result.Z = -Z;
		return Result;
	}

	TVector operator/(int _Value) const
	{
		TVector Result;
		Result.X = X / _Value;
		Result.Y = Y / _Value;
		Result.Z = Z / _Value;
		return Result;
	}

	TVector operator/(const TVector& Other) const
	{
		TVector Result;
		Result.X = X / Other.X;
		Result.Y = Y / Other.Y;
		Result.Z = Z / Other.Z;
		return Result;
	}

	bool operator==(const TVector& _Other) const
	{
		return X == _Other.X && Y == _Other.Y && Z == _Other.Z;
	}

	bool EqualToInt(TVector _Other) const
	{
		return iX() == _Other.iX() && iY() == _Other.iY() && iZ() == _Other.iZ();
	}

	TVector& operator+=(const TVector& _Other)
	{
		X += _Other.X;
		Y += _Other.Y;
		Z += _Other.Z;
		return *this;
	}

	TVector& operator*=(const TVector& _Other)
	{
		X *= _Other.X;
		Y *= _Other.Y;
		Z *= _Other.Z;
		return *this;
	}

	TVector& operator/=(const TVector& _Other)
	{
		X /= _Other.X;
		Y /= _Other.Y;
		Z /= _Other.Z;
		return *this;
	}

	TVector& operator*=(float _Other)
	{
		X *= _Other;
		Y *= _Other;
		Z *= _Other;
		return *this;
	}

	std::string ToString()
	{
		std::string Stream;

		Stream += "X : [";
		Stream += std::to_string(X);
		Stream += "] Y : [";
		Stream += std::to_string(Y);
		Stream += "] Z : [";
		Stream += std::to_string(Z);
		Stream += "] W : [";
		Stream += std::to_string(W);
		Stream += "]";
		return Stream;
	}

	class FQuat DegAngleToQuaternion();

};

template<>
const TVector<float> TVector<float>::WHITE = TVector<float>(1.0f, 1.0f, 1.0f, 1.0f);
template<>
const TVector<float> TVector<float>::BLACK = TVector<float>(0.0f, 0.0f, 0.0f, 1.0f);
template<>
const TVector<float> TVector<float>::RED = TVector<float>(1.0f, 0.0f, 0.0f, 1.0f);
template<>
const TVector<float> TVector<float>::BLUE = TVector<float>(0.0f, 0.0f, 1.0f, 1.0f);
template<>
const TVector<float> TVector<float>::GREEN = TVector<float>(0.0f, 1.0f, 0.0f, 1.0f);;


template<>
const TVector<float> TVector<float>::NONE = TVector<float>(0.0f, 0.0f, 0.0f, 0.0f);

template<>
const TVector<float> TVector<float>::ZERO = TVector<float>(0.0f, 0.0f, 0.0f, 1.0f);

template<>
const TVector<float> TVector<float>::LEFT = TVector<float>(-1.0f, 0.0f, 0.0f, 0.0f);;

template<>
const TVector<float> TVector<float>::RIGHT = TVector<float>(1.0f, 0.0f, 0.0f, 0.0f);;

template<>
const TVector<float> TVector<float>::UP = TVector<float>(0.0f, 1.0f, 0.0f, 0.0f);;

template<>
const TVector<float> TVector<float>::DOWN = TVector<float>(0.0f, -1.0f, 0.0f, 0.0f);;

template<>
const TVector<float> TVector<float>::FORWARD = TVector<float>(0.0f, 0.0f, 1.0f, 0.0f);;

template<>
const TVector<float> TVector<float>::BACK = TVector<float>(0.0f, 0.0f, -1.0f, 0.0f);;


class FQuat
{
public:
	union
	{
		struct
		{
			float X;
			float Y;
			float Z;
			float W;
		};

		float Arr2D[1][4];
		float Arr1D[4];

		DirectX::XMFLOAT4 DirectFloat4;
		DirectX::XMVECTOR DirectVector;

	};

	TVector<float> QuaternionToEulerDeg() const;
	TVector<float> QuaternionToEulerRad() const;
};

using FVector = TVector<float>;
using float4 = TVector<float>;

class FMatrix
{
public:
	union
	{
		float Arr2D[4][4] = { 0, };
		FVector ArrVector[4];
		float Arr1D[16];
		DirectX::XMMATRIX DirectMatrix;

		struct
		{
			float _00;
			float _01;
			float _02;
			float _03;
			float _10;
			float _11;
			float _12;
			float _13;
			float _20;
			float _21;
			float _22;
			float _23;
			float _30;
			float _31;
			float _32;
			float _33;
		};

	};

	FMatrix()
	{
		Identity();
	}

	void Identity()
	{
		DirectMatrix = DirectX::XMMatrixIdentity();
	}

	FVector GetFoward()
	{
		FVector Dir = ArrVector[2];
		Dir.Normalize();
		return Dir;
	}

	FVector GetRight()
	{
		FVector Dir = ArrVector[0];
		Dir.Normalize();
		return Dir;
	}

	FVector GetUp()
	{
		FVector Dir = ArrVector[1];
		Dir.Normalize();
		return Dir;
	}

	ENGINEAPI FMatrix operator*(const FMatrix& _Value);

	void Scale(const FVector& _Value)
	{
		DirectMatrix = DirectX::XMMatrixScalingFromVector(_Value.DirectVector);
	}

	void Position(const FVector& _Value)
	{
		DirectMatrix = DirectX::XMMatrixTranslationFromVector(_Value.DirectVector);
	}

	void RotationDeg(const FVector& _Angle)
	{
		RotationRad(_Angle * UEngineMath::D2R);
	}

	void RotationRad(const FVector& _Angle)
	{
		DirectMatrix = DirectX::XMMatrixRotationRollPitchYawFromVector(_Angle.DirectVector);
	}

	void Transpose()
	{
		for (size_t y = 0; y < 4; y++)
		{
			for (size_t x = y; x < 4; x++)
			{
				float Swap = Arr2D[y][x];
				Arr2D[y][x] = Arr2D[x][y];
				Arr2D[x][y] = Swap;
			}
		}

	}

	void View(const FVector& _Pos, const FVector& _Dir, const FVector& _Up)
	{
		Identity();
		DirectMatrix = DirectX::XMMatrixLookToLH(_Pos.DirectVector, _Dir.DirectVector, _Up.DirectVector);
		return;
	}

	void OrthographicLH(float _Width, float _Height, float _Near, float _Far)
	{
		Identity();
		DirectMatrix = DirectX::XMMatrixOrthographicLH(_Width, _Height, _Near, _Far);
	}

	void PerspectiveFovDeg(float _FovAngle, float _Width, float _Height, float _Near, float _Far)
	{
		PerspectiveFovRad(_FovAngle * UEngineMath::D2R, _Width, _Height, _Near, _Far);
	}

	void PerspectiveFovRad(float _FovAngle, float _Width, float _Height, float _Near, float _Far)
	{
		Identity();
		DirectMatrix = DirectX::XMMatrixPerspectiveFovLH(_FovAngle, _Width / _Height, _Near, _Far);
	}

	// 화면 확대 -1~1사이의 값이 됐으니까
	// +좌표축 변경 중점 변경
	// 화면의 정중앙을 0,0으로 만듭니다
	// Y축 반전도 여기서 합니다.
	// 뷰포트는 directx에서는 내가 곱해줄 필요가 없다. 다이렉에 넣어주면 다이렉트가 자동으로 해주는 것이다.
	// directx::viewportsetting(ViewPort_desc);

	// 위치와 크기 양쪽영향을 주는 행렬이다.
	// 그것조차도 내마음대로 정할수 있어.
	
	//                 1280          720        640           360            누가 앞에 나오고 누가 뒤에 나올거냐
	void ViewPort(float _Width, float _Height, float _Left, float _Top, float _ZMin, float _ZMax)
	{
		Identity();
		Arr2D[0][0] = _Width * 0.5f;
		Arr2D[1][1] = -_Height * 0.5f;

		Arr2D[2][2] = _ZMax != 0.0f ? 1.0f : _ZMin / _ZMax;

		Arr2D[3][0] = Arr2D[0][0] + _Left;
		Arr2D[3][1] = -Arr2D[1][1] + _Top;
		Arr2D[3][2] = _ZMax != 0.0f ? 1.0f : _ZMin / _ZMax;
	}


	void RotationXDeg(float _Angle)
	{
		RotationXRad(_Angle * UEngineMath::D2R);
	}

	void RotationXRad(float _Angle)
	{
		Identity();
		Arr2D[1][1] = cosf(_Angle);
		Arr2D[1][2] = -sinf(_Angle);
		Arr2D[2][1] = sinf(_Angle);
		Arr2D[2][2] = cosf(_Angle);
	}

	void RotationYDeg(float _Angle)
	{
		RotationYRad(_Angle * UEngineMath::D2R);
	}

	void RotationYRad(float _Angle)
	{
		Identity();
		Arr2D[0][0] = cosf(_Angle);
		Arr2D[0][2] = sinf(_Angle);
		Arr2D[2][0] = -sinf(_Angle);
		Arr2D[2][2] = cosf(_Angle);
	}

	FMatrix InverseReturn()
	{
		FMatrix Result;

		Result.DirectMatrix = DirectX::XMMatrixInverse(nullptr, DirectMatrix);

		return Result;
	}

	void Decompose(FVector& _Scale, FQuat& _RotQuaternion, FVector& _Pos)
	{
		DirectX::XMMatrixDecompose(&_Scale.DirectVector, &_RotQuaternion.DirectVector, &_Pos.DirectVector, DirectMatrix);
	}

	void RotationZDeg(float _Angle)
	{
		RotationZRad(_Angle * UEngineMath::D2R);
	}

	void RotationZRad(float _Angle)
	{
		Identity();
		Arr2D[0][0] = cosf(_Angle);
		Arr2D[0][1] = -sinf(_Angle);
		Arr2D[1][0] = sinf(_Angle);
		Arr2D[1][1] = cosf(_Angle);
	}

};

using float4x4 = FMatrix;


enum class ECollisionType
{
	Point,
	Rect,
	CirCle,
	OBB2D,
	Sphere,
	AABB,
	OBB,
	Max
};

struct FCollisionData
{
	union 
	{
		DirectX::BoundingSphere Sphere; 
		DirectX::BoundingBox AABB;
		DirectX::BoundingOrientedBox OBB;
	};

	FCollisionData()
	{

	}
};

struct FTransform
{
	float4 Scale;
	float4 Rotation;
	FQuat Quat;
	float4 Location;

	float4 RelativeScale;
	float4 RelativeRotation;
	FQuat RelativeQuat;
	float4 RelativeLocation;

	float4 WorldScale;
	float4 WorldRotation;
	FQuat WorldQuat;
	float4 WorldLocation;

	float4x4 ScaleMat;
	float4x4 RotationMat;
	float4x4 LocationMat;
	float4x4 RevolveMat;
	float4x4 ParentMat;
	float4x4 LocalWorld;
	float4x4 World;
	float4x4 View;
	float4x4 Projection;
	float4x4 WVP;

	FTransform()
		: Scale(FVector(1.0f, 1.0f, 1.0f, 1.0f ))
	{

	}

public:
	ENGINEAPI void TransformUpdate(bool _IsAbsolut = false);

	ENGINEAPI void Decompose();


	FVector GetWorldFoward()
	{
		return World.GetFoward();;
	}

	FVector GetWorldRight()
	{
		return World.GetRight();
	}

	FVector GetWorldUp()
	{
		return World.GetUp();
	}

	FVector GetLocalFoward()
	{
		return LocalWorld.GetFoward();;
	}

	FVector GetLocalRight()
	{
		return LocalWorld.GetRight();
	}

	FVector GetLocalUp()
	{
		return LocalWorld.GetUp();
	}

private:
	friend class CollisionFunctionInit;

	static std::function<bool(const FTransform&, const FTransform&)> AllCollisionFunction[static_cast<int>(ECollisionType::Max)][static_cast<int>(ECollisionType::Max)];

public:
	ENGINEAPI static bool Collision(ECollisionType _LeftType, const FTransform& _Left, ECollisionType _RightType, const FTransform& _Right);

	ENGINEAPI static bool PointToCirCle(const FTransform& _Left, const FTransform& _Right);
	ENGINEAPI static bool PointToRect(const FTransform& _Left, const FTransform& _Right);

	ENGINEAPI static bool RectToRect(const FTransform& _Left, const FTransform& _Right);
	ENGINEAPI static bool RectToCirCle(const FTransform& _Left, const FTransform& _Right);

	ENGINEAPI static bool CirCleToCirCle(const FTransform& _Left, const FTransform& _Right);
	ENGINEAPI static bool CirCleToRect(const FTransform& _Left, const FTransform& _Right);

	ENGINEAPI static bool OBB2DToOBB2D(const FTransform& _Left, const FTransform& _Right);
	ENGINEAPI static bool OBB2DToRect(const FTransform& _Left, const FTransform& _Right);
	ENGINEAPI static bool OBB2DToPoint(const FTransform& _Left, const FTransform& _Right);
	ENGINEAPI static bool OBB2DToCirCle(const FTransform& _Left, const FTransform& _Right);

	ENGINEAPI static bool OBBToSphere(const FTransform& _Left, const FTransform& _Right);
	ENGINEAPI static bool OBBToOBB(const FTransform& _Left, const FTransform& _Right);
	ENGINEAPI static bool OBBToAABB(const FTransform& _Left, const FTransform& _Right);

	ENGINEAPI static bool SphereToSphere(const FTransform& _Left, const FTransform& _Right);
	ENGINEAPI static bool SphereToOBB(const FTransform& _Left, const FTransform& _Right);
	ENGINEAPI static bool SphereToAABB(const FTransform& _Left, const FTransform& _Right);

	ENGINEAPI static bool AABBToSphere(const FTransform& _Left, const FTransform& _Right);
	ENGINEAPI static bool AABBToOBB(const FTransform& _Left, const FTransform& _Right);
	ENGINEAPI static bool AABBToAABB(const FTransform& _Left, const FTransform& _Right);

	FCollisionData GetCollisionData() const
	{
		FCollisionData Result;
		Result.OBB.Center = WorldLocation.DirectFloat3;
		Result.OBB.Extents = (WorldScale * 0.5f).ABSVectorReturn().DirectFloat3;
		Result.OBB.Orientation = WorldQuat.DirectFloat4;
		return Result;
	}

	FVector ZAxisCenterLeftTop() const
	{
		return FVector(Location.X - Scale.Half().X, Location.Y + Scale.Half().Y);
	}

	FVector ZAxisCenterLeftBottom() const
	{
		FVector Result;
		Result.X = Location.X - Scale.hX();
		Result.Y = Location.Y + Scale.hY();
		return Result;
	}

	float ZAxisCenterLeft() const
	{
		return Location.X - Scale.hX();
	}

	float ZAxisCenterTop() const
	{
		return Location.Y + Scale.hY();
	}

	FVector ZAxisCenterRightTop() const
	{
		FVector Result;
		Result.X = Location.X + Scale.hX();
		Result.Y = Location.Y + Scale.hY();
		return Result;
	}

	FVector ZAxisCenterRightBottom() const
	{
		return FVector(Location.X + Scale.Half().X, Location.Y - Scale.Half().Y);
	}

	float ZAxisCenterRight() const
	{
		return Location.X + Scale.hX();
	}

	float ZAxisCenterBottom() const
	{
		return Location.Y - Scale.hY();
	}
};

class FIntPoint
{
public:
	int X = 0;
	int Y = 0;

	static const FIntPoint LEFT;
	static const FIntPoint RIGHT;
	static const FIntPoint UP;
	static const FIntPoint DOWN;

	ENGINEAPI FIntPoint()
	{

	}

	FIntPoint(int _X, int _Y) : X(_X), Y(_Y)
	{

	}

	FIntPoint operator+(FIntPoint _Other) const
	{
		FIntPoint Result;
		Result.X = X + _Other.X;
		Result.Y = Y + _Other.Y;
		return Result;
	}

	FIntPoint operator/(int _Value) const
	{
		FIntPoint Result;
		Result.X = X / _Value;
		Result.Y = Y / _Value;
		return Result;
	}


	bool operator==(FIntPoint _Other) const
	{
		return X == _Other.X && Y == _Other.Y;
	}

	FIntPoint& operator+=(FIntPoint _Other)
	{
		X += _Other.X;
		Y += _Other.Y;
		return *this;
	}


};


template<typename ValueType>
class TColor
{
public:
	static const TColor WHITE;
	static const TColor BLACK;
	static const TColor RED;
	static const TColor BLUE;
	static const TColor GREEN;

	union
	{
		unsigned int Color;
		struct
		{
			unsigned char R;
			unsigned char G;
			unsigned char B;
			unsigned char A;
		};
	};

	TColor(unsigned long _Value)
		:Color(_Value)
	{

	}

	bool operator==(const TColor& _Other)
	{
		return R == _Other.R && G == _Other.G && B == _Other.B;
	}


	TColor(unsigned char _R, unsigned char _G, unsigned char _B, unsigned char _A)
		:R(_R), G(_G), B(_B), A(_A)
	{

	}
};

using UColor = TColor<unsigned char>;

template<>
const TColor<unsigned char> TColor<unsigned char>::WHITE = TColor<unsigned char>( 255, 255, 255, 255 );

template<>
const TColor<unsigned char> TColor<unsigned char>::BLACK = TColor<unsigned char>(0, 0, 0, 255);

template<>
const TColor<unsigned char> TColor<unsigned char>::RED = TColor<unsigned char>(255, 0, 0, 255);

template<>
const TColor<unsigned char> TColor<unsigned char>::BLUE = TColor<unsigned char>(0, 0, 255, 255);

template<>
const TColor<unsigned char> TColor<unsigned char>::GREEN = TColor<unsigned char>(0, 255, 0, 255);

