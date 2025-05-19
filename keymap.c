#include <stdio.h>
#include <math.h>

#define X_SIZE 128
#define Y_SIZE 64
#define N 20
#define ZOOM 0.7
#define N_FRAMES 50
#define FRAME_TIMEOUT (1000/30)

#define FRAME_SIZE (Y_SIZE * X_SIZE / 8)

char frames[N_FRAMES][FRAME_SIZE] = {{0}};
static uint16_t anim_timer = 0;
static int frame_cnt = 0;

void plot(char frame[FRAME_SIZE], int x, int y) {
    frame[(y / 8) * X_SIZE + x] |= 1 << y % 8;
}

void render_frame(char frame[FRAME_SIZE], int cnt) {
    double phi = 2.0 * M_PI * cnt / N_FRAMES;
    for (int i = 0; i <= N; i++) {
        for (int j = 0; j <= N; j++) {
            double x = (i - N / 2.0) * 2 / N;
            double y = (j - N / 2.0) * 2 / N;
            double d = sqrt(x * x + y * y);
            double z = (sqrt(2) - d) * sin(phi - d * 5) * ZOOM;
            double rotation = phi / 4;
            int xp = X_SIZE * (1 + (x * sin(rotation) + y * cos(rotation)) * ZOOM) / 2;
            int yp = Y_SIZE * (1 + (z + (x * cos(rotation) - y * sin(rotation)) / 2) * ZOOM) / 2;
            plot(frame, xp, yp);
        }
    }
}

static void render_anim(void) {
    if (timer_elapsed(anim_timer) > FRAME_TIMEOUT) {
        anim_timer = timer_read();
        if (is_oled_on()) {
            oled_write_raw(frames[frame_cnt], FRAME_SIZE);
            if (++frame_cnt == N_FRAMES) {
                frame_cnt = 0;
            };
        }
    }
}

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    for (int i=0; i < N_FRAMES; i++) {
        render_frame(frames[i], i);
    }
    anim_timer = timer_read();
    return rotation;
}

bool oled_task_user(void)
{
    if (! is_keyboard_master()) {
        render_anim();
    }

    return false;
}

