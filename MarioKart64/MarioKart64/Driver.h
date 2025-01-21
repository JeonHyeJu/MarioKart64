#pragma once
#include <EngineCore/Pawn.h>
#include "CData.h"

class USpriteRenderer;
class LineRenderer;
class UCollision;
class ADriver : public APawn
{
public:
	ADriver();
	~ADriver();

	ADriver(const ADriver& _Other) = delete;
	ADriver(ADriver&& _Other) noexcept = delete;
	ADriver& operator=(const ADriver& _Other) = delete;
	ADriver& operator=(ADriver&& _Other) noexcept = delete;

	void SetMap(class ATestMap* _ptr);

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

private:
	void Move(float _deltaTime);
	bool CheckCollision(const FVector& _loc, int& _refIdx, float& _refDist);
	bool CheckCollision(const FVector& _loc, int _startIdx, int& _refIdx, float& _refDist);
	void CheckCollisionOfAllMap();
	float GetSlope();
	void GetForwardPhysics(float _deltaTime, float& _refDx, bool _isCollided=true, bool _isComputer=true);
	void GetHandleRotation(float _deltaTime, float& _refRot);
	void CheckLab();

	void OnCollisionEnter(UCollision* _this, UCollision* _other);
	void PickItem(float _deltaTime);
	void CheckUsingItem(float _deltaTime);

	void UseItem_Shell(const EItemType& _itemType);
	void UseItem_Mushroom(const EItemType& _itemType);
	void UseItem_Banana(const EItemType& _itemType);
	void UseItem_Star(const EItemType& _itemType);
	void UseItem_Thunder(const EItemType& _itemType);
	void UseItem_Ghost(const EItemType& _itemType);
	void UseItem_FakeItemBox(const EItemType& _itemType);

	std::shared_ptr<USpriteRenderer> Renderer;
	std::shared_ptr<USpriteRenderer> RendererDebug;
	std::shared_ptr<LineRenderer> LineDebug;
	std::shared_ptr<UCollision> CollisionItem = nullptr;

	float Speed = 5.f;
	float SpeedWeight = 0.f;

	const float FRICTIONAL_FORCE = .05f;
	const float MAX_SPEED = 20.f;
	const float MAX_TURN = 5.f;

	const float WEIGHT = 50.f;
	const float ACCELERATION = WEIGHT * 5.f;	// 350px/s
	//const float ACCELERATION = WEIGHT * 7.f;	// 350px/s
	const float FRICTION_FORCE = WEIGHT * 2.f;	// 100px/s
	const float MAX_VELOCITY = 1000.f * 2;
	const float GRAVITY_FORCE = -300.f;

	float Velocity = 0.f;
	float VelocityV = 0.f;

	int PrevIdx = -1;
	int PrevGroupIdx = -1;
	ATestMap* TestMapPtr = nullptr;

	bool IsTouchLastTriangle = false;
	int Lab = 0;

	bool IsPickingItem = false;
	SItemRoulette ItemRoulette;
	int ItemIndex = -1;

	FVector PrevLoc = FVector::NONE;

	// Temp
	std::shared_ptr<class USpriteRenderer> DebugItem = nullptr;

	int DstRouteNavIdx = 1;
	int CurRouteIdx = 0;
	std::map<int, int> TempRouteIdx;
	std::vector<int> TempRouteIdxInit = {
		366, 382, 381, 367, 368, 369, 370, 371, 372, 356, 357, 358, 359, 360, 361, 362, 354, 355, 347, 346, 349, 348, 351, 350, 353, 352, 344, 345, 343, 342, 340, 341, 338, 339, 336, 337, 324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335, 318, 319, 316, 317, 314, 315, 320, 321, 322, 323, 306, 307, 308, 309, 304, 305, 312, 313, 310, 311, 300, 301, 298, 299, 296, 297, 294, 295, 292, 293, 290, 291, 302, 303, 280, 281, 282, 283, 284, 285, 286, 287, 288, 289, 278, 279, 270, 271, 272, 273, 274, 275, 276, 277, 262, 263, 264, 265, 267, 266, 268, 269, 253, 252, 261, 260, 254, 255, 256, 257, 258, 259, 251, 250, 249, 248, 247, 246, 244, 245, 243, 242, 241, 240, 239, 238, 237, 236, 235, 234, 233, 232, 227, 226, 229, 228, 231, 230, 225, 224, 223, 222, 221, 220, 219, 218, 217, 216, 215, 214, 213, 212, 209, 208, 211, 210, 207, 206, 205, 204, 203, 202, 201, 200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 188, 189, 176, 177, 179, 178, 180, 181, 182, 183, 185, 184, 186, 187, 174, 175, 165, 164, 166, 167, 168, 169, 171, 170, 172, 173, 155, 154, 156, 157, 159, 158, 160, 161, 163, 162, 152, 153, 143, 142, 144, 145, 147, 146, 148, 149, 151, 150, 137, 136, 138, 139, 140, 141, 379, 380, 133, 132, 134, 135, 128, 129, 131, 130, 378, 377, 376, 375, 374, 373, 
		125,124,127,126,121,120,123,122,118,119,114,115,116,117,98,99,100,101,96,97,94,95,92,93,102,103,112,113,110,111,108,109,106,107,104,105,78,79,76,77,74,75,72,73,80,81,90,91,88,89,86,87,84,85,82,83,56,57,54,55,52,53,50,51,48,49,47,46,65,64,63,62,61,60,45,44,59,58,71,70,69,68,66,67,33,32,31,30,29,28,27,26,35,34,43,42,41,40,38,39,37,36,14,15,17,16,19,18,20,21,22,23,24,25,6,7,4,5,2,3,0,1,8,9,13,12,10,11,364,363,365 
	};

	int FutureNavIdx = -1;
};

