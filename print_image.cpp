#include <iostream>
#include <sys/ioctl.h>
#define cimg_display 0
#include "args.hpp"

#include <CImg/CImg.h>

using namespace cimg_library;

// Get a Value in an initial range (domain) and convert it into another range
// eg. sin(x) : [-1, 1] -> [3, 7]
template<typename T>
T map(T current_value,
	  T minimum_domain_value,
	  T maximum_domain_value,
	  T minimum_range_value,
	  T maximum_range_value);

// Get the Terminal size using ioctl and store it in these variables
// Fullscreen on my machine is (191x52)
void getSize(unsigned short &width, unsigned short &height);

/// MAIN FUNCTION
int main(int argc, char **argv)
{
	std::ios_base::sync_with_stdio(false);
	// Setup The Arguments
	Args arg_parser(argc, argv);
	arg_parser.convert();

	// Find Terminal Size
	unsigned short term_width, term_height;
	getSize(term_width, term_height);

	std::string image_path;

	for(auto &item: arg_parser.getOpts())
		if(item.second.mode == Option::str && item.second.name != argv[0U])
		{
			image_path = item.second.name;
			break;
		}

	CImg<unsigned char> image(image_path.c_str());
	/* image.blur(2.5f); */
	const bool grey_scale = image.spectrum() == 1U;

	// TODO If image size < term_size * [4, 8] just iterate over
	const unsigned int new_height = term_height * 8U;
	const unsigned int new_width = term_width * 4U;
	const float image_aspect = (float)image._width / (float)image._height;

	if(image_aspect < 1.f)
		// Vertical
		image.resize(new_height * image_aspect, new_height, -100, -100, 5);
	else
		// Horizontal
		image.resize(new_width, new_width / image_aspect, -100, -100, 5);

	for(unsigned int rows = 0U; rows <= image.height() - 8; rows += 8)
	{
		for(unsigned int cols = 0U; cols <= image.width() - 4; cols += 4)
		{
			if(grey_scale)
			{
				uint16_t b = image._atXY(cols, rows, 0, 0);
				printf("\033[38;2;%u;%u;%um", b, b, b);
				b = image._atXY(cols, rows + 4U, 0U, 0U);
				printf("\033[48;2;%u;%u;%um", b, b, b);
			}
			else
			{
				uint16_t r = image._atXY(cols, rows, 0, 0);
				uint16_t g = image._atXY(cols, rows, 0, 1);
				uint16_t b = image._atXY(cols, rows, 0, 2);
				printf("\033[38;2;%u;%u;%um", r, g, b);
				r = image._atXY(cols, rows + 4, 0, 0);
				g = image._atXY(cols, rows + 4, 0, 1);
				b = image._atXY(cols, rows + 4, 0, 2);
				printf("\033[48;2;%u;%u;%um", r, g, b);
			}
			printf("\u2580");
		}
		printf("\033[m\n");
	}
	return 0;
}

// Functions

template<typename T>
T map(T val, T min_dom, T max_dom, T min_ran, T max_ran)
{
	return ((val - min_dom) * (max_ran - min_ran) / (max_dom - min_dom) + min_ran);
}

void getSize(unsigned short &width, unsigned short &height)
{
	struct winsize size;
	ioctl(1, TIOCGWINSZ, &size);
	width = size.ws_col;
	height = size.ws_row;
}

