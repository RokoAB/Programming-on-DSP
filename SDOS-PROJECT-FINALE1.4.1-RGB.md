#include <sys\exception.h>
#include <cdefBF533.h>
#include "sysreg.h"
#include "ccblkfn.h"

void inicijalizacija_EBIU(void);
void postavi_SCLK_54MHz(void);
void Init_SDRAM(void);
void median_filter(void);

#define stupaca 384 // sirina slike u pikselima
#define redova 256  // visina slike u pikselima
#define ukPix (stupaca * redova * 3)  // Total pixels times 3 (RGB)

unsigned char *ulaz, *izlaz;

void main(void)
{
    sysreg_write(reg_SYSCFG, 0x32);  // Initialize System Configuration Register
    postavi_SCLK_54MHz();
    inicijalizacija_EBIU();
    Init_SDRAM();

    ulaz = (unsigned char *)0x080000;   // Pointer to RGB image data
    izlaz = (unsigned char *)0x090000;  // Pointer to output buffer for filtered data

    median_filter();
}

// Helper function to calculate the median of 9 values
unsigned char median_of_9(unsigned char values[9])
{
    int i, j;
    unsigned char temp;

    // Sort the 9 elements using bubble sort
    for (i = 0; i < 9 - 1; i++) {
        for (j = i + 1; j < 9; j++) {
            if (values[i] > values[j]) {
                temp = values[i];
                values[i] = values[j];
                values[j] = temp;
            }
        }
    }

    // Return the median value (middle of the sorted array)
    return values[4];
}

void median_filter(void)
{
    int x, y, c, i, j, k;
    unsigned char window[9];

    // Loop over each row and column
    for (y = 0; y < redova; y++) {
        for (x = 0; x < stupaca; x++) {
            // Loop over each color channel: R, G, B
            for (c = 0; c < 3; c++) {
                i = 0;

                // Fill the window with neighboring pixel values for the current channel, considering edges
                for (j = -1; j <= 1; j++) {
                    int yIndex = y + j;
                    if (yIndex < 0) yIndex = 0;  // Top edge
                    if (yIndex >= redova) yIndex = redova - 1;  // Bottom edge

                    int xOffsets[] = {-1, 0, 1};
                    for (k = 0; k < 3; k++) {
                        int xIndex = x + xOffsets[k];
                        if (xIndex < 0) xIndex = 0;  // Left edge
                        if (xIndex >= stupaca) xIndex = stupaca - 1;  // Right edge

                        // Access the correct pixel channel
                        window[i++] = ulaz[(yIndex * stupaca + xIndex) * 3 + c];
                    }
                }

                // Apply the median filter to the current window
                izlaz[(y * stupaca + x) * 3 + c] = median_of_9(window);
            }
        }
    }
}

void inicijalizacija_EBIU(void)
{
    /* flash memorija spojena je na procesor preko EBIU sabirnice, ova
    funkcija definira njen nacin rada (na nasim vjezbama ce to biti
    uvijek iste vrijednosti) */
    *pEBIU_AMBCTL0 = 0x7bb07bb0;
    *pEBIU_AMBCTL1 = 0x7bb07bb0;
    *pEBIU_AMGCTL = 0x000f;
}

void postavi_SCLK_54MHz(void)
{
    *pPLL_DIV = 5;
    *pPLL_CTL = 0x1400;
}

// SDRAM Setup
void Init_SDRAM(void)
{
    if (*pEBIU_SDSTAT & SDRS) {
        // SDRAM Refresh Rate Control Register
        //*pEBIU_SDRRC = 0x00000817;
        *pEBIU_SDRRC = 0x0000019c;

        // SDRAM Memory Bank Control Register
        *pEBIU_SDBCTL = 0x00000013;
        // ili moze ic:
        *pEBIU_SDBCTL = 0x00000025;

        // SDRAM Memory Global Control Register
        *pEBIU_SDGCTL = 0x0091998d;

        ssync();
    }
}


To adjust the provided code so that it works for RGB images, you need to modify the median_filter() function to apply the median filter separately to each color channel (Red, Green, and Blue). Since the RGB image is stored in a single buffer with the format [R, G, B, R, G, B, ...], you'll need to handle each channel independently.

                                 
