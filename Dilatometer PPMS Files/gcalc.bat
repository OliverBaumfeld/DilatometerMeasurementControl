rem batch to calibrate

rem $0 c0 b ri ra input_datafile silver_expansion_file
rem Otherwise if the parameters are omitted entirely, the default values of

rem    c0 = 4pF ; b = 9.794mm ; ri = 2.33 ; ra = 6.25 ; will be assumed (which

rem    corresponds to the W cell). Additionly the recommended values of silver

rem    expansion from Touloukian et al. will be used. E.g

rem gcalc.exe 4.66 9.294 2.584 6.1 %1 silver.lit  --old cell

rem new cell 01/12/11 -a one liner header with {} is a must in the data file!!
gcalc.exe 5.12 9.8 2.33 6.25 BASF004.dat AgOB400.dat
