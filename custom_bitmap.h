#pragma once

#include <vector>

using namespace std;

typedef unsigned char BYTE; // 1 Byte = 8 bits
typedef unsigned short WORD; // 2 Bytes = 16 bits
typedef unsigned long DWORD; // 4 Bytes = 32 bits

class Pixel {
	BYTE red;
	BYTE green;
	BYTE blue;
public:
	void setColor(BYTE red, BYTE green, BYTE blue);
	void setColor(DWORD value);
	BYTE getRed(){ return red; }
	BYTE getGreen(){ return green; }
	BYTE getBlue(){ return blue; }
};

class Word {
	WORD dec_value;
	WORD word_value;
	BYTE byte[2];
public:
	void setValue(WORD value);

	BYTE getByte(int index) { return byte[index]; }
	WORD getDec() { return dec_value; }
	WORD getWord() { return word_value; }
};

class DWord {
	DWORD dec_value;
	DWORD dword_value;
	BYTE byte[4];
public:
	DWord() {}
	DWord(DWORD value);

	void setValue(DWORD value);

	BYTE getByte(int index) { return byte[index]; }
	DWORD getDec() { return dec_value; }
	DWORD getDword() { return dword_value; }
};

class BMP_file{
	BYTE padding;
	BYTE file_header[14];
	BYTE bitmap_header[40];
	vector<BYTE> bitmap_data;
public:
	void setHeader(int width, int height);
	void setData(Pixel* pixelMap, int width, int heigth);
	int get_bitmap_size() { return bitmap_data.size(); }

	BYTE get_file_header(int index);
	BYTE get_bitmap_header(int index);
	BYTE get_bitmap_data(int index);
	BYTE get_padding() { return padding; }
};