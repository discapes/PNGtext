#include "lodepng.h"
#include <iostream>
#include <fstream>
#include <vector>
#pragma warning ( disable : 26451 )

std::vector<unsigned char> upscaleVector(std::vector<unsigned char>& vec, int scale);
std::vector<unsigned char> downscaleVector(std::vector<unsigned char>& vec, int scale);
void decode(const char* inputFile, int scale);
void encode(const char* inputFile, int scale);

int main()
{
	encode("beemovie.txt", 10); // scale 100 > encode > decode > cool results
	decode("out.png", 10);
}

std::vector<unsigned char> downscaleVector(std::vector<unsigned char>& vec, int scale)
{
	if (scale == 1) return vec;
	std::vector<unsigned char> result(vec.size() / scale);
	for (int i = 0; i < result.size(); i++)
	{
		result[i] = vec[i * scale + rand() % scale]; // add +1 or +2 etc for epicness (lower scale more epicness)
	}
	return result;
}

std::vector<unsigned char> upscaleVector(std::vector<unsigned char>& vec, int scale)
{
	if (scale == 1) return vec;
	std::vector<unsigned char> result(vec.size() * scale);
	for (int i = 0; i < vec.size(); i++)
	{
		for (int j = 0; j < scale; j++)
		{
			result[i * scale + j] = vec[i];
		}
	}
	return result;
}

void encode(const char* inputFile, int scale)
{
	std::ifstream file(inputFile);
	std::vector<unsigned char> image;
	if (!file.eof() && !file.fail())
	{
		file.seekg(0, std::ios_base::end);
		std::streampos fileSize = file.tellg();
		image.resize(fileSize);

		file.seekg(0, std::ios_base::beg);
		file.read((char*)&image[0], fileSize);
	}
	image = upscaleVector(image, scale);
	int pixels = (int)(image.size() / 4 + image.size() % 4);
	int width = (int)ceil(sqrt(pixels));
	int height = width;
	image.resize((width * width) * 4);
	unsigned error = lodepng::encode("out.png", image, width, height);
	if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}

void decode(const char* inputFile, int scale)
{
	std::vector<unsigned char> image;
	unsigned width, height;

	unsigned error = lodepng::decode(image, width, height, inputFile);
	if (error)
	{
		std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;
		std::exit(error);
	}
	image = downscaleVector(image, scale);
	for (auto it = image.begin(); it != image.end(); it++)
	{
		//if ((it - image.begin()) % 4 == 3) continue; // skip alpha
		putchar(*it);
	}
}