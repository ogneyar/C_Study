;---------------------------------
;
; Copyright (c) 2019 IAR Systems AB.
;
; Licensed under the Apache License, Version 2.0 (the "License");
; you may not use this file except in compliance with the License.
; You may obtain a copy of the License at
;     http://www.apache.org/licenses/LICENSE-2.0
;
; Unless required by applicable law or agreed to in writing, software
; distributed under the License is distributed on an "AS IS" BASIS,
; WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either expressed or implied.
; See the License for the specific language governing permissions and
; limitations under the License.
;
; Functions accessed by the debugger to perform a flash download.
; All public symbols and the function FlashBreak() are looked up and called
; by the debugger.
;
;---------------------------------

        PUBLIC  FlashInitEntry
        PUBLIC  FlashWriteEntry
        PUBLIC  FlashEraseWriteEntry
        PUBLIC  FlashChecksumEntry
        PUBLIC  FlashSignoffEntry
        PUBLIC  FlashBufferStart
        PUBLIC  FlashBufferEnd

        EXTERN  FlashBreak
        EXTERN  Fl2FlashInitEntry
        EXTERN  Fl2FlashWriteEntry
        EXTERN  Fl2FlashEraseWriteEntry
        EXTERN  Fl2FlashChecksumEntry
        EXTERN  Fl2FlashSignoffEntry

        SECTION CSTACK:DATA:NOROOT(3)


;---------------------------------
;
; FlashInitEntry()
; Debugger interface function.
;
;---------------------------------

        SECTION `.text`:CODE:ROOT(2)
FlashInitEntry:
        ;; Set up the normal stack pointer.
        lui     sp, %hi(SFE(CSTACK))
        addi    sp, sp, %lo(SFE(CSTACK))
        jal     Fl2FlashInitEntry
        jal     FlashBreak


;---------------------------------
;
; FlashWriteEntry()
; Debugger interface function.
;
;---------------------------------

        SECTION `.text`:CODE:ROOT(2)
FlashWriteEntry:
        jal     Fl2FlashWriteEntry
        jal     FlashBreak


;---------------------------------
;
; FlashEraseWriteEntry
; Debugger interface function.
;
;---------------------------------

        SECTION `.text`:CODE:ROOT(2)
FlashEraseWriteEntry:
        jal     Fl2FlashEraseWriteEntry
        jal     FlashBreak


;---------------------------------
;
; FlashChecksumEntry
; Debugger interface function.
;
;---------------------------------

        SECTION `.text`:CODE:NOROOT(2)
FlashChecksumEntry:
        jal     Fl2FlashChecksumEntry
        jal     FlashBreak


;---------------------------------
;
; FlashSignoffEntry
; Debugger interface function.
;
;---------------------------------

        SECTION `.text`:CODE:NOROOT(2)
FlashSignoffEntry:
        jal     Fl2FlashSignoffEntry
        jal     FlashBreak


;---------------------------------
;
; Flash buffer.
;
;---------------------------------

        SECTION LOWEND:DATA(8)
FlashBufferStart:

        SECTION HIGHSTART:DATA
FlashBufferEnd:


        END
