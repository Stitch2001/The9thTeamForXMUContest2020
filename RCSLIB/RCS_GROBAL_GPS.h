/*@filename: RCS_GROBAL_GPS.h
 *@author     ��־ΰ       
 *@brief:     ȫ����λ���ն�
 *@date: 2020-7-17
*/
#ifndef _RCS_GROBAL_GPS_H_
#define _RCS_GROBAL_GPS_H_

#include "rcs.h"

void RCS_GPS_Init(void);
void Get_GPS_Msg(float GPS_values[]);
float RCS_Get_X(void);
float RCS_Get_Y(void);
float RCS_Get_Z(void);

#endif
