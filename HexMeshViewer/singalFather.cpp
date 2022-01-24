#include "singalFather.h"

SingalFather *SingalFather::instance = nullptr;

SingalFather::SingalFather() : QObject(nullptr) {}


/**************************************************
@brief   : ����һ���¶��� ʹ�õ���ģʽ  ˵ʵ���������ûɶ�ã�����Ϊ�˳н� ������ʾ�źŵĶ���
@author  : lee
@input   ��none
@output  ��none
@time    : none
**************************************************/
SingalFather *SingalFather::getInstance() {
	if (!instance)
		instance = new SingalFather;
	return instance;
}
