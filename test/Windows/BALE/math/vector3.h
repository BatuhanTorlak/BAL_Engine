#ifndef VECTOR3_H
#define VECTOR3_H

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

#endif // VECTOR3_H