#include "PreCompile.h"
#include "EngineMath.h"

const FVector FVector::ZERO = { 0.0f, 0.0f };
const FVector FVector::LEFT = { -1.0f, 0.0f };
const FVector FVector::RIGHT = { 1.0f, 0.0f };
const FVector FVector::UP = { 0.0f, 1.0f };
const FVector FVector::DOWN = { 0.0f, -1.0f };
const FVector FVector::FORWARD = { 0.0f, 0.0f, 1.0f };
const FVector FVector::BACK = { 0.0f, 0.0f , -1.0f };

const FIntPoint FIntPoint::LEFT = { -1, 0 };
const FIntPoint FIntPoint::RIGHT = { 1, 0 };
const FIntPoint FIntPoint::UP = { 0, -1 };
const FIntPoint FIntPoint::DOWN = { 0, 1 };


const UColor UColor::WHITE = { 255, 255, 255, 0 };
const UColor UColor::BLACK = { 0, 0, 0, 0 };

FIntPoint FVector::ConvertToPoint() const
{
	return { iX(), iY() };
}


std::function<bool(const FTransform&, const FTransform&)> FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::Max)][static_cast<int>(ECollisionType::Max)];

class CollisionFunctionInit
{
public:
	CollisionFunctionInit()
	{
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::Rect)][static_cast<int>(ECollisionType::Rect)] = FTransform::RectToRect;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::CirCle)][static_cast<int>(ECollisionType::CirCle)] = FTransform::CirCleToCirCle;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::Rect)][static_cast<int>(ECollisionType::CirCle)] = FTransform::RectToCirCle;
		FTransform::AllCollisionFunction[static_cast<int>(ECollisionType::CirCle)][static_cast<int>(ECollisionType::Rect)] = FTransform::CirCleToRect;

	}
};

CollisionFunctionInit Inst = CollisionFunctionInit();


FVector FQuat::QuaternionToEulerDeg() const
{
	return QuaternionToEulerRad() * UEngineMath::R2D;
}

FVector FQuat::QuaternionToEulerRad() const
{
	FVector result;

	float sinrCosp = 2.0f * (W * Z + X * Y);
	float cosrCosp = 1.0f - 2.0f * (Z * Z + X * X);
	result.Z = atan2f(sinrCosp, cosrCosp);

	float pitchTest = W * X - Y * Z;
	float asinThreshold = 0.4999995f;
	float sinp = 2.0f * pitchTest;

	if (pitchTest < -asinThreshold)
	{
		result.X = -(0.5f * UEngineMath::PI);
	}
	else if (pitchTest > asinThreshold)
	{
		result.X = (0.5f * UEngineMath::PI);
	}
	else
	{
		result.X = asinf(sinp);
	}
 
	float sinyCosp = 2.0f * (W * Y + X * Z);
	float cosyCosp = 1.0f - 2.0f * (X * X + Y * Y);
	result.Y = atan2f(sinyCosp, cosyCosp);

	return result;
}

bool FTransform::Collision(ECollisionType _LeftType, const FTransform& _Left, ECollisionType _RightType, const FTransform& _Right)
{
	return FTransform::AllCollisionFunction[static_cast<int>(_LeftType)][static_cast<int>(_RightType)](_Left, _Right);
}

bool FTransform::PointToCirCle(const FTransform& _Left, const FTransform& _Right)
{
	FTransform LeftTrans = _Left;
	LeftTrans.Scale = FVector::ZERO;
	return CirCleToCirCle(LeftTrans, _Right);
}

bool FTransform::PointToRect(const FTransform& _Left, const FTransform& _Right)
{
	FTransform LeftTrans = _Left;
	LeftTrans.Scale = FVector::ZERO;
	return RectToRect(LeftTrans, _Right);
}

bool FTransform::CirCleToCirCle(const FTransform& _Left, const FTransform& _Right)
{
	FVector Len = _Left.Location - _Right.Location;

	if (Len.Length() < _Left.Scale.hX() + _Right.Scale.hX())
	{
		return true;
	}

	return false;
}

bool FTransform::RectToRect(const FTransform& _Left, const FTransform& _Right)
{

	if (_Left.ZAxisCenterLeft() > _Right.ZAxisCenterRight())
	{
		return false;
	}

	if (_Left.ZAxisCenterRight() < _Right.ZAxisCenterLeft())
	{
		return false;
	}

	if (_Left.ZAxisCenterTop() > _Right.ZAxisCenterBottom())
	{
		return false;
	}

	if (_Left.ZAxisCenterBottom() < _Right.ZAxisCenterTop())
	{
		return false;
	}

	return true;
}

bool FTransform::RectToCirCle(const FTransform& _Left, const FTransform& _Right)
{
	return CirCleToRect(_Right, _Left);
}


bool FTransform::CirCleToRect(const FTransform& _Left, const FTransform& _Right)
{
	FTransform WTransform = _Right;
	WTransform.Scale.X += _Left.Scale.X;

	FTransform HTransform = _Right;
	HTransform.Scale.Y += _Left.Scale.X;

	if (true == PointToRect(_Left, WTransform) || true == PointToRect(_Left, HTransform))
	{
		return true;
	}

	FVector ArrPoint[4];

	ArrPoint[0] = _Right.ZAxisCenterLeftTop();
	ArrPoint[1] = _Right.ZAxisCenterLeftBottom();
	ArrPoint[2] = _Right.ZAxisCenterRightTop();
	ArrPoint[3] = _Right.ZAxisCenterRightBottom();

	FTransform PointCirCle;
	PointCirCle.Scale = _Left.Scale;
	for (size_t i = 0; i < 4; i++)
	{
		PointCirCle.Location = ArrPoint[i];
		if (true == PointToCirCle(_Left, PointCirCle))
		{
			return true;
		}
	}

	return false;
}

FVector FVector::Transform(const FVector& _Vector, const class FMatrix& _Matrix)
{
	return _Vector * _Matrix;
}

FVector FVector::TransformCoord(const FVector& _Vector, const class FMatrix& _Matrix)
{
	FVector Copy = _Vector;
	Copy.W = 1.0f;
	return Copy * _Matrix;
}

FVector FVector::TransformNormal(const FVector& _Vector, const class FMatrix& _Matrix)
{
	FVector Copy = _Vector;
	Copy.W = 0.0f;
	return Copy * _Matrix;
}

FVector FVector::operator*(const class FMatrix& _Matrix) const
{
	FVector Result;

	Result.X = Arr2D[0][0] * _Matrix.Arr2D[0][0] + Arr2D[0][1] * _Matrix.Arr2D[1][0] + Arr2D[0][2] * _Matrix.Arr2D[2][0] + Arr2D[0][3] * _Matrix.Arr2D[3][0];
	Result.Y = Arr2D[0][0] * _Matrix.Arr2D[0][1] + Arr2D[0][1] * _Matrix.Arr2D[1][1] + Arr2D[0][2] * _Matrix.Arr2D[2][1] + Arr2D[0][3] * _Matrix.Arr2D[3][1];
	Result.Z = Arr2D[0][0] * _Matrix.Arr2D[0][2] + Arr2D[0][1] * _Matrix.Arr2D[1][2] + Arr2D[0][2] * _Matrix.Arr2D[2][2] + Arr2D[0][3] * _Matrix.Arr2D[3][2];
	Result.W = Arr2D[0][0] * _Matrix.Arr2D[0][3] + Arr2D[0][1] * _Matrix.Arr2D[1][3] + Arr2D[0][2] * _Matrix.Arr2D[2][3] + Arr2D[0][3] * _Matrix.Arr2D[3][3];


	//std::cout << "X : " << Arr2D[0][0] << "*" << _Matrix.Arr2D[0][0] << "+" << Arr2D[0][1] << "*" << _Matrix.Arr2D[1][0] << "+" << Arr2D[0][2] << "*" << _Matrix.Arr2D[2][0] << "+" << Arr2D[0][3] << "*" << _Matrix.Arr2D[3][0] << std::endl;
	//std::cout << "Y : " << Arr2D[0][0] << "*" << _Matrix.Arr2D[0][1] << "+" << Arr2D[0][1] << "*" << _Matrix.Arr2D[1][1] << "+" << Arr2D[0][2] << "*" << _Matrix.Arr2D[2][1] << "+" << Arr2D[0][3] << "*" << _Matrix.Arr2D[3][1] << std::endl;
	//std::cout << "Z : " << Arr2D[0][0] << "*" << _Matrix.Arr2D[0][2] << "+" << Arr2D[0][1] << "*" << _Matrix.Arr2D[1][2] << "+" << Arr2D[0][2] << "*" << _Matrix.Arr2D[2][2] << "+" << Arr2D[0][3] << "*" << _Matrix.Arr2D[3][2] << std::endl;
	//std::cout << "W : " << Arr2D[0][0] << "*" << _Matrix.Arr2D[0][3] << "+" << Arr2D[0][1] << "*" << _Matrix.Arr2D[1][3] << "+" << Arr2D[0][2] << "*" << _Matrix.Arr2D[2][3] << "+" << Arr2D[0][3] << "*" << _Matrix.Arr2D[3][3] << std::endl;

	//std::cout << Result.ToString() << std::endl;


	return Result;
}

FVector& FVector::operator*=(const FMatrix& _Matrix)
{
	DirectVector = DirectX::XMVector4Transform(DirectVector, _Matrix.DirectMatrix);
	return *this;
}

FMatrix FMatrix::operator*(const FMatrix& _Matrix)
{
	FMatrix Result;
	Result.DirectMatrix = DirectX::XMMatrixMultiply(DirectMatrix, _Matrix.DirectMatrix);
	return Result;

}

ENGINEAPI void FTransform::Decompose()
{
	World.Decompose(WorldScale, WorldQuat, WorldLocation);

	LocalWorld.Decompose(RelativeScale, RelativeQuat, RelativeLocation);
}

void FTransform::TransformUpdate(bool _IsAbsolut /*= false*/)
{
	ScaleMat.Scale(Scale);
	RotationMat.RotationDeg(Rotation);
	LocationMat.Position(Location);

	FMatrix CheckWorld = ScaleMat * RotationMat * LocationMat;

	if (true == _IsAbsolut)
	{
		World = CheckWorld;
		LocalWorld = CheckWorld * ParentMat.InverseReturn();
	}
	else 
	{
		LocalWorld = ScaleMat * RotationMat * LocationMat;
		World = ScaleMat * RotationMat * LocationMat * RevolveMat * ParentMat;
	}
	Decompose();
}