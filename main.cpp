#include <iostream>
#include <string>

#include "Setup.h"

using namespace std;

const int CENTER = -1;
const int n_colors = 64;
const double e = exp(1);
const double pi = atan(1) * 4;

enum StyleList {Exp, Log, Sqrt, Line, Quad, Sine, Clean};

double distanceFromCenter(double a, double b);

void convert(int x, int y, double &newx, double &newy, Setup set0);
void printTestPage(Setup &set0);

double getStyle(double iter, double maxIter, int style);
double avgStyle(double iter, double maxIter, int style1, int style2);
double avgStyle2(double iter, double maxIter, int style1, int style2);

int userInput(Setup &set0);


int main()
{
	Setup set1;

	double distance;
	double mathx, mathy;
	double newx, newy, tempx;
	double xSq, ySq;
	double final_style;

	int width, height;
	int iter_limit;
	int style1, style2;
	int isMandel = 1;

	char draw;

	char colors[n_colors] = "#:,.:-_-_-YVY:;:;:;nm**^*^*ooocgcgcgc1I1I1I1I1I+o+*'/|-_-;:,.,.";

	printTestPage(set1);

	do 
	{
		height = set1.getHeight();
		width = set1.getWidth();
		iter_limit = set1.getMaxIter();
		style1 = set1.getStyle1();
		style2 = set1.getStyle2();

		for (int y=0; y<height; y++)
		{
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
						final_style = avgStyle(iter, iter_limit, style1, style2);

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
				else if (x==40 && y==40)
					cout << 'O';
				else if (isMandel==1)
					cout << ' ';
				else {
					cout << draw;
				}
			}

		}

	} while (userInput(set1) != 1);

	return 0;
}

double distanceFromCenter(double a, double b)
{
	if (a==0 && b==0)
		return -1;
	return sqrt(a*a + b*b);
}

double avgStyle2(double iter, double maxIter, int style1, int style2)
{
	if (style1 == style2)
		return getStyle(iter, maxIter, style1);
	return sqrt(getStyle(iter, maxIter, style1) * getStyle(iter, maxIter, style2));
}

double avgStyle(double iter, double maxIter, int style1, int style2)
{
	if (style1 == style2)
		return getStyle(iter, maxIter, style1);
	return (getStyle(iter, maxIter, style1) + getStyle(iter, maxIter, style2)) / 2;
}

void convert(int x, int y, double &newx, double &newy, Setup set0)
{
	double zoomMultip = pow(2, set0.getZoom());

	newx = zoomMultip* (x + x - set0.getWidth()) - set0.getOffsetX();
	newx /= set0.getWidth() + set0.getWidth();

	newy = zoomMultip* (y + y - 1 - set0.getHeight()) - set0.getOffsetY();
	newy /= set0.getHeight() + set0.getHeight();
}

double getStyle(double iter, double maxIter, int style)
{
	// 0:
	if (style == Exp) // 1 - { 1 / [e ^ ( 10 * iter / 1000 ) ] }       (DAFUQ?!)
	{
		iter = iter * 10;
		iter = iter / maxIter;
		iter = pow(e, iter);
		iter = 1 / iter;
		iter = 1 - iter;
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

	else if (command == "q") set0.zoomIn(1);
	else if (command == "e") set0.zoomIn(-1);
	else if (command == "Q") set0.zoomIn(0.25);
	else if (command == "E") set0.zoomIn(-0.25);

	else if (command == "style")
	{
		string styleNames[7] = { "Exp", "Log", "Sqrt", "Line", "Quad", "Sine", "Clean" };

		cout << "style 1: " << styleNames[set0.getStyle1()] << endl;
		cout << "style 2: " << styleNames[set0.getStyle2()] << endl;
		cout << "set new styles? (y/n): ";
		cin >> command;

		if (command == "y" || command == "Y" || command == "yes")
		{
			int s1, s2;

			for (int i = 0; i < 7; i++)
				cout << styleNames[i] << "=" << i << ", ";
			
			cout << endl;
			cout << "style 1:";
			cin >> s1;
			cout << "style 2:";
			cin >> s2;

			set0.setStyle(s1, s2);
		}
	}

	else if (command == "iter")
	{
		int iter;

		cout << "iteration limit: " << set0.getMaxIter() << endl;
		cout << "new iteration limit: ";
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
		convert(40, 40, x, y, set0);

		cout << "(" << x;
		cout << ", " << y;
		cout << "i), at zoom level " << "2^" << -set0.getZoom() << " (" << pow(2, -set0.getZoom()) << "x)";
		cout << endl;

		return userInput(set0);
	}

	else if (command == "help")
	{
		cout << "\tw\t - \tmove up" << endl;
		cout << endl << "\ta\t - \tmove left" << endl;
		cout << endl << "\ts\t - \tmove down" << endl;
		cout << endl << "\td\t - \tmove right" << endl;
		cout << endl << "\tq\t - \tnormal zoom out" << endl;
		cout << endl << "\tQ\t - \tquarter zoom out" << endl;
		cout << endl << "\te\t - \tnormal zoom in" << endl;
		cout << endl << "\tE\t - \tnormal zoom in" << endl;
		cout << endl << "\tstyle\t - \tview and set gradient color picking style" << endl;
		cout << endl << "\titer\t - \tshow and change iteration limit" << endl;
		cout << endl << "\treset\t - \tredraw image and reset position and zoom" << endl;
		cout << endl << "\tgetinfo\t - \tget current position info" << endl;
		cout << endl << "\thelp\t - \tshow this" << endl;
		cout << endl << "\texit\t - \texit" <<  endl;

		//for (int i = 0; i < 52; i++)
		//	cout << endl;

		return userInput(set0);
	}

	else if (command == "exit") return 1;

	else return 0;

	return 0;
}

void printTestPage(Setup &set0)
{
	//for (int i = 0; i < 80; i++)
	//	cout << ".";

	//for (int j = 1; j < 76; j++)
	//	for (int i = 0; i < 80; i++)
	//		if (i == 0 || i == 79)
	//			cout << ".";
	//		else
	//			cout << " ";

	//for (int i = 0; i < 80; i++)
	//	cout << ".";

	cout << endl << "Set the console font to \"raster font\" with size 8x8 pixels." << endl << endl;
	cout << "Maximize the console window" << endl << endl;
	cout << "Type 'reset' to close this screen or 'help' for list of commands." << endl;

	//cout << "Scroll up! ";
	userInput(set0);
}