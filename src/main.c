#include "framebuffer.h"
#include "graphics.h"
#include "math.h"
#include "stdlib.h"
#include "stdio.h"
#include "time.h"

#define FPS 16

struct point3 {
  float x;
  float y;
  float z;
};

int main(int argc, char** argv) {
  framebuffer_t fb = new_framebuffer(120, 60);
  struct timespec waittime = {.tv_sec = 0, .tv_nsec = 999999999 / FPS};

	double v = 1.0; 
  double a = 0.0;

  int midpoint_x = fb->width >> 1;
  int midpoint_y = fb->height >> 1;

  fb_clear(fb);
  
	const char colors[3][1024] = { 
		FB_COLOR_RED,
		FB_COLOR_YELLOW,
		FB_COLOR_PURPLE 
	};

	int color_i = 0;
  while (1) {
    draw_line(fb, 20, 0, 100, 0);
    draw_line(fb, 20, 0, 20, 60);
    draw_line(fb, 100, 0, 100, 60);
    draw_line(fb, 20, 59, 101, 59);
    printf("%s", FB_COLOR_RESET);

		for (int i = 0; i < 1024; i++) {
      int c_x = midpoint_x + (int)v*cos(a);
      int c_y = midpoint_y + (int)v*sin(a);
			fb_set_pixel_with_color(fb, c_x, c_y, colors[color_i]);
			a = a + 0.001;
			v = v + 0.001;
			if (a > 2.0 * 3.14159) {
				a = 0.0;
			}
			if (v > 50.0) {
				v = 1.0;
        color_i = (color_i + 1) % 3;
			}
		}
    blit(fb);
    nanosleep(&waittime, NULL);
    fb_clear(fb);
    system("clear");
  };
}
