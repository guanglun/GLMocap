#include "image.h"
#include <stdio.h>
#include <stdlib.h>

const int resolution[][2] = {
	{0, 0},
	// C/SIF Resolutions
	{88, 72},	/* QQCIF     */
	{176, 144}, /* QCIF      */
	{352, 288}, /* CIF       */
	{88, 60},	/* QQSIF     */
	{176, 120}, /* QSIF      */
	{352, 240}, /* SIF       */
	// VGA Resolutions
	{40, 30},	/* QQQQVGA   */
	{80, 60},	/* QQQVGA    */
	{160, 120}, /* QQVGA     */
	{320, 240}, /* QVGA      */
	{640, 480}, /* VGA       */
	{60, 40},	/* HQQQVGA   */
	{120, 80},	/* HQQVGA    */
	{240, 160}, /* HQVGA     */
	// FFT Resolutions
	{64, 32},	/* 64x32     */
	{64, 64},	/* 64x64     */
	{128, 64},	/* 128x64    */
	{128, 128}, /* 128x64    */
	// Other
	{128, 160},	  /* LCD       */
	{128, 160},	  /* QQVGA2    */
	{720, 480},	  /* WVGA      */
	{752, 480},	  /* WVGA2     */
	{800, 600},	  /* SVGA      */
	{1024, 768},  /* XGA       */
	{1280, 1024}, /* SXGA      */
	{1600, 1200}, /* UXGA      */
    {240, 240}, /* LCD      */
};

Image::Image()
{
    width   = IMG_MAX_WIDTH;
    high    = IMG_MAX_HIGH;
    size    = width*high;
    

    
    index = 0;  
}

void Image::setImgSize(uint8_t frame_size_num)
{
    width   = resolution[frame_size_num][0];
    high    = resolution[frame_size_num][1];
    size    = width*high*gs_bpp;
    

    index = 0;    
    
}

bool Image::recv(unsigned char* data,int len)
{
    static int recv_count = 0;
    if(recv_count>= size)
    {
        return false;
    }
    
//    if(gs_bpp == 2)
//    {
//        len/=2;
//        for(int i=0;i<len;i++)
//            temp[i] = data[i*2];
        
//        memcpy(data,temp,len);
//    }
    
    if(index + len >= size)
    {
        memcpy(img+index,data,size - index);
        memcpy(img,data + (size - index),len - (size - index));
        index = len - (size - index);
    }else{
        memcpy(img+index,data,len);
        index+=len;
    }
    
    recv_count += len;
    
    if(recv_count >= size)
    {
        recv_count -= size;
        return true;
    }
    return false;
    
}

unsigned char Image::CONVERT_ADJUST(double tmp)  
{  
    return (unsigned char)((tmp >= 0 && tmp <= 255)?tmp:(tmp < 0 ? 0 : 255));  
}  

//YUV420P to RGB24  
void Image::CONVERT_YUV420PtoRGB24(unsigned char* yuv_src,unsigned char* rgb_dst,int nWidth,int nHeight)  
{  
    unsigned char *tmpbuf = (unsigned char *)malloc(nWidth*nHeight*3);  
    unsigned char Y,U,V,R,G,B;  
    unsigned char* y_planar,*u_planar,*v_planar;  
    int rgb_width , u_width;  
    rgb_width = nWidth * 3;  
    u_width = (nWidth >> 1);  
    int ypSize = nWidth * nHeight;  
    int upSize = (ypSize>>2);  
    int offSet = 0;  
  
    y_planar = yuv_src;  
    u_planar = yuv_src + ypSize;  
    v_planar = u_planar + upSize;  
  
    for(int i = 0; i < nHeight; i++)  
    {  
        for(int j = 0; j < nWidth; j ++)  
        {  
            // Get the Y value from the y planar  
            Y = *(y_planar + nWidth * i + j);  
            // Get the V value from the u planar  
            offSet = (i>>1) * (u_width) + (j>>1);  
            V = *(u_planar + offSet);  
            // Get the U value from the v planar  
            U = *(v_planar + offSet);  
  
            // Cacular the R,G,B values  
            R = CONVERT_ADJUST((Y + (1.4075 * (V - 128))));  
            G = CONVERT_ADJUST((Y - (0.3455 * (U - 128) - 0.7169 * (V - 128))));  
            B = CONVERT_ADJUST((Y + (1.7790 * (U - 128))));  
            
            offSet = rgb_width * i + j * 3;  
  
            rgb_dst[offSet] = B;  
            rgb_dst[offSet + 1] = G;  
            rgb_dst[offSet + 2] = R;  
        }  
    }  
    free(tmpbuf);  
}
