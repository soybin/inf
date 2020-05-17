#include <iostream>
#include <fstream>
#include <cmath>

const double PI = 3.14159265359;
const double MIN_DIST = 0.001;
const int MAX_STEP = 64;
const int W = 1920;
const int H = 1080;

struct vec3 {
	double x, y, z;
	vec3(double v) : x(v), y(v), z(v) {}
	vec3(double x, double y, double z) : x(x), y(y), z(z) {}
	vec3 operator + (const vec3& r) {
		return vec3(x + r.x, y + r.y, z + r.z);
	}
	vec3 operator - (const vec3& r) {
		return vec3(x - r.x, y - r.y, z - r.z);
	}
	vec3 operator * (const vec3& r) {
		return vec3(x * r.x, y * r.y, z * r.z);
	}
	vec3 operator * (double v) {
		return vec3(x * v, y * v, z * v);
	}
	vec3 operator / (const vec3& r) {
		return vec3(x / r.x, y / r.y, z / r.z);
	}
	vec3 operator / (double v) {
		return vec3(x / v, y / v, z / v);
	}
};

double length(vec3 v) {
	return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3 normalize(vec3 r) {
	return r / length(r);
}

double mod(double x, double y) {
	return x - y * std::floor(x / y);
}

double sphereDE(vec3 point) {
	point.x = mod(point.x, 1.0) - 0.5;
	point.y = mod(point.y, 1.0) - 0.5;
	point.z = mod(point.z, 1.0) - 0.5;
	return length(point) - 0.2;
}

vec3 calculateRayDirection(double fov, double ycoord, double xcoord) {
	double x = xcoord - W / 2.0;
	double y = ycoord - H / 2.0;
	double z = H / std::tan(fov * PI / 180.0 / 2.0);
	return normalize(vec3(x, y, -z));
}

double march(vec3 ori, vec3 dir) {
	double totalDistance = 0.01;
	int step = 0;
	for (; step < MAX_STEP; ++step) {
		double dist = sphereDE(ori + dir * totalDistance);
		if (dist < MIN_DIST) {
			break;
		}
		totalDistance += dist;
	}
	return ((double)step / (double)MAX_STEP);
}

int main() {
	std::ofstream out("inf.ppm");
	out << "P3\n" << W << ' ' << H << ' ' << "255\n";
	double fov = 45.0;
	vec3 ori(0.0);
	for (double ycoord = 0.5; ycoord < (double)H; ++ycoord) {
		for (double xcoord = 0.5; xcoord < (double)W; ++xcoord) {
			vec3 dir = calculateRayDirection(fov, ycoord, xcoord);
			vec3 col(march(ori, dir));
			col = col * 255.0;
			out << (int)col.x << ' ' << (int)col.y << ' ' << (int)col.z << '\n';
		}
	}
}
