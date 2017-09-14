===================================================================================
  Calculating the gap between capacitance plates from capacitance and temperature
===================================================================================

 Content:
  1) Comments
  2) C++ code snippets

===================================================================================
   1) Comments
===================================================================================

To calculate the gap the calibration constants of the dilatometer cell are required
to be given in the file 'DilConf.txt' in the following format:

# Kommentar:
#
C0=5.12  # parallel plate capacitance
b=9.8    # pivot point
ro=6.05  # outer radius
ri=2.35  # inner radius

===================================================================================
   2) C++ code snippets
===================================================================================

This is the part of the program which uses equation 1 in Ref [1] to calculate the
gap by an iterative method.

    double C, T;    // measured capacitance and temperature
    double dllAg, kT, AoT, AiT, C2, dstep, d, gamma2_o, gamma2_i, gamma_o, gamma_i;
    double k0 = EPSILON0/C0 * (ro*ro*PI - ri*ri*PI);
    
    kT = k0;                    // removed *(1- dllAg);
    AoT = ro*ro*PI*(1+dllAg);   // removed *(1- dllAg);
    AiT = ri*ri*PI*(1+dllAg);   // removed *(1- dllAg);
   
    C2 = 1000;
    dstep = 0.000001;
    d = (AoT - AiT) * EPSILON0/C;
    if ( d < kT*ro/(b+ro) )
        d = 1.1 *  kT*ro/(b+ro);
    int oldsign = 0;
    int ii=0;
    while (abs(C - C2) > 1E-9 && dstep/d > 1E-15 && ii < 1000000)
    {
        ii++;
        gamma_o  = ro/b * (kT/d-1.);
        gamma2_o = gamma_o*gamma_o;
        gamma_i  = ri/b * (kT/d-1.);
        gamma2_i = gamma_i*gamma_i;
        C2 = AoT*EPSILON0/d *2./gamma2_o * (1-sqrt(1-gamma2_o));
        C2 = C2 - AiT*EPSILON0/d *2./gamma2_i * (1.-sqrt(1.-gamma2_i));
        d = d + sgn(C2-C)* dstep;

        if (oldsign != sgn(C2-C))
        {
            oldsign = sgn(C2-C);
            dstep = dstep/10;
        }
    }
    
The formulas 
    AoT = ro*ro*PI*(1+dllAg); 
    AiT = ri*ri*PI*(1+dllAg);
are slightly different compared to Ref [1] as is also the case in gcalc.bas
and thefunc.py (the latter is from the PAT programs)
     
The justification given in gcalc.bas and thefunc.py is: 
    ' nicht quadrieren, da capacitaetssensor
    ' einerseits durch vergroessern der platten mit
    ' steigender T um (1+dllag)^2 zu grosse cap werte
    ' liefert, andererseits aber wegen der thermischen
    ' verkuerzung des plattenabstands um (1-dllag) zu
    ' kleine cap werte liefert - beide effekte zusammen
    ' sollten der korrekturfaktor (1+dllag) am
    ' besten beschreiben
A sloppy translation:
 Do not square. On one hand the increase of the area of the capacitor plates 
 leads to a capacitance increase of (1+dllag)^2. On the other hand the 
 decrease of the distance between the plates leads to a decrease of the
 capacitance of (1-dllag). Both effect combined should be described best by
 a correction factor of (1+dllag).
    
The formula  
    kT = k0;
is slightly different compared to Ref [1] as is also the case in gcalc.bas
but not in thefunc.py

The justification given in gcalc.bas for kT=k0 is:
    ' the pivot changes with temperature - however
    ' this compensates with the fact that we look
    ' at a virtual gap = real distance /(1+dllAg#)

[1] M. Rotter , H. Müller, E. Gratz, M. Doerr and M. Loewenhaupt: 
    Rev. Sci. Instrum. 69, 2742 (1998)