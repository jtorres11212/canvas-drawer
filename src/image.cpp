

#include "image.h"
#include "string.h"

#include <cassert>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace agl {


Image::Image() {
   wt=0;
   ht=0;
   chan=3;
   dad=NULL;
}

Image::Image(int width,int height)  {
   wt=width;
   ht=height;
   chan=3;

   dad=new Pixel[width * height]; 
}

Image::Image(const Image& orig) {
   wt=orig.wt;
   ht=orig.ht;
   chan=orig.chan;
   
   dad=new Pixel[wt * ht];
   memcpy(dad,orig.dad,sizeof(Pixel)* wt * ht);
  
}

Image& Image::operator=(const Image& orig) {
  if (&orig == this) {
    return *this;
  }
  if(dad != NULL){
     delete[] dad;
     dad=NULL;
  }
  wt=orig.wt;
  ht=orig.ht;
  dad=new Pixel[wt * ht];
  memcpy(dad,orig.dad,sizeof(Pixel)* wt * ht);
  //wt=nw;
  //ht=nh;
  //dad=ndata;
  return *this;
}

Image::~Image() {
   delete[] dad;
}

int Image::width() const { 
   return wt;
}

int Image::height() const {
   return ht;
}

char* Image::data() const {
   return (char*)dad;
}

void Image::set(int width,int height,unsigned char* data) {
   wt=width;
   ht=height;
   if(dad != NULL){
     delete[] dad;
     dad=NULL;
  }
  dad=new Pixel[wt * ht];
  memcpy(dad,(Pixel*) data,sizeof(Pixel)* wt * ht);
}

bool Image::load(const std::string& filename,bool flip) {
   if (dad != NULL) {
      delete[] dad;
      dad=NULL;
   }

  Pixel* temp=(Pixel*) stbi_load(filename.c_str(),&wt,&ht,&chan,3);

  if(temp != NULL){
     dad=new Pixel[wt * ht];
     memcpy(dad,temp,sizeof(Pixel)* wt * ht);
     stbi_image_free(temp);
     return true;
  }
  else{
     return false;
  }
  
}


bool Image::save(const std::string& filename,bool flip) const {
   if(dad == NULL){
      return false;
   }
   else{
      stbi_write_png(filename.c_str(),wt,ht,chan,dad,wt*3);
      return true;
   }
  
}

Pixel Image::get(int row,int col) const {
   Pixel temp=dad[row*wt+col];
   return temp;
}

void Image::set(int row,int col,const Pixel& color) {
   dad[row*wt+col]=color;
}


Pixel Image::get(int i) const{
   Pixel tmp=dad[i];
   return tmp;
}

void Image::set(int i,const Pixel& c){
   dad[i]=c;
}

Image Image::resize(int w,int h) const {
   Image result(w,h);
   float pr,prpc;
   int prev,pc;
   for(int i=0;i<h;i++){
      for(int j=0;j<w;j++){
         pr=(float) i / (h-1);
         prev=floor(pr * (ht -1));
         prpc=(float) j / (w -1);
         pc=floor(prpc * (wt -1));
         result.set(i,j,get(prev,pc));
      }
   }
   return result;
}

Image Image::flipHorizontal() const {
   Image result(wt,ht);
   int nh=0;
   int ph=ht;
   int ccol=0;
   while (nh<ht){
      while(ccol<wt){
         result.set(nh,ccol,get(ph,ccol));
         ccol=ccol++;
      }
      nh=nh++;
      ph=ph--;
      ccol=0;
   }
   return result;

}



Image Image::flipVertical() const {
   Image result(wt,ht);
   for(int i=0;i<ht;i++){
      for(int j=0;j<wt;j++){
         result.set(i,wt-1 -j,get(i,j));
      }
   }
   return result;
}

Image Image::crank90() const {
   Image result(ht,wt);
   for(int i=0;i<ht;i++){
      for(int j=0;j<wt;j++){
         result.set(wt -1 -j,i,get(i,j));
      }
   }
   return result;
}

Image Image::blur() const{
   Image result(wt,ht);
   for(int i=0;i<wt;i++){
      for(int j=0;j<ht;j++){
         Pixel cen;
         int ar=0;
         int ag=0;
         int ab=0;
         for(int k=i-1;k<=i+1;k++){
            for(int l=j-1;l<=j+1;l++){
               Pixel sec=get(k,l);
               ar=ar+sec.r;
               ag=ag+sec.g;
               ab=ab+sec.b;
            }
         }
         cen.r=ar/9;
         cen.g=ag/9;
         cen.b=ab/9;
         result.set(i,j,cen);
      }
   }
   return result;
}

Image Image::extract() const{
   Image result(wt,ht);
   for(int i=0;i<wt;i++){
      for(int j=0;j<ht;j++){
         Pixel tmp=get(i,j);
         tmp.r=0;
         tmp.g=0;
         result.set(i,j,tmp);
      }
   }
   return result;
}

Image Image::border() const{
   Image result(wt+20,ht+20);
   for(int i=0;i<wt+20;i++){
      for(int j=0;j<ht+20;j++){
         Pixel temp={255,255,255};
         result.set(i,j,temp);
      } 
   }
   result.replace(*this,10,10);

   return result;
}


Image Image::subimage(int startx,int starty,int w,int h) const {
   Image sub(w,h);
   for(int i=starty;i<starty+h;i++){
      for(int j=startx;j<startx+w;j++){
         sub.set(i -starty,j -startx,get(i,j));
      }
   }
    return sub;
}

void Image::replace(const Image& image,int startx,int starty) {
   for(int i=starty;i<(starty+image.height());i++){
      for(int j=startx;j<(startx+image.width());j++){
         set(i,j,image.get(i -starty,j -startx));
      }
   }
}

Image Image::swirl() const {
   Image result(wt,ht);
   for(int i=0;i<wt;i++){
      for(int j=0;j<ht;j++){
         Pixel temp=get(i,j);
         Pixel sw;
         sw.r=temp.g;
         sw.g=temp.b;
         sw.b=temp.r;
         result.set(i,j,sw);
      }
   }
   
   return result;
}

Image Image::add(const Image& other) const {
   Image result(0,0);
  
   return result;
}

Image Image::subtract(const Image& other) const {
   Image result(0,0);
   
   return result;
}

Image Image::multiply(const Image& other) const {
   Image result(0,0);
   
   return result;
}

Image Image::difference(const Image& other) const {
   Image result(0,0);
  
   return result;
}

Image Image::lightest(const Image& other) const {
   Image result(0,0);
  
   return result;
}

Image Image::darkest(const Image& other) const {
   Image result(0,0);
  
   return result;
}

Image Image::gammaCorrect(float gamma) const {
   Image result(wt,ht);
   float red,green,blue;
   for(int i=0;i<ht;i++){
      for(int j=0;j<wt;j++){
         struct Pixel tmp= get(i,j);
            red=pow(((float)tmp.r / 255),(1/gamma));
            tmp.r=red * 255;
            green=pow(((float)tmp.g / 255),(1/gamma));
            tmp.g=green * 255;
            blue=pow(((float)tmp.b / 255),(1/gamma));
            tmp.b=blue * 255;
            result.set(i,j,tmp);
      }
   }
   return result;
}

Image Image::alphaBlend(const Image& other,float alpha) const {
   Image result(wt,ht);
   for(int i=0;i<ht;i++){
      for(int j=0;j<wt;j++){
         Pixel thisIm=get(i,j);
         Pixel otherIm=other.get(i,j);
         Pixel temp;
         temp.r=(thisIm.r * alpha)+(otherIm.r * (1 -alpha));
         temp.g=(thisIm.g * alpha)+(otherIm.g * (1 -alpha));
         temp.b=(thisIm.b * alpha)+(otherIm.b * (1 -alpha));
         result.set(i,j,temp);
      }
   }
   return result;
}

Image Image::invert() const {
   Image image(wt,ht);
   for(int i=0;i<ht;i++){
      for(int j=0;j<ht;j++){
         Pixel inv=get(i,j);
         inv.r=255 -inv.r;
         inv.g=255 -inv.g;
         inv.b=255 -inv.b;
         image.set(i,j,inv);
      }
   }
   return image;
}

Image Image::grayscale() const {
   Image result(wt,ht);
   for(int i=0;i<ht;i++){
      for(int j=0;j<wt;j++){
         Pixel temp=get(i,j);
         unsigned char inten=(temp.r * 0.3)+(temp.g * 0.59)+(temp.b * 0.11);
         Pixel correct={inten,inten,inten};
         result.set(i,j,correct);
      }
   }
   return result;
}

Image Image::colorJitter(int size) const {
   Image image(wt,wt);
   for(int i=0;i<wt;i++){
      for(int j=0;j<ht;j++){
         Pixel temp=get(i,j);
         temp.r=(temp.r -size)+(rand() % size *2);
         temp.g=(temp.g -size)+(rand() % size *2);
         temp.b=(temp.b -size)+(rand() % size *2);
         image.set(i,j,temp);
      }
   }
   return image;
}

Image Image::bitmap(int size) const {
   Image image(0,0);
   
   return image;
}

void Image::fill(const Pixel& c) {
  }

}  // namespace agl