#pragma once
#include <vector>
using namespace std;
namespace charOblast {
	class Vector2;
	class Point {
	public:
		float x;
		float y;
		int errorCode;
		bool zero = false;
		bool operator==(Point pt);
		bool operator!=(Point pt);
		Point operator=(Point pt);
		void Move(Vector2 shift);
		Point Moved(Vector2 shift);
		Point(int code);
		Point(float x, float y);
		Point();
	};

	class Vector2 {
	public:
		Point from; //x
		Point to; //y
		Point coordinatePoint;
		bool operator==(Vector2 dir);
		Vector2 direction();
		static float Distance(Point a, Point b);
		float Distance();
		Point Coordinates();
		Vector2(Point from, Point to);
		Vector2(Point coordinatePoint);
		Vector2();
	};

	class edge {
	public:
		Point a;
		Point aPlusOne;
	};

	class Line {
	public:
		Point ptA;
		Point ptB;
		Point intersection(Line lineB);
		Line(Point pointA, Point pointB);
		Line(Vector2 direction);
		Line();
		static Line DrawHorizontalLine(Point pt);
		static Line DrawVerticalLine(Point pt);
	};
	class Ray :Line {
	public:
		Point start;
		Vector2 rayDir;
		Ray(Point start, Vector2 dir);
	};
	class Poly {
	public:
		vector<Point> verts;
		Point NextVertexAfter(Point pt);
		vector<Point> FindIntersectionsPolyLine(Line line);
		Point FindIntersectionsPolyRay(Ray ray);
		vector<Point> FindIntersectionsPolyPoly(Poly poly);
		Poly(const Poly& copy);
		Poly();
		//int CharPoly(Poly polyF, Poly * charpoly);
		//int OuterCharPoly(Poly polyF, Poly * charpoly);
		bool MovePossibility(Poly polyF, Vector2 movement);
		Poly ParallelMoved(Point moveFrom, Point moveTo);
		Poly ParallelMoved(Vector2 shift);
		void ParallelMove(Vector2 shift);
		void ParallelMove(Point moveFrom, Point moveTo);
		void AddVert(float x, float y);
	};
	bool geomSearch(Poly poly, Point pt, int excludeEdges = 0);
		/*
	int CharPoly(Poly polyF, Poly polyM, Poly * charpoly);
	int OuterCharPoly(Poly polyF, Poly polyM, Poly * charpoly);
	class Scene {
	public:
		Poly polyF;
		Poly polyM1;
		Poly polyM2;
		vector<Poly> innerCharPoly;
		vector<Poly> outerCharPoly;

		Scene();
		Scene(Poly polyf);
		Scene(Poly polyf, Poly polym1);
		Scene(Poly polyf, Poly polym1, Poly polym2);
		int PlacementPossibility(bool touchF, bool touchEachM);
		//void AddPolyM(Poly poly);
		//void OuterCharPolyCreate(int indexF, int indexM);
	};

	*/
}