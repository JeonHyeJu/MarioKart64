#include "PreCompile.h"
#include "CGlobal.h"
#include <EngineBase/EngineDirectory.h>

const char* CGlobal::ModelPath = "MarioKart64\\Resources\\Models";
const char* CGlobal::OBJ_SHADER_NAME = "Object";
const char* CGlobal::OBJ_SPRITE_SHADER_NAME = "ObjectSprite";
const char* CGlobal::OBJ_SKY_SHADER_NAME = "Skybox";
const char* CGlobal::OBJ_LINE_SHADER_NAME = "Line";
const char* CGlobal::WIDGET_SHADER = "WidgetShader";
const char* CGlobal::COLOR_WIDGET_MATERIAL = "ColorWidgetMaterial";
const char* CGlobal::SHRINK_EFFECT = "ShrinkFx";
const char* CGlobal::EXPAND_EFFECT = "ExpandFx";
const int CGlobal::WINDOW_W = 625 * 2;
const int CGlobal::WINDOW_H = 395 * 2;
const float CGlobal::FWINDOW_W = static_cast<float>(625 * 2);
const float CGlobal::FWINDOW_H = static_cast<float>(395 * 2);

std::string CGlobal::GetModelPath(std::string_view _append="", std::string_view _fileName="")
{
	UEngineDirectory dir;
	dir.MoveParentToDirectory(CGlobal::ModelPath);
	if (_append.size() > 0)
	{
		dir.Append(_append.data());
	}

	if (_fileName.size() == 0)
	{
		return dir.GetPathToString() + "\n";
	}

	dir.Append(_fileName.data());
	return dir.GetPathToString();
}

void CGlobal::GetRouteIdxLuigi(std::map<int, int>& _refMap)
{
	std::vector<int> routeInit = {
		283, 279, 278, 282, 432, 430, 277, 281, 433, 431, 276, 280, 438, 437, 284, 289, 436, 435, 285, 286, 439, 434, 287, 288, 444, 443, 290, 295, 442, 441, 291, 292, 445, 440, 293, 294, 448, 451, 453, 450, 302, 305, 303, 300, 452, 455, 449, 446, 298, 301, 299, 296, 454, 447, 297, 304, 461, 456, 306, 311, 460, 459, 307, 308, 458, 457, 309, 310, 465, 463, 312, 314, 464, 462, 313, 315, 423, 421, 272, 274, 422, 420, 273, 275, 397, 395, 249, 251, 394, 396, 248, 250, 400, 401, 253, 255, 398, 399, 252, 254, 404, 405, 256, 259, 402, 403, 257, 258, 408, 409, 263, 262, 406, 407, 260, 261, 413, 411, 264, 266, 412, 410, 265, 267, 417, 414, 269, 268, 416, 415, 270, 271, 418, 419, 318, 316, 466, 469, 319, 317, 467, 468, 358, 359, 393, 392, 356, 357, 391, 390, 360, 361, 363, 362, 504, 505, 507, 506, 365, 364, 367, 366, 513, 512, 515, 514, 372, 373, 375, 374, 510, 511, 508, 509, 369, 368, 371, 370, 520, 521, 523, 522, 380, 381, 383, 382, 518, 519, 516, 517, 377, 376, 379, 378, 524, 526, 387, 385, 525, 527, 386, 384, 531, 529, 388, 389, 530, 528, 355, 353, 503, 501, 352, 354, 500, 502, 321, 323, 473, 471, 320, 322, 472, 470, 326, 327, 477, 475, 324, 325, 476, 474, 328, 329, 478, 481, 331, 330, 480, 479, 334, 335, 482, 483, 332, 333, 485, 484, 336, 339, 486, 489, 337, 338, 488, 487, 343, 344, 493, 491, 341, 342, 492, 490, 340, 345, 498, 497, 346, 351, 496, 495, 347, 348, 499, 494, 349, 350, 429, 424, 242, 247, 428, 427, 243, 244, 426, 425, 245, 246, 241, 240
	};

	for (size_t i = 0, size = routeInit.size(); i < size; ++i)
	{
		_refMap[routeInit[i]] = static_cast<int>(i);
	}
}

void CGlobal::GetRouteIdxKoopa(std::map<int, int>& _refMap)
{
	std::vector<int> routeInit = {
	};

	for (size_t i = 0, size = routeInit.size(); i < size; ++i)
	{
		_refMap[routeInit[i]] = static_cast<int>(i);
	}
}

void CGlobal::GetRouteIdxMario(std::map<int, int>& _refMap)
{
	std::vector<int> routeInit = {
	};

	for (size_t i = 0, size = routeInit.size(); i < size; ++i)
	{
		_refMap[routeInit[i]] = static_cast<int>(i);
	}
}

void CGlobal::GetRouteIdxWario(std::map<int, int>& _refMap)
{
	std::vector<int> routeInit = {
	};

	for (size_t i = 0, size = routeInit.size(); i < size; ++i)
	{
		_refMap[routeInit[i]] = static_cast<int>(i);
	}
}

void CGlobal::GetRouteIdxSherbet(std::map<int, int>& _refMap)
{
	std::vector<int> routeInit = {
	};

	for (size_t i = 0, size = routeInit.size(); i < size; ++i)
	{
		_refMap[routeInit[i]] = static_cast<int>(i);
	}
}

void CGlobal::GetRouteIdxRoyal(std::map<int, int>& _refMap)
{
	std::vector<int> routeInit = {
		366, 382, 381, 367, 368, 369, 370, 371, 372, 356, 357, 358, 359, 360, 361, 362, 354, 355, 347, 346, 349, 348, 351, 350, 353, 352, 344, 345, 343, 342, 340, 341, 338, 339, 336, 337, 324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335, 318, 319, 316, 317, 314, 315, 320, 321, 322, 323, 306, 307, 308, 309, 304, 305, 312, 313, 310, 311, 300, 301, 298, 299, 296, 297, 294, 295, 292, 293, 290, 291, 302, 303, 280, 281, 282, 283, 284, 285, 286, 287, 288, 289, 278, 279, 270, 271, 272, 273, 274, 275, 276, 277, 262, 263, 264, 265, 267, 266, 268, 269, 253, 252, 261, 260, 254, 255, 256, 257, 258, 259, 251, 250, 249, 248, 247, 246, 244, 245, 243, 242, 241, 240, 239, 238, 237, 236, 235, 234, 233, 232, 227, 226, 229, 228, 231, 230, 225, 224, 223, 222, 221, 220, 219, 218, 217, 216, 215, 214, 213, 212, 209, 208, 211, 210, 207, 206, 205, 204, 203, 202, 201, 200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 188, 189, 176, 177, 179, 178, 180, 181, 182, 183, 185, 184, 186, 187, 174, 175, 165, 164, 166, 167, 168, 169, 171, 170, 172, 173, 155, 154, 156, 157, 159, 158, 160, 161, 163, 162, 152, 153, 143, 142, 144, 145, 147, 146, 148, 149, 151, 150, 137, 136, 138, 139, 140, 141, 379, 380, 133, 132, 134, 135, 128, 129, 131, 130, 378, 377, 376, 375, 374, 373,
		125,124,127,126,121,120,123,122,118,119,114,115,116,117,98,99,100,101,96,97,94,95,92,93,102,103,112,113,110,111,108,109,106,107,104,105,78,79,76,77,74,75,72,73,80,81,90,91,88,89,86,87,84,85,82,83,56,57,54,55,52,53,50,51,48,49,47,46,65,64,63,62,61,60,45,44,59,58,71,70,69,68,66,67,33,32,31,30,29,28,27,26,35,34,43,42,41,40,38,39,37,36,14,15,17,16,19,18,20,21,22,23,24,25,6,7,4,5,2,3,0,1,8,9,13,12,10,11,364,363,365
	};

	for (size_t i = 0, size = routeInit.size(); i < size; ++i)
	{
		_refMap[routeInit[i]] = static_cast<int>(i);
	}
}

void CGlobal::GetRouteIdxBowsers(std::map<int, int>& _refMap)
{
	std::vector<int> routeInit = {
	};

	for (size_t i = 0, size = routeInit.size(); i < size; ++i)
	{
		_refMap[routeInit[i]] = static_cast<int>(i);
	}
}

void CGlobal::GetRouteIdxRainbow(std::map<int, int>& _refMap)
{
	std::vector<int> routeInit = {
	};

	for (size_t i = 0, size = routeInit.size(); i < size; ++i)
	{
		_refMap[routeInit[i]] = static_cast<int>(i);
	}
}
