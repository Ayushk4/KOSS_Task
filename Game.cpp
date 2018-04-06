#include<iostream>
#include<math.h>
#include<cstdlib>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;
using namespace std;

class point
{
    public:
    int x,y,theta;

    point()
    {
        x=0;
        y=0;
    }

};
const int  obj_c_fixed=60,obj_r_fixed=60;
int obj_r=60,obj_c=60,status=0,speed=10,score=0,click_status=0,wait_factor=0,size_factor=0,last_score=0,score_fact5=0;
char life='4';
point pt,pt_clicked;

void update_screen(Mat show,point pt,Mat object)
{
    int i,j;

    for (int i=0;i<obj_r;i++)
        for(int j=0;j<obj_c;j++)
            show.at<Vec3b>(i+pt.x-obj_r/2,j+pt.y-obj_c/2)=object.at<Vec3b>(i,j);

}

void erase_object(Mat show,point pt)
{
    int i,j;

    for (int i=0;i<obj_r;i++)
        for(int j=0;j<obj_c;j++)
        {   show.at<Vec3b>(i+pt.x-obj_r/2,j+pt.y-obj_c/2)[0]=255;
            show.at<Vec3b>(i+pt.x-obj_r/2,j+pt.y-obj_c/2)[1]=255;
            show.at<Vec3b>(i+pt.x-obj_r/2,j+pt.y-obj_c/2)[2]=255;
        }
}

void change_score(int score,Mat show)
{
    for(int i=520;i<560;i++)
        for(int j=430;j<500;j++)
        {   show.at<Vec3b>(i,j)[0]=0;
            show.at<Vec3b>(i,j)[1]=255;
            show.at<Vec3b>(i,j)[2]=255;
        }
    cout<<score<<" \n";
    char cha1=(score%10)+48,cha2=(score/10)+48;
    
    const char ch1[1]={cha2};
    const char ch2[1]={cha1};
    
    putText(show, ch1, cvPoint(430,560),FONT_HERSHEY_COMPLEX_SMALL, 2.0, cvScalar(200,0,0), 1, CV_AA);
   // putText(show, ch2, cvPoint(480,560),FONT_HERSHEY_COMPLEX_SMALL, 2.0, cvScalar(200,0,0), 1, CV_AA);
    
}

void bound_point(point pt)
{
    if(((pt.y-obj_c)<2*speed||(pt.y+obj_r)>(600-2*speed))||((pt.x-obj_c)<2*speed))
    {
        status=0;
        life--;
    }
}

void change_life(char l,Mat show)
{
    for(int i=520;i<560;i++)
        for(int j=130;j<160;j++)
        {   show.at<Vec3b>(i,j)[0]=0;
            show.at<Vec3b>(i,j)[1]=255;
            show.at<Vec3b>(i,j)[2]=255;
        }
    const char ch[1]={l};

    putText(show, ch, cvPoint(130,560),FONT_HERSHEY_COMPLEX_SMALL, 2.0, cvScalar(200,0,0), 1, CV_AA);
    
}

void check_click(int y, int x,Mat show)
{   cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
    if(((x<(pt.x+obj_r/2)&&x>(pt.x-obj_r/2))&&(y<(pt.y+obj_c/2)&&y>(pt.y-obj_c/2))))
    {   
        status=0;
        score++;
        click_status=0;
        if(score==100)
        {   cout<<"\n\n\n\n\n\nYou Won\n\n\n\n\n\n\n";
            exit(1);
        }   
    }

    if(click_status==1)
    {
        status=0;
        life--;
        change_life(life,show);
        click_status=0;
    }      
}

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    if  ( event == EVENT_LBUTTONDOWN )
    {
        pt_clicked.x=x;
        pt_clicked.y=y;
       
        click_status=1;
     }
}

void change_difficulty(int score,Mat show)
{
    if(last_score!=score)
    {
        change_score(score,show);
        if(wait_factor<30)
             wait_factor++;
        last_score=score;
    }

    if(score_fact5!=(int(score/5)))
    {
        if(size_factor<20);
        {   size_factor++;
            obj_c=obj_c_fixed-(2*size_factor);
            obj_r=obj_r_fixed-(2*size_factor);
        }

        wait_factor-=2;
        score_fact5=(int(score/5));
    }
}

int main()
{
    Mat object(obj_r,obj_c,CV_8UC3,Scalar(0,0,255));
    Mat show(600,600,CV_8UC3,Scalar(255,255,255));
    for(int i=500;i<600;i++)
        for(int j=0;j<600;j++)
        {   show.at<Vec3b>(i,j)[0]=0;
            //back_ground.at<Vec3b>(i,j)[2]=255;
        }
    //char ch=life;

    putText(show, "Lives", cvPoint(10,560),FONT_HERSHEY_COMPLEX_SMALL, 2.0, cvScalar(200,0,0), 1, CV_AA);
    putText(show, "Score", cvPoint(300,560),FONT_HERSHEY_COMPLEX_SMALL, 2.0, cvScalar(200,0,0), 1, CV_AA);
    srand(time (NULL));
    namedWindow("w",WINDOW_AUTOSIZE);
    
    change_life('3',show);
    change_score(0,show);
    while(1)
    {
        change_difficulty(score,show);
        
        bound_point(pt);

        if (life=='0')
            break;
        //cout<<"\n"<<wait_factor<<" waitfactor "<<size_factor<<" size factor"<<"\n\n\n";
        if(status==0)
        {   pt.y=rand()%(400-obj_c)+100+obj_c/2;
            pt.x=500-obj_r/2;
            pt.theta=((rand()%(157))+75);
            status=1;
        }

        else
        {
            pt.y+=int(speed*cos(float(pt.theta)/100));
            pt.x-=int(speed*sin(float(pt.theta)/100));
            cout<<pt.x<<" "<<pt.y<<" "<<pt.theta<<" "<<speed<<" "<<life<<"\n";
        }

        update_screen(show,pt,object);
        setMouseCallback("w",CallBackFunc,NULL);
        if(click_status==1)
            check_click(pt_clicked.x,pt_clicked.y,show);

        imshow("w",show);
        waitKey(200-5*wait_factor);
        //waitKey(50);
        erase_object(show,pt);

    }
    
    return 0;
}