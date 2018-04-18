// Diplom1.cpp : Defines the entry point for the console application.
//
#define _CRT_SECURE_NO_WARNINGS

#include "stdafx.h"




using namespace std;
using namespace charOblast;
namespace charOblast {

	void LogMove(string object, string name, int nameNum, Point from, Point to) {
		ofstream fout;

		fout.open("output.txt", ios::app);

		fout << "move ";
		fout << object << " ";
		fout << name << nameNum << " ";
		fout << "from " << from.x << ":" << from.y << " ";
		fout << "to " << to.x << ":" << to.y;
		fout << endl;

		fout.close();

		return;
	}

	void LogNew(string object, string name, int nameNum, vector<Point> params) {
		ofstream fout;

		fout.open("output.txt", ios::app);

		fout << "new ";
		fout << object << " ";
		fout << name << nameNum << " ";

		
		for (int i = 0; i < params.size(); i++) {
			fout << params[i].x << ":" << params[i].y << " ";
		}

		fout << endl;

		fout.close();
		return;
	}

	bool DoesNotContainAsVertex(Poly poly, Point pt) {
		bool check = true;

		for (int i = 0; i < poly.verts.size(); i++) {
			if (poly.verts[i] == pt) {
				check = false;
				break;
			}
		}
		return check;
	}

	float PseudoScal(Point a, Point from, Point to) { //псевдоскалярное произведение
		Vector2 first(from, to);
		Vector2 second(from, a);

		float res = first.Coordinates().x*second.Coordinates().y -
			first.Coordinates().y*second.Coordinates().x;
		return res;
	}

	float PseudoScal(Vector2 first, Vector2 second) {
		float res = first.Coordinates().x*second.Coordinates().y -
			first.Coordinates().y*second.Coordinates().x;
		return res;
	}

	float Scalar(Point a1, Point a2, Point b1, Point b2) { //скалярное произведение
		Vector2 first(a1, a2);
		Vector2 second(b1, b2);

		float res = first.Coordinates().x*second.Coordinates().x +
			first.Coordinates().y*second.Coordinates().y;
		return res;
	}

	float Scalar(Vector2 first, Vector2 second) {
		float res = first.Coordinates().x*second.Coordinates().x +
			first.Coordinates().y*second.Coordinates().y;
		return res;
	}


	bool Point::operator==(Point pt) {
		if (pt.x == x) {
			if (pt.y == y) {
				return true;
			}
		}
		return false;
	}
	bool Point::operator!=(Point pt) {
		if (*this == pt) {
			return false;
		}
		return true;
	}
	Point Point::operator=(Point pt) {
		x = pt.x;
		y = pt.y;
		return *this;
	}
	void Point::Move(Vector2 shift) {
		x += shift.Coordinates().x;
		y += shift.Coordinates().y;
		
	}
	Point Point::Moved(Vector2 shift) {
		Point moved;
		moved.x = x + shift.Coordinates().x;
		moved.y = y + shift.Coordinates().y;
		return moved;
	}
	Point::Point(int code) {
		errorCode = code;
		x = 0;
		y = 0;
	}
	Point::Point(float x, float y) {
		this->x = x;
		this->y = y;
		errorCode = 0;
	}
	Point::Point() {
		Point(0, 0);
	}


	bool Vector2::operator==(Vector2 dir) {
		if (from == dir.from) {
			if (to == dir.to) {
				return true;
			}
		}
		return false;
	}
	Vector2 Vector2::direction() {
		return Vector2();
	}
	float Vector2::Distance(Point a, Point b) {
		float x = b.x - a.x;
		float y = b.y - a.y;
		return sqrt(x*x + y * y);
	}
	float Vector2::Distance() {
		return Distance(from, to);
	}
	Point Vector2::Coordinates() {
		return this->coordinatePoint;
	}
	Vector2::Vector2(Point from, Point to) {
		this->from = from;
		this->to = to;
		this->coordinatePoint = Point(to.x - from.x, to.y - from.y);
	}
	Vector2::Vector2(Point coordinate) {
		this->coordinatePoint = coordinate;
		this->from = Point(0, 0);
		this->to = coordinate;
	}
	Vector2::Vector2() {
		this->from = Point(0, 0);
		this->to = Point(0, 0);
	}

	Point Line::intersection(Line lineB) {

		float zn = (ptA.x - ptB.x)*(lineB.ptA.y - lineB.ptB.y) -
			(ptA.y - ptB.y)*(lineB.ptA.x - lineB.ptB.x);

		Point intersection;
		if (zn != 0) {
			intersection.x = (((ptA.x*ptB.y - ptA.y*ptB.x)*(lineB.ptA.x - lineB.ptB.x) -
				(ptA.x - ptB.x)*(lineB.ptA.x*lineB.ptB.y - lineB.ptA.y*lineB.ptB.x)) / zn) + 0.0;
			intersection.y = (((ptA.x*ptB.y - ptA.y*ptB.x)*(lineB.ptA.y - lineB.ptB.y) -
				(ptA.y - ptB.y)*(lineB.ptA.x*lineB.ptB.y - lineB.ptA.y*lineB.ptB.x)) / zn) + 0.0;


		}
		else intersection.errorCode = 1;
		return intersection;
	}
	Line::Line(Point pointA, Point pointB) {
		this->ptA = pointA;
		this->ptB = pointB;
	}
	Line::Line(Vector2 direction) {
		this->ptA = direction.from;
		this->ptB = direction.to;
	}
	Line::Line() {
		this->ptA = 0;
		this->ptB = 0;
	}
	Line Line::DrawHorizontalLine(Point pt) {
		return Line(pt, Point(pt.x + 1, pt.y));
	}
	Line Line::DrawVerticalLine(Point pt) {
		return Line(pt, Point(pt.x, pt.y + 1));
	}

	Ray::Ray(Point start, Vector2 dir) : Line() {
		this->start = start;
		this->rayDir = dir;
	}

	const double EPS = 1e-5;

	Point Poly::NextVertexAfter(Point pt) {
		for (size_t i = 0; i < verts.size(); i++) {
			if ((i != verts.size()-1) && (verts[i] == pt)) {
				return verts[i + 1];
			}
			else if ((i == verts.size() - 1) && (verts[i] == pt)) {
				return verts[0];
			}
		}
		return Point(-1);
	}
	vector<Point> Poly::FindIntersectionsPolyLine(Line line) {
		vector<Point> result;
		vector<Point> allIntersections;
		for (int i = 0; i < verts.size(); i++) {
			int nextIndex;
			if (i != verts.size() - 1) {
				nextIndex = i + 1;
			}
			else {
				nextIndex = 0;
			}
			Line edgeContinuation(verts[i], verts[nextIndex]);
			Point newInter;
			newInter = line.intersection(edgeContinuation);
			if (newInter.errorCode != 1) {
				float condition = PseudoScal(newInter, verts[i], verts[nextIndex]);
				if (((condition < EPS) && (condition >= 0)) ||
					((condition > -EPS) && (condition <= 0))) {
					if ((verts[i].x < verts[nextIndex].x) || (verts[i].y < verts[nextIndex].y)) {
						if (((newInter.x > verts[i].x) && (newInter.x < verts[nextIndex].x)) ||
							((newInter.y > verts[i].y) && (newInter.y < verts[nextIndex].y))) {
							allIntersections.push_back(newInter);
						}
					}
					else {
						if (((newInter.x > verts[nextIndex].x) && (newInter.x < verts[i].x)) ||
							((newInter.y > verts[nextIndex].y) && (newInter.y < verts[i].y))) {
							allIntersections.push_back(newInter);
						}
					}
				}
			}
			else continue;
		}

		return allIntersections;
	}
	Point Poly::FindIntersectionsPolyRay(Ray ray) {

		Line rayToLine(ray.start, ray.start.Moved(ray.rayDir));
		vector<Point> all = FindIntersectionsPolyLine(rayToLine);
		Point inter;
		for (int i = 0; i < all.size(); i++) {
			if ((ray.rayDir.Coordinates().x > 0) || (ray.rayDir.Coordinates().y > 0)) {
				if ((all[i].x > ray.start.x) || (all[i].y > ray.start.y)) {
					inter = all[i];
					return inter;
				}
				else continue;
			}
			else {
				if ((all[i].x < ray.start.x) || (all[i].y < ray.start.y)) {
					inter = all[i];
					return inter;
				}
				else continue;
			}

		}
		return inter;
	}
	vector<Point> Poly::FindIntersectionsPolyPoly(Poly poly) {
		vector<Point> intersections;
		
		for (int i = 0; i < this->verts.size(); i++) {
			if (geomSearch(poly, this->verts[i])) {
				int nextIndex;
				if (i != this->verts.size() - 1) {
					nextIndex = i + 1;
				}
				else {
					nextIndex = 0;
				}

				if (!geomSearch(poly, this->verts[nextIndex])) {
					Line edge(this->verts[i], this->verts[nextIndex]);
					vector<Point> newinters = poly.FindIntersectionsPolyLine(edge);
					for (int j = 0; j < newinters.size(); j++){
						float condition = Scalar(newinters[j], this->verts[i], newinters[j], this->verts[nextIndex]);
						if (condition <= 0) {
							intersections.push_back(newinters[j]);
						}
					}
				}
			}
			else {
				int nextIndex;
				if (i != this->verts.size() - 1) {
					nextIndex = i + 1;
				}
				else {
					nextIndex = 0;
				}
				if (geomSearch(poly, this->verts[nextIndex])) {
					Line edge(this->verts[i], this->verts[nextIndex]);
					vector<Point> newinters = poly.FindIntersectionsPolyLine(edge);
					for (int j = 0; j < newinters.size(); j++) {
						float condition = Scalar(newinters[j], this->verts[i], newinters[j], this->verts[nextIndex]);
						if (condition <= 0) {
							intersections.push_back(newinters[j]);
						}
					}
				}
			}
		}

		return intersections;
	}
	Poly::Poly(const Poly& copy) {
		verts = copy.verts;
	}
	Poly::Poly() {

	}
	bool Poly::MovePossibility(Poly polyF, Vector2 movement) {
		//Poly charM;
		//int errorM = CharPoly(polyF, polyM, &charM);

		Point initial = this->verts[0];
		this->ParallelMove(movement);
		for (int i = 0; i < this->verts.size(); i++) {
			if (!geomSearch(polyF, this->verts[i])) {
				this->ParallelMove(this->verts[0], initial);
				return false; //movement impossible
			}
		}
		return true;
	}
	Poly Poly::ParallelMoved(Point moveFrom, Point moveTo) {
		return ParallelMoved(Vector2(moveFrom, moveTo));
	}
	Poly Poly::ParallelMoved(Vector2 shift) {
		for (int i = 0; i < verts.size(); ++i) {
			verts[i].Move(shift);
		}
		return *this;
	}
	void Poly::ParallelMove(Vector2 shift) {
		*this = ParallelMoved(shift);
	}
	void Poly::ParallelMove(Point moveFrom, Point moveTo) {
		//LogMove("poly", "polyM", moveFrom, moveTo);
		ParallelMove(Vector2(moveFrom, moveTo));
	}
	void Poly::AddVert(float x, float y) {
		verts.push_back(Point(x, y));
	}
	Point FindMinByLength(vector<Point> p, Point a) {
		if (p.size() < 1) {
			return Point(-1); //Invalid vector, error code -1
		}
		if (p.size() == 1) { //Array consists of 1 point
			return p[0];
		}
		float x = a.x - p[0].x;
		float y = a.y - p[0].y;
		float min = sqrt((x*x) + (y*y));
		int indexOfMin = 0;

		for (int i = 1; i < p.size(); i++) {
			float x = a.x - p[i].x;
			float y = a.y - p[i].y;
			float len = sqrt((x*x) + (y*y));
			if (len < min) {
				min = len;
				indexOfMin = i;
			}
			else continue;
		}
		if ((indexOfMin >= 0) && (indexOfMin < p.size())) {
			return p[indexOfMin];
		}
		else {
			return Point(-1); //Should never happen
		}
	}

	Vector2 GetVectorDir(Point from, Point to) {
		Vector2 d;
		float norm = sqrt((to.x - from.x)*(to.x - from.x) + (to.y - from.y)*(to.y - from.y));
		d.from = (to.x - from.x) / norm;
		d.to = (to.y - from.y) / norm;
		return d;
	}
	edge OnWhichEdge(Poly poly, Point p) {
		for (int i = 0; i < poly.verts.size(); i++) {
			int nextIndex;
			if (i != poly.verts.size() - 1) {
				nextIndex = i + 1;
			}
			else {
				nextIndex = 0;
			}
			edge e;
			e.a = poly.verts[i];
			e.aPlusOne = poly.verts[nextIndex];

			float condition = PseudoScal(p, e.a, e.aPlusOne);
			if (((condition < EPS) && (condition >= 0)) ||
				((condition > -EPS) && (condition <= 0))) {
				if ((e.a.x < e.aPlusOne.x) || (e.a.y < e.aPlusOne.y)) {
					if (((p.x > e.a.x) && (p.x < e.aPlusOne.x)) ||
						((p.y > e.a.y) && (p.y < e.aPlusOne.y))) {
						return e;
					}
				}
				else {
					if (((p.x > e.aPlusOne.x) && (p.x < e.a.x)) ||
						((p.y > e.aPlusOne.y) && (p.y < e.a.y))) {
						return e;
					}
				}
			}
			else continue;

		}
		return edge();
	}
	/*
	Scene::Scene() {
		polyF = Poly();
		polyM1 = Poly();
		innerCharPoly = vector<Poly>();
		outerCharPoly = vector<Poly>();
	}
	Scene::Scene(Poly polyf) {
		Scene::Scene();
		polyF = polyf;
	}
	Scene::Scene(Poly polyf, Poly polym1) {
		polyF = polyf;
		polyM1 = polym1;

		innerCharPoly.resize(1);
		int error = CharPoly(polyF, polyM1, &innerCharPoly[0]);

	}
	Scene::Scene(Poly polyf, Poly polym1, Poly polym2) {
		polyF = polyf;
		polyM1 = polym1;
		polyM2 = polym2;

		innerCharPoly.resize(2);
		int error1 = CharPoly(polyF, polyM1, &innerCharPoly[0]);
		int error2 = CharPoly(polyF, polyM2, &innerCharPoly[1]);

		outerCharPoly.resize(2);
		int error1 = OuterCharPoly(polyM1, polyM2, &outerCharPoly[0]);
		int error2 = OuterCharPoly(polyM2, polyM1, &outerCharPoly[1]);

		//to do: if charpoly doesn't exist
	}
	int Scene::PlacementPossibility(bool touchF, bool touchEachM) {
		int answer = 0;
		
		if (touchF == true) {
			bool alwaysIntersects = true;
			for (int i = 0; i < innerCharPoly[0].verts.size(); i++) {
				if (!geomSearch(outerCharPoly[0], innerCharPoly[0].verts[i])) {
					alwaysIntersects = false;
				}
			}
			if (!alwaysIntersects) {
				answer = 1;
			}
		}

		if (touchEachM == true) {

		}
	}
	*/
	bool geomSearch(Poly poly, Point pt, int excludeEdges) {

		int polyDim = poly.verts.size() - 1;

		float firstCondition = PseudoScal(pt, poly.verts[0], poly.verts[1]);
		float secondCondition = PseudoScal(pt, poly.verts[0], poly.verts[polyDim]);

		
		if (((firstCondition < EPS) && (firstCondition >= 0)) ||
			((firstCondition > -EPS) && (firstCondition <= 0))) {
			float conditionOnEdge = Scalar(pt, poly.verts[0], pt, poly.verts[1]);
			if ((conditionOnEdge <= 0) && (excludeEdges == 0)) {
				return true;
			}
			else {
				return false;
			}
			
		}
		if (((secondCondition < EPS) && (secondCondition >= 0)) ||
			((secondCondition > -EPS) && (secondCondition <= 0))) {
			float conditionOnEdge = Scalar(pt, poly.verts[0], pt, poly.verts[polyDim]);
			if ((conditionOnEdge <= 0) && (excludeEdges == 0)) {
				return true;
			}
			else {
				return false;
			}
		}

		if ((firstCondition < 0) && (secondCondition > 0)) {

			int r = polyDim;
			int l = 0;
			int j = floor((polyDim) / 2);

			while ((r - l) > 1) {

				float thirdCondition = PseudoScal(pt, poly.verts[0], poly.verts[j]);
				
				if (thirdCondition < 0) {
					
					l = j;
					j = floor((r + j) / 2);
				}
				else if (((thirdCondition < EPS) && (thirdCondition >= 0)) ||
					((thirdCondition > -EPS) && (thirdCondition <= 0))) {

					float conditionOnEdge = Scalar(pt, poly.verts[0], pt, poly.verts[j]);
					if (conditionOnEdge < 0) {
						return true;
					}
					else if ((conditionOnEdge == 0) && (excludeEdges == 0)) {
						return true;
					}
					else{
						return false;
					}
				}
				else {
					
					r = j;
					j = floor(j / 2);

				}
			}

			float lastCondition;
			if (l < r) {
				lastCondition = PseudoScal(pt, poly.verts[l], poly.verts[r]);
			}
			else {
				lastCondition = PseudoScal(pt, poly.verts[r], poly.verts[l]);
			}
			
			if ((lastCondition <= 0) && (excludeEdges == 0)) {
				return true;
				
			}
			else if (lastCondition < 0) {
				return true;
			}
			else {
				return false;
				
			}
		}
		else {
			return false;
		
		}

	}
	Poly polyF; //fixed polygon
	Poly polyM; //moveable polygon

	int CharPolyAlter(Poly polyF, Poly polyM, Poly * charpoly) { //внутренняя характеристическая область
		polyM.ParallelMove(polyM.verts[0], polyF.verts[0]); //совмещаем переносом вершины m0 и f0

		//начинаем искать исходное положение для начала скольжения подвижного внутри неподвижного
		vector<int> intersect_m;
		vector<Point> intersect_s;
		vector<Vector2> directions;
		Point minimal(0, 0);
		bool flag = true;
		for (int i = polyM.verts.size() - 1; i > 0; i--) {
			if (!geomSearch(polyF, polyM.verts[i])) {
				intersect_m.push_back(i);
				Line hor = Line::DrawHorizontalLine(polyM.verts[i]);
				Line vert = Line::DrawVerticalLine(polyM.verts[i]);
				vector<Point> intersectionsHor = polyF.FindIntersectionsPolyLine(hor);
				vector<Point> intersectionsVert = polyF.FindIntersectionsPolyLine(vert);
				vector<Point> intersections;
				for (int i = 0; i < intersectionsHor.size(); i++) {
					intersections.push_back(intersectionsHor[i]);
				}
				for (int i = 0; i < intersectionsVert.size(); i++) {
					intersections.push_back(intersectionsVert[i]);
				}
				minimal = FindMinByLength(intersections, polyM.verts[i]);
				intersect_s.push_back(minimal);
				Vector2 newdir = GetVectorDir(polyM.verts[i], minimal);
				/*
				int j = 0;
				while (j < directions.size()) {
					if (newdir == directions[j]) {
						return 0; //такого положения нет, хар.области не существует
					}
					else {
						j++;
					}
				}
				*/
				directions.push_back(newdir);
				polyM.ParallelMove(polyM.verts[i], minimal);
				i = polyM.verts.size();
			}
		}
		//исходное положение найдено

		//проверка: лежит ли подвижный полностью внутри неподвижного после нахождения искходного положения
		for (int i = 0; i < polyM.verts.size(); i++) {
			if (!geomSearch(polyF, polyM.verts[i])) {
				return 0; //хар.области не существует
			}
		}

		//данные для начала цикла скольжения
		Point p = polyM.verts[intersect_m[intersect_m.size()-1]];
		edge e = OnWhichEdge(polyF, p); //изначальное ребро неподвижного
		Point pointA = e.a; 
		Point pointB = e.aPlusOne;
		float d1 = 0;
		int glideVertexIndex = intersect_m[intersect_m.size() - 1]; //изначальная вершина подвижного
		//цикл скольжения. заканчивается, когда пришли к началу первого ребра неподвижного
		while (pointB != e.a) {
			d1 = Vector2::Distance(p, pointB); //расстояние от текущей вершины подвижного до конца ребра неподвижного
			Vector2 pb(p, pointB); //направление движения

			charpoly->verts.push_back(polyM.verts[0]);//добавление новой вершины в хар.область

			vector<Point> intersect_c;
			Point zero(0);
			zero.zero = true;

			//выпускаем лучи из всех вершин подвижного, кроме текущей, в направлении движения 
			//и ищем их пересечения со сторонами подвижного
			for (int i = 0; i < polyM.verts.size(); i++) {
				if (p != polyM.verts[i]) {
					Ray ray(polyM.verts[i], pb);
					Point c = polyF.FindIntersectionsPolyRay(ray);
					intersect_c.push_back(c);
				}
				else {
					intersect_c.push_back(zero);
				}
			}

			//находим минимальное расстояние от вершины до соответсвующего ей пересечения и запоминаем их индекс
			float d2 = 999;
			int indexMin;
			for (int i = 0; i < polyM.verts.size(); i++) {
				if (intersect_c[i].zero == false) {
					if (d2 > Vector2::Distance(polyM.verts[i], intersect_c[i])) {
						d2 = Vector2::Distance(polyM.verts[i], intersect_c[i]);
						indexMin = i;
					}
				}
			}

			
			if (d1 > d2) { //если расстояние до конца ребра больше, то совмещаем вершину с минимальным расстоянием
				//до пересечения с соответсвующей точкой пересечения
				polyM.ParallelMove(polyM.verts[indexMin], intersect_c[indexMin]);
			}
			else {
				//иначе совмещаем текущую вершину с концом текущего ребра
				polyM.ParallelMove(p, pointB);
			}
			if (d1 <= d2) {//если расстояние до конца ребра меньше или равно расстоянию до пересечения
				//то делаем текущим следующее ребро за этим неподвижного, вершину неподвижного оставляем ту же
				p = polyM.verts[glideVertexIndex];
				pointA = pointB;
				pointB = polyF.NextVertexAfter(pointB);
			}
			else { //иначе в качестве текущей вершины подв. берём вершину, в которой достигается минимальное расстояние
				// до пересечения, а в качестве текущего ребра неподвижного берём ребро, содержащее это пересечение
				p = polyM.verts[indexMin];
				glideVertexIndex = indexMin;
				edge e1 = OnWhichEdge(polyF, intersect_c[indexMin]);
				pointA = e1.a;
				pointB = e1.aPlusOne;
			}
		//повторяем цикл с новыми данными
		}
		
		charpoly->verts.push_back(polyM.verts[0]); //добавляем последнюю вершину в хар.область, хар.область построена
		return 1;
	}
	int OuterCharPolyAlter(Poly polyF, Poly polyM, Poly * charpoly, Point specialPoint = Point(-1)) {
		//построение внешней хар.области, возможно построение с использованием некой особой точки,
		//не являющейся вершиной подвижного, по умолчанию строится с использованием нулевой вершины
		if (specialPoint.errorCode != -1) {//если есть особая точка, всегда будем двигать её в том же направлении,
			//что и подвижный многоугольник
			specialPoint.Move(Vector2(polyM.verts[0], polyF.verts[0]));
		}
		polyM.ParallelMove(polyM.verts[0], polyF.verts[0]); //совмещаем нулевые вершины многоугольников переносом

		int activeVertexIndexM = 0;
		int activeVertexIndexF = 0;

		for (int i = polyM.verts.size() - 1; i >= 0; i--) { //определяем положение начала движения
			//выносим подвижный вовне неподвижного так, чтобы одна из вершин подвижного была в нулевой вершине
			//неподвижного и многоугольники не пересекались
			if (geomSearch(polyF, polyM.verts[i])) {
				if (specialPoint.errorCode != -1) {
					specialPoint.Move(Vector2(polyM.verts[i], polyF.verts[0]));
				}
				polyM.ParallelMove(polyM.verts[i], polyF.verts[0]);
				int prevIndex;
				int nextIndex;
				if ((i != (polyM.verts.size() - 1)) && (i != 0)) {
					nextIndex = i + 1;
					prevIndex = i - 1;
				}
				else if (i == 0) {
					prevIndex = polyM.verts.size() - 1;
					nextIndex = i + 1;
				}
				else {
					nextIndex = 0;
					prevIndex = i - 1;
				}
				Line edge1(polyM.verts[i - 1], polyM.verts[i]);
				Line edge2(polyM.verts[i], polyM.verts[nextIndex]);
				vector<Point> inters1 = polyF.FindIntersectionsPolyLine(edge1);
				vector<Point> inters2 = polyF.FindIntersectionsPolyLine(edge2);
				for (int j = 0; j < inters1.size(); j++) {
					if (inters1[j] != polyF.verts[0]) {
						float check = Scalar(inters1[j], polyM.verts[i - 1], inters1[j], polyM.verts[i]);
						if (check <= 0) {
							i = polyM.verts.size() - 1;
							continue;
						}
					}
				}
				for (int j = 0; j < inters2.size(); j++) {
					if (inters2[j] != polyF.verts[0]) {
						float check = Scalar(inters2[j], polyM.verts[i], inters2[j], polyM.verts[nextIndex]);
						if (check <= 0) {
							i = polyM.verts.size() - 1;
							continue;
						}
					}
				}
				activeVertexIndexM = i;//запоминаем индекс активной вершины подвижного
				i = polyM.verts.size() - 1;
			}
		}

		//добавляем первую вершину в хар.область
		if (specialPoint.errorCode == -1) {//если задана особая точка
			charpoly->verts.push_back(polyM.verts[0]);
		}
		else {//если не задана
			charpoly->verts.push_back(specialPoint);
		}
		//запомним изначальные активные вершины подв. и неподв. соответсвенно
		int initialVertexIndexM = activeVertexIndexM;
		int initialVertexIndexF = activeVertexIndexF;

		do { //цикл скольжения, выполняется до тех пор, пока активные вершины не совпадут с изначальными

			//определим рёбра подвижного, которые могут скользить по вершине
			int prevIndexM;
			int nextIndexM;
			if ((activeVertexIndexM != (polyM.verts.size() - 1)) && (activeVertexIndexM != 0)) {
				nextIndexM = activeVertexIndexM + 1;
				prevIndexM = activeVertexIndexM - 1;
			}
			else if (activeVertexIndexM == 0) {
				prevIndexM = polyM.verts.size() - 1;
				nextIndexM = activeVertexIndexM + 1;
			}
			else {
				nextIndexM = 0;
				prevIndexM = activeVertexIndexM - 1;
			}
			int nextIndexF;
			if (activeVertexIndexF != polyF.verts.size() - 1) {
				nextIndexF = activeVertexIndexF + 1;
			}
			else {
				nextIndexF = 0;
			}

			//проверим, могут ли эти рёбра скользить по вершине
			//для этого сдвинем активную вершину в направлении движения ребра и посмотрим
			//будет ли она лежать в полуплоскости, содержащей многоугольник
			Vector2 shift1(polyM.verts[prevIndexM], polyM.verts[activeVertexIndexM]);
			Vector2 shift2(polyM.verts[nextIndexM], polyM.verts[activeVertexIndexM]);
			float condition1 = PseudoScal(polyM.verts[activeVertexIndexM].Moved(shift1),
				polyF.verts[activeVertexIndexF],
				polyF.verts[nextIndexF]);
			float condition2 = PseudoScal(polyM.verts[activeVertexIndexM].Moved(shift2),
				polyF.verts[activeVertexIndexF],
				polyF.verts[nextIndexF]);
			if ((condition1 > 0) && (condition2 < 0)) { //если возможно скольжение первого ребра, но не первого, по вершине неподв.
				//то совершаем это скольжение и соответственно меняем активные вершины и повторяем цика
				if (specialPoint.errorCode == -1) {
					polyM.ParallelMove(polyM.verts[prevIndexM], polyF.verts[activeVertexIndexF]);
				}
				else {
					specialPoint.Move(Vector2(polyM.verts[prevIndexM], polyF.verts[activeVertexIndexF]));
					polyM.ParallelMove(polyM.verts[prevIndexM], polyF.verts[activeVertexIndexF]);
				}
				activeVertexIndexM = prevIndexM;
				if (specialPoint.errorCode == -1) {
					charpoly->verts.push_back(polyM.verts[0]);
				}
				else {
					charpoly->verts.push_back(specialPoint);
				}
			}
			else if ((condition1 < 0) && (condition2 > 0)) { //аналогично, если возможно скольжение второго ребра, но не первого
				
				if (specialPoint.errorCode == -1) {
					polyM.ParallelMove(polyM.verts[nextIndexM], polyF.verts[activeVertexIndexF]);
				}
				else {
					specialPoint.Move(Vector2(polyM.verts[nextIndexM], polyF.verts[activeVertexIndexF]));
					polyM.ParallelMove(polyM.verts[nextIndexM], polyF.verts[activeVertexIndexF]);
				}
				activeVertexIndexM = nextIndexM;
				if (specialPoint.errorCode == -1) {
					charpoly->verts.push_back(polyM.verts[0]);
				}
				else {
					charpoly->verts.push_back(specialPoint);
				}
			}
			else { //если невозможно скольжение рёбрами, скользим вершиной и соответсвенно меняем активные вершины
				//и повторяем цикл
				if (specialPoint.errorCode == -1) {
					polyM.ParallelMove(polyM.verts[activeVertexIndexM], polyF.verts[nextIndexF]);
				}
				else {
					specialPoint.Move(Vector2(polyM.verts[activeVertexIndexM], polyF.verts[nextIndexF]));
					polyM.ParallelMove(polyM.verts[activeVertexIndexM], polyF.verts[nextIndexF]);
				}
				activeVertexIndexF = nextIndexF;
				if (specialPoint.errorCode == -1) {
					charpoly->verts.push_back(polyM.verts[0]);
				}
				else {
					charpoly->verts.push_back(specialPoint);
				}
			}
		} while ((initialVertexIndexF != activeVertexIndexF) ||
			(initialVertexIndexM != activeVertexIndexM));

		charpoly->verts.pop_back();
		//хар. область построена
		return 1;

	}
	int CharPoly(Poly polyF, Poly polyM, Poly * charpoly) {
		vector<Vector2> normalsInnerPolyF;
		for (int i = 0; i < polyF.verts.size(); i++) {
			int nextIndex;
			if (i != polyF.verts.size() - 1) {
				nextIndex = i + 1;
			}
			else {
				nextIndex = 0;
			}
			Vector2 normal(Point(polyF.verts[nextIndex].y - polyF.verts[i].y, polyF.verts[i].x - polyF.verts[nextIndex].x));
			normalsInnerPolyF.push_back(normal);
		}

		vector<Vector2> movements;
		for (int i = 0; i < normalsInnerPolyF.size(); i++) {
			int nextIndexF;
			if (i != normalsInnerPolyF.size() - 1) {
				nextIndexF = i + 1;
			}
			else {
				nextIndexF = 0;
			}
			for (int j = 0; j < polyM.verts.size(); j++) {
				int prevIndex;
				int nextIndex;
				if (j == 0) {
					prevIndex = polyM.verts.size() - 1;
					nextIndex = j + 1;
				}
				else if (j == polyM.verts.size() - 1) {
					prevIndex = j - 1;
					nextIndex = 0;
				}
				else {
					prevIndex = j - 1;
					nextIndex = j + 1;
				}
				float condition1 = Scalar(Vector2(polyM.verts[j], polyM.verts[prevIndex]), normalsInnerPolyF[i]);
				float condition2 = Scalar(Vector2(polyM.verts[j], polyM.verts[nextIndex]), normalsInnerPolyF[i]);
				if ((condition1 >= 0) && (condition2 >= 0)) {
					Point from;
					from = polyF.verts[i];
					from.Move(Vector2(polyM.verts[j], polyM.verts[0]));
					Point to;
					to = polyF.verts[nextIndexF];
					to.Move(Vector2(polyM.verts[j], polyM.verts[0]));
					Vector2 movement(from, to);
					movements.push_back(movement);
				}
			}
		}

		vector<vector<pair<Point,int>>> moveInters;
		moveInters.resize(movements.size());
		for (int i = 0; i < movements.size(); i++) {
			for (int j = 0; j < movements.size(); j++) {
				Line currentMove(movements[i]);
				if (i != j) {
					pair<Point, int> inter;
					Line anotherMove(movements[j]);
					Point intersection = currentMove.intersection(anotherMove);
					if (intersection.errorCode != 1) {
						float condition = Scalar(intersection, movements[i].from, intersection, movements[i].to);
						if (condition <= 0) {
							inter.first = intersection;
							inter.second = j;
							moveInters[i].push_back(inter);
						}
					}
				}
			}
			
			for (int j = 0; j < moveInters[i].size(); j++) {
				vector<float> distances(moveInters[i].size());
				for (int k = j; k < moveInters[i].size(); k++) {
					float distance = Vector2::Distance(movements[i].from, moveInters[i][k].first);
					distances[k] = distance;
				}
				int indexMin = j;
				int distanceMin = 9999;
				for (int k = j; k < distances.size(); k++) {
					if (distances[k] < distanceMin) {
						indexMin = k;
						distanceMin = distances[k];
					}
				}
				pair<Point, int> buf;
				buf.first = moveInters[i][j].first;
				buf.second = moveInters[i][j].second;
				moveInters[i][j].first = moveInters[i][indexMin].first;
				moveInters[i][j].second = moveInters[i][indexMin].second;
				moveInters[i][indexMin].first = buf.first;
				moveInters[i][indexMin].second = buf.second;
			}
		}

		int currentMove;
		int nextMove;
		int nextMovePointIndex;
		int initialMove = 0;
		Point initialPoint;
		Point nextPoint;
		vector<vector<bool>> checked;
		Poly temp;
		checked.resize(moveInters.size());
		for (int i = 0; i < moveInters.size(); i++) {
			checked[i].resize(moveInters[i].size());
		}
		for (int i = 0; i < moveInters.size(); i++) {
			for (int j = 0; j < moveInters[i].size(); j++) {
				if (checked[i][j] == false) {
					if (j != moveInters[i].size() - 1) {
						if ((!geomSearch(polyF, moveInters[i][j].first)) ||
							(!geomSearch(polyF, moveInters[i][j + 1].first))) {
							checked[i][j] = true;
							continue;
						}
						else {
							initialPoint = moveInters[i][j].first;
							nextMove = moveInters[i][j + 1].second;
							for (int k = 0; k < moveInters[nextMove].size(); k++) {
								if ((moveInters[i][j + 1].first == moveInters[nextMove][k].first) &&
									(k != moveInters[nextMove].size() - 1)) {
									nextPoint = moveInters[nextMove][k + 1].first;
									nextMovePointIndex = k + 1;
									break;
								}
								else {
									nextPoint = Point(-1);
								}
							}
							temp.verts.push_back(initialPoint);
							temp.verts.push_back(moveInters[i][j + 1].first);
							checked[i][j] = true;
							checked[i][j + 1] = true;
							while ((nextPoint != initialPoint) &&
								(geomSearch(polyF, nextPoint)) &&
								(nextPoint.errorCode != -1)) {
								temp.verts.push_back(nextPoint);
								checked[nextMove][nextMovePointIndex] = true;
								currentMove = nextMove;
								nextMove = moveInters[nextMove][nextMovePointIndex].second;
								for (int k = 0; k < moveInters[nextMove].size(); k++) {
									if ((moveInters[currentMove][nextMovePointIndex].first == moveInters[nextMove][k].first) &&
										(k != moveInters[nextMove].size() - 1)) {
										nextPoint = moveInters[nextMove][k + 1].first;
										nextMovePointIndex = k + 1;
										break;
									}
									else {
										checked[nextMove][k] = true;
										nextPoint = Point(-1);
									}
								}
							}
						}
					}
				}
			}
		}

		for (int i = 0; i < temp.verts.size(); i++) {
			if (DoesNotContainAsVertex(*charpoly, temp.verts[i])) {
				charpoly->verts.push_back(temp.verts[i]);
			}
		}

		return 1;
	}
	int OuterCharPoly(Poly polyF, Poly polyM, Poly * charpoly) {
		vector<Vector2> normalsOuterPolyF;
		for (int i = 0; i < polyF.verts.size(); i++) {
			int nextIndex;
			if (i != polyF.verts.size() - 1) {
				nextIndex = i + 1;
			}
			else {
				nextIndex = 0;
			}
			Vector2 normal(Point(polyF.verts[i].y - polyF.verts[nextIndex].y, polyF.verts[nextIndex].x - polyF.verts[i].x));
			normalsOuterPolyF.push_back(normal);
		}
		vector<Vector2> normalsOuterPolyM;
		for (int i = 0; i < polyM.verts.size(); i++) {
			int nextIndex;
			if (i != polyM.verts.size() - 1) {
				nextIndex = i + 1;
			}
			else {
				nextIndex = 0;
			}
			Vector2 normal(Point(polyM.verts[i].y - polyM.verts[nextIndex].y, polyM.verts[nextIndex].x - polyM.verts[i].x));
			normalsOuterPolyM.push_back(normal);
		}

		vector<Vector2> movements;
		for (int i = 0; i < normalsOuterPolyF.size(); i++) {
			int nextIndexF;
			if (i != normalsOuterPolyF.size() - 1) {
				nextIndexF = i + 1;
			}
			else {
				nextIndexF = 0;
			}
			for (int j = 0; j < polyM.verts.size(); j++) {
				int prevIndex;
				int nextIndex;
				if (j == 0) {
					prevIndex = polyM.verts.size() - 1;
					nextIndex = j + 1;
				}
				else if (j == polyM.verts.size() - 1) {
					prevIndex = j - 1;
					nextIndex = 0;
				}
				else {
					prevIndex = j - 1;
					nextIndex = j + 1;
				}
				float condition1 = Scalar(Vector2(polyM.verts[j], polyM.verts[prevIndex]), normalsOuterPolyF[i]);
				float condition2 = Scalar(Vector2(polyM.verts[j], polyM.verts[nextIndex]), normalsOuterPolyF[i]);
				if ((condition1 >= 0) && (condition2 >= 0)) {
					Point from;
					from = polyF.verts[i];
					from.Move(Vector2(polyM.verts[j], polyM.verts[0]));
					Point to;
					to = polyF.verts[nextIndexF];
					to.Move(Vector2(polyM.verts[j], polyM.verts[0]));
					Vector2 movement(from, to);
					movements.push_back(movement);
				}
			}
		}
		for (int i = 0; i < normalsOuterPolyM.size(); i++) {
			int nextIndexM;
			if (i != normalsOuterPolyM.size() - 1) {
				nextIndexM = i + 1;
			}
			else {
				nextIndexM = 0;
			}
			for (int j = 0; j < polyF.verts.size(); j++) {
				int prevIndex;
				int nextIndex;
				if (j == 0) {
					prevIndex = polyF.verts.size() - 1;
					nextIndex = j + 1;
				}
				else if (j == polyF.verts.size() - 1) {
					prevIndex = j - 1;
					nextIndex = 0;
				}
				else {
					prevIndex = j - 1;
					nextIndex = j + 1;
				}
				float condition1 = Scalar(Vector2(polyF.verts[j], polyF.verts[prevIndex]), normalsOuterPolyM[i]);
				float condition2 = Scalar(Vector2(polyF.verts[j], polyF.verts[nextIndex]), normalsOuterPolyM[i]);
				if ((condition1 >= 0) && (condition2 >= 0)) {
					Point from;
					from = polyF.verts[j];
					from.Move(Vector2(polyM.verts[i], polyM.verts[0]));
					Point to;
					to = polyF.verts[j];
					to.Move(Vector2(polyM.verts[nextIndexM], polyM.verts[0]));
					Vector2 movement(from, to);
					movements.push_back(movement);
				}
			}
		}
		
		vector<vector<pair<Point, int>>> moveInters;
		moveInters.resize(movements.size());
		for (int i = 0; i < movements.size(); i++) {
			for (int j = 0; j < movements.size(); j++) {
				Line currentMove(movements[i]);
				if (i != j) {
					pair<Point, int> inter;
					Line anotherMove(movements[j]);
					Point intersection = currentMove.intersection(anotherMove);
					if (intersection.errorCode != 1) {
						float condition = Scalar(intersection, movements[i].from, intersection, movements[i].to);				
						if (condition <= 0) {
							inter.first = intersection;
							inter.second = j;
							moveInters[i].push_back(inter);
						}
					}
				}
			}

			for (int j = 0; j < moveInters[i].size(); j++) {
				vector<float> distances(moveInters[i].size());
				for (int k = j; k < moveInters[i].size(); k++) {
					float distance = Vector2::Distance(movements[i].from, moveInters[i][k].first);
					distances[k] = distance;
				}
				int indexMin = j;
				int distanceMin = 9999;
				for (int k = j; k < distances.size(); k++) {
					if (distances[k] < distanceMin) {
						indexMin = k;
						distanceMin = distances[k];
					}
				}
				pair<Point, int> buf;
				buf.first = moveInters[i][j].first;
				buf.second = moveInters[i][j].second;
				moveInters[i][j].first = moveInters[i][indexMin].first;
				moveInters[i][j].second = moveInters[i][indexMin].second;
				moveInters[i][indexMin].first = buf.first;
				moveInters[i][indexMin].second = buf.second;
			}
		}

		int currentMove;
		int nextMove;
		int nextMovePointIndex;
		int initialMove = 0;
		Point initialPoint;
		Point nextPoint;
		vector<vector<bool>> checked;
		vector<bool> checkedMoves;
		Poly temp;
		checked.resize(moveInters.size());
		for (int i = 0; i < moveInters.size(); i++) {
			checked[i].resize(moveInters[i].size());
		}
		checkedMoves.resize(moveInters.size());
		for (int i = 0; i < moveInters.size(); i++) {
			for (int j = 0; j < moveInters[i].size(); j++) {
				if ((checked[i][j] == false) && (checkedMoves[i] == false)) {
					if (j != moveInters[i].size() - 1) {
						if ((geomSearch(polyF, moveInters[i][j].first, 1)) ||
							(geomSearch(polyF, moveInters[i][j + 1].first, 1))) {
							checked[i][j] = true;
							continue;
						}
						else {
							initialPoint = moveInters[i][j].first;
							nextMove = moveInters[i][j + 1].second;
							for (int k = 0; k < moveInters[nextMove].size(); k++) {
								if ((moveInters[i][j + 1].first == moveInters[nextMove][k].first) &&
									(k != moveInters[nextMove].size() - 1)) {
									nextPoint = moveInters[nextMove][k + 1].first;
									nextMovePointIndex = k + 1;
									break;
								}
								else {
									nextPoint = Point(-1);
								}
							}
							temp.verts.push_back(initialPoint);
							temp.verts.push_back(moveInters[i][j + 1].first);
							checked[i][j] = true;
							checked[i][j + 1] = true;
							checkedMoves[i] = true;
							while ((nextPoint != initialPoint) &&
								(!geomSearch(polyF, nextPoint)) &&
								(nextPoint.errorCode != -1)) {
								temp.verts.push_back(nextPoint);
								checked[nextMove][nextMovePointIndex] = true;
								checkedMoves[nextMove] = true;
								currentMove = nextMove;
								nextMove = moveInters[nextMove][nextMovePointIndex].second;
								for (int k = 0; k < moveInters[nextMove].size(); k++) {
									if ((moveInters[currentMove][nextMovePointIndex].first == moveInters[nextMove][k].first) &&
										(k != moveInters[nextMove].size() - 1)) {
										nextPoint = moveInters[nextMove][k + 1].first;
										nextMovePointIndex = k + 1;
										break;
									}
									else {
										checked[nextMove][k] = true;
										nextPoint = Point(-1);
									}
								}
							}
							
						}
					}
				}
			}
		}

		for (int i = 0; i < temp.verts.size(); i++) {
			if (DoesNotContainAsVertex(*charpoly, temp.verts[i])) {
				charpoly->verts.push_back(temp.verts[i]);
			}
		}

		return 1;
	}
	vector<Point> TrajectoryForB(Poly charBF, Poly x) {
		vector<Point> intersection;
		for (int i = 0; i < charBF.verts.size(); i++) {
			int nextIndex;
			if (i == charBF.verts.size() - 1) {
				nextIndex = 0;
			}
			else {
				nextIndex = i + 1;
			}

			if (geomSearch(x, charBF.verts[i])) {
				intersection.push_back(charBF.verts[i]);
				if (geomSearch(x, charBF.verts[nextIndex])) {
					continue;
				}
				else {
					Line edge(charBF.verts[i], charBF.verts[nextIndex]);
					vector<Point> edgeInters = x.FindIntersectionsPolyLine(edge);
					for (int j = 0; j < edgeInters.size(); j++) {
						float condition = Scalar(edgeInters[j], charBF.verts[i], edgeInters[i], charBF.verts[nextIndex]);
						if (condition <= 0) {
							intersection.push_back(edgeInters[j]);
						}
					}
				}
			}
			else {
				if (geomSearch(x, charBF.verts[nextIndex])) {
					Line edge(charBF.verts[i], charBF.verts[nextIndex]);
					vector<Point> edgeInters = x.FindIntersectionsPolyLine(edge);
					for (int j = 0; j < edgeInters.size(); j++) {
						float condition = Scalar(edgeInters[j], charBF.verts[i], edgeInters[i], charBF.verts[nextIndex]);
						if (condition <= 0) {
							intersection.push_back(edgeInters[j]);
						}
					}
				}
			}
		}
		LogNew("chain", "intersection", 0, intersection);
		return intersection;
	}

	vector<pair<Point, Point>>* TouchPositions(Poly polyF, Poly polyA, Poly polyB) {
		Poly charAF;
		Poly charBF;
		Poly outCharAB;
		Poly x;
		int error1 = CharPoly(polyF, polyA, &charAF);
		LogNew("poly", "charpolyinner", 1, charAF.verts);
		int error2 = CharPoly(polyF, polyB, &charBF);
		LogNew("poly", "charpolyinner", 2, charBF.verts);
		int error3 = OuterCharPoly(polyB, polyA, &outCharAB);
		LogNew("poly", "charpolyouter", 1, outCharAB.verts);
		int error4 = OuterCharPoly(charAF, outCharAB, &x);
		LogNew("poly", "charpolyouter", 2, x.verts);

		bool alwaysIntersects = true;
		for (int i = 0; i < charAF.verts.size(); i++) {
			if (!geomSearch(outCharAB, charAF.verts[i])) {
				alwaysIntersects = false;
			}
		}
		if (alwaysIntersects) {
			return 0;
		}
		vector<pair<Point, Point>> positions;
		vector<Point> trajectoryB = TrajectoryForB(charBF, x);
		if (trajectoryB.size() != 0) {
			
			for (int i = 0; i < trajectoryB.size(); i++) {
				LogMove("poly", "charpolyouter", 1, outCharAB.verts[0], trajectoryB[i]);
				outCharAB.ParallelMove(outCharAB.verts[0], trajectoryB[i]);
				vector<Point> newinters = outCharAB.FindIntersectionsPolyPoly(charAF);
				for (int j = 0; j < newinters.size(); j++) {
					positions.push_back(pair<Point, Point>(newinters[j], trajectoryB[i]));
				}
			}
			
			return &positions;
		}
		else {
			return &positions;
		}
	}
	
}

class Command {
public:
	string action;
	string object;
	int objectDim;
	vector<Point> params;
	vector<int> paramsIndex;

	Command(istream& is) {
		getline(is, action, ' ');

		if (action == "new") {
			getline(is, object, ' ');

			string buf;

			getline(is, buf);
			objectDim = stoi(buf);

			params.resize(objectDim);
			for (int i = 0; i < objectDim; i++) {
				getline(is, buf, ':');
				params[i].x = stof(buf);
				getline(is, buf);
				params[i].y = stof(buf);
			}
		}
		else if (action == "create") {
			getline(is, object, ' ');

			if (object == "charpolyinner") {
				string buf;
				paramsIndex.resize(1);
				getline(is, buf);
				paramsIndex[0] = stoi(buf);

			}
			else if ((object == "charpolyouter") || (object == "touchp")) {
				string buf;
				paramsIndex.resize(2);
				getline(is, buf, ' ');
				paramsIndex[0] = stoi(buf);
				getline(is, buf);
				paramsIndex[1] = stoi(buf);
			}
		}
	}

};

int main()
{
	ofstream ffout;
	ffout.open("output.txt", ios::trunc);
	ffout.close();

	ifstream fin;
	fin.open("input.txt");
	
	Poly polyF;
	vector<Poly> polyM;

	while (fin) {
		Command newcom(fin);

		if (newcom.action == "new") {
			if (newcom.object == "polyF") {
				polyF.verts = newcom.params;
				LogNew("poly", "polyF", 0, polyF.verts);
			}
			else if (newcom.object == "polyM") {
				Poly buf;
				buf.verts = newcom.params;
				polyM.push_back(buf);
				
				//char buf2;
				//_itoa(newcom.params.size(), &buf2, 10);
				string object = "polyM"; //+ buf2;
				LogNew("poly", object, polyM.size(), newcom.params);
			}
		}
		else if (newcom.action == "create") {
			if (newcom.object == "charpolyinner") {
				Poly charpoly;
				int error = CharPoly(polyF, polyM[newcom.paramsIndex[0]], &charpoly);
				LogNew("poly", "charpoly", 0, charpoly.verts);
			}
			else if (newcom.object == "charpolyouter") {
				Poly charpolyout;
				int error = OuterCharPoly(polyM[newcom.paramsIndex[0]], polyM[newcom.paramsIndex[1]], &charpolyout);
				LogNew("poly", "charpolyouter", 0, charpolyout.verts);
			}
			else if (newcom.object == "touchp") {
				vector<pair<Point, Point>>* result;
				result = TouchPositions(polyF, polyM[newcom.paramsIndex[0]], polyM[newcom.paramsIndex[1]]);
				//add log for points
			}
		}
	}
	fin.close();

	//if(InputTest(1) != 0) return 0;
	/*
	polyF.AddVert(0, 0);
	polyF.AddVert(0, 4);
	polyF.AddVert(4, 4);
	polyF.AddVert(4, 0);
	*/

	/*
	polyF.AddVert(0, 0);
	polyF.AddVert(0, 4);
	polyF.AddVert(4, 4);
	polyF.AddVert(4, 2);
	polyF.AddVert(3, 0);
	*/
	//LogNew("poly", "polyF", polyF.verts);

	/*
	polyF.AddVert(1, 0);
	polyF.AddVert(0, 2);
	polyF.AddVert(0, 5);
	polyF.AddVert(3, 6);
	polyF.AddVert(5, 6);
	polyF.AddVert(5, 0);
	*/
	/*
	Point p1(1, 3);
	Point p2(3, 1);
	Point p3(1, 1);
	Point p4(0, 1);
	Point p5(1, 0);
	Point p6(4, 1);
	Point p7(1, 4);
	Point p8(5, 5);
	Point p9(5, 4);
	Point p10(4, 5);
	Point p11(-1, 0);
	Point p12(0, -1);
	Point p13(-1, -1);
	Point p14(-1, 1);
	Point p15(1, -1);
	Point p16(7, 8);
	Point p17(8, 8);

	

	

	bool test1 = geomSearch(polyF, p1);
	bool test2 = geomSearch(polyF, p2);
	bool test3 = geomSearch(polyF, p3);
	bool test4 = geomSearch(polyF, p4);
	bool test5 = geomSearch(polyF, p5);
	bool test6 = geomSearch(polyF, p6);
	bool test7 = geomSearch(polyF, p7);
	bool test8 = geomSearch(polyF, p8);
	bool test9 = geomSearch(polyF, p9);
	bool test10 = geomSearch(polyF, p10);
	bool test11 = geomSearch(polyF, p11);
	bool test12 = geomSearch(polyF, p12);
	bool test13 = geomSearch(polyF, p13);
	bool test14 = geomSearch(polyF, p14);
	bool test15 = geomSearch(polyF, p15);
	bool test16 = geomSearch(polyF, p16);
	bool test17 = geomSearch(polyF, p17);

	Point a(0,3);
	Point b(3, 3);
	Line l(a, b);
	vector<Point> test = polyF.FindIntersectionsPolyLine(l);
	*/


	/*
	polyF.AddVert(1, 0);
	polyF.AddVert(0, 2);
	polyF.AddVert(0, 5);
	polyF.AddVert(3, 6);
	polyF.AddVert(5, 6);
	polyF.AddVert(5, 0);
	*/
	/*
	polyF.AddVert(0, 0);
	polyF.AddVert(0, 4);
	polyF.AddVert(4, 4);
	polyF.AddVert(4, 0);
	*/
	/*
	polyM.AddVert(1, 0);
	polyM.AddVert(0, 1);
	polyM.AddVert(1, 2);
	polyM.AddVert(2, 1);
	*/
	/*
	polyM.AddVert(2, 1);
	polyM.AddVert(0, 2);
	polyM.AddVert(1, 4);
	LogNew("poly", "polyM", polyM.verts);
	*/
	/*
	Poly charPoly;
	int error1 = CharPolyAlter(polyF, polyM, &charPoly);
	LogNew("poly", "charPoly", charPoly.verts);
	*/
	//int error = CharPoly(polyF, polyM, &charPoly);
	//int error = CharPoly(polyF, polyM, &charPoly);
	//Point special(1, 1);
	//int error = OuterCharPoly(polyF, polyM, &charPoly, special);
	//Output(poly, "output.txt");
    return 0;
}