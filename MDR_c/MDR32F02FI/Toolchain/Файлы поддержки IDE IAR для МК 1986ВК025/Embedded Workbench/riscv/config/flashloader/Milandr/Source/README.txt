  /**
  @page Source project for the MDR32F02 (К1986ВК025) IAR FlashLoader
  @latexonly
  @verbatim
  ******************** (C) COPYRIGHT 2022 Milandr ******************************
  * @file    README.txt
  * @author  Milandr Application Team
  * @version V1.0.0
  * @date    12/08/2022
  * @brief   Description of the MDR32F02 (К1986ВК025) IAR FlashLoader.
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, MILANDR SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
  @endverbatim
  @endlatexonly

@par Example Description

This is source project for MDR32F02 (К1986ВК025) IAR FlashLoader.


@par Directory contains:

    - Utils         Contains IAR Flashloader docs and objcopy.exe for output file formatting
    - src\          IAR FlashLoader source code
    - OutputFiles\  Output files to copy to "...\riscv\config\flashloader\Milandr" folder

@par How to use.

To compile flashloader output files, you must do the following:
    - Open FlashMLDR187.eww IAR workspace file;
    - Compile project;
    - Run "prepare_output_file.bat" script to create proper MLDR187.out file;
    - Copy contenes of "OutputFiles\" folder to IAR flashloaders folder ("...\riscv\config\flashloader\Milandr").

 * <h3><center>&copy; COPYRIGHT 2022 Milandr</center></h3>
 */

