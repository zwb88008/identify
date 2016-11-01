    #include "cv.h"  
    #include <iostream>  
    #include <cxcore.h>  
    #include <highgui.h>  
    #include <math.h>   
    #include <vector>  
    #include <algorithm>  
    #include <leptonica/allheaders.h>  
    #include <tesseract/baseapi.h>  
    #include <tesseract/renderer.h>  
      
    #pragma comment(lib,"opencv_core2410d.lib")      
    #pragma comment(lib,"opencv_highgui2410d.lib")      
    #pragma comment(lib,"opencv_imgproc2410d.lib")   
      
    
    using namespace std;   
      
    typedef struct  
    {  
        CvPoint cP;  
        int height;  
        int width;  
      
    } RecP;  
    //自定义排序函数  
    namespace my  
    {  
    bool less(const RecP& s1, const RecP& s2)  
    {  
        //if(s1.cP.x < s2.cP.x && s1.cP.y < s2.cP.y)  
        return s1.cP.x < s2.cP.x;          //依次增大  
      
    }   
    }  
      
    void PrintVector( vector<RecP> & vec)    
    {      
        for(vector<RecP>::iterator n = vec.begin() ; n != vec.end() ; n++ )    
        {    
            cout<< n->cP.x <<'\t'<< n->cP.y <<'\t'<< n->height<<'\t'<< n->width <<endl;    
        }    
    }    
      
    IplImage* src; 
    IplImage* test;     
    IplImage* img;   
    IplImage* dst;   
    IplImage* bianyuan;
    IplImage*  cut;
    IplImage*  cuted;
    CvMemStorage* storage=NULL; 
    CvRect maxRect; 
      
    int thresh=50;  
      
    void on_trackbar(int pos)  
    {           
        CvSeq* contour=0;  
        if(storage==NULL)  
        {  
            dst=cvCreateImage(cvGetSize(bianyuan), 8, 3);   
            storage=cvCreateMemStorage(0);   
        }  
        else   
        {  
            cvClearMemStorage(storage);   
        }   
        cvSmooth(bianyuan, bianyuan, CV_GAUSSIAN, 3, 3, 0, 0);  //邻阈滤波
        cvThreshold( bianyuan, img, thresh, 200, CV_THRESH_BINARY);  
      
        cvNamedWindow( "threshold", 1);  
        cvShowImage( "threshold", img );  
      
        cvFindContours(img, storage, &contour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, cvPoint(0,0));  //查找轮廓  
        cvZero( dst );     //将数组中所有通道的所有元素的值都设置为0  
       
        vector<RecP> vecP;  
      
        int n=0;

        CvRect maxRect;
        int max = 0;
        for( ; contour; contour = contour->h_next )                                                             
        {  
            CvRect rect=cvBoundingRect(contour,1);  
           
            if (max < (rect.width + rect.height))
            {
		max = rect.width + rect.height;
                maxRect.x = rect.x;
		maxRect.y = rect.y;
		maxRect.width = rect.width;
		maxRect.height = rect.height;
	    }
	/*cout<<rect.x<<" "<<rect.y<<" "<<rect.width<<" "<<rect.height<<endl;*/
           
        }   
	cout<<maxRect.x<<" "<<maxRect.y<<" "<<maxRect.width<<" "<<maxRect.height<<endl;

	CvPoint pt1=cvPoint(maxRect.x, maxRect.y), pt2=cvPoint(maxRect.x+maxRect.width, maxRect.y+maxRect.height);   //定义矩形对顶点  
	/*
        const char* a = "abcd.jpg";  
        test = cvLoadImage(a, 0);  
	src=cvCreateImage(cvSize(800,500),IPL_DEPTH_8U,1);  
        cvResize(test,src,CV_INTER_LINEAR);  
        cvSmooth(src,src,CV_GAUSSIAN,5,5,0,0);
  	cvRectangle(src, pt1, pt2, CV_RGB(255,0,0), 1, CV_AA, 0);      //绘制矩形边框  
	cvLine(src, pt1, pt2, CV_RGB(0,255,0), 1, CV_AA, 0);  
	cvNamedWindow( "test",1);                                                                                 
        cvShowImage( "test", src);  
        */  
	cvRectangle(dst, pt1, pt2, CV_RGB(255,0,0), 1, CV_AA, 0);      //绘制矩形边框  
	cvLine(dst, pt1, pt2, CV_RGB(0,255,0), 1, CV_AA, 0);           //矩形对角线相连  

	pt1=cvPoint(maxRect.x, maxRect.y+maxRect.height),  
	pt2=cvPoint(maxRect.x+maxRect.width, maxRect.y);  

	cvLine(dst, pt1, pt2, CV_RGB(0,255,0), 1, CV_AA,0);            //矩形对角线相连  

	RecP tmp;  
	CvPoint p1;  
	p1 = cvPoint(maxRect.x + maxRect.width/2, maxRect.y + maxRect.height/2);   //矩形中心坐标  

	
      
        cvShowImage( "Components", dst );   
	cvSetImageROI(src , cvRect(maxRect.x,maxRect.y,maxRect.width,maxRect.height));
        cut = cvCreateImage(cvSize(maxRect.width,maxRect.height),IPL_DEPTH_8U,1);
	cvCopy(src,cut,0);
	cvNamedWindow( "test1",1);                                                                                 
        cvShowImage( "test1", cut);  
	cvSaveImage("/home/zwb/test.jpg",cut);
        cuted = cvLoadImage("/home/zwb/test.jpg", 0);    
	cvSetImageROI(cuted , cvRect(0,maxRect.height*4/5.4,maxRect.width*2.6/8.4,maxRect.height*1.4/5.4));
        //cvCreateImage(cvSize(maxRect.width,maxRect.height),IPL_DEPTH_8U,1);
        cvSaveImage("/home/zwb/test2.jpg",cuted);
	cvNamedWindow( "test2",1);                                                                                 
        cvShowImage( "test2", cuted);  
	
    }                                                                                                          
    int main()                                                                             
    {                                                                                                               
      
        const char* a = "abcd.jpg";  
        test = cvLoadImage(a, 0);  
	src=cvCreateImage(cvSize(800,500),IPL_DEPTH_8U,1);  
        cvResize(test,src,CV_INTER_LINEAR);  
        cvSmooth(src,src,CV_GAUSSIAN,5,5,0,0);  
        cvNamedWindow( "Source0000",1);                                                                                 
        cvShowImage( "Source0000", src);    
        IplImage* bw =NULL;  
        IplImage* color=NULL;  
        IplImage* jh=NULL;  
        IplImage* sm=NULL;  
        if( !src )  
            return -1;  
        jh = cvCreateImage( cvGetSize(src), 8, 1 );  
        sm = cvCreateImage( cvGetSize(src), 8, 1 );  
        bw = cvCreateImage( cvGetSize(src), 8, 1 );  
        color = cvCreateImage( cvGetSize(src), 8, 3 );  
        cvEqualizeHist( src, jh);  
        cvSmooth(jh, sm, CV_MEDIAN, 3, 3, 0, 0);  
      
        cvCanny(sm,bw,100,200,3);  
        cvCvtColor( bw, color, CV_GRAY2BGR );  
        cvSaveImage("color.bmp",color);  
      
        const char* b = "color.bmp";  
        bianyuan = cvLoadImage(b, 0);  
        img=cvCreateImage(cvGetSize(bianyuan),8,1);   
      
        cvNamedWindow( "Source",1);                                                                                 
        cvShowImage( "Source", bianyuan);    
      
        cvNamedWindow( "Components",1);                                                                             
      
        on_trackbar(0);   
	
/*
	const char * image = "abcd.jpg";  
        const char * outputbase = "out";  
      
    tesseract::TessBaseAPI api;  
    api.SetOutputName(outputbase);  
  
    int rc = api.Init(NULL, "chi_sim", tesseract::OEM_DEFAULT);  
    if (rc) {  
        printf("Could not initialize tesseract.\n");  
        exit(1);  
    }  
  
    //api.SetVariable("tessedit_char_whitelist", "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");  
  
    Pix* pixs = pixRead(image);  
    if (!pixs) {  
        printf("Cannot open input file: %s\n", image);  
        exit(2);  
    }  
    api.SetImage(pixs);  
  
    tesseract::TessResultRenderer* renderer = new tesseract::TessTextRenderer(outputbase);  
    if (!api.ProcessPages(image, NULL, 0, renderer)) {  
        printf("Error during processing.\n");  
    }  
  
    delete renderer;  
    pixDestroy(&pixs);
 */
        cvWaitKey(0);                                                                                               
        cvDestroyWindow( "sorce" );                                                                                 
        cvDestroyWindow( "threshold" );                                                                             
        cvDestroyWindow( "Components" );                                                                            
        cvReleaseImage( &src);                                                                                      
        cvReleaseImage( &img );                                                                                     
        cvReleaseImage(&dst);                                                                                       
        cvReleaseMemStorage(&storage);   
        return 0;  
    }  
