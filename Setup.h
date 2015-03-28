#pragma once

#include <cmath>

class Setup {
	double zoom;
	double offsetx;
	double offsety;
	double width;
	double height;
	int maxIter;
	int style1;
	int style2;

	double multip() { return 10 * pow(2, zoom); }

public:
	int getMaxIter(){ return maxIter; }
	int getStyle1() { return style1; }
	int getStyle2() { return style2; }
	double getZoom() { return zoom; }
	double getHeight() { return height; }
	double getWidth() { return width; }
	double getOffsetX() { return offsetx; }
	double getOffsetY() { return offsety; }

	void setStyle	(int s1, int s2);
	void setMaxIter	(int iter) { maxIter = iter; }

	void goUp		()					{ offsety += multip(); }
	void goDown		()					{ offsety -= multip(); }
	void goRight	()					{ offsetx -= multip(); }
	void goLeft		()					{ offsetx += multip(); }
	void zoomIn		(double amount)		{ zoom += amount; }
	void reset();

	Setup();

	Setup(double zoom, double offset_x, double offset_y, double width, double height, int max_iter, int style1, int style2);
};