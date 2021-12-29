#ifndef CONFETTI_COMMON
#define CONFETTI_COMMON

void waitTouchDown(int16_t& x, int16_t& y) {
  while (!ttgo->getTouch(x, y)) { }
}

void waitTouchUp(int16_t& x, int16_t& y) {
  while (ttgo->getTouch(x, y)) { }
}

#define MIN(x,y) (((x)<(y))?(x):(y))
#define X_IN_FIRST_QUARTER(x) ((x) < 60)
#define X_IN_SECOND_QUARTER(x) (60 <= (x) && (x) < 120)
#define X_IN_THIRD_QUARTER(x) (120 <= (x) && (x) < 180)
#define X_IN_FOURTH_QUARTER(x) (180 <= (x))

#endif
