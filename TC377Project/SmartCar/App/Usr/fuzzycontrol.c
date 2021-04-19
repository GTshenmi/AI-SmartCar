#include "fuzzycontrol.h"
 
float fuzzyRuleKp[7][7]={
	PL,	PL,	PM,	PM,	PS,	PS,	ZE,
	PL,	PL,	PM,	PM,	PS,	ZE,	ZE,
	PM,	PM,	PM,	PS,	ZE,	NS,	NM,
	PM,	PS,	PS,	ZE,	NS,	NM,	NM,
	PS,	PS,	ZE,	NS,	NS,	NM,	NM,
	ZE,	ZE,	NS,	NM,	NM,	NM,	NL,
	ZE,	NS,	NS,	NM,	NM,	NL,	NL
};
 
float fuzzyRuleKi[7][7]={
	NL,	NL,	NL,	NM,	NM,	ZE,	ZE,
	NL,	NL,	NM,	NM,	NS,	ZE,	ZE,
	NM,	NM,	NS,	NS,	ZE,	PS,	PS,
	NM,	NS,	NS,	ZE,	PS,	PS,	PM,
	NS,	NS,	ZE,	PS,	PS,	PM,	PM,
	ZE,	ZE,	PS,	PM,	PM,	PL,	PL,
	ZE,	ZE,	PS,	PM,	PL,	PL,	PL
};
 
float fuzzyRuleKd[7][7]={
	PS,	PS,	ZE,	ZE,	ZE,	PL,	PL,
	NS,	NS,	NS,	NS,	ZE,	NS,	PM,
	NL,	NL,	NM,	NS,	ZE,	PS,	PM,
	NL,	NM,	NM,	NS,	ZE,	PS,	PM,
	NL,	NM,	NS,	NS,	ZE,	PS,	PS,
	NM,	NS,	NS,	NS,	ZE,	PS,	PS,
	PS,	ZE,	ZE,	ZE,	ZE,	PL,	PL
};
 
/*
  线性量化
*/
void  LinearQuantization(float *this_error,float *delta_error,float max_error,float min_error) // -6~+6
{  
   *this_error = 6.0*(*this_error)/(max_error-min_error);
   *delta_error = 6.0*(*delta_error)/(max_error-min_error);  
}
/*
  隶属度计算
*/
void CalcMembership(float *ms,float qv,int * index)
{

}

/*
  模糊推理
*/
void FuzzyComputation(float pv,float *deltaK)
{

}


 
 void FuzzyCtrl()
 {

 } 
 
 
#if 0 
pid_ctrl_t fuzzy(float e,float ec)
{
 
     float etemp,ectemp;
     float eLefttemp,ecLefttemp;
     float eRighttemp ,ecRighttemp;
 
     int eLeftIndex,ecLeftIndex;
     int eRightIndex,ecRightIndex;  //-95 ...-25 -15 -5 0 5 15 25 35 45 55 65 75 85 95 
     pid_ctrl_t      fuzzy_PID;
     
     
     if(e>90.0||e<-90.0)
     {
        etemp = 8.5;//MAX       
     }
     else if(e <= 5 && e>=-5)
     {
        etemp = 0.0;
     }   
     else
     {
        if(e>=0.0)
        {
            if (e>=85.0)
              etemp = 8.5;
            else if(e>=75.0)
              etemp = 7.5;
            else if(e>=65.0)
              etemp = 6.5;
            else if(e>=55.0)
              etemp = 5.5;
            else if (e>=45.0)  
              etemp = 4.5;
            else if (e>=35.0) 
               etemp = 3.5;
            else if (e>=25.0)  
              etemp = 2.5;
            else if (e>=15.0) 
               etemp = 1.5;   
            else
               etemp = 0.5;
        }
        else
        {
          
            if (e>=-85.0)
              etemp = -8.5;
            else if(e>=75.0)
              etemp = -7.5;
            else if(e>=65.0)
              etemp = -6.5;
            else if(e>=55.0)
              etemp = -5.5;
            else if (e>=45.0)  
              etemp = -4.5;
            else if (e>=35.0) 
               etemp = -3.5;
            else if (e>=25.0)  
              etemp = -2.5;
            else if (e>=15.0) 
               etemp = -1.5;   
            else
               etemp = -0.5;      
        }     
     }
     //etemp = e > 3.0 ? 0.0 : (e < - 3.0 ? 0.0 : (e >= 0.0 ? (e >= 2.0 ? 2.5: (e >= 1.0 ? 1.5 : 0.5)) : (e >= -1.0 ? -0.5 : (e >= -2.0 ? -1.5 : (e >= -3.0 ? -2.5 : 0.0) ))));
 
     eLeftIndex = (int)e;
     eRightIndex = eLeftIndex;
     eLeftIndex = (int)((etemp-0.5) + 10);        //[-10,10] -> [0,20]
     eRightIndex = (int)((etemp+0.5) + 10);
 
     eLefttemp =etemp == 0.0 ? 0.0:((etemp+0.5)-e);
     eRighttemp=etemp == 0.0 ? 0.0:( e-(etemp-0.5));
 
     if(ec>90.0||ec<-90.0)
     {
        ectemp = 8.5;//MAX       
     }
     else if(ec <= 5 && ec>=-5)
     {
        ectemp = 0.0;
     }   
     else
     {
        if(ec>=0.0)
        {
            if (ec>=85.0)
              ectemp = 8.5;
            else if(ec>=75.0)
              ectemp = 7.5;
            else if(ec>=65.0)
              ectemp = 6.5;
            else if(ec>=55.0)
              ectemp = 5.5;
            else if (ec>=45.0)  
              ectemp = 4.5;
            else if (ec>=35.0) 
               ectemp = 3.5;
            else if (ec>=25.0)  
              ectemp = 2.5;
            else if (ec>=15.0) 
               ectemp = 1.5;   
            else
               ectemp = 0.5;
        }
        else
        {
          
            if (ec>=-85.0)
              ectemp = -8.5;
            else if(ec>=75.0)
              ectemp = -7.5;
            else if(ec>=65.0)
              ectemp = -6.5;
            else if(ec>=55.0)
              ectemp = -5.5;
            else if (ec>=45.0)  
              ectemp = -4.5;
            else if (ec>=35.0) 
               ectemp = -3.5;
            else if (ec>=25.0)  
              ectemp = -2.5;
            else if (ec>=15.0) 
               ectemp = -1.5;   
            else
               ectemp = -0.5;      
        }     
     }
     //ectemp = ec > 3.0 ? 0.0 : (ec < - 3.0 ? 0.0 : (ec >= 0.0 ? (ec >= 2.0 ? 2.5: (ec >= 1.0 ? 1.5 : 0.5)) : (ec >= -1.0 ? -0.5 : (ec >= -2.0 ? -1.5 : (ec >= -3.0 ? -2.5 : 0.0) ))));
 
     ecLeftIndex = (int)((ectemp-0.5) + 10);        //[-10,10] -> [0,20]
     ecRightIndex = (int)((ectemp+0.5) + 10);
 
     ecLefttemp =ectemp == 0.0 ? 0.0:((ectemp+0.5)-ec);
     ecRighttemp=ectemp == 0.0 ? 0.0:( ec-(ectemp-0.5));
 
/*************************************反模糊*************************************/
 
 
 
 
	fuzzy_PID.Kp = (eLefttemp * ecLefttemp *  fuzzyRuleKp[ecLeftIndex][eLeftIndex]                    
					+ eLefttemp * ecRighttemp * fuzzyRuleKp[ecRightIndex][eLeftIndex]
					+ eRighttemp * ecLefttemp * fuzzyRuleKp[ecLeftIndex][eRightIndex]
					+ eRighttemp * ecRighttemp * fuzzyRuleKp[ecRightIndex][eRightIndex]);
 
	fuzzy_PID.Ki =   (eLefttemp * ecLefttemp * fuzzyRuleKi[ecLeftIndex][eLeftIndex]
					+ eLefttemp * ecRighttemp * fuzzyRuleKi[ecRightIndex][eLeftIndex]
					+ eRighttemp * ecLefttemp * fuzzyRuleKi[ecLeftIndex][eRightIndex]
					+ eRighttemp * ecRighttemp * fuzzyRuleKi[ecRightIndex][eRightIndex]);
 
	fuzzy_PID.Kd = (eLefttemp * ecLefttemp *    fuzzyRuleKd[ecLeftIndex][eLeftIndex]
					+ eLefttemp * ecRighttemp * fuzzyRuleKd[ecRightIndex][eLeftIndex]
					+ eRighttemp * ecLefttemp * fuzzyRuleKd[ecLeftIndex][eRightIndex]
					+ eRighttemp * ecRighttemp * fuzzyRuleKd[ecRightIndex][eRightIndex]);
        return fuzzy_PID;
 
}
#endif


#if 0
float speed_pid()
{
        float tar = 0,cur = 0;                //目标值 , 实际值
	static pid_ctrl_t pid= {0, 0, 0};              //赋予初值kp，ki，kd
	static int sumE = 0;                   //累加偏差
	static int lastE = 0;
 
	pid_ctrl_t OUT = {0, 0, 0};
	float e = -1,ec = -2.6;
 
 
 
	e = tar - cur;             //目标值 - 实际值
	ec = e - lastE;            //误差变化率
	sumE += e;
	lastE = (int) e;
	OUT = fuzzy(e, ec);      //模糊控制调整  kp，ki，kd
 
	return (pid.Kp+OUT.Kp)*e + (pid.Kd+OUT.Kd)*ec + (pid.Ki+OUT.Ki)*sumE;
}
#endif