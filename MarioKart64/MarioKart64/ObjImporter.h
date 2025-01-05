#pragma once
class ObjImporter
{
public:
	ObjImporter();
	~ObjImporter();

	ObjImporter(const ObjImporter& _other) = delete;
	ObjImporter(ObjImporter&& _other) noexcept = delete;
	ObjImporter& operator=(const ObjImporter& _other) = delete;
	ObjImporter& operator=(ObjImporter&& _other) noexcept = delete;
};
