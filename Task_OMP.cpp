#define _USE_MATH_DEFINES
#include <iostream>
#include <fstream>
#include <string>
#include <omp.h>
#include <vector>
#include <cmath>
#include <random>

using namespace std;

std::random_device rd{};
std::mt19937 gen{ rd() };

struct Point
{
	Point(double x, double y) : x{ x }, y{ y } {}
	double x;
	double y;
};

class Polygon
{
public:
	int n = 300000000;
	Polygon(){}

	Polygon(int countPoint) {
		n = countPoint;
	}

	Point getPoint(int i) {
		// считает долше, но площадь рандомная, не очень для статистики(
		// double r_i = clip(normal_distribution<double>(100, clip(0.2, 0, 1) * 100)(gen), 0.0, 2 * 100);
		double r_i = 100 + i % 50;
		double x = 250 + r_i * cos(2 * M_PI * i / n);
		double y = 250 + r_i * sin(2 * M_PI * i / n);
		return Point{ x, y };
	}

	double area() {
		double start = omp_get_wtime();
		double t = 0;
		for (int i = 0; i < n; ++i) {
			Point point_1 = getPoint(i);
			Point point_2 = getPoint((i + 1) % n);
			t += point_1.x * point_2.y;
			t -= point_1.y * point_2.x;
		}
		if (t < 0) t = -t;
		std::cout << "area = " << t / 2 << std::endl;
		return (float)(omp_get_wtime() - start);
	}

	double areaOpenMP(int const N) {
		double start = omp_get_wtime();
		double t = 0;
#pragma omp parallel  for reduction(+:t) num_threads(N)
		for (int i = 0; i < n; ++i) {
			Point point_1 = getPoint(i);
			Point point_2 = getPoint((i + 1) % n);
			t += point_1.x * point_2.y;
			t -= point_1.y * point_2.x;
		}
		if (t < 0) t = -t;
		std::cout << "area = " << t / 2 << std::endl;
		return (float)(omp_get_wtime() - start);
	}

	double clip(double x, double min, double max) {
		if (min > max) {
			return x;
		}
		else if (x < min) {
			return min;
		}
		else if (x > max) {
			return max;
		}
		else {
			return x;
		}
	}
};


int main(int argc, char** argv) {
	Polygon pol = Polygon();
	std::cout << "Time for executing area() " \
		<< pol.area() << " seconds" \
		<< std::endl;
	for (int i = 1; i <= 8; ++i) {
		std::cout << "Time for executing areaOpenMP(" << i << ") " \
			<< pol.areaOpenMP(i) << " seconds" \
			<< std::endl;
	}
	return 0;
}