#pragma once
#include <EngineBase/EngineMath.h>
#include <EngineCore/EngineVertex.h>

static std::vector<FEngineVertex> SingleTriangle = {
	FEngineVertex{ FVector(-0.5f, -0.5f, 0.0f), {0.0f , 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f} },
	FEngineVertex{ FVector(-0.5f, 0.5f, 0.0f), {1.0f , 0.0f } , {0.0f, 1.0f, 0.0f, 1.0f} },
	FEngineVertex{ FVector(0.5f, 0.5f, 0.0f), {0.0f , 1.0f } , {0.0f, 0.0f, 1.0f, 1.0f} },
};

static std::vector<FEngineVertex> SingleLine = {
	FEngineVertex{ FVector(0.f, -.5f, 0.0f), {0.0f , 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f} },
	FEngineVertex{ FVector(0.f, .5f, 0.0f), {1.0f , 0.0f } , {0.0f, 1.0f, 0.0f, 1.0f} }
};

static std::vector<FEngineVertex> MultipleTriangles = {
	FEngineVertex{ FVector{ -172.000015f, 0.000000f, -316.000000f }, {0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}},
	FEngineVertex{ FVector{ -145.000015f, 0.000000f, -269.000000f }, {0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}},
	FEngineVertex{ FVector{ -93.000015f, 23.000000f, -374.000000f }, {0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}},
	FEngineVertex{ FVector{ -145.000015f, 0.000000f, -269.000000f }, {0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}},
	FEngineVertex{ FVector{ -56.999985f, 19.000000f, -312.000000f }, {0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}},
	FEngineVertex{ FVector{ -93.000015f, 23.000000f, -374.000000f }, {0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}},
	FEngineVertex{ FVector{ -203.000015f, 0.000000f, -348.000000f }, {0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}},
	FEngineVertex{ FVector{ -172.000015f, 0.000000f, -316.000000f }, {0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}},
	FEngineVertex{ FVector{ -131.000015f, 25.000000f, -412.000000f }, {0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}},
	FEngineVertex{ FVector{ -172.000015f, 0.000000f, -316.000000f }, {0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}},
	FEngineVertex{ FVector{ -93.000015f, 23.000000f, -374.000000f }, {0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}},
	FEngineVertex{ FVector{ -131.000015f, 25.000000f, -412.000000f }, {0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}},
	FEngineVertex{ FVector{ -234.000015f, 0.000000f, -376.000000f }, {0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}},
	FEngineVertex{ FVector{ -203.000015f, 0.000000f, -348.000000f }, {0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}},
	FEngineVertex{ FVector{ -176.000015f, 27.000000f, -453.000000f }, {0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}},
	FEngineVertex{ FVector{ -203.000015f, 0.000000f, -348.000000f }, {0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}},
	FEngineVertex{ FVector{ -131.000015f, 25.000000f, -412.000000f }, {0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}},
	FEngineVertex{ FVector{ -176.000015f, 27.000000f, -453.000000f }, {0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}},
	FEngineVertex{ FVector{ -276.000031f, 0.000000f, -401.999969f }, {0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}},
	FEngineVertex{ FVector{ -234.000015f, 0.000000f, -376.000000f }, {0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}},
	FEngineVertex{ FVector{ -235.000031f, 30.000000f, -487.999969f }, {0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}},
	FEngineVertex{ FVector{ -234.000015f, 0.000000f, -376.000000f }, {0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}},
	FEngineVertex{ FVector{ -176.000015f, 27.000000f, -453.000000f }, {0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}},
	FEngineVertex{ FVector{ -235.000031f, 30.000000f, -487.999969f }, {0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}},
	FEngineVertex{ FVector{ -145.000015f, 0.000000f, -269.000000f }, {0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}},
	FEngineVertex{ FVector{ -126.000015f, 0.000000f, -220.000000f }, {0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}},
	FEngineVertex{ FVector{ -56.999985f, 19.000000f, -312.000000f }, {0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}},
	FEngineVertex{ FVector{ -126.000015f, 0.000000f, -220.000000f }, {0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}},
	FEngineVertex{ FVector{ -32.999954f, 16.000000f, -252.000000f }, {0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}},
	FEngineVertex{ FVector{ -56.999985f, 19.000000f, -312.000000f}, {0.f, 0.f}, {0.f, 0.f, 0.f, 1.f}}
};

static std::vector<FEngineVertex> SingleCube = {
	FEngineVertex{ FVector(-0.5f, -0.5f, -0.5f), { 0.0f , 1.0f }, {1.0f, 0.0f, 0.0f, 0.8f} },
	FEngineVertex{ FVector(0.5f, -0.5f, -0.5f), { 0.0f , 1.0f }, {0.0f, 1.0f, 0.0f, 0.8f} },

	FEngineVertex{ FVector(-0.5f, 0.5f, -0.5f), { 0.0f , 1.0f }, {0.0f, 0.0f, 1.0f, 0.8f} },
	FEngineVertex{ FVector(0.5f, 0.5f, -0.5f), { 0.0f , 1.0f }, {1.0f, 1.0f, 1.0f, 0.8f} },

	FEngineVertex{ FVector(-0.5f, -0.5f, 0.5f), { 0.0f , 1.0f }, {1.0f, 0.0f, 0.0f, 0.8f} },
	FEngineVertex{ FVector(0.5f, -0.5f, 0.5f), { 0.0f , 1.0f }, {0.0f, 1.0f, 0.0f, 0.8f} },

	FEngineVertex{ FVector(-0.5f, 0.5f, 0.5f), { 0.0f , 1.0f }, {0.0f, 0.0f, 1.0f, 0.8f} },
	FEngineVertex{ FVector(0.5f, 0.5f, 0.5f), { 0.0f , 1.0f }, {1.0f, 1.0f, 1.0f, 0.8f} },
};

