#include <string>
#include <fstream>
#include <iostream>

#include "custom_bitmap.h"
#include "Setup.h"

using namespace std;

const int CENTER = -1;
const int n_colors = 64;
const double e = exp(1);
const double pi = atan(1) * 4;

enum StyleList {LogLog, Log, Sqrt, Line, Quad, Sine, Clean};

double distanceFromCenter(double a, double b);

void convert(int x, int y, double &newx, double &newy, Setup set0);
void printTestPage();

double getStyle(double iter, double maxIter, int style);
double avgStyle(double iter, double maxIter, int style1, int style2);
double avgStyle2(double iter, double maxIter, int style1, int style2);

int userInput(Setup &set0);

void renderBMP(Setup set0);

int main()
{
	Setup set1;

	double distance;
	double mathx, mathy;
	//double testx, testy;
	double newx, newy, tempx;
	double xSq, ySq;
	double final_style;

	int width, height;
	int iter_limit;
	int style1;
	int isMandel = 1;

	char draw;

	char colors[n_colors] = "#:,.:-_-_-YVY:;:;:;nm**^*^*ooocgcgcgc1I1I1I1I1I+o+*'/|-_-;:,.,.";

	printTestPage();

	while (!userInput(set1))
	{
		height = int(set1.getHeight());
		width = int(set1.getWidth());
		iter_limit = set1.getMaxIter();
		style1 = set1.getStyle1();

		for (int y=0; y<height; y++)
			for (int x=0; x<width; x++)
			{
				convert(x, y, mathx, mathy, set1);
				
				newx = mathx;
				newy = mathy;	

				isMandel = 1;
				draw = ' ';

				for (int iter = 0; iter < iter_limit; iter++)
				{
					xSq = newx*newx;
					ySq = newy*newy;

					if (4 <= xSq + ySq)
					{
						isMandel=0;
						final_style = getStyle(iter, iter_limit, style1);

						if (final_style < 0) final_style = 0;
						else if (final_style > 1) final_style = 1;

						draw = colors [(int)(n_colors * final_style)];

						break;
					}

					tempx = xSq - ySq;
					newy = (newx + newx) * newy;
					newx = tempx;
					newy += mathy;
					newx += mathx;
				}

				distance=distanceFromCenter(mathx,mathy);
				
				if (distance==CENTER)
					cout << '+';
				else if (x==40 && y==39)
					cout << 'o';
				else if (isMandel==1)
					cout << ' ';
				else {
					cout << draw;
				}
				
			}

		/* DEBUG ONLY
		cout << endl;
		convert(0, 0, testx, testy, set1);
		cout << "screen's ( 0,  0) equals to mathematic (" << testx << ", " << testy << ")" << endl;
		convert(40, 39, testx, testy, set1);
		cout << "screen's (40, 39) equals to mathematic (" << testx << ", " << testy << ")" << endl;
		convert(80, 78, testx, testy, set1);
		cout << "screen's (80, 78) equals to mathematic (" << testx << ", " << testy << ")" << endl;
		cout << "zoom = 2^" << set1.getZoom() << " (" << pow(2, set1.getZoom()) << ")" << endl;
		*/
	}

	return 0;
}

double distanceFromCenter(double a, double b)
{
	if (a==0 && b==0)
		return -1;
	return sqrt(a*a + b*b);
}

void convert(int x, int y, double &newx, double &newy, Setup set0)
{
	double z = pow(2,set0.getZoom());
	double ox = set0.getOffsetX();
	double oy = set0.getOffsetY();
	double w = set0.getWidth();
	double h = set0.getHeight();

	newx = x + ox*z;
	newx /= z;
	newx -= (w / 2) / z;

	newy = y + oy*z;
	newy /= z;
	newy -= (h / 2) / z;
}

double getStyle(double iter, double maxIter, int style)
{
	// 0:
	if (style == LogLog) // loglog ftw
	{
		iter = iter + 1;
		iter = log(iter);
		iter = iter / log(maxIter + 1);
		iter = iter + 1;
		iter = log(iter);
		iter = iter / log(maxIter + 1);
	}

	// 1:
	else if (style == Log) // logarithmic growth: log(iter+1) / log(1000+1)
	{
		iter = iter + 1;
		iter = log(iter);
		iter = iter / log(maxIter + 1);
	}

	// 2:
	else if (style == Sqrt) // exponential growth (square root): sqrt(iter/1000)
	{
		iter = iter / maxIter;
		iter = sqrt(iter);
	}

	// 3:
	else if (style == Line) // linear growth
	{
		iter = iter / maxIter;
	}

	// 4:
	else if (style == Quad) // quadratic growth
	{
		iter = iter / maxIter;
		iter = iter * iter;
	}

	// 5:
	else if (style == Sine) // sinusoidal growth
	{
		iter = iter / maxIter;
		iter = iter + 1.5;
		iter = iter * pi;
		iter = sin(iter);
		iter = iter / 2;
		iter = iter + 0.5;
	}

	// 6:
	else if (style == Clean) // always pick brightest
	{
		iter = 1;
		iter = iter / n_colors;
	}

	// 7-9:
	else // always pick darkest
	{
		iter = n_colors - 1;
		iter = iter / n_colors;
	}

	return iter;
}

int userInput(Setup &set0)
{
	string command;
	cout << "> ";
	cin >> command;

	if		(command == "w") set0.goUp();
	else if (command == "s") set0.goDown();
	else if (command == "a") set0.goLeft();
	else if (command == "d") set0.goRight();

	else if (command == "q") set0.zoomIn(-1);
	else if (command == "e") set0.zoomIn(1);
	else if (command == "Q") set0.zoomIn(-0.25);
	else if (command == "E") set0.zoomIn(0.25);

	else if (command == "style")
	{
		string styleNames[7] = { "LogLog", "Log", "Square Root", "Linear", "Quadratic", "Sine", "High Contrast" };

		cout << "Current style: " << styleNames[set0.getStyle1()] << endl;
		cout << "Set new style? (y/n): ";
		cin >> command;

		if (command == "y" || command == "Y" || command == "yes")
		{
			int s1;

			for (int i = 0; i < 7; i++)
				cout << i << "=" << styleNames[i] << ", ";

			cout << endl;
			cout << "Style number:";
			cin >> s1;

			set0.setStyle(s1);
		}
	}

	else if (command == "iter")
	{
		int iter;

		cout << "Current iteration limit: " << set0.getMaxIter() << endl;
		cout << "New iteration limit: ";
		cin >> iter;

		set0.setMaxIter(iter);
	}

	else if (command == "reset")
	{
		set0.reset();
	}

	else if (command == "getinfo")
	{
		double x, y;
		convert(40, 39, x, y, set0);

		cout << "Position: (" << x << ", " << y << "i)" << endl; 
		cout << "Zoom level: " << "2^" << set0.getZoom() << " (" << pow(2, set0.getZoom()) << "x)";
		cout << endl;

		return userInput(set0);
	}

	else if (command == "render" || command == "reset")
	{
		renderBMP(set0);
		return userInput(set0);
	}

	else if (command == "help")
	{
		cout << "\tw\t - \tmove up" << endl;
		cout << endl << "\t" << "a"			<< "\t - \t" << "move left" << endl;
		cout << endl << "\t" << "s"			<< "\t - \t" << "move down" << endl;
		cout << endl << "\t" << "d"			<< "\t - \t" << "move right" << endl;
		cout << endl << "\t" << "q"			<< "\t - \t" << "normal zoom out" << endl;
		cout << endl << "\t" << "Q"			<< "\t - \t" << "quarter zoom out" << endl;
		cout << endl << "\t" << "e"			<< "\t - \t" << "normal zoom in" << endl;
		cout << endl << "\t" << "E"			<< "\t - \t" << "normal zoom in" << endl;
		cout << endl << "\t" << "style"		<< "\t - \t" << "view and set gradient color picking style" << endl;
		cout << endl << "\t" << "iter"		<< "\t - \t" << "show and change iteration limit" << endl;
		cout << endl << "\t" << "reset"		<< "\t - \t" << "redraw image and reset position and zoom" << endl;
		cout << endl << "\t" << "getinfo"	<< "\t - \t" << "get current position info" << endl;
		cout << endl << "\t" << "help"		<< "\t - \t" << "show this" << endl;
		cout << endl << "\t" << "exit"		<< "\t - \t" << "exit" <<  endl;
		cout << endl << "\t" << "export"	<< "\t - \t" << "exports the current screen in .bmp format" << endl;

		return userInput(set0);
	}

	else if (command == "exit")
		return 1;

	else
	{
		cout << "Unrecognised command. Try entering 'help'" << endl;
		return userInput(set0);
	}

	return 0;
}

void printTestPage()
{
	cout << endl << "Set the console font to \"raster font\" with size 8x8 pixels." << endl << endl;
	cout << "Maximize the console window" << endl << endl;
	cout << "Type 'render' to close this screen or 'help' for list of commands." << endl << endl;
}

void renderBMP(Setup set0)
{
	double mathx, mathy;
	double newx, newy, tempx;
	double xSq, ySq;
	double final_style;
	double scale;

	DWORD color;
	BYTE padding;

	string filename;
	char inputChar;

	int width, height;
	int minDimension;
	int iter_limit;
	int style1, style2;
	int renderNo=0;

	iter_limit = set0.getMaxIter();
	style1 = set0.getStyle1();

	BMP_file bitmap01;

	fstream bmpfile;

	cout << "Image width: ";
	cin >> width;
	cout << "Image height: ";
	cin >> height;

	minDimension = width;
	if (height < minDimension)
		minDimension = height;
	
	scale = log(minDimension / 80) / log(2);

	set0.zoomIn(scale);
	set0.setRes(width, height);

	padding = ((4 - ((width * 3) % 4)) % 4);
	Pixel * pixMap = new Pixel[(width + padding)*height];

	cout << "File name: ";
	cin >> filename;

	bmpfile.open(filename);

	if (bmpfile.is_open())
	{
		cout << "Owerwrite? (y/n) ";
		cin >> inputChar;

		if (inputChar == 'n')
		{
			cout << "Export canceled." << endl;
			bmpfile.close();
			return;
		}
	}

	bmpfile.close();

	cout << "Fetching screen data... ";

	for (int y = 0; y<height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			convert(x, y, mathx, mathy, set0);

			newx = mathx;
			newy = mathy;

			color = 0;

			for (int iter = 0; iter < iter_limit; iter++)
			{
				xSq = newx*newx;
				ySq = newy*newy;

				if (4 <= xSq + ySq)
				{
					final_style = getStyle(iter, iter_limit, style1);

					if (final_style < 0) final_style = 0;
					else if (final_style > 1) final_style = 1;

					color = 0xFFFFFF;
					color *= final_style;
					if (color == 0)
						color = 0xFFFFFF;

					break;
				}

				tempx = xSq - ySq;
				newy = (newx + newx) * newy;
				newx = tempx;
				newy += mathy;
				newx += mathx;
			}

			if (x == int(width / 2) && y == int(height / 2))
				color = 0xFF0000;

			//color = 0xFFFFFF;
			pixMap[y*width + x].setColor(color);
		}
	}

	cout << "done! " << endl;
	cout << "Creating virtual BMP... ";

	bitmap01.setHeader(width, height);
	//cout << "DEBUG##" << filename << "/padding :" << int(bitmap01.get_padding()) << endl; DEBUG ONLY
	bitmap01.setData(pixMap, width, height);

	cout << "done! " << endl;

	delete pixMap;

	char * charTemp = new (char);

	//filename = "render.bmp";
	bmpfile.open(filename, fstream::out | fstream::trunc | fstream::binary);

	cout << "Writing header... ";
	for (int i = 0; i < 14; i++)
	{
		*charTemp = bitmap01.get_file_header(i);
		bmpfile.write(charTemp, sizeof(BYTE));
	}
	for (int i = 0; i < 40; i++)
	{
		*charTemp = bitmap01.get_bitmap_header(i);
		bmpfile.write(charTemp, sizeof(BYTE));
	}

	cout << "done!" << endl << "Writing bitmap... ";
	for (int i = 0; i < bitmap01.get_bitmap_size(); i++)
	{
		*charTemp = bitmap01.get_bitmap_data(i);
		bmpfile.write(charTemp, sizeof(BYTE));
	}

	cout << "done!" << endl;

	delete charTemp;

	cout << "Exporting done. File saved as: " << filename << endl;

	bmpfile.close();
}