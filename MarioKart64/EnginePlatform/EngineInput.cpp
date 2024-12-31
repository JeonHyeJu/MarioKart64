#include "PreCompile.h"
#include "EngineInput.h"

void UEngineInput::UEngineKey::KeyCheck(float _DeltaTime)
{
	if (0 != GetAsyncKeyState(Key))
	{
		if (true == IsPress)
		{
			PressTime += _DeltaTime;
		}

		if (true == IsFree)
		{
			IsDown = true;
			IsPress = true;
			IsFree = false;
			IsUp = false;
		}
		else if(true == IsDown)
		{
			FreeTime = 0.0f;
			IsDown = false;
			IsPress = true;
			IsFree = false;
			IsUp = false;
		}

	} else 
	{
		if (true == IsFree)
		{
			FreeTime += _DeltaTime;
		}
		
		if (true == IsPress)
		{
			IsDown = false;
			IsPress = false;
			IsFree = true;
			IsUp = true;
		}
		else if (true == IsUp)
		{
			PressTime = 0.0f;
			IsDown = false;
			IsPress = false;
			IsFree = true;
			IsUp = false;
		}

	}
}

void UEngineInput::UEngineKey::EventCheck()
{
	if (true == IsDown)
	{
		for (size_t i = 0; i < DownEvents.size(); i++)
		{
			DownEvents[i]();
		}
	}

	if (true == IsPress)
	{
		for (size_t i = 0; i < PressEvents.size(); i++)
		{
			PressEvents[i]();
		}
	}

	if (true == IsFree)
	{
		for (size_t i = 0; i < FreeEvents.size(); i++)
		{
			FreeEvents[i]();
		}
	}

	if (true == IsUp)
	{
		for (size_t i = 0; i < UpEvents.size(); i++)
		{
			UpEvents[i]();
		}
	}
}

UEngineInput& UEngineInput::GetInst()
{
	static UEngineInput Inst = UEngineInput();
	return Inst;
}

UEngineInput::UEngineInput()
{
	Keys.insert({ 'Q', UEngineKey('Q') });
	Keys.insert({ 'W', UEngineKey('W') });
	Keys.insert({ 'E', UEngineKey('E') });
	Keys.insert({ 'R', UEngineKey('R') });
	Keys.insert({ 'T', UEngineKey('T') });
	Keys.insert({ 'Y', UEngineKey('Y') });
	Keys.insert({ 'U', UEngineKey('U') });
	Keys.insert({ 'I', UEngineKey('I') });
	Keys.insert({ 'O', UEngineKey('O') });
	Keys.insert({ 'P', UEngineKey('P') });
	Keys.insert({ 'A', UEngineKey('A') });
	Keys.insert({ 'S', UEngineKey('S') });
	Keys.insert({ 'D', UEngineKey('D') });
	Keys.insert({ 'F', UEngineKey('F') });
	Keys.insert({ 'G', UEngineKey('G') });
	Keys.insert({ 'H', UEngineKey('H') });
	Keys.insert({ 'J', UEngineKey('J') });
	Keys.insert({ 'K', UEngineKey('K') });
	Keys.insert({ 'L', UEngineKey('L') });
	Keys.insert({ 'Z', UEngineKey('Z') });
	Keys.insert({ 'X', UEngineKey('X') });
	Keys.insert({ 'C', UEngineKey('C') });
	Keys.insert({ 'V', UEngineKey('V') });
	Keys.insert({ 'B', UEngineKey('B') });
	Keys.insert({ 'N', UEngineKey('N') });
	Keys.insert({ 'M', UEngineKey('M') });

	Keys.insert({ '1', UEngineKey('1') });
	Keys.insert({ '2', UEngineKey('2') });
	Keys.insert({ '3', UEngineKey('3') });
	Keys.insert({ '4', UEngineKey('4') });
	Keys.insert({ '5', UEngineKey('5') });
	Keys.insert({ '6', UEngineKey('6') });
	Keys.insert({ '7', UEngineKey('7') });
	Keys.insert({ '8', UEngineKey('8') });
	Keys.insert({ '9', UEngineKey('9') });
	Keys.insert({ '0', UEngineKey('0') });

	Keys.insert({ VK_RETURN, UEngineKey(VK_RETURN) });

	Keys.insert({ VK_LBUTTON , UEngineKey(VK_LBUTTON) });
	Keys.insert({ VK_RBUTTON , UEngineKey(VK_RBUTTON) });
	Keys.insert({ VK_LEFT , UEngineKey(VK_LEFT) });
	Keys.insert({ VK_RIGHT , UEngineKey(VK_RIGHT) });
	Keys.insert({ VK_UP , UEngineKey(VK_UP) });
	Keys.insert({ VK_DOWN , UEngineKey(VK_DOWN) });

	Keys.insert({ VK_LEFT, UEngineKey(VK_LEFT) });
	Keys.insert({ VK_RIGHT, UEngineKey(VK_RIGHT) });
	Keys.insert({ VK_UP, UEngineKey(VK_UP) });
	Keys.insert({ VK_DOWN, UEngineKey(VK_DOWN) });

	Keys.insert({ VK_SPACE		, UEngineKey(VK_SPACE) });
	Keys.insert({ VK_PRIOR		, UEngineKey(VK_PRIOR) });
	Keys.insert({ VK_NEXT		, UEngineKey(VK_NEXT) });
	Keys.insert({ VK_END		, UEngineKey(VK_END) });
	Keys.insert({ VK_HOME		, UEngineKey(VK_HOME) });
	Keys.insert({ VK_SELECT		, UEngineKey(VK_SELECT) });
	Keys.insert({ VK_PRINT		, UEngineKey(VK_PRINT) });
	Keys.insert({ VK_EXECUTE	, UEngineKey(VK_EXECUTE) });
	Keys.insert({ VK_SNAPSHOT	, UEngineKey(VK_SNAPSHOT) });
	Keys.insert({ VK_INSERT		, UEngineKey(VK_INSERT) });
	Keys.insert({ VK_DELETE		, UEngineKey(VK_DELETE) });
	Keys.insert({ VK_HELP		, UEngineKey(VK_HELP) });


	Keys.insert({ VK_NUMPAD0	, UEngineKey(VK_NUMPAD0) });
	Keys.insert({ VK_NUMPAD1	, UEngineKey(VK_NUMPAD1) });
	Keys.insert({ VK_NUMPAD2	, UEngineKey(VK_NUMPAD2) });
	Keys.insert({ VK_NUMPAD3	, UEngineKey(VK_NUMPAD3) });
	Keys.insert({ VK_NUMPAD4	, UEngineKey(VK_NUMPAD4) });
	Keys.insert({ VK_NUMPAD5	, UEngineKey(VK_NUMPAD5) });
	Keys.insert({ VK_NUMPAD6	, UEngineKey(VK_NUMPAD6) });
	Keys.insert({ VK_NUMPAD7	, UEngineKey(VK_NUMPAD7) });
	Keys.insert({ VK_NUMPAD8	, UEngineKey(VK_NUMPAD8) });
	Keys.insert({ VK_NUMPAD9	, UEngineKey(VK_NUMPAD9) });
	Keys.insert({ VK_MULTIPLY	, UEngineKey(VK_MULTIPLY) });
	Keys.insert({ VK_ADD		, UEngineKey(VK_ADD) });
	Keys.insert({ VK_SEPARATOR	, UEngineKey(VK_SEPARATOR) });
	Keys.insert({ VK_SUBTRACT	, UEngineKey(VK_SUBTRACT) });
	Keys.insert({ VK_DECIMAL	, UEngineKey(VK_DECIMAL) });
	Keys.insert({ VK_DIVIDE	, UEngineKey(VK_DIVIDE) });
	Keys.insert({ VK_F1		, UEngineKey(VK_F1) });
	Keys.insert({ VK_F2		, UEngineKey(VK_F2) });
	Keys.insert({ VK_F3		, UEngineKey(VK_F3) });
	Keys.insert({ VK_F4		, UEngineKey(VK_F4) });
	Keys.insert({ VK_F5		, UEngineKey(VK_F5) });
	Keys.insert({ VK_F6		, UEngineKey(VK_F6) });
	Keys.insert({ VK_F7		, UEngineKey(VK_F7) });
	Keys.insert({ VK_F8		, UEngineKey(VK_F8) });
	Keys.insert({ VK_F9		, UEngineKey(VK_F9) });
	Keys.insert({ VK_F10		, UEngineKey(VK_F10) });
	Keys.insert({ VK_F11		, UEngineKey(VK_F11) });
	Keys.insert({ VK_F12		, UEngineKey(VK_F12) });
	Keys.insert({ VK_F13		, UEngineKey(VK_F13) });
	Keys.insert({ VK_F14		, UEngineKey(VK_F14) });
	Keys.insert({ VK_F15		, UEngineKey(VK_F15) });
	Keys.insert({ VK_F16		, UEngineKey(VK_F16) });
	Keys.insert({ VK_F17		, UEngineKey(VK_F17) });
	Keys.insert({ VK_F18		, UEngineKey(VK_F18) });
	Keys.insert({ VK_F19		, UEngineKey(VK_F19) });
	Keys.insert({ VK_F20		, UEngineKey(VK_F20) });
	Keys.insert({ VK_F21		, UEngineKey(VK_F21) });
	Keys.insert({ VK_F22		, UEngineKey(VK_F22) });
	Keys.insert({ VK_F23		, UEngineKey(VK_F23) });
	Keys.insert({ VK_F24		, UEngineKey(VK_F24) });

}

void UEngineInput::EventCheck(float _DeltaTime)
{
	std::map<int, UEngineKey>::iterator StartIter = GetInst().Keys.begin();
	std::map<int, UEngineKey>::iterator EndIter = GetInst().Keys.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		UEngineKey& CurKey = StartIter->second;
		CurKey.EventCheck();
	}
}

void UEngineInput::KeyCheck(float _DeltaTime)
{
	std::map<int, UEngineKey>::iterator StartIter = GetInst().Keys.begin();
	std::map<int, UEngineKey>::iterator EndIter = GetInst().Keys.end();

	for (; StartIter != EndIter; ++StartIter)
	{
		UEngineKey& CurKey = StartIter->second;
		CurKey.KeyCheck(_DeltaTime);
	}

	EventCheck(_DeltaTime);
}

UEngineInput::~UEngineInput()
{
}

void UEngineInput::BindAction(int _KeyIndex, KeyEvent _EventType, std::function<void() > _Function)
{
	if (false == Keys.contains(_KeyIndex))
	{
		MSGASSERT("아직도 등록되지 않은 키가 존재합니다.");
		return;
	}

	switch (_EventType)
	{
	case KeyEvent::Down:
		Keys[_KeyIndex].DownEvents.push_back(_Function);
		break;
	case KeyEvent::Press:
		Keys[_KeyIndex].PressEvents.push_back(_Function);
		break;
	case KeyEvent::Free:
		Keys[_KeyIndex].FreeEvents.push_back(_Function);
		break;
	case KeyEvent::Up:
		Keys[_KeyIndex].UpEvents.push_back(_Function);
		break;
	default:
		break;
	}


}