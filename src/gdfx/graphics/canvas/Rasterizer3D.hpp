// ----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
// ----------------------------------------------------------------------------
/**
* Rasterizer3D.hpp
* 
* A lot of the 3D concepts here (and the frustum clipping code) are from
* "3D Graphics Programming from Scratch" by Gustavo Pezzi (pikuma.com).
*/
#ifndef GDFX_GRAPHICS_CANVAS_RASTERIZER3D_HPP
#define GDFX_GRAPHICS_CANVAS_RASTERIZER3D_HPP

#include <cstdint>
#include <vector>

#include <gdfx/math/Vector2.hpp>
#include <gdfx/math/Vector3.hpp>
#include <gdfx/math/Matrix4.hpp>
#include <gdfx/graphics/canvas/Canvas.hpp>

namespace gdfx {

/*
* Rasterizer3D - A software-based 3D rasterizer.
*/
class Rasterizer3D {
public:
	enum class RenderMode {
		WIREFRAME,
		FLAT_SHADED,
		FLAT_SHADED_WIRE
	};

	enum TriangleFlags {
		TRI_FLAG_NONE = 0x00,
		TRI_FLAG_EMISSIVE = 0x01
	};

	struct Triangle3D {
		Vector3 vertices[3];
		float z; // average z depth
		int color;
		uint8_t flags;
	};

	struct Light {
		Vector3 direction;
	};

	struct Plane {
		Vector3 point;
		Vector3 normal;
	};

	static const int MAX_NUM_POLY3D_VERTICES = 10;
	static const int NUM_FRUSTUM_PLANES = 6;

	struct Polygon3D {
		Vector3 vertices[MAX_NUM_POLY3D_VERTICES];
		int numVertices;
		float z; // average z depth
		int color;
		uint8_t flags;
	};

	struct Frustum {
		enum class FrustumPlane {
			LEFT,
			RIGHT,
			TOP,
			BOTTOM,
			NEAR,
			FAR
		};

		Plane planes[NUM_FRUSTUM_PLANES];

		void initialize(float fovx, float fovy, float z_near, float z_far)
		{
			float cos_half_fovx = cos(fovx / 2);
			float sin_half_fovx = sin(fovx / 2);

			float cos_half_fovy = cos(fovy / 2);
			float sin_half_fovy = sin(fovy / 2);

			planes[(int)FrustumPlane::LEFT].point = Vector3{ 0, 0, 0 };
			planes[(int)FrustumPlane::LEFT].normal.x = cos_half_fovx;
			planes[(int)FrustumPlane::LEFT].normal.y = 0;
			planes[(int)FrustumPlane::LEFT].normal.z = sin_half_fovx;

			planes[(int)FrustumPlane::RIGHT].point = Vector3{ 0, 0, 0 };
			planes[(int)FrustumPlane::RIGHT].normal.x = -cos_half_fovx;
			planes[(int)FrustumPlane::RIGHT].normal.y = 0;
			planes[(int)FrustumPlane::RIGHT].normal.z = sin_half_fovx;

			planes[(int)FrustumPlane::TOP].point = Vector3{ 0, 0, 0 };
			planes[(int)FrustumPlane::TOP].normal.x = 0;
			planes[(int)FrustumPlane::TOP].normal.y = -cos_half_fovy;
			planes[(int)FrustumPlane::TOP].normal.z = sin_half_fovy;

			planes[(int)FrustumPlane::BOTTOM].point = Vector3{ 0, 0, 0 };
			planes[(int)FrustumPlane::BOTTOM].normal.x = 0;
			planes[(int)FrustumPlane::BOTTOM].normal.y = cos_half_fovy;
			planes[(int)FrustumPlane::BOTTOM].normal.z = sin_half_fovy;

			planes[(int)FrustumPlane::NEAR].point = Vector3{ 0, 0, z_near };
			planes[(int)FrustumPlane::NEAR].normal.x = 0;
			planes[(int)FrustumPlane::NEAR].normal.y = 0;
			planes[(int)FrustumPlane::NEAR].normal.z = 1;

			planes[(int)FrustumPlane::FAR].point = Vector3{ 0, 0, z_far };
			planes[(int)FrustumPlane::FAR].normal.x = 0;
			planes[(int)FrustumPlane::FAR].normal.y = 0;
			planes[(int)FrustumPlane::FAR].normal.z = -1;
		}
	};

public:
	Rasterizer3D();
	~Rasterizer3D();

	void create(Canvas *canvas);

	int getWidth() const;
	int getHeight() const;
	int getHalfWidth() const;
	int getHalfHeight() const;

	void drawLine3D(float x1, float y1, float z1, float x2, float y2, float z2, int color);
	void drawTriangle3D(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, int color);
	void drawFilledTriangle3D(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, int color);

	// delayed 3D rendering with depth sorting, etc
	void renderSetWireframeColor(int color);
	void renderEnableLighting(bool enable);
	bool renderIsLightingEnabled() const;
	void renderSetLight(float x, float y, float z);
	void renderBegin(RenderMode mode);
	void renderEnd();
	void renderTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, int color, uint8_t flags);

	int getNumTriangles() const { return numTriangles; }
	int getNumTrianglesAfterClipping() const { return numTrianglesAfterClipping; }
	int getNumTrianglesCulled() const { return numTrianglesCulled; }
	int getNumTrianglesDrawn() const { return numTrianglesDrawn; }

protected:
	void depthSortTriangles();
	Vector3 calculateNormal(const Triangle3D& tri);
	bool shouldCull(const Triangle3D& tri, const Vector3& normal);
	int applyEmissiveLighting(const Triangle3D& tri);
	int applyLighting(const Triangle3D& tri, const Vector3& normal);
	void triangleToPolygon(const Triangle3D& tri, Polygon3D& poly);
	void triangulate(const Polygon3D& poly, std::vector<Triangle3D>& triangles);
	void clipPolygonAgainstPlane(Polygon3D& poly, int plane);
	void clipPolygon(Polygon3D& poly);
	
private:
	Canvas *canvas;
	int halfWidth;
	int halfHeight;
	float hfov;
	float vfov;
	float horizPerspCorrection;
	float vertPerspCorrection;
	std::vector<Triangle3D> triangles;
	std::vector<Triangle3D> clippedTriangles;
	RenderMode mode;
	int renderWireframeColor;
	bool lightingEnabled;
	Light light;
	Matrix4 projMatrix;
	Frustum frustum;
	int numTriangles;
	int numTrianglesAfterClipping;
	int numTrianglesCulled;
	int numTrianglesDrawn;
};

} // gdfx

#endif // GDFX_GRAPHICS_CANVAS_RASTERIZER3D_HPP


