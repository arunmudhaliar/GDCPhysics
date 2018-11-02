//
//  untitled.mm
//  GEARv1.0
//
//  Created by Samarth on 24/06/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#include "Timer.h"
#include <CoreFoundation/CFDate.h>

float	Timer::m_fFPS=0.0f;             //frames
float	Timer::m_fDT=0.0f;              //in sec
int		Timer::m_iDT=0;                 //in milli sec
float	Timer::m_fElapsedTime=0.0f;     //in sec
double	Timer::m_fPrevTime=0.0f;
float   Timer::m_fAveragingTime=0.5f;   //in sec
int     Timer::m_iFrameCount=0;
double  Timer::m_fLastTime=0.0f;
float  Timer::m_fTimeScale=0.0f;

void Timer::init()
{
	reset();
}

void Timer::update()
{
	double curTime=0.0;
	curTime=CFAbsoluteTimeGetCurrent();
	m_fDT=(float)(curTime-m_fPrevTime);
	if(m_fDT<=0.0f)
	{
		m_fDT=0.03f;
	}
	m_iDT=(int)(m_fDT*1000.0f);
	m_fElapsedTime+=m_fDT;
    
    if(((float)(curTime-m_fLastTime))>=m_fAveragingTime)
    {
        m_fFPS=m_iFrameCount/((float)(curTime-m_fLastTime));
        m_iFrameCount=0;
        m_fLastTime=curTime;
    }
    else
    {
        m_iFrameCount++;
    }
	m_fPrevTime=curTime;
}

void Timer::update(float targetFPS)
{
	double curTime=0.0;
	curTime=CFAbsoluteTimeGetCurrent();
	m_fDT=(float)(curTime-m_fPrevTime);
	if(m_fDT<=0.0f)
	{
		m_fDT=0.03f;
	}
	while(m_fDT<(1.0f/targetFPS))
	{
		curTime=CFAbsoluteTimeGetCurrent();
		m_fDT=(float)(curTime-m_fPrevTime);
	}
	m_iDT=(int)(m_fDT*1000.0f);
	m_fElapsedTime+=m_fDT;
	m_fFPS=1.0f/m_fDT;
	m_fPrevTime=curTime;
}

void Timer::reset()
{
	m_fTimeScale=1.0f;
	m_fFPS=0.0f;		
	m_fDT=0.0f;		
	m_iDT=0;			
	m_fElapsedTime=0.0f;
	m_fPrevTime=CFAbsoluteTimeGetCurrent();
    m_fLastTime=m_fPrevTime;
}

double Timer::getCurrentTimeInSec()
{
	return CFAbsoluteTimeGetCurrent();
}

unsigned long Timer::getCurrentTimeInMilliSec()
{
	return CFAbsoluteTimeGetCurrent()*1000;
}
