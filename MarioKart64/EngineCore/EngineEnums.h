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

// 1바이트 정수 로 쉐이더를 만들고 싶으면 아래와 같이 하면 됩니다.
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
