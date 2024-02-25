#ifndef MATH_H
#define MATH_H

#include "vector2.h"
#include "vector3.h"
#include "point.h"

#define PI 3.1415926535897932
#define RAD_TO_DEG(x) (x * PI / 180)
#define DEG_TO_RAD(x) (x / 180 * PI)

/// @brief 
/// @param number radian
/// @return length
float Sinf(float number);
/// @brief 
/// @param number radian
/// @return 
float Cosf(float number);
/// @brief 
/// @param number radian
/// @return length
float Tanf(float number);
/// @brief 
/// @param number radian
/// @return length
float Cotf(float number);
/// @brief 
/// @param number length
/// @return length
float Asinf(float number);
/// @brief 
/// @param number length
/// @return radian
float Acosf(float number);
/// @brief 
/// @param number length
/// @return radian
float Atanf(float number);
/// @brief 
/// @param number length
/// @return radian
float Acotf(float number);
float Sqrtf(float number);
float Powerf(float number, float power);
float Roundf(float number);
float Absf(float number);
float Clampf(const float number, const float min, const float max);

int RoundInt(float number);
int AbsInt(int number);

double Sin(double number);
double Cos(double number);
double Tan(double number);
double Cot(double number);
double Asin(double number);
double Acos(double number);
double Atan(double number);
double Acot(double number);
double Sqrt(double number);
double Power(double number, double power);

#endif