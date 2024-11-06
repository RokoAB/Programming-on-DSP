
      # include <sys\exception.h>
      # include <cdefBF533.h>
      # include "sysreg.h"
      # include "ccblkfn.h"
      
      void inicijalizacija_EBIU(void);
      void postavi_SCLK_54MHz(void);
      void Init_SDRAM(void);
      void median_filter(void);
      
      #define stupaca 384 //irina slike u pikselima
      #define redova 256//isina slike u pikselima
      #define ukPix (stupaca*redova)
      
      unsigned char *ulaz,*izlaz;  
      
      void main(void)
      {
      sysreg_write(reg_SYSCFG, 0x32);           //Initialize System Configuration Register
      //postavi_SCLK_54MHz();
      //inicijalizacija_EBIU();
      Init_SDRAM();
     
      ulaz = (unsigned char *)0x080000;        // Pointer to RGB image data
      izlaz = (unsigned char *)0x090000;//Pointer to output buffer for filtered data
     
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
          int x, y, i, j,k;
          unsigned char window[9];
      
          for (y = 0; y < redova; y++) {
              for (x = 0; x < stupaca; x++) {
                  // Fill the window with the neighboring grayscale pixel values, considering edges
                  i = 0;
                  for (j = -1; j <= 1; j++) {
                      int yIndex = y + j;
                      if (yIndex < 0) yIndex = 0;  // Top edge
                      if (yIndex >= redova) yIndex = redova - 1;  // Bottom edge
      
                      int xOffsets[] = {-1, 0, 1};
                      for (k = 0; k < 3; k++) {
                          int xIndex = x + xOffsets[k];
                          if (xIndex < 0) xIndex = 0;  // Left edge
                          if (xIndex >= stupaca) xIndex = stupaca - 1;  // Right edge
      
                          window[i++] = ulaz[yIndex * stupaca + xIndex];
                      }
                  }
      
                  // Apply the median filter to the current window
                  izlaz[y * stupaca + x] = median_of_9(window);
              }
          }
      }



      //SDRAM Setup
      void Init_SDRAM(void)
      {
            if (*pEBIU_SDSTAT & SDRS) {
                  //SDRAM Refresh Rate Control Register
                  //*pEBIU_SDRRC = 0x00000817;  
                  *pEBIU_SDRRC = 0x0000019c;    
      
                  //SDRAM Memory Bank Control Register
                  *pEBIU_SDBCTL = 0x00000013;
                  // ili moze ic:
                  *pEBIU_SDBCTL = 0x00000025;
      
                  //SDRAM Memory Global Control Register    
                  *pEBIU_SDGCTL = 0x0091998d;  
      
                  ssync();
            }
      }
