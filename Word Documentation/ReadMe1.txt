===================================================================================
    Dilatometer Measurement, Instructions and Help
===================================================================================
Version: 1.6
Revised: 14 February 2015
Contact: o.baumfeld12@imperial.ac.uk

 Content:
  1) How the program works
  2) How to use the program
  3) Initialisation and buttons
  4) Detailed explanation of the advisories
  5) Potential issues when using this program at an other experimental setup

===================================================================================
   1) How the program works
===================================================================================
  ____________                     ____________                       ____________ 
 |            |    ----------->   |            |  <--------------->  |            |
 |            |   sends advisory  |    This    |  Sends Measurement  |            |
 |    PPMS    |    <--------->    |            |     command and     |   AH2550A  |
 |            |   requests time,  |  Program   |   reads response    |            |
 |____________|      T and H      |____________|                     |____________|
       |                                 |
       |                                 |
       V                                 V
 Sets Temperature               Writes Information
 and Field                         into a file

The PPMS MultiVu sends an advisory which is caught by the program. This causes the 
program to get the current time stamp, temperature, magnetic field and sample space
pressure of the PPMS and to send a measurement command to the capacitance bridge
and read its response.

The communication between the program and the PPMS and between the program and the 
capacitance bridge is handled via GPIB.
 
===================================================================================
   2) How to use the program
===================================================================================
 a) Start the program. (OK that one was easy)
 b) Set the output file.
 d) Write a sequence for the PPMS and whenever a measurement should be taken insert
    an advisory with an advisory number of 1. The most convenient way is to use the
    'Scan Field', 'Scan Temperature' and 'Scan Time' commands.
 e) Keep in mind that the time it takes to do a single measurement is about 3.5 sec
    for an average exponent of 5 ( which is the default value). When the advisory 
    frequency is higher than that, points will be omitted since the sequence is 
    halted as long as a measurement is taken. Two strategies can be used: First, 
    make the time steps between advisories bigger than 3 seconds. Second make the 
    time steps between advisories really small, which causes a measurement quickly 
    afterwards the sequence is continued. If you do not know what I am talking 
    about have a look at section 4.
 f) Start the sequence and have a cup of tea.
 
  Example Sequence:

Scan Field from 0.0Oe to 5000.0Oe at 100.0Oe/sec, in 51 steps, Uniform, Sweep, Persistent
    Set Advise Number 1
End Scan
End Sequence

This sequence would sweep through the field from 0 to 5 T with 100 Oe/sec and take
51 measurements (hence every 1000 Oe or every 10 seconds). The magnetic field would 
be kept at 5 T in persistent mode in the end.

===================================================================================
   3) Initialisation and buttons
===================================================================================
 a) What happens when the program is started?
    o) The GPIB communication with the capacitance bridge is initialized.
    o) The Average Exponent of the capacitance bridge is set to 5. If a different 
       Average Exponent is desired it can be changed either manually after the
       initialization (see manual of the capacitance bridge) or in the source code 
       of the program itself.
    o) The program registers to receive advisories from the PPMS MultiVu Software.
    
 b) What happens on pressing 'Choose Save File'?
    o) A Save File dialog is opened.
    o) The program checks if the file already exists. If so it will ask if you if
       you want to append or overwrite the existing file. Otherwise the file will
       be created.
    o) A line containing the labels is written to the file.

===================================================================================
   4) Detailed explanation of the advisories
===================================================================================
Advisory 1:
 o) The execution of the sequence file is halted by the HOLDOFF command.
 o) The Temperature(K), the Magnetic Field(H), the User Bridge Channel 3(Ohm) and
    the sample space pressure are requested from the PPMS.
 o) The capacitance and the loss are requested from the capacitance bridge AH2550A.
 o) The information is written to the data file.
 o) A second HOLDOFF command causes the PPMS to continue the execution of the 
    sequence file.

Advisory 2:
 o) Displays the number of advisories received in the dialogue box.
 o) Resets the advisory counter to zero

===================================================================================
   5) Potential issues when using this program at another experimental setup
===================================================================================
The following issues can be changed either in the source code of the program itself
or in the capacitance bridge (see manual).
 a) The GPIB number of the capacitance bridge HAS TO BE 28 (default value).
 b) The return string of the capacitance bridge HAS TO BE in a certain format.
    Format as given in the manual, the loss must be in nanosiemens.