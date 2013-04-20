//PathName of Files to be corrected
	
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include "highgui.h"
#include "cv.h"


using namespace std;

float mhi[480][640]={0.0};
float* extract_feature(IplImage* extr_b,IplImage* extr_tt1,int p1i,int p2i,int p1j,int p2j,int lvl,float *tmp_fetr)
{
	int i,j,rl,cl,m,n,ii,jj,pc,ri[100],ci[100],no_of_black=0,no_of_white=0;
	int max=0,box_number_x=0,box_number_y=0,flag=0;
	float mhit[480][640]={0.0};
	int alpha=0.4;
	CvScalar s;
	m=(p2i-p1i)/14;
	n=(p2j-p1j)/14;
	
	int ind=0; 
	float* fetr=tmp_fetr;   
	for(int i=0;i<2940;i++)
	{
		fetr[i]=0.0;

		
	//std::cout<<fetr[i];
	}

	int r[15],c[15];
//-------Dividing into 14 rectangles--------------------
	for(int i=0;p1i<=p2i;i++)
	{
		r[i]=p1i;
		p1i=p1i+m;
rl=i;
	}
	rl=rl+1;

	for(int i=0;p1j<=p2j;i++)
	{
		c[i]=p1j;
		p1j=p1j+n;
	cl=i;

	}
	cl=cl+1;

//---------------For Depth Profile----------------------

int k=1;

for(ii=0;ii<rl-1;ii++)
{
	for(jj=0;jj<cl-1;jj++)
	{	
        	pc=0;                                                            
        	
		flag=0;
       		for(i=r[ii];i<r[ii+1];i++)	
		{
      		      for(j=c[jj];j<c[jj+1];j++)
		      {
				if(flag==0)
				{
					k++;
					flag=1;
				}
				
      			        pc=pc+1; 
				s=cvGet2D(extr_b,i,j);
				if(s.val[0]==0)
				{
					no_of_black++;

				}
				if (s.val[0]==0 && s.val[0]<5)                                
      			              fetr[ind]=fetr[ind]+1;
      			        else if (s.val[0]>=5 && s.val[0]<10)
      			              fetr[ind+1]=fetr[ind+1]+1;
				else if (s.val[0]>=10 && s.val[0]<20)
				    fetr[ind+2]=fetr[ind+2]+1;
				else if (s.val[0]>=20 && s.val[0]<30)
				    fetr[ind+3]=fetr[ind+3]+1;
				else if (s.val[0]>=30 && s.val[0]<40)
				    fetr[ind+4]=fetr[ind+4]+1;
				else if (s.val[0]>=40 && s.val[0]<45)
				    fetr[ind+5]=fetr[ind+5]+1;
				else if (s.val[0]>=45 && s.val[0]<60)
				    fetr[ind+6]=fetr[ind+6]+1;
				else if (s.val[0]>=60 && s.val[0]<80)
				    fetr[ind+7]=fetr[ind+7]+1;
				else if (s.val[0]>=80 && s.val[0]<255)
				    fetr[ind+8]=fetr[ind+8]+1;
				


                	}                
            	}
//		if (ii==13 && jj==13)
//			cout<<"Olaa! Siniora!"<<endl;
        	fetr[ind]=(float)fetr[ind]/pc;
		fetr[ind+1]=(float)fetr[ind+1]/pc;
		fetr[ind+2]=(float)fetr[ind+2]/pc;
		fetr[ind+3]=(float)fetr[ind+3]/pc;
		fetr[ind+4]=(float)fetr[ind+4]/pc;
		fetr[ind+5]=(float)fetr[ind+5]/pc;
		fetr[ind+6]=(float)fetr[ind+6]/pc;
		fetr[ind+7]=(float)fetr[ind+7]/pc;
		fetr[ind+8]=(float)fetr[ind+8]/pc;

		if(no_of_black>max)
		{
			max=no_of_black;
			box_number_x=ii;
			box_number_y=jj;
		}

       	        fetr[ind+9]=(float)box_number_x/pc;
		fetr[ind+10]=(float)box_number_y/pc;
		//cout<<ind<<" grid box:"<<ii*(rl-1)+jj<<endl;
		//std::cout<<"INdex:"<<ind<<"\t"<<fetr[ind+1]<<"\n";
       		ind=ind+11;

	}
}


	max=0;
//-----------------For Motion Profile------------------------
ind=196*lvl;
for(int ii=0;ii<rl-1;ii++)
{
	for(int jj=0;jj<cl-1;jj++)
	{
        	pc=0;
        	for (int i=r[ii];i<r[ii+1];i++)	
		{            		
			for(int j=c[jj];j<c[jj+1];j++)
			{                	
				pc=pc+1;
				s=cvGet2D(extr_tt1,i,j);
                		if (s.val[0]!=0)
                    			fetr[ind]=fetr[ind]+1;
                			no_of_white++;
        		}
		}

     
        fetr[ind]=fetr[ind]/pc;

		if(no_of_white>max)
		{
			max=no_of_white;
			box_number_x=ii;
			box_number_y=jj;
		}

       	        fetr[ind+1]=(float)box_number_x/pc;
		fetr[ind+2]=(float)box_number_y/pc;
		ind=ind+3;
        	
	}
}
int lkj=0;
//cout<<"Signiora! "<<ind<<endl;
//---------------------MHI Feature----------------------

for(int i=0;i<480;i++)
{
	for(int j=0;j<640;j++)
	{
		s=cvGet2D(extr_b,i,j);
		if(s.val[0]!=0)
		{
			s.val[0]=200;
			cvSet2D(extr_b,i,j,s);
		}
		if(s.val[0]==0)
		{
			s.val[0]=255;
			cvSet2D(extr_b,i,j,s);
		}
		if(s.val[0]==20)
		{
			s.val[0]=0;
			cvSet2D(extr_b,i,j,s);
		}
	}
}

for(int i=0;i<480;i++)
{
	for(int j=0;j<640;j++)
	{
		s=cvGet2D(extr_b,i,j);
		mhi[i][j]=(float)mhi[i][j]*alpha+s.val[0]*(1-alpha);
		mhit[i][j]=mhi[i][j];	
	}
}

for(int i=0;i<480;i++)
{
	for(int j=0;j<640;j++)
	{
		if(mhi[i][j]<1)
			mhit[i][j]=0;
		
	}
}

for(int i=0;i<480;i++)
{
	for(int j=0;j<640;j++)
	{
		if(mhit[i][j]>0)
			mhit[i][j]=255;
		
	}
}
//ind=196*(lvl+3)+1;

for(int ii=0;ii<rl-1;ii++)
{
	for(int jj=0;jj<cl-1;jj++)
	{
        	pc=0;
        	for (int i=r[ii];i<r[ii+1];i++)	
		{            		
			for(int j=c[jj];j<c[jj+1];j++)
			{                	
				pc=pc+1;
                		if (mhi[i][j]==255)
                    			fetr[ind]=fetr[ind]+1;
                	}
		}

     
        fetr[ind]=fetr[ind]/pc;
		ind=ind+1;
        	
	}
}
//int sada=0;
	//std::cout<<ind;
	return fetr;
}

int threshold_image(IplImage *img)
{
		
	int nbHistlevels=256;
	//Histogram
	//int *histData=(int *)calloc(nbHistlevels,sizeof(int));
	int histData[256]={0};
	int ptr=0;
	while(ptr<img->imageSize)
	{
		int h=img->imageData[ptr];
		histData[h]++;
		ptr++;
	
	}
	//Total number of pixels
	int total=img->imageSize;
	
	float sum=0;
	int t;
	for(t=0;t<nbHistlevels;t++)
		sum+=t*histData[t];
		
	float sumB=0;
	int wB=0;
	int wF=0;
	float varMax=0;
	int threshold=0;
	
	for(t=0;t<nbHistlevels;t++)
	{
		wB += histData[t];
		if(wB==0)
			continue;
		wF=total-wB;
		if(wF==0)
			break;
		sumB+=(float)(t*histData[t]);
		float mB=sumB/wB;
		float mF=(sum-sumB)/wF;
		
		float varBetween=(float)wB*(float)wF*(mB-mF)*(mB-mF);
		
		if(varBetween>varMax)
		{
			varMax=varBetween;
			threshold=t;
		}
	
	}
	
	return threshold;
	


}

int main()
{

	char imgname[70],imgname2[70];
	int num=1;
	int i,j,len,ll,l,ind,var;
	int k[1000];
	int g[40][700];
	IplImage *img1,*img2,*b,*img21,*img_2;
	int gray_lvl=11;
	CvSize z;
	CvScalar s;
	int p1i,p2i,p1j,p2j,m=1,flag1=0,flag2=0;
	clock_t t1,t2;
	float *tmp_fetr;
	//float **feature=new float*[1435];
	t1=clock();
	int fetr_num;
	ofstream out("training_set.txt");

	
	
	
	
	
	if(!out)
	{
		cout<<"Cannnot Open File";
	}
	
/*	for( int i=0;i<2156;i++)
	{
		feature[i]=new float[2156];
		
	}*/
	

	
	cvNamedWindow("Display",1);
//	cvNamedWindow("IMAGE 2",1);
for(int set=0;set<6;set++)
{
	for(int train=0;train<8;train++)
	{
		
//		var=i;
		len=281;
		for(ll=0;ll<len;ll++)
		{
			//cout<<"Value of i:"<<i;
			
			sprintf(imgname,"set%d/Train%d/depth%04d.png\0",set+1,train+1,ll+1);
			sprintf(imgname2,"set%d/Train%d/depth%04d.png\0",set+1,train+1,ll);//(i+1),k[ll]
//			sprintf(imgname,"/media/HDD/Train/Train_new%d/depth%04d.png\0",i+1,k[ll]+1);
//			sprintf(imgname2,"/media/HDD/Train/Train_new%d/depth%04d.png\0",i+1,k[ll]);
			//imgname[59]='\0';
			//imgname2[59]='\0';
				tmp_fetr=new float[2940];					

			img1=cvLoadImage(imgname,CV_LOAD_IMAGE_GRAYSCALE);
			img2=cvLoadImage(imgname2,CV_LOAD_IMAGE_GRAYSCALE);
			

			if(img1==NULL || img2==NULL)
			{
				std::cout<<"Image Can't Be Loaded"<<std::endl;

			}
			//else
			//	std::cout<<"Image Loaded"<<std::endl;


		
		
		//	img_1=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);
			
		//	img2=cvCreateImage(cvGetSize(img_2),IPL_DEPTH_8U,1);

		//	cvCvtColor(img_1,img_1,CV_RGB2GRAY);
		//	cvCvtColor(img_2,img_2,CV_RGB2GRAY);
			b=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);

			
			img21=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);
//---------------------------Pre-processing------------------------------------------

			cvAbsDiff(img2,img1,img2);
		
			l=threshold_image(img1);
//			l=127;			


      		       for(int i=0;i<480;i++)         
			{                                                
           			for(int j=0;j<640;j++)
				{
					s=cvGet2D(img1,i,j);			
               				 if (s.val[0]==0 || s.val[0]>=l) 
						{                  
							s.val[0]=255;              
                   				 cvSet2D(img1,i,j,s);
				                 }       
				                 if(i>455)
				                 {
				                 	s.val[0]=255;
				                 	cvSet2D(img1,i,j,s);
				                 
				                 }                     
					s=cvGet2D(img2,i,j);	                
               				 if (s.val[0]>100 )          
						{                  
							s.val[0]=0;              
                   				 cvSet2D(img2,i,j,s);
				                 }           
           			 }
      			}	
	

			cvErode(img1,img1,NULL,2);
//				cvShowImage("Display",img1);					 //To store the motion profile
//				cvWaitKey(33);

			
			
			//Histogram Equalisation
			cvEqualizeHist(img1,b);

		
       			
			cvThreshold(img2,img2,80,255,CV_THRESH_BINARY);//im2bw conversion in matlab
			cvErode(img2,img2,NULL,1);
	
		

			//cvShowImage("IMAGE 2",img2);
			//	cvWaitKey(60);
			
//------------------------Find ROI----------------------------------------
p1j=0;p2j=640;p1i=0;p2i=480;

		 for(int j=0;j<640;j++)
		 {	for(int i=0;i<480;i++)
		  	{
				s=cvGet2D(b,i,j);
				if(s.val[0]!=255 && flag1==0)                                     
                  		{
					p1j=j;                                                  
		            		flag1=1;                                                    
                		}  
				s=cvGet2D(b,i,640-j-1);                                                   
                		if(s.val[0]!=255 && flag2==0)                                  
                 		{ 
					p2j=640-j-1;                                             
		            		flag2=1;                                                     
                		}                                                        
            		}                                                           
		    	if (flag1==1 && flag2==1)                                              
		        	break;                                                  
		                                                                           
		}
		
		for(int i=0;i<480;i++)
		{	for(int j=0;j<640;j++)
		    	{ 
				s=cvGet2D(b,i,j);
				if(s.val[0]!=255 && flag1==1)
		        	{
					p1i=i;
		            		flag1=0;
		        	}
				s=cvGet2D(b,480-i-1,j);
		        	if(s.val[0]!=255 && flag2==1)
		        	{
					p2i=480-i-1;
		            		flag2=0;
		        	}
		    	}
		    	if (flag1==0 && flag2==0)
		        	break;
		    
		}

//----------Rouding the coordinates to the nearest 14--------------
		if(p1i>13)
		    p1i=p1i-p1i%14;
		else
		    p1i=14;
		
		if(p2i<467)
		    p2i=p2i+(14-p2i%14);
		else 
		    p2i=476;
		
		if(p1j>13)
		    p1j=p1j-p1j%14;
		else
		    p1j=14;
		
		if(p2j<627)
		    p2j=p2j+(14-p2j%14);
		else 
		    p2j=630;

		int k=0;
		extract_feature(b,img2,p1i,p2i,p1j,p2j,gray_lvl,tmp_fetr);  
		if(ll%40==0)
		{      
		out<<train+1;
     		fetr_num=0;
     		}
     		for( int i=0;i<2940;i++)
		{
			out<<" "<<fetr_num+1<<":"<<tmp_fetr[i];
			fetr_num++;
		}
		if(ll%40==39)
		out<<"\n";
		delete[] tmp_fetr;
		
		
		cvReleaseImage(&img1);
		cvReleaseImage(&img2);
		cvReleaseImage(&b);
		cvReleaseImage(&img21);
		
		}
	
	

	}
	}
	t2=clock();
	//cout<<"Starting Time:"<<t1<<"Ending Time:"<<t2;
	
	out.close();
	
	return 0;
}
