#include "PreCompile.h"
#include "EngineDebug.h"


namespace UEngineDebug
{
	void LeakCheck()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	}

	void OutPutString(const std::string& Text)
	{
		std::string ResultText = Text + "\n";
		OutputDebugStringA(ResultText.c_str());
	}


}