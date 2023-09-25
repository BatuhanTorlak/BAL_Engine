#ifndef MATH_H
#define MATH_H

#include "Vector2.h"
#include "Vector3.h"
#include "Point.h"

#define PI 3.1415926535897932
#define RAD_TO_DEG(x) (x * PI / 180)
#define DEG_TO_RAD(x) (x / 180 * PI)

/// @brief 
/// @param number radian
/// @return 
float sinf(float number);
/// @brief 
/// @param number radian
/// @return 
float cosf(float number);
/// @brief 
/// @param number radian
/// @return 
float tanf(float number);
/// @brief 
/// @param number radian
/// @return 
float cotf(float number);
/// @brief 
/// @param number sinus
/// @return 
float asinf(float number);
/// @brief 
/// @param number cosinus
/// @return radian
float acosf(float number);
/// @brief 
/// @param number sin/cos
/// @return radian
float atanf(float number);
/// @brief 
/// @param number cos/sin
/// @return radian
float acotf(float number);
float sqrtf(float number);
float powerf(float number, float power);

double sin(double number);
double cos(double number);
double tan(double number);
double cot(double number);
double asin(double number);
double acos(double number);
double atan(double number);
double acot(double number);
double sqrt(double number);
double power(double number, double power);

#endif