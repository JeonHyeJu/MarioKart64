#pragma once
#include <vector>
#include <string>
#include "EngineMath.h"

class UEngineSerializer
{
public:
	ENGINEAPI UEngineSerializer();
	ENGINEAPI ~UEngineSerializer();

	//UEngineSerializer(const UEngineSerializer& _Other) = delete;
	//UEngineSerializer(UEngineSerializer&& _Other) noexcept = delete;
	//UEngineSerializer& operator=(const UEngineSerializer& _Other) = delete;
	//UEngineSerializer& operator=(UEngineSerializer&& _Other) noexcept = delete;

	ENGINEAPI void Write(const void* _Data, unsigned int _Size);

	void operator<<(const int& _Data)
	{
		Write(&_Data, sizeof(int));
	}

	void operator<<(const bool& _Data)
	{
		Write(&_Data, sizeof(bool));
	}

	void operator<<(const FVector& _Data)
	{
		Write(&_Data, sizeof(FVector));
	}

	void operator<<(const FIntPoint& _Data)
	{
		Write(&_Data, sizeof(FIntPoint));
	}

	void operator<<(const std::string& _Data)
	{
		int Size = static_cast<int>(_Data.size());
		operator<<(Size);
		if (0 != Size)
		{
			Write(&_Data[0], static_cast<int>(_Data.size()));
		}
	}

	void operator<<(class ISerializObject& _Data);

	template<typename DataType>
	void operator<<(std::vector<DataType>& _vector)
	{
		int Size = static_cast<int>(_vector.size());
		operator<<(Size);

		for (size_t i = 0; i < _vector.size(); i++)
		{
			operator<<(_vector[i]);
		}
	}

	ENGINEAPI void Read(void* _Data, unsigned int _Size);

	void operator>>(int& _Data)
	{
		Read(&_Data, sizeof(int));
	}

	void operator>>(bool& _Data)
	{
		Read(&_Data, sizeof(bool));
	}

	void operator>>(FVector& _Data)
	{
		Read(&_Data, sizeof(FVector));
	}

	void operator>>(FIntPoint& _Data)
	{
		Read(&_Data, sizeof(FIntPoint));
	}

	void operator>>(std::string& _Data)
	{
		int Size;
		operator>>(Size);
		_Data.resize(Size);

		Read(&_Data[0], static_cast<int>(_Data.size()));
	}

	void operator>>(class ISerializObject& _Data);

	template<typename DataType>
	void operator>>(std::vector<DataType>& _vector)
	{
		int Size = 0;
		operator>>(Size);
		_vector.resize(Size);

		for (size_t i = 0; i < _vector.size(); i++)
		{
			operator>>(_vector[i]);
		}
	}

	void* GetDataPtr()
	{
		return &Data[0];
	}

	int GetWriteOffset()
	{
		return WriteOffset;
	}

	void DataResize(int _Value)
	{
		Data.resize(_Value);
	}

	size_t GetDataSize()
	{
		return Data.size();
	}

protected:

private:
	int WriteOffset = 0;
	int ReadOffset = 0;
	std::vector<char> Data;
};

class ISerializObject
{
public:
	ENGINEAPI virtual ~ISerializObject() = 0
	{

	}

public:
	ENGINEAPI virtual void Serialize(UEngineSerializer& _Ser);
	ENGINEAPI virtual void DeSerialize(UEngineSerializer& _Ser);
};

