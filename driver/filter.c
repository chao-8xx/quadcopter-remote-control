#include "filter.h"


//二阶butterworth-lpf
float butterworth_lpf(float now_input,_Butterworth_data *buffer, _Butterworth_parameter *parameter)
{
    buffer->input_data[2] = now_input;

    /* Butterworth LPF */
    buffer->output_data[2] =   parameter->b[0] * buffer->input_data[2]
                             + parameter->b[1] * buffer->input_data[1]
                             + parameter->b[2] * buffer->input_data[0]
                             - parameter->a[1] * buffer->output_data[1]
                             - parameter->a[2] * buffer->output_data[0];
    /* x(n) 保存 */
    buffer->input_data[0] = buffer->input_data[1];
    buffer->input_data[1] = buffer->input_data[2];
    /* y(n) 保存 */
    buffer->output_data[0] = buffer->output_data[1];
    buffer->output_data[1] = buffer->output_data[2];
    
    return buffer->output_data[2];
}

