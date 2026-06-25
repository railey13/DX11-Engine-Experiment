#pragma once
#include "Prerequisites.h"

class Point {
public:

	Point() : m_x(0), m_y(0) {

	}
	
	Point(i32 x, i32 y) : m_x(x), m_y(y) {

	}

	Point(const Point& point) : m_x(point.m_x), m_y(point.m_y) {

	}

	~Point() {

	}

public:
	i32 m_x = 0, m_y = 0;
};