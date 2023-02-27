#include <iostream>
#include "canvas.h"
using namespace std;
using namespace agl;

void test(Canvas& drawer,PrimitiveType type, int ax, int ay, int bx, int by, const std::string& savename)
{
   drawer.background(180, 220, 178);
   drawer.begin(type);
   drawer.color(0,0,0);
   drawer.vertex(ax,ay);
   drawer.color(180,220,178);
   drawer.vertex(bx,by);
   drawer.star(70,70,5);
   drawer.mirror(180);
   drawer.end();
   drawer.save(savename);
}

int main(int argc, char** argv)
{
   Canvas drawer(640, 380);
   drawer.color(180,220,178);
   test(drawer,TRIANGLES, 0, 50, 100, 50, "custom_test.png");
   test(drawer,POINT, 0, 50, 100, 50, "custom_test2.png");
   test(drawer,LINES, 0, 50, 100, 50, "custom_test2.png");
   drawer.begin(POINT);
   drawer.color(0,0,0);
   drawer.vertex(20,30);
   drawer.color(180,220,178);
   drawer.vertex(60,60);
   drawer.star(70,70,5);
   drawer.mirror(180);
   drawer.end();
   drawer.save("custom_test3.png");


}

