#pragma once
#include <EngineBase/EngineDelegate.h>
#include <EngineBase/EngineDebug.h>

#include <vector>
#include <functional>

enum class KeyEvent
{
	Down,
	Press,
	Free,
	Up,
};

class UEngineInput
{
public:
	~UEngineInput();

	UEngineInput(const UEngineInput& _Other) = delete;
	UEngineInput(UEngineInput&& _Other) noexcept = delete;
	UEngineInput& operator=(const UEngineInput& _Other) = delete;
	UEngineInput& operator=(UEngineInput&& _Other) noexcept = delete;

private:
	class UEngineKey
	{
	public:
		int Key = -1;

		bool IsDown = false;

		bool IsPress = false;

		bool IsUp = false;
		
		bool IsFree = true;

		float PressTime = 0.0f;
		float FreeTime = 0.0f;

		std::vector<std::function<void()>> PressEvents;
		std::vector<std::function<void()>> DownEvents;
		std::vector<std::function<void()>> UpEvents;
		std::vector<std::function<void()>> FreeEvents;

		void Reset()
		{
			IsDown = false;
			IsPress = false;
			IsUp = false;
			IsFree = true;
			PressTime = 0.0f;
			FreeTime = 0.0f;
		}

		UEngineKey()
		{
		}

		UEngineKey(int _Key)
			: Key(_Key)
		{
		}

		void EventCheck();

		void KeyCheck(float _DeltaTime);
		
	};

	ENGINEAPI static UEngineInput& GetInst();
	ENGINEAPI static void EventCheck(float _DeltaTime);

public:
	ENGINEAPI static void KeyCheck(float _DeltaTime);

	ENGINEAPI static void KeyReset();

	static bool IsDoubleClick(int _KeyIndex, float _Time)
	{
		if (false == GetInst().Keys.contains(_KeyIndex))
		{
			MSGASSERT("아직도 등록되지 않은 키가 존재합니다.");
			return false;
		}

		return GetInst().Keys[_KeyIndex].IsDown && GetInst().Keys[_KeyIndex].FreeTime < _Time;
	}

	static bool IsDown(int _KeyIndex)
	{
		if (false == GetInst().Keys.contains(_KeyIndex))
		{
			MSGASSERT("아직도 등록되지 않은 키가 존재합니다.");
			return false;
		}

		return GetInst().Keys[_KeyIndex].IsDown;
	}

	static bool IsUp(int _KeyIndex)
	{
		if (false == GetInst().Keys.contains(_KeyIndex))
		{
			MSGASSERT("아직도 등록되지 않은 키가 존재합니다.");
			return false;
		}

		return GetInst().Keys[_KeyIndex].IsUp;
	}

	static bool IsPress(int _KeyIndex)
	{
		if (false == GetInst().Keys.contains(_KeyIndex))
		{
			MSGASSERT("아직도 등록되지 않은 키가 존재합니다.");
			return false;
		}

		return GetInst().Keys[_KeyIndex].IsPress;
	}

	static float IsPressTime(int _KeyIndex)
	{
		if (false == GetInst().Keys.contains(_KeyIndex))
		{
			MSGASSERT("아직도 등록되지 않은 키가 존재합니다.");
			return false;
		}

		return GetInst().Keys[_KeyIndex].PressTime;
	}


	static bool IsFree(int _KeyIndex)
	{
		if (false == GetInst().Keys.contains(_KeyIndex))
		{
			MSGASSERT("아직도 등록되지 않은 키가 존재합니다.");
			return false;
		}

		return GetInst().Keys[_KeyIndex].IsFree;
	}

	void BindAction(int _KeyIndex, KeyEvent _EventType,  std::function<void()> _Function);

protected:

private:
	std::map<int, UEngineKey> Keys;

	UEngineInput();

};

