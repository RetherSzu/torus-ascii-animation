#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define WIDTH 50
#define HEIGHT 100
#define BACKGROUND_ASCII_CODE ' '

float pi = 3.141592653589793;

float A = 0, B = 0, C = 0;
float zBuffer[WIDTH][HEIGHT];

float K2 = 6;
float R1 = 1;
float R2 = 2;
float thetaIncrementSpeed = 0.07;
float phiIncrementSpeed = 0.2;

char buffer[WIDTH][HEIGHT];

void clearBufferAndZBuffer()
{
    memset(buffer, BACKGROUND_ASCII_CODE, WIDTH * HEIGHT);
    memset(zBuffer, 0, WIDTH * HEIGHT * sizeof(float));
}

int main()
{
    float precision = 10 * pi;
    float K1 = WIDTH * K2 * 3 / (8 * (R1 + R2));

    printf("\x1b[2J");
    while (1)
    {
        printf("\x1b[H");
        clearBufferAndZBuffer();

        float cosA = cos(A), sinA = sin(A);
        float cosB = cos(B), sinB = sin(B);

        for (float theta = 0; theta < precision; theta += thetaIncrementSpeed)
        {
            float costheta = cos(theta), sintheta = sin(theta);
            for (float phi = 0; phi < precision; phi += phiIncrementSpeed)
            {
                float cosphi = cos(phi), sinphi = sin(phi);

                float circleX = R2 + R1 * costheta;
                float circleY = R1 * sintheta;

                float x = circleX * (cosB * cosphi + sinA * sinB * sinphi) - circleY * cosA * sinB;
                float y = circleX * (sinB * cosphi - sinA * cosB * sinphi) + circleY * cosA * cosB;
                float z = K2 + cosA * circleX * sinphi + circleY * sinA;
                float ooz = 1 / z;

                int xp = (int)(WIDTH / 2 + K1 * ooz * x);
                int yp = (int)(HEIGHT / 2 - K1 * ooz * y);

                float L = cosphi * costheta * sinB - cosA * costheta * sinphi - sinA * sintheta + cosB * (cosA * sintheta - costheta * sinA * sinphi);

                if (L >= 0 && ooz > zBuffer[xp][yp])
                {
                    zBuffer[xp][yp] = ooz;
                    int luminance_index = L * 8;
                    buffer[xp][yp] = ".,-~:;=!*#$@"[luminance_index];
                }
            }
        }

        printf("\x1b[H");
        for (int j = 0; j < WIDTH; j++)
        {
            for (int i = 0; i < HEIGHT; i++)
            {
                putchar(buffer[j][i]);
            }
            putchar('\n');
        }

        A += 0.02;
        B += 0.02;
        C += 0.01;
        usleep(16000);
    }
    return 0;
}
