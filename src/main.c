#include "framebuffer.h"
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

void draw_line(framebuffer_t fb, int x1, int y1, int x2, int y2) {
  // y = mx + b
  float m = (float)(y2 - y1) / (float)(x2 - x1);
  int b = y2 - (int)(x2 * m);
 
  int x_step = 1;
  if (x1 > x2) {
    x_step = -1;
  }
  
  int y_step = 1;
  if (y1 > y2) {
    y_step = -1;
  }
  
  // When m is infinite we take special measures.
  if (m == 1.0/0.0 || m == -1.0/0.0) {
    for (int steps = 0; steps < abs(y1 -y2); steps++) {
      int y = y1 + (steps * y_step);
      set_pixel(fb, x1, y);
    }
    return;
  }

  int last_y = y1;
  for (int steps = 0; steps < abs(x2 - x1); steps++) {
    int x = x1 + (steps * x_step);
    int last_y = (int)(x * m) + b;
    set_pixel(fb, x, last_y);
  }

  for (int steps = 0; steps < abs(y2 - last_y); steps++) {
    int y = last_y + (steps * y_step); 
    int x = (y - b) / m;
    set_pixel(fb, x, y);
    y = y + y_step; 
  }
}

int main(int argc, char** argv) {
  framebuffer_t fb = new_framebuffer(120, 60);
  struct timespec waittime = {.tv_sec = 0, .tv_nsec = 999999999 / FPS};

	double v = 1.0; 
  double a = 0.0;

  int midpoint_x = fb->width >> 1;
  int midpoint_y = fb->height >> 1;

  clear_fb(fb);
  
  draw_line(fb, 20, 0, 100, 0);
  draw_line(fb, 20, 0, 20, 60);
  draw_line(fb, 100, 0, 100, 60);
  draw_line(fb, 20, 59, 101, 59);
	printf("%s", COLOR_RESET);

	const char colors[3][1024] = { 
		COLOR_RED,
		COLOR_YELLOW,
		COLOR_PURPLE 
	};

	int color_i = 0;
  while (1) {

    //draw_line(fb, midpoint_x, midpoint_y, c_x, c_y);
		for (int i = 0; i < 512; i++) {
      int c_x = midpoint_x + (int)v*cos(a);
      int c_y = midpoint_y + (int)v*sin(a);
			set_pixel_with_color(fb, c_x, c_y, colors[color_i]);
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
    draw_fb(fb);
    nanosleep(&waittime, NULL);
    system("clear");
    //clear_fb(fb);
  };
}
