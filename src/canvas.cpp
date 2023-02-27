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
{  _canvas.fill(bround);
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
   _canvas.fill(bround);
   buff.x=p1.x;
   buff.y=p1.y;
   p1.rgb=buff.rgb;
   _canvas.set(p1.x,p1.y,p1.rgb);
}
void Canvas::scaleline(float s,dad p){
   p.x=p.x*s;
   p.y=p.y*s;
}

float Canvas::mth(dad v1,dad v2,dad v3){//see week 4/5 slides
   return (((float)((v2.y-v3.y)*v1.x)+(float)((v3.x-v2.x)*v1.y)+(float)(v2.x*v3.y)-(float)(v3.x* v2.y)));
}
void Canvas::triangle(dad p1,dad p2,dad p3){
   int xm=max(max(p1.x,p2.x),p3.x);
   int xmn=min(min(p1.x,p2.x),p3.x);
   int ym=max(max(p1.y,p2.y),p3.y);
   int ymn=min(min(p1.y,p2.y),p3.y);
   for(int y=ymn;y<ym;y++){
      for(int x=xmn;x<xm;x++){
         buff.x=x;
         buff.y=y;
         buff.rgb=current;
         float alpha= mth(buff,p2,p3)/mth(p1,p2,p3);
         float beta=mth(buff,p3,p1)/mth(p2,p3,p1);
         float gamma=mth(buff,p1,p2)/mth(p3,p1,p2);

         if(alpha>0 && beta>0 &&gamma>0){
            Pixel tmp; 
            tmp.r=p1.rgb.r*alpha+p2.rgb.r*beta+p3.rgb.r*gamma;
            tmp.g=p1.rgb.g*alpha+p2.rgb.g*beta+p3.rgb.g*gamma;
            tmp.b=p1.rgb.b*alpha+p2.rgb.b*beta+p3.rgb.b*gamma;
            _canvas.set(y,x,tmp);
         }
      }
   }
}
void Canvas::star(int x,int y,int r){
   _canvas.fill(bround);
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

void Canvas::pslope(dad a,dad b){
   int x=a.x,w=b.x -a.x,h=b.y -a.y,dx=1;
   if(w<0){
      dx=-1;
      w=-1*w;
   }
   int f=2*w-h;
   for(int y=a.y;y<=b.y; y++){
      float t=sqrt(pow(dada[0].x-x,2)+pow(dada[0].y-y,2))/sqrt(pow(dada[1].x-x,2)+pow(dada[1].y-dada[0].y,2));
      Pixel tmp; 
      tmp.r=dada[0].rgb.r*(1-t)+dada[1].rgb.r*t;
      tmp.g=dada[0].rgb.g*(1-t)+dada[1].rgb.g*t;
      tmp.b=dada[0].rgb.b*(1-t)+dada[1].rgb.b*t;
      _canvas.set(y,x,tmp);
      if(f>0){
         x+=dx;
         f+=2*(w-h);
      }
      else{
         f+=2*w;
      }
   }
}

void Canvas::nslope(dad p1,dad p2){
   int y=p1.y,w=p2.x-p1.x,h=p2.y-p1.y,dy=1;
   if(h<0){
      dy=-1;
      h=-1*h;
   }
   int f=2*h-w;
   for(int x=p1.x;x<=p2.x;x++){
      float t=sqrt(pow(dada[0].x-x,2)+pow(dada[0].y-y,2))/sqrt(pow(dada[1].x-x,2)+pow(dada[1].y-dada[0].y,2));
      Pixel tmp; 
      tmp.r=dada[0].rgb.r*(1-t)+dada[1].rgb.r*t;
      tmp.g=dada[0].rgb.g*(1-t)+dada[1].rgb.g*t;
      tmp.b=dada[0].rgb.b*(1-t)+dada[1].rgb.b*t;
      _canvas.set(y,x,tmp);
      if(f>0){
         y+=dy;
         f+=2*(h-w);
      }
      else{
         f+=2*h;
      }
   }
}
void Canvas::rectangle(int x,int y,int w,int h) {
    int x1=x-w/2,x2=x+w/2,y1=y-h/2,y2=y+h/2;
    begin(LINES);
    vertex(x1,y1);
    vertex(x2,y1);
    end();
    begin(LINES);
    vertex(x2,y1);
    vertex(x2,y2);
    end();
    begin(LINES);
    vertex(x2,y2);
    vertex(x1,y2);
    end();
    begin(LINES);
    vertex(x1,y2);
    vertex(x1,x1);
    end();
}
   
void Canvas::line(dad p1,dad p2){
   _canvas.fill(bround);
   int w =p2.x-p1.x;
   int h=p2.y-p1.y;
   if(abs(h)<abs(w)){
      if(p1.x>p2.x){
         nslope(p2,p1);
      }
      else{
         nslope(p1,p2);
      }
   }
   else{
      if(p1.y>p2.y){
         pslope(p2,p1);
      }
      else{
         pslope(p1,p2);
      }
   }
}
