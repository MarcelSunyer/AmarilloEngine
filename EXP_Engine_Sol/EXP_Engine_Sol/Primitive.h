
#pragma once
#include "Color.h"
#include "MathGeoLib/include/Math/float4x4.h"
#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Math/TransformOps.h"

enum PrimitiveTypes
{
	Primitive_Point,
	Primitive_Line,
	Primitive_Plane,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder
};

class Primitive
{
public:

	Primitive();

	virtual void	Render() const;
	virtual void	InnerRender() const;
	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const float3& u);
	void			Scale(float x, float y, float z);
	PrimitiveTypes	GetType() const;

public:
	
	Color color;
	float4x4 transform;
	bool axis,wire;

protected:
	PrimitiveTypes type;
};

// ============================================
class Cube : public Primitive
{
public :
	Cube();
	Cube(float sizeX, float sizeY, float sizeZ);
	void InnerRender() const;
public:
	float3 size;
};

// ============================================
class CCylinder : public Primitive
{
public:
	CCylinder();
	CCylinder(float radius, float height);
	void InnerRender() const;
public:
	float radius;
	float height;
};

// ============================================
class CLine : public Primitive
{
public:
	CLine();
	CLine(float x, float y, float z);
	void InnerRender() const;
public:
	float3 origin;
	float3 destination;
};

// ============================================
class CPlane : public Primitive
{
public:
	CPlane();
	CPlane(float x, float y, float z, float d);
	void InnerRender() const;
public:
	float3 normal;
	float constant;
};