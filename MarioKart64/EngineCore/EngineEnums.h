#pragma once

typedef __int8 int8;


enum class EProjectionType : int8
{
	Perspective,
	Orthographic,
};

enum class EEngineCameraType : int8
{
	MainCamera,
	UICamera = 127,
};

// 1����Ʈ ���� �� ���̴��� ����� ������ �Ʒ��� ���� �ϸ� �˴ϴ�.
enum class EShaderType : int8
{
	VS, // Hertex shader
	HS, // Hull shader
	DS, // D
	GS, // Geometry shader
	PS, // Pixel shader
	CS, // C
	MAX
};
