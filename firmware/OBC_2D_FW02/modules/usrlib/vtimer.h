//���ⶨʱ��
#ifndef _VTIMER_H__
#define _VTIMER_H__

#include "datatypes.h"
#include "systick.h"

#ifdef __cplusplus
extern "C" {
#endif  

#define VTIMERS_NUM       32             //���256��,������8��������
#define VTIMER_NULL       255           //��Ч��ʱ���� 

bool vtimer_ovf(uint8_t n);               //�Ƿ�ʱ�䵽 
void vtimer_service(void);              //������̺���,ÿ��ϵͳ�����е���һ��
bool vtimer_init(void);                 //��ʼ�����ⶨʱ��
bool vtimer_deinit(void);
void vtimer_set(uint8_t n,uint32_t t);    //n�Ŷ�ʱ����ʱ�����趨Ϊt��ϵͳ����
uint8_t vtimer_alloc(void);               //����һ����ʱ��,����255��ʾû�ж��ඨʱ����
uint32_t vtimer_timeleft(uint8_t n);      //ʣ�����ʱ��
void vtimer_free(uint8_t n);              //�ͷ�һ������Ҫ�õĶ�ʱ��

#ifdef __cplusplus
}
#endif

#endif
