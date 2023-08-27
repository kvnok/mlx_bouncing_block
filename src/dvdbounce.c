#include "stuff.h"

#define WIDTH 2560
#define HEIGHT 1440

#define BLOCKWIDTH 180
#define BLOCKHEIGHT 100

static mlx_image_t* image;

int32_t r_color = 0;
int32_t g_color = 0;
int32_t b_color = 255;
int32_t a_color = 255;

uint32_t xdirection = 4;
uint32_t ydirection = 4;

uint32_t color_counter = 0;

int32_t pixel_select(int32_t r, int32_t g, int32_t b, int32_t a)
{
    return (r << 24 | g << 16 | b << 8 | a);
}

void update_color()
{
	switch(color_counter % 3)
	{
		case(0):
			r_color = 255;
			b_color = 0;
			break;
		case(1):
			r_color = 0;
			g_color = 255;
			break;
		case(2):
			g_color = 0;
			b_color = 255;
			break;
		default:
			break;
	}
	color_counter++;
}

void fill_square(void* param)
{
	for (int32_t i = 0; i < image->width; ++i)
	{
		for (int32_t y = 0; y < image->height; ++y)
		{
			uint32_t color = pixel_select(
				r_color, // R
				g_color, // G
				b_color, // B
				a_color  // A
			);
			mlx_put_pixel(image, i, y, color);
		}
	}
}

// 60 frames per second, so this function runs 60 times every second
void bounce(void* param)
{
	image->instances[0].x += xdirection;
	image->instances[0].y += ydirection;
	if (image->instances[0].y >= HEIGHT - BLOCKHEIGHT)
	{
		update_color();
		ydirection = -4;
	}
	if (image->instances[0].x >= WIDTH - BLOCKWIDTH)
	{
		update_color();
		xdirection = -4;
	}
	if (image->instances[0].x <= 0)
	{
		update_color();
		xdirection = 4;
	}
	if (image->instances[0].y <= 0)
	{
		update_color();
		ydirection = 4;
	}
}

int32_t main(int32_t argc, const char* argv[])
{
	mlx_t* mlx;

	// Gotta error check this stuff
	if (!(mlx = mlx_init(WIDTH, HEIGHT, "bouncing", true)))
	{
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	if (!(image = mlx_new_image(mlx, BLOCKWIDTH, BLOCKHEIGHT)))
	{
		mlx_close_window(mlx);
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	if (mlx_image_to_window(mlx, image, 0, 0) == -1)
	{
		mlx_close_window(mlx);
		puts(mlx_strerror(mlx_errno));
		return(EXIT_FAILURE);
	}
	mlx_loop_hook(mlx, fill_square, mlx);
	mlx_loop_hook(mlx, bounce, mlx);

	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (EXIT_SUCCESS);
}
