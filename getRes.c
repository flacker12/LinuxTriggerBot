#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int width, height;
  printf("Enter your screen width: \n");
  scanf("%d", &width);
  printf("Enter your screen height: \n");
  scanf("%d", &height);

  int centerWidth, centerHeight;
  centerWidth = (width / 2) - 5;
  centerHeight = (height / 2) - 5;
  printf("%d,%d 10x10", centerWidth, centerHeight);
}
