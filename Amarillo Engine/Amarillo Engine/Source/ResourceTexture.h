#ifndef __ResoruceTexture_H__
#define __ResoruceTexture_H__

#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "Color.h"
#include "Resource.h"
#include "../External/MathGeoLib/include/Math/float2.h"
#include "../External/MathGeoLib/include/Math/float3.h"

class ResourceTexture : public Resource
{
public:

	ResourceTexture(uuids::uuid unique_id);
	virtual ~ResourceTexture();

	void SetData(byte* _texture_data, uint _texture_data_lenght, uint _width, uint _height, int _format, uint _wrap_s, uint _wrap_t, uint _mag, uint _min);
	void SetFlipped(bool set);

	uint GetTextureId();
	float2 GetSize();
	byte* GetTextureData();
	uint GetTextureDataSize();
	bool GetFlipped();

private:
	void LoadToMemory();
	void UnloadFromMemory();

private:
	byte*  texture_data = nullptr;
	uint   texture_data_size = 0;
	uint   texture_id = 0;
	int	   format = 0;
	float2 size = float2(0, 0);
	uint   wrap_s = 0;
	uint   wrap_t = 0;
	uint   mag = 0;
	uint   min = 0;
	bool   flipped = false;

};
#endif //__ResoruceTexture_H__
