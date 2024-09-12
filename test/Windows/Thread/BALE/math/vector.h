#ifndef VECTOR_H
#define VECTOR_H

// ---------------------------------------------------------
// Vector 2
// ---------------------------------------------------------

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
float Vector2Dot(Vector2 first, Vector2 second);
float Vector2Cross(Vector2 first, Vector2 second);

// ---------------------------------------------------------
// Vector 3
// ---------------------------------------------------------

#define Vector3Create(x, y, z) (Vector3){(float)x, (float)y, (float)z}
#define Vector3Add(left, right) (Vector3){(float)(left.x + right.x), (float)(left.y + right.y), (float)(left.z + right.z)}
#define Vector3Sub(left, right) (Vector3){(float)(left.x - right.x), (float)(left.y - right.y), (float)(left.z - right.z)}
#define Vector3Mul(left, right) (Vector3){(float)(left.x * right.x), (float)(left.y * right.y), (float)(left.z * right.z)}
#define Vector3Div(left, right) (Vector3){(float)(left.x / right.x), (float)(left.y / right.y), (float)(left.z / right.z)}

typedef struct Vector3_t
{
    float x;
    float y;
    float z;
}Vector3;

float Vector3Distance(Vector3 start, Vector3 end);

#endif // VECTOR_H