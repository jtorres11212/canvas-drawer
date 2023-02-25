#include <iostream>
#include "canvas.h"
using namespace std;
using namespace agl;

void test(Canvas& drawer, int ax, int ay, int bx, int by, const std::string& savename)
{
   drawer.background(0, 0, 0);
   drawer.begin(POINT);
   drawer.color(180,220,178);
   drawer.vertex(ax,ay);
   drawer.vertex(bx,by);
   drawer.star(70,70,5);
   drawer.mirror(180);
   drawer.end();
   drawer.save(savename);
}

int main(int argc, char** argv)
{
   Canvas drawer(640, 380);
   //drawer.color(180,220,178);
   test(drawer, 0, 50, 100, 50, "custom_test.png");
}

