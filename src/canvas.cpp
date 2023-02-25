#include "canvas.h"
#include <cassert>
#include "image.h"
#include "math.h"


using namespace std;
using namespace agl;


Canvas::Canvas(int w,int h) : _canvas(w,h)
{
   wt=w;
   ht=h;
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
   else if(type==POINT){
      check=4;
   }
   dada.clear();
}

void Canvas::end()
{
   if(check==2 && dada.size()%2==0){//%2= modulo 2
      for(int i=0;i<dada.size();i+=2){
         line(dada[i],dada[i+1]);
         printf("dada size:%d\n",dada.size());
      }
   }
   else if(check==3 && dada.size()%3==0){
      for(int i=0;i<dada.size();i+=3){
         triangle(dada[i],dada[i+1],dada[i+2]);
         printf("dada size:%d\n",dada.size());
      }
   }
   else if(check==4&&dada.size()%1==0){
      for(int i=0;i<dada.size();i+=1){
         point(dada[i]);
      }
   }
   else{
      check=0;
      dada.clear();
   }
}

void Canvas::vertex(int x,int y)
{
   buff.x=x;
   buff.y=y;
   dada.push_back(buff);
}

void Canvas::mirror(int ang){//to mirror set ang to 180
   for(int i=0;i<=(wt*ht);i++){
         dad p=dada[i];
         float s=sin(ang),c=cos(ang);
         float x=(p.x*c)-(p.y*s);
         float y=(p.x*s)+(p.y*c);
         p.x=x+p.x;
         p.y=y+p.y;
         dada.push_back(p);
   }

}

void Canvas::color(unsigned char r,unsigned char g,unsigned char b)
{
   buff.rgb.r=r;
   buff.rgb.g=g;
   buff.rgb.b=b;  
   dada.push_back(buff);//see blobilism

}

void Canvas::background(unsigned char r,unsigned char g,unsigned char b)
{
   bround.r=r;
   bround.g=g;
   bround.b=b;
   _canvas.fill(bround);//fills the canvas with specified color/makes background
}
void Canvas::point(dad p1){
   buff.x=p1.x;
   buff.y=p1.y;
   p1.rgb=buff.rgb;
   _canvas.set(p1.x,p1.y,p1.rgb);
}
void Canvas::scaleline(float s, dad p){
   p.x=p.x*s;
   p.y=p.y*s;
}
void Canvas::line(dad p1,dad p2){
   printf("drawing line\n");
   scaleline(1,p2);
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
         float cr=sqrt(pow(dada[0].x-x,2)+pow(dada[0].y-y,2))/sqrt(pow(dada[1].x-dada[0].x,2)+pow(dada[1].y-dada[0].y,2));
         buff.rgb.r=(dada[0].rgb.r*(1-cr))+(dada[1].rgb.r*cr);
         buff.rgb.g=(dada[0].rgb.g*(1-cr))+(dada[1].rgb.g*cr);
         buff.rgb.b=(dada[0].rgb.b*(1-cr))+(dada[1].rgb.b*cr);
         _canvas.set(x,y,buff.rgb);
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
         if(p1.x<p2.x){
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
               float t=sqrt(pow(dada[0].x-x,2)+pow(dada[0].y-y,2))/sqrt(pow(dada[1].x-dada[0].x,2)+pow(dada[1].y-dada[0].y,2));
               buff.rgb.r=(dada[0].rgb.r*(1-t))+(dada[1].rgb.r*t);
               buff.rgb.g=(dada[0].rgb.g*(1-t))+(dada[1].rgb.g*t);
               buff.rgb.b=(dada[0].rgb.b*(1-t))+(dada[1].rgb.b*t);
               _canvas.set(x,y,buff.rgb);
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
      dada.clear();
}
float Canvas::mth(dad v1,dad v2,dad v3){//see week 4/5 slides
   return (((float)((v2.y-v3.y)*v1.x)+(float)((v3.x-v2.x)*v1.y)+(float)(v2.x*v3.y)-(float)(v3.x* v2.y)));
}
void Canvas::triangle(dad p1,dad p2,dad p3){
   printf("drawing triangle\n");
   int mx=max(max(p1.x,p2.x),p3.x);
   int my=max(max(p1.y,p2.y),p3.y);
   int mnx=min(min(p1.x,p2.x),p3.x);
   int mny=min(min(p1.y,p2.y),p3.y);
   for(int y=mny;y<my;y++){
      for(int x=mnx;x<mx;x++){
         buff.x=x;
         buff.y=y;
         float alpha;
         float beta,gamma;
         alpha=mth(buff,p2,p3)/mth(p1,p2,p3);
         beta=mth(buff,p3,p1)/mth(p2,p3,p1);
         gamma=mth(buff,p1,p2)/mth(p3,p1,p2);
         _canvas.set(y,x,buff.rgb);
      }
   }
   dada.clear();
}
void Canvas::star(int x,int y,int r){
   begin(TRIANGLES);
   vertex(x-(r/2),y);
   vertex(x+(r/2),y);
   vertex(x,y+r);
   vertex(x,y-(r/2));
   vertex(x,y+(r/2));
   vertex(x+r,y);
   vertex(x-(r/2),y);
   vertex(x+(r/2),y);
   vertex(x,y-r);
   vertex(x,y-(r/2));
   vertex(x,y+(r/2));
   vertex(x-r,y);
   end();
}