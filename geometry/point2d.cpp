#include <cmath>
#include <utility>
#include <vector>

using namespace std;

template <typename T> struct Point2D {
  public:
	T x, y;
	Point2D(T x, T y, T z) {
		this->x = x;
		this->y = y;
	}
	Point2D(Point2D<T>& old) {
		this->x = old.x;
		this->y = old.y;
	}
	Point2D<T> operator+(const Point2D<T> other) {
		Point2D<T> tmp(*this);
		tmp.x += other.x;
		tmp.y += other.y;
		return tmp;
	}
	Point2D<T> operator-(const Point2D<T> other) {
		Point2D<T> tmp(*this);
		tmp.x -= other.x;
		tmp.y -= other.y;
		return tmp;
	}
	T dot(const Point2D<T> other) {
		return this->x * other.x + this->y * other.y;
	}
	T cross(const Point2D<T> other) {
		return this->x * (other.y) - other.x * this->y;
	}
	double magnitude() { sqrt(pow(this->x, 2) + pow(this->y, 2)); }
};

template <typename T>
pair<T, bool> poly_winding(const vector<Point2D<T>>& poly, Point2D<T> pt) {
	int wn = 0;
	Point2D<T> prev = poly.back() - pt;
	for (Point2D<T> curr : poly) {
		curr = curr - pt;
		if (prev.cross(curr) == 0 && prev * curr <= 0)
			return pair<int, bool>(0, true);
		else if (prev.y <= 0 && 0 < curr.y && prev.cross(curr) > 0)
			++wn;
		else if (curr.y <= 0 && 0 < prev.y && prev.cross(curr) < 0)
			--wn;
		prev = curr;
	}
	return pair<T, bool>(wn, false);
}

template <typename T> T poly_twice_area(const vector<Point2D<T>>& poly) {
	coord_t twice_area = 0;
	Point2D<T> prev = poly.back();
	for (Point2D<T> curr : poly) {
		twice_area += prev.cross(curr);
		prev = curr;
	}
	return twice_area;
}
template <typename T> T poly_area(const vector<Point2D<T>>& poly) {
	return poly_twice_area(poly) / 2;
}

template <typename T> vector<Point2D<T>> convex_hull(vector<Point2D<T>> pts) {
	sort(pts.begin(), pts.end());
	vector<Point2D<T>> upr, lwr;
	for (Point2D<T>& pt : pts) {
		while (upr.size() >= 2 &&
			   (upr.back() - upr[upr.size() - 2]).cross(pt - upr.back()) >= 0)
			upr.pop_back();
		upr.push_back(pt);
		while (lwr.size() >= 2 &&
			   (lwr.back() - lwr[lwr.size() - 2]).cross(pt - lwr.back()) <= 0)
			lwr.pop_back();
		lwr.push_back(pt);
	}
	reverse(upr.begin(), upr.end());
	upr.pop_back();
	lwr.pop_back();
	lwr.insert(lwr.end(), upr.begin(), upr.end());
	return lwr;
}
