#include "canvas.h"
#include <cassert>
#include "image.h"
#include "math.h"


using namespace std;
using namespace agl;


Canvas::Canvas(int w, int h) : _canvas(w, h)
{
   Image wh(w,h);
   this->_canvas=wh;
}

Canvas::~Canvas()
{

}

void Canvas::save(const std::string& filename)
{
   _canvas.save(filename);
}

void Canvas::begin(PrimitiveType type)
{
   check=0;
   if(type==LINES){
      check=2;
   }
   else if(type==TRIANGLES){
      check=3;
   }
   else if(type==UNDEFINED){
      dada.clear();
   }
   dada.clear();
}

void Canvas::end()
{
   printf("dada size: %d\n", dada.size());
   if(check=2 && dada.size()%2==0){//%2= modulo 2
      for(int i=0;i<dada.size();i+=2){
         line(dada[i],dada[i+1]);
      }
   }
   else if(check=3 && dada.size()%3==0){
      printf("here\n");
      for(int i=0;i<dada.size();i+=3){
         triangle(dada[i],dada[i+1],dada[i+2]);
      }
   }
   else{
      check=0;
      dada.clear();
   }
}

void Canvas::vertex(int x, int y)
{
   buff.x=x;
   buff.y=y;
}

void Canvas::color(unsigned char r, unsigned char g, unsigned char b)
{
   buff.rgb=rgb;
   dada.push_back(buff);//see blobilism
   rgb.r=r;
   rgb.g=g;
   rgb.b=b;
}

void Canvas::background(unsigned char r, unsigned char g, unsigned char b)
{
   bround.r=r;
   bround.g=g;
   bround.b=b;
   _canvas.fill(bround);//fills the canvas with specified color/makes background
}
void Canvas::line(dad p1,dad p2){
   printf("drawing line\n");
   int w=p2.x-p1.x;
   int h=p2.y-p1.y;
   if(p1.y>p2.y){
      int x=p1.x;
      int w=p2.x-p1.x;
      int h=p2.y-p1.y;
      int dx=1;
      if(w<0){
         dx=-1;
         w=-w;
      }
      int f=(2*h)-w;
      for(int y=p1.y;y<=p2.y;y++){
         float cr=sqrt(pow(dada[0].x-x,2)+pow(dada[0].y-y,2))/sqrt(pow(dada[1].x-x,2)+pow(dada[1].y-dada[0].y,2));
         tmp.r=(dada[0].rgb.r*(1-cr))+(dada[1].rgb.r*cr);
         tmp.g=(dada[0].rgb.g*(1-cr))+(dada[1].rgb.g*cr);
         tmp.b=(dada[0].rgb.b*(1-cr))+(dada[1].rgb.b*cr);
         _canvas.set(x,y,tmp);
         if(f>0){
            x+=dx;
            f+=2*(h-w);
         }
         else{
            f+=2*w;
         }
      }
   }
      if(abs(h)<abs(w)){
         if(p1.x>p2.x){
            int y=p1.y;
            int dy=1;
            int w=p2.x-p1.x;
            int h=p2.y-p1.y;
            if(h<0){
               dy=-1;
               h=-h;
            }
            int f=(2*h)-w;
            for(int x=p1.x;x<=p2.x;x++){
               float cr=sqrt(pow(dada[0].x-x,2)+pow(dada[0].y-y,2))/sqrt(pow(dada[1].x-x,2)+pow(dada[1].y-dada[0].y,2));
               tmp.r=(dada[0].rgb.r*(1-cr))+(dada[1].rgb.r*cr);
               tmp.r=(dada[0].rgb.g*(1-cr))+(dada[1].rgb.g*cr);
               tmp.r=(dada[0].rgb.b*(1-cr))+(dada[1].rgb.b*cr);
               _canvas.set(x,y,tmp);
               if(f<0){
                  y+=dy;
                  f+=2*(h-w);
               }
               else{
                  f+=2*h;
               }
            }
         }
      }
   
}

float Canvas::mth(dad v1,dad v2,dad v3){//see week 4/5 slides
   return (((float)((v2.y-v3.y)*v1.x)+(float)((v3.x-v2.x)*v1.y)+(float)(v2.x*v3.y)-(float)(v3.x* v2.y)));
}

void Canvas::triangle(dad p1, dad p2, dad p3){
   printf("drawing triangle\n");
   int mx=max(max(p1.x,p2.x),p3.x);
   int my=max(max(p1.y,p2.y),p3.y);
   int mnx=min(min(p1.x,p2.x),p3.x);
   int mny=min(min(p1.y,p2.y),p3.y);
   for(int y=mny;y<my;y++){
      for(int x=mnx;x<mx;x++){
         buff.x=x;
         buff.y=y;
         buff.rgb=rgb;
         float alpha;
         float beta,gamma;
         alpha=mth(buff,p2,p3)/mth(p1,p2,p3);
         beta=mth(buff,p3,p1)/mth(p2,p3,p1);
         gamma=mth(buff,p1,p2)/mth(p3,p1,p2);
         _canvas.set(y,x,buff.rgb);
      }
   }
}
