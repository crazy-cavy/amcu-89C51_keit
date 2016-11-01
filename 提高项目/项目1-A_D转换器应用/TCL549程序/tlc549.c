/*
*�� �� �� �� �� �� ʱ ��  06-8-9 22:08
*�� �� �� �⣺TLC549 AD�����ӳ���
*�� �� ˵ ������TLC549��ȡ����ֵ
*_________________________________________
*
*/

#include "intrins.h"

#define    Wait1us        _nop_();
#define    Wait2us        {_nop_();_nop_();}
#define    Wait4us        {Wait2us;Wait2us;}
#define    Wait8us        {Wait4us;Wait4us;}
#define    Wait10us      {Wait8us;Wait2us;}
#define    Wait30us      {Wait10us;Wait8us;Wait4us;Wait2us;}


/************����ӿ�����**************/
sbit Clock = P1 ^ 5; //ʱ�ӿ���
sbit DataOut = P1 ^ 4; //�����������
sbit ChipSelect = P1 ^ 3; //Ƭѡ����
/************�ӿ����߶������**********/


/*********�������ƣ�ADCSelChannel********
*
*���ܣ�ת��ָ��ͨ����ģ����Ϊ������
*
*˵����
*
*λ�ã�1
*
****************************************/
unsigned char ADCSelChannel(void)
{
      unsigned char ConvertValue = 0;
      unsigned char i;
      ChipSelect = 1; //оƬ��λ
      ChipSelect = 0;
      ChipSelect = 1;
      Clock = 0;
      Wait4us;
      ChipSelect = 0; //оƬ��ʼ
      Wait4us; //�ȴ���ʱ
      for (i = 0; i < 8; i ++) //�������ת��ʱ��
      {
            Clock = 1;
            Clock = 0;
      }
      ChipSelect = 1; //��ʼת��
      Wait10us; //�ȴ�ת������
      ChipSelect = 0; //��ȡת�����
      Wait4us;
      for (i = 0; i < 8; i ++) //
      {
            Clock = 1;
            ConvertValue <<= 1;
            if (DataOut)
            {
                   ConvertValue = 0x1;
            }
            Clock = 0;
      }
      ChipSelect = 1;
      return (ConvertValue); //����ת�����
}



