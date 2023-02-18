// Copyright 2021, Aline Normoyle, alinen

#include "image.h"
#include <cassert>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace agl {
//cnstrctr
Image::Image() {
}
//cnstrctr2
Image::Image(int width, int height)  {
   dad=new Pixel[width*height];
   chan=3;
   wt=width;
   ht=height;
   
}
//cpy cnstrctr(to make temps)
Image::Image(const Image& orig) {
   dad=orig.dad;//theres a joke to be made here
   chan=orig.chan;
   wt=orig.wt;
   ht=orig.ht;
}
Image& Image::operator=(const Image& orig) {
  if (&orig == this) {
    return *this;
  }

  return *this;
}

Image::~Image() {
}

int Image::width() const { 
   return wt;
}

int Image::height() const {
   return ht;
}

char* Image::data() const {
   return (char*)dad;//orphans be like
   
}

void Image::set(int width, int height, unsigned char* data) {
}

bool Image::load(const std::string& filename, bool flip) {
   int x,y;
   dad=(Pixel*)(stbi_load(filename.c_str(),&x,&y,&chan,3));
   if (dad!=NULL){
      wt=x;
      ht=y;
      return true;
   }
   return false;
}

bool Image::save(const std::string& filename, bool flip) const {  
   stbi_flip_vertically_on_write(flip);
   if(dad!= NULL){
      Pixel*img=(Pixel*)(stbi_write_png(filename.c_str(),wt,ht,chan,dad,sizeof(struct Pixel)*wt));
      return false;
   }
   return true;
}

Pixel Image::get(const int row,const int col) const{
  Pixel mom;
  mom=dad[row*wt+col];
  return mom;
}
void Image::set(int row, int col, const Pixel& color) {

}
Pixel Image::get(int i) const
{
   Pixel next=dad[i];
   return next;
}
void Image::set(int i, const Pixel& c)
{
   dad[i]=c;
}
Image Image::resize(int w, int h) const {
   Image result(w, h);
   float ww,hh;
   int first;
   for (int i=0;i<w;i++) {
      for (int j=0;j<h;j++) {
         ww=(((float)ht-1)*(float)j)/(float)(h-1);
         hh=(((float)wt-1)*(float)i)/(float)(w-1);//gets img cords
         first=floor(ww)*wt+floor(hh);//does the resizing by rounding down
         Pixel set=get(first);
         result.set((j*w),set);
      }
   }
   return result;
}
Image Image::flipHorizontal() const {
   /**(unsigned char**)t;                 //wish this worked
   for(int i=1;i<sizeof(dad);i++){
      for(int i=0;i<ht;i++){
         for(int j=0;j<ht;j++){
            if (i!=0){
               t[i][j]=dad[(i*(wt))+1];
            }
         }
      }
         for(int i=0;i<1;i++){
            for (int j=0;j<ht;j++){
               t[i][j]=dad[i];
            }
         }**/
   int www;
   Image result(wt,ht);
   for(int i=0;i<wt;i++){
      for(int j=0;j<ht;j++){
         www=wt-(j+1);
         Pixel set=get(j*wt+i);
         result.set((i*wt+www),set);
      }
   }
   return result;
}
Image Image::flipVertical() const {
   int hhhh;
   Image result(wt,ht);
   for(int i=0;i<wt;i++){
      for(int j=0;j<ht;j++){
         hhhh=wt-(j+1);
         Pixel set=get(j*wt+i);
         result.set((i*wt+hhhh),set);
      }
   }   return result;
}
Image Image::subimage(int startx, int starty, int w, int h) const {
   Image subImage(w, h);
   for(int i=starty;i<starty+h;i++){
      for(int j=startx;j<startx+w;j++){
         Pixel set=get(j*wt+i);
         subImage.set(((j-startx)*w+(i-starty)),set);
      }
   }
    return subImage;
}
void Image::replace(const Image& image, int startx, int starty) {
   for(int i=starty;i<starty+image.width();i++){
      for(int j=startx;j<startx+image.height();j++){
         Pixel next=image.get((j-startx)*image.width()+(i-starty));
         set((j*wt+i),next);
      }
   }
}
Image Image::swirl() const {
   Image result(0, 0);
   return result;
}
Image Image::add(const Image& other) const {
   Image result(0, 0);
  
   return result;
}
Image Image::subtract(const Image& other) const {
   Image result(0, 0);
   
   return result;
}
Image Image::multiply(const Image& other) const {
   Image result(wt,ht);
   for (int i = 0; i < ht; i++){
      for (int j = 0; j < wt; j++){
         Pixel tmp = get(i,j);
         Pixel other_pixels = other.get(i, j);
         tmp.r=tmp.r * other_pixels.r;
         tmp.g=tmp.g * other_pixels.g;
         tmp.b=tmp.b * other_pixels.b;
         result.set(i,j,tmp);;
      }
   }
   return result;
}
Image Image::difference(const Image& other) const {
   Image result(wt,ht);
      for (int i=0;i<ht;i++){
         for (int j=0;j<wt;j++){
            Pixel tmp=get(i,j);
            Pixel nottmp=other.get(i,j);
            tmp.r=tmp.r-nottmp.r;
            tmp.g=tmp.g-nottmp.g;
            tmp.b=tmp.b-nottmp.b;
            result.set(i,j,tmp);;
         }
      }
   return result;
}
Image Image::lightest(const Image& other) const {
   Image result(wt, ht);
   for (int i=0;i<ht;i++){
      for (int j=0;j<wt;j++){
         Pixel tmp=get(i,j);
         Pixel otherother=other.get(i,j);
         int t1=tmp.r+tmp.g+tmp.b;
         int t2=otherother.r+otherother.g+otherother.b;
         if (t1<t2){
            tmp.r=otherother.r;
            tmp.g=otherother.g;
            tmp.b=otherother.b;
         }
      result.set(i,j,tmp);;
      }
   }
return result;
}

Image Image::darkest(const Image& other) const {
   Image result(wt, ht);
   for (int i=0;i<ht;i++){
      for (int j=0;j<wt;j++){
         Pixel tmp=get(i,j);
         Pixel otherother=other.get(i,j);
         int t1=tmp.r+tmp.g+tmp.b;
         int t2=otherother.r+otherother.g+otherother.b;
         if (t1>t2){
            tmp.r=otherother.r;
            tmp.g=otherother.g;
            tmp.b=otherother.b;
         }
      result.set(i,j,tmp);;
      }
   }
return result;
}
Image Image::gammaCorrect(float gamma) const {
   Image result(wt, ht);
   for(int i=0;i<wt;i++){
      for(int j=0;j<ht;j++){
         Pixel set=get(j*wt+i);
         float r,g,b;
         r=pow(((float)set.r/255),1/gamma);
         g=pow(((float)set.g/255),1/gamma);
         b=pow(((float)set.b/255),1/gamma);
         set.r=r*255;
         set.g=g*255;
         set.b=b*255;
         result.set((j*wt+i),set);
      }
   }
   return result;
}
Image Image::alphaBlend(const Image& other, float alpha) const {
   Image result(wt,ht);
   for(int i=0;i<wt;i++){
      for(int j=0;j<ht;j++){
         Pixel img1=get(j*wt+i);
         Pixel img2=other.get(j*other.wt+i);
         Pixel set;
         set.r=(img2.r*(1-alpha))+(img1.r*alpha);
         set.g=(img2.g*(1-alpha))+(img1.g*alpha);
         set.b=(img2.b*(1-alpha))+(img1.b*alpha);
         result.set((j*wt+i),set);
      }
   }
   return result;
}
Image Image::invert() const {
   Image image(0, 0);
   
   return image;
}
Image Image::crank90()const{
   Image result(wt,ht);
   for(int i=0;i<wt;i++){
      for(int j=0;j<ht;j++){
         Pixel getter=get(wt,ht);
         Pixel flip;
         flip.r=255-getter.r;
         flip.g=255-getter.g;
         flip.b=255-getter.b;
         result.set((j*wt+i),flip);
      }
   }
   return result;
}
Image Image::glow()const{//doesent work :(
   Image result(wt,ht);
   for(int i=1;i<wt-1;i++){
      for(int j=1;j<ht-1;j++){
         Pixel getter=get(wt,ht);
         int avr=0;
         int avb= 0;
         int avg=0;
         for(int w=i-1;w<=i+1;w++){
            for(int h=j-1;h<=j+1;h++){
               Pixel next=get(h,w);
               while(getter.r>150&&getter.b>150&&getter.g>150){
                  avr+=next.r;
                  avg+=next.g;
                  avb+=next.b;
               }
            }
         }
         getter.r=avr/9;
         getter.g=(avg/9);
         getter.b=avb/9;
         result.set((j*wt+i),getter);
      }
   }
   return result;
}
Image Image::brr()const{
   Image result(wt,ht);
   for(int i=1;i<wt-1;i++){
      for(int j=1;j<ht-1;j++){
         Pixel getter=get(wt,ht);
         int avr=0;
         int avb=0;
         int avg=0;
         for(int w=i-1;w<=i+1;w++){
            for(int h=j-1;h<=j+1;h++){
               Pixel next=get(h,w);
               avr+=next.r;
               avg+=next.g;
               avb+=next.b;
            }
         }
         getter.r=avr/9;
         getter.g=(avg/9);
         getter.b=avb/9;
         result.set((j*wt+i),getter);
      }
   }
   return result;
}
Image Image::glitch()const{
   Image result(wt,ht);
   for(int i=1;i<wt-1;i++){
      for(int j=1;j<ht-1;j++){
         Pixel getter=get(wt,ht);
         int avr,avb,avg=0;
         for(int w=i-1;w<=i+2;w++){
            for(int h=j-1;h<=j+2;h++){
               Pixel next=get(h,w);
               avr+=next.r;
               avg+=next.g;
               avb+=next.b;
            }
         }
         getter.r=avr/9;
         getter.g=avg/9;
         getter.b=avb/9;
         result.set((j*wt+i),getter);
      }
   }
   return result;
}
Image Image::deepfry(const Image& other)const{//doesent work either :(
   Image result(wt,ht);
   for(int i=0;i<wt;i++){
      for(int j=0;j<ht;j++){
      Pixel fry=get(j*wt+i);
      fry.r=fry.r+((255-fry.g)/2);
      result.set((j*wt+i),fry);
      }
   }
   return result;
}
Image Image::grayscale() const {// divide all rgb by 3
   int av;
   Image result(wt,ht);
   for(int i=0;i<wt;i++){
      for(int j=0;j<ht;j++){
         Pixel set=get(j*wt+i);
         av=(set.r+set.g+set.b)/3;
         set.r=av;
         set.g=av;
         set.b=av;
         result.set((j*wt+i),set);
      }
   }
   return result;
}
Image Image::colorJitter(int size) const {
   Image image(0, 0);
  
   return image;
}
Image Image::bitmap(int size) const {
   Image image(0, 0);
   
   return image;
}
void Image::fill(const Pixel& c) {

  }

}
