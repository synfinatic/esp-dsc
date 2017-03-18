#ifndef __ESP_DSC_H__
#define __ESP_DSC_H__

#define DEBUG 1

/* Encoder pins on RJ45 */
#define CHAN_RA_A   2     // RJ45 pin 3
#define CHAN_RA_B  15     // RJ45 pin 1
#define CHAN_DEC_A  5     // RJ45 pin 7
#define CHAN_DEC_B  4     // RJ45 pin 5

#define ESP_DSC_VERSION "0.0.1"

// Resolution of encoders after quadrature decoding
// must be a float!
#define RA_RESOLUTION 10000.0
#define DEC_RESOLUTION 10000.0

#define SERIAL_SPEED 115200 // 9600

#endif // __ESP_DSC_H__
