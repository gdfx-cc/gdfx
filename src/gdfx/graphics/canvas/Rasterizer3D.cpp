// ----------------------------------------------------------------------------
// Copyright (C) GDFX Authors
// ----------------------------------------------------------------------------
#include <cassert>
#include <cstring>
#include <algorithm>

#include <gdfx/graphics/canvas/Rasterizer3D.hpp>
#include <gdfx/math/Math.hpp>
#include <gdfx/math/Bezier.hpp>

namespace gdfx {

Rasterizer3D::Rasterizer3D() :
	canvas{ nullptr },
	halfWidth{ 0 },
	halfHeight{ 0 },
	hfov{ 0 },
	vfov{ 0 },
	horizPerspCorrection{ 0 },
	vertPerspCorrection{ 0 },
	triangles{},
	clippedTriangles{},
	mode{ RenderMode::WIREFRAME },
	renderWireframeColor{ 1 },
	lightingEnabled{ true },
	light{},
	frustum{},
	numTriangles{0},
	numTrianglesAfterClipping{0},
	numTrianglesCulled{0},
	numTrianglesDrawn{0}
{
}

Rasterizer3D::~Rasterizer3D()
{
}

void Rasterizer3D::create(Canvas *canvas)
{
	this->canvas = canvas;

	// prepare for 3d projection
	halfWidth = canvas->getWidth() / 2;
	halfHeight = canvas->getHeight() / 2;
	hfov = Math::degreesToRadians(75.0f);
	vfov = Math::degreesToRadians(60.0f);
	horizPerspCorrection = (float)halfWidth / tan(hfov / 2.0f);
	vertPerspCorrection = (float)halfHeight / tan(vfov / 2.0f);

	// initialize the perspective projection matrix
	float aspectx = (float)canvas->getWidth() / (float)canvas->getHeight();
	float aspecty = (float)canvas->getHeight() / (float)canvas->getWidth();
	float fovy = Math::PI / 3.0f; // 60 degrees
	float fovx = atan(tan(fovy / 2.0) * aspectx) * 2.0;
	float z_near = 0.1f;
	float z_far = 500.0f;
	projMatrix.makePerspective(fovy, aspecty, z_near, z_far);

	// Initialize frustum planes with a point and a normal
	frustum.initialize(fovx, fovy, z_near, z_far);
}

int Rasterizer3D::getWidth() const
{
	return canvas->getWidth();
}

int Rasterizer3D::getHeight() const
{
	return canvas->getHeight();
}

int Rasterizer3D::getHalfWidth() const
{
	return halfWidth;
}

int Rasterizer3D::getHalfHeight() const
{
	return halfHeight;
}

void Rasterizer3D::drawLine3D(float x1, float y1, float z1, float x2, float y2, float z2, int color)
{
	// transform 3d to 2d
	Vector4 v[2] = {
		{ x1, y1, z1, 1.0f },
		{ x2, y2, z2, 1.0f }
	};

	for (int i = 0; i < 2; i++) {
		v[i] = projMatrix.multiplyAndProject(v[i]);
	
		// Scale into the view
		v[i].x *= halfWidth - 1;
		v[i].y *= halfHeight - 1;

		// Invert y value to account for flipped screen y coordinate
		v[i].y *= -1;

		// Translate the projected points to the middle of the screen
		v[i].x += halfWidth;
		v[i].y += halfHeight;
	}
	
	// draw line using transformed points
	canvas->drawLine(v[0].x, v[0].y, v[1].x, v[1].y, color);
}

void Rasterizer3D::drawTriangle3D(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, int color)
{
	// transform 3d to 2d
	Vector4 v[3] = {
		{ x1, y1, z1, 1.0f },
		{ x2, y2, z2, 1.0f },
		{ x3, y3, z3, 1.0f }
	};

	for (int i = 0; i < 3; i++) {
		v[i] = projMatrix.multiplyAndProject(v[i]);
	
		// Scale into the view
		v[i].x *= halfWidth - 1;
		v[i].y *= halfHeight - 1;

		// Invert y value to account for flipped screen y coordinate
		v[i].y *= -1;

		// Translate the projected points to the middle of the screen
		v[i].x += halfWidth;
		v[i].y += halfHeight;
	}
	
	// draw wireframe triangle using transformed points
	canvas->drawTriangle(v[0].x, v[0].y, v[1].x, v[1].y, v[2].x, v[2].y, color);
}

void Rasterizer3D::drawFilledTriangle3D(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, int color)
{
	// transform 3d to 2d
	Vector4 v[3] = {
		{ x1, y1, z1, 1.0f },
		{ x2, y2, z2, 1.0f },
		{ x3, y3, z3, 1.0f }
	};

	for (int i = 0; i < 3; i++) {
		v[i] = projMatrix.multiplyAndProject(v[i]);

		// Scale into the view
		v[i].x *= halfWidth - 1;
		v[i].y *= halfHeight - 1;

		// Invert y value to account for flipped screen y coordinate
		v[i].y *= -1;

		// Translate the projected points to the middle of the screen
		v[i].x += halfWidth;
		v[i].y += halfHeight;
	}

	// draw filled triangle using transformed points
	canvas->drawFilledTriangle(v[0].x, v[0].y, v[1].x, v[1].y, v[2].x, v[2].y, color);
}

void Rasterizer3D::renderSetWireframeColor(int color)
{
	renderWireframeColor = color;
}

void Rasterizer3D::renderEnableLighting(bool enable)
{
	lightingEnabled = enable;
}

bool Rasterizer3D::renderIsLightingEnabled() const
{
	return lightingEnabled;
}

void Rasterizer3D::renderSetLight(float x, float y, float z)
{
	light.direction.x = x;
	light.direction.y = y;
	light.direction.z = z;
}

void Rasterizer3D::renderBegin(RenderMode mode)
{
	triangles.clear();
	clippedTriangles.clear();
	this->mode = mode;
}

void Rasterizer3D::renderEnd()
{
	int numAfterClipping, numCulled, numDrawn;
	Polygon3D poly;

	numAfterClipping = 0;
	numCulled = 0;
	numDrawn = 0;

	depthSortTriangles();

	switch (mode) {
	case RenderMode::WIREFRAME:
		for (auto& t : triangles) {
			
			triangleToPolygon(t, poly);
			clipPolygon(poly);
			triangulate(poly, clippedTriangles);

			numAfterClipping += clippedTriangles.size();

			for (auto& tri : clippedTriangles) {
				drawTriangle3D(
					tri.vertices[0].x, tri.vertices[0].y, tri.vertices[0].z,
					tri.vertices[1].x, tri.vertices[1].y, tri.vertices[1].z,
					tri.vertices[2].x, tri.vertices[2].y, tri.vertices[2].z,
					renderWireframeColor
				);
				numDrawn++;
			}
		}
		break;
	case RenderMode::FLAT_SHADED:
		for (auto& t : triangles) {
			Vector3 normal = calculateNormal(t);
			if (shouldCull(t, normal)) {
				numCulled++;
				continue;
			}

			triangleToPolygon(t, poly);
			clipPolygon(poly);
			triangulate(poly, clippedTriangles);

			numAfterClipping += clippedTriangles.size();

			for (auto& tri : clippedTriangles) {
				int color = 0;
				if (lightingEnabled) {
					if (tri.flags & TRI_FLAG_EMISSIVE)
						color = applyEmissiveLighting(tri);
					else
						color = applyLighting(tri, normal);
				}
				else {
					color = tri.color;
				}

				drawFilledTriangle3D(
					tri.vertices[0].x, tri.vertices[0].y, tri.vertices[0].z,
					tri.vertices[1].x, tri.vertices[1].y, tri.vertices[1].z,
					tri.vertices[2].x, tri.vertices[2].y, tri.vertices[2].z,
					color
				);
				numDrawn++;
			}
		}
		break;
	case RenderMode::FLAT_SHADED_WIRE:
		for (auto& t : triangles) {
			Vector3 normal = calculateNormal(t);
			if (shouldCull(t, normal)) {
				numCulled++;
				continue;
			}

			triangleToPolygon(t, poly);
			clipPolygon(poly);
			triangulate(poly, clippedTriangles);
			
			numAfterClipping += clippedTriangles.size();

			for (auto& tri : clippedTriangles) {
				//int color = applyLighting(tri, normal);

				drawFilledTriangle3D(
					tri.vertices[0].x, tri.vertices[0].y, tri.vertices[0].z,
					tri.vertices[1].x, tri.vertices[1].y, tri.vertices[1].z,
					tri.vertices[2].x, tri.vertices[2].y, tri.vertices[2].z,
					tri.color
				);
				drawTriangle3D(
					tri.vertices[0].x, tri.vertices[0].y, tri.vertices[0].z,
					tri.vertices[1].x, tri.vertices[1].y, tri.vertices[1].z,
					tri.vertices[2].x, tri.vertices[2].y, tri.vertices[2].z,
					renderWireframeColor
				);
				numDrawn++;
			}
		}
		break;
	default:
		break;
	}

	numTriangles = triangles.size();
	numTrianglesAfterClipping = numAfterClipping;
	numTrianglesCulled = numCulled;
	numTrianglesDrawn = numDrawn;
}

void Rasterizer3D::renderTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, int color, uint8_t flags)
{
	Triangle3D t;

	t.vertices[0].x = x1;
	t.vertices[0].y = y1;
	t.vertices[0].z = z1;

	t.vertices[1].x = x2;
	t.vertices[1].y = y2;
	t.vertices[1].z = z2;

	t.vertices[2].x = x3;
	t.vertices[2].y = y3;
	t.vertices[2].z = z3;

	// store average z for depth sorting
	t.z = (t.vertices[0].z + t.vertices[1].z + t.vertices[2].z) / 3.0f; 
	t.color = color;
	t.flags = flags;

	triangles.push_back(t);
}

static int triSort(const void *a, const void *b)
{
	auto t1 = *(Rasterizer3D::Triangle3D *)a;
	auto t2 = *(Rasterizer3D::Triangle3D *)b;

	if (t2.z < t1.z)
		return -1;
	else if (t2.z == t1.z)
		return 0;
	else
		return 1;
}

void Rasterizer3D::depthSortTriangles()
{
	qsort(&triangles[0], triangles.size(), sizeof(Triangle3D), triSort);
}

Vector3 Rasterizer3D::calculateNormal(const Triangle3D& tri)
{
	Vector3 a = tri.vertices[0];
	Vector3 b = tri.vertices[1];
	Vector3 c = tri.vertices[2];

	Vector3 ab = b - a;
	Vector3 ac = c - a;

	ab.normalize();
	ac.normalize();

	Vector3 normal = Vector3::cross(ab, ac);
	normal.normalize();

	return normal;
}

// backface culling
bool Rasterizer3D::shouldCull(const Triangle3D& tri, const Vector3& normal)
{
	// find the vector between a point in the triangle and the camera origin
	Vector3 origin(0, 0, 0);
	Vector3 cameraRay = origin - tri.vertices[0];

	// calcuate how aligned the camera ray is with the face normal using dot product
	float dotNormalCamera = Vector3::dot(normal, cameraRay);

	if (dotNormalCamera < 0)
		return true;

	return false;
}

int Rasterizer3D::applyEmissiveLighting(const Triangle3D& tri)
{
	float lightIntensityFactor = 1.0f;
	int shift = (int)(((lightIntensityFactor * 2.0f) - 1.0f) * 4.0f);

	// offset to the middle 16-color portion of the palette and then shift left or right depending on the light intensity to make it darker or brighter
	return tri.color + (5*16) + (shift*16);
}

int Rasterizer3D::applyLighting(const Triangle3D& tri, const Vector3& normal)
{
	light.direction.normalize();

	float lightIntensityFactor = -Vector3::dot(normal, -light.direction);
	
	if (lightIntensityFactor < 0.0f)
		lightIntensityFactor = 0.0f;
	if (lightIntensityFactor > 1.0f)
		lightIntensityFactor = 1.0f;

	int shift = (int)(((lightIntensityFactor * 2.0f) - 1.0f) * 4.0f);

	// offset to the middle 16-color portion of the palette and then shift left or right depending on the light intensity to make it darker or brighter
	return tri.color + (5*16) + (shift*16);
}

void Rasterizer3D::triangleToPolygon(const Triangle3D& tri, Polygon3D& poly)
{
	poly.vertices[0] = tri.vertices[0];
	poly.vertices[1] = tri.vertices[1];
	poly.vertices[2] = tri.vertices[2];
	poly.numVertices = 3;
	poly.color = tri.color;
	poly.flags = tri.flags;
	poly.z = tri.z;
}

void Rasterizer3D::triangulate(const Polygon3D& poly, std::vector<Triangle3D>& triangles)
{
	triangles.clear();

	for (int i = 0; i < poly.numVertices - 2; i++) {
		int v0 = 0;
		int v1 = i + 1;
		int v2 = i + 2;

		Triangle3D tri;

		tri.vertices[0] = poly.vertices[v0];
		tri.vertices[1] = poly.vertices[v1];
		tri.vertices[2] = poly.vertices[v2];
		tri.color = poly.color;
		tri.flags = poly.flags;
		tri.z = poly.z;

		triangles.push_back(tri);
	}
}

void Rasterizer3D::clipPolygonAgainstPlane(Polygon3D& poly, int plane)
{
	Vector3 plane_point = frustum.planes[plane].point;
	Vector3 plane_normal = frustum.planes[plane].normal;

	// The array of inside vertices that will be part of the final polygon returned via parameter
	Vector3 inside_vertices[MAX_NUM_POLY3D_VERTICES];
	int num_inside_vertices = 0;

	// Start current and previous vertex with the first and last polygon vertices
	Vector3 *current_vertex = &poly.vertices[0];
	Vector3 *previous_vertex = &poly.vertices[poly.numVertices - 1];

	// Start the current and previous dot product to help determine if a point is inside a plane
	float current_dot = 0;
	float previous_dot = Vector3::dot(*previous_vertex - plane_point, plane_normal);

	// Loop while the current verte is different than the last vertex
	while (current_vertex != &poly.vertices[poly.numVertices]) {
		current_dot = Vector3::dot(*current_vertex - plane_point, plane_normal);

		// If we changed from inside to outside or vice-versa
		if (current_dot * previous_dot < 0) {
			// calculate the interpolation factor; t = dotQ1 / (dotQ1 - dotQ2)
			float t = previous_dot / (previous_dot - current_dot);
			
			// calculate the intersection point, I = Q1 + t(Q2-Q1)
			Vector3 intersection_point = Vector3::lerp(*previous_vertex, *current_vertex, t);

			// Insert the new intersection point in the list of "inside vertices"
			inside_vertices[num_inside_vertices] = intersection_point;
			num_inside_vertices++;
		}

		// If the current point is inside the plane
		if (current_dot > 0) {
			// Insert current vertex in the list of "inside vertices"
			inside_vertices[num_inside_vertices] = *current_vertex;
			num_inside_vertices++;
		}

		// Move to the next vertex
		previous_dot = current_dot;
		previous_vertex = current_vertex;
		current_vertex++;
	}

	// Copy all the vertices from the inside_vertices into the destination polygon parameter
	for (int i = 0; i < num_inside_vertices; i++) {
		poly.vertices[i] = inside_vertices[i];
	}
	poly.numVertices = num_inside_vertices;
}

void Rasterizer3D::clipPolygon(Polygon3D& poly)
{
	for (int i = 0; i < NUM_FRUSTUM_PLANES; i++)
		clipPolygonAgainstPlane(poly, i);
}

} // gdfx


