#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

#define threshold 386 //9.81 x 1.5 , or 9.8 x 1.5 = 14.7. The accelerometer sensitivity threshold. 257 x 1.5 = 385.5
#define norm_nom = 252 //norm_nom = (260 + 262 + 233) / 3 = 252
int32_t acc_norm;

int
acc_check(int32_t acc_norm)
{
    int flag;
    if (acc_norm < threshold)
       {
        flag = 1;
       }
    else
    {
        flag = 0;
       }
    return flag;
}
int
step_increment(int flag, int next_flag, int step_count)
{
    if (flag == 1 && next_flag == 0)
    {
        step_count = step_count + 1;
    }
    return step_count; //jeremy
}


int32_t
norm(int32_t x_mean, int32_t y_mean, int32_t z_mean)
{
    int32_t norm;
    //uint32_t norm_in_g;
    norm = (sqrt( (x_mean * x_mean) + (y_mean * y_mean) + (z_mean * z_mean) ));
    //norm_in_g = (int)(((norm/norm_nom)-((int)(norm/norm_nom)))*100));
    return norm;
}
