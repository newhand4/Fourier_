/************************************************************************/
/*      
@author : hsj
@email : 1005542693@qq.com
@project : 使用傅里叶变换实现音频从时域转换为频域,并显示出来.
@problem : 声音的输入问题,中间傅里叶变换问题,处理后的显示问题
@about this file : 解决声音输入问题.对外提供一个接口,两个实现(工厂模式) : 1.口语输入 2.本地音频文件输入.
@create time : 2017.04.05 16:07
*/
/************************************************************************/


#include "stdafx.h"
#include "wave.h"
#include <Windows.h>
#include <stdio.h>
#include "mmsystem.h"
#pragma comment(lib, "winmm.lib")


//函数声明
void PlayMusi();
void WaveInitFormat(LPWAVEFORMATEX m_WaveFormat, WORD nCh,DWORD nSampleRate,WORD BitsPerSample);
DWORD CALLBACK MicCallback(HWAVEIN hwavein, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
void RecordWave();

void main()
{
	//PlayMusi();
	RecordWave();
	while(1);
}
void RecordWave()
{
	int count = waveInGetNumDevs();//1
	printf("\n音频输入数量：%d\n",count);

	WAVEINCAPS waveIncaps;
	MMRESULT mmResult = waveInGetDevCaps(0,&waveIncaps,sizeof(WAVEINCAPS));//2
	printf("\n音频输入设备：%s\n",waveIncaps.szPname);

	if(MMSYSERR_NOERROR==mmResult)
	{
		HWAVEIN phwi;
		WAVEFORMATEX pwfx;
		WaveInitFormat(&pwfx,1,8000,8);
		printf("\n请求打开音频输入设备");
		printf("\n采样参数：单声道 8kHz 8bit\n");
		mmResult=waveInOpen(&phwi,WAVE_MAPPER,&pwfx,(DWORD)(MicCallback),NULL,CALLBACK_FUNCTION);//3

		if(MMSYSERR_NOERROR==mmResult)
		{
			WAVEHDR pwh1;
			char buffer1[10240];
			pwh1.lpData=buffer1;
			pwh1.dwBufferLength=10240;
			pwh1.dwUser=1;
			pwh1.dwFlags=0;
			mmResult=waveInPrepareHeader(phwi,&pwh1,sizeof(WAVEHDR));//4
			printf("\n准备缓冲区1");

			WAVEHDR pwh2;
			char buffer2[10240];
			pwh2.lpData=buffer2;
			pwh2.dwBufferLength=10240;
			pwh2.dwUser=2;
			pwh2.dwFlags=0;
			mmResult=waveInPrepareHeader(phwi,&pwh2,sizeof(WAVEHDR));//4
			printf("\n准备缓冲区2\n");

			if(MMSYSERR_NOERROR==mmResult)
			{
				mmResult=waveInAddBuffer(phwi,&pwh1,sizeof(WAVEHDR));//5
				printf("\n将缓冲区1加入音频输入设备");
				mmResult=waveInAddBuffer(phwi,&pwh2,sizeof(WAVEHDR));//5
				printf("\n将缓冲区2加入音频输入设备\n");

				if(MMSYSERR_NOERROR==mmResult)
				{
					mmResult=waveInStart(phwi);//6
					printf("\n请求开始录音\n");
				}
			}
		}

	}
}
DWORD CALLBACK MicCallback(HWAVEIN hwavein, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2)
{
	switch(uMsg)
	{
	case WIM_OPEN:
		printf("\n设备已经打开...\n");
		break;

	case WIM_DATA:
		printf("\n缓冲区%d存满...\n",((LPWAVEHDR)dwParam1)->dwUser);
		waveInAddBuffer (hwavein, (LPWAVEHDR)dwParam1, sizeof (WAVEHDR)) ;
		break;

	case WIM_CLOSE:
		printf("\n设备已经关闭...\n");
		break;
	default:
		break;
	}
	return 0;
}
void WaveInitFormat(LPWAVEFORMATEX m_WaveFormat, WORD nCh,DWORD nSampleRate,WORD BitsPerSample)
{
	m_WaveFormat->wFormatTag = WAVE_FORMAT_PCM;
	m_WaveFormat->nChannels = nCh;
	m_WaveFormat->nSamplesPerSec = nSampleRate;
	m_WaveFormat->nAvgBytesPerSec = nSampleRate * nCh * BitsPerSample/8;
	m_WaveFormat->nBlockAlign = m_WaveFormat->nChannels * BitsPerSample/8;
	m_WaveFormat->wBitsPerSample = BitsPerSample;
	m_WaveFormat->cbSize = 0;
}
void PlayMusi()
{
	int error = mciSendString(_T("open C:\\Users\\Angel\\Desktop\\有多少爱可以重来.mp3 alias myDivece"), NULL, 0, NULL);
	if (error == 0)
	{
		mciSendString(_T("play myDivece"), NULL, 0, NULL); //播放
	}
}