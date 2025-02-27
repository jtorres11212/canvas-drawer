/*-----------------------------------------------
 * Author:
 * Date:
 * Description:
 ----------------------------------------------*/

#ifndef canvas_H_
#define canvas_H_

#include <string>
#include <vector>
#include "image.h"

namespace agl
{
   enum PrimitiveType {UNDEFINED, LINES, TRIANGLES,POINT};
   struct dad{
      Pixel rgb;
      int x,y;
      //unsigned char r;
      //unsigned char g;
      //unsigned char b;
   };
   class Canvas
   {
   public:
   
      Canvas(int w, int h);
      virtual ~Canvas();

      // Save to file
      void save(const std::string& filename);

      // Draw primitives with a given type (either LINES or TRIANGLES)
      // For example, the following draws a red line followed by a green line
      // begin(LINES);
      //    color(255,0,0);
      //    vertex(0,0);
      //    vertex(100,0);
      //    color(0,255,0);
      //    vertex(0, 0);
      //    vertex(0,100);
      // end();
      void begin(PrimitiveType type);
      void end();

      // Specifiy a vertex at raster position (x,y)
      // x corresponds to the column; y to the row
      void vertex(int x, int y);

      void scaleline(float s,dad p);//sets line width
      // Specify a color. Color components are in range [0,255]
      void color(unsigned char r, unsigned char g, unsigned char b);

      void line(dad p1,dad p2);

      void point(dad p1);

      void star(int x,int y,int r);

      void triangle(dad p1,dad p2,dad p3);

      float mth(dad v2,dad v1,dad v3);

      void mirror(int ang);//rotates 

      void nslope(dad p1,dad p2);

      void pslope(dad p1,dad p2);
      
      void rectangle(int x,int y,int w,int h);
      // Fill the canvas with the given background color
      void background(unsigned char r, unsigned char g, unsigned char b);

   private:
      Image _canvas;
      int check;
      std::vector<dad> dada;
      dad buff;//buffer variable
      Pixel bround,current;
      int wt,ht;
   };
}

#endif
