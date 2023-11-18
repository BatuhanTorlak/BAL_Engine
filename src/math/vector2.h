#ifndef VECTOR2_H
#define VECTOR2_H

#define Vector2Create(x, y) (Vector2){(float)x, (float)y}
#define Vector2Add(left, right) (Vector2){(float)(left.x + right.x), (float)(left.y + right.y)}
#define Vector2Sub(left, right) (Vector2){(float)(left.x - right.x), (float)(left.y - right.y)}
#define Vector2Mul(left, right) (Vector2){(float)(left.x * right.x), (float)(left.y * right.y)}
#define Vector2Div(left, right) (Vector2){(float)(left.x / right.x), (float)(left.y / right.y)}

typedef struct Vector2_t
{
    float x;
    float y;
}Vector2;

float Vector2Distance(Vector2 start, Vector2 end);

#endif // VECTOR2_H