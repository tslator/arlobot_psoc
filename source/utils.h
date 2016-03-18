#ifndef UTILS_H
#define UTILS_H
    
#include <project.h>

/* Clear bit a in value b */
#define CLEAR_BIT(a, b) ((~(1<<a)) & (b))
/* Set bit a in value b */
#define SET_BIT(a, b) ((1<<a) | (b))
/* Get bit a in value b */
#define GET_BIT(a, b) ((b>>a) & (1))

#define min(a, b) (a < b ? a : b)
#define max(a, b) (a > b ? a : b)
#define constrain(a, b, c) (min(max(a, b), c))
// Scale X -> Y
//    X          Y
//  -----    = -----  => y = x * y range / x range
//  x range    y range
#define scale_integer(x, x_range, y_range) ( (int32) (x * y_range) / (int32) x_range )
#define scale_float(x, x_range, y_range) ( (float) (x * y_range) / (float) x_range )

typedef struct _moving_average_tag
{
    uint32 n;
    uint32 last;
} MOVING_AVERAGE_TYPE;
    
uint32 MovingAverage(MOVING_AVERAGE_TYPE* ma, uint32 value);


#endif