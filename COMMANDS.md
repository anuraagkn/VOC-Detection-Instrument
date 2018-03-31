### Instrument Commands
The instrument accepts SCPI-style control commandss through the serial port. Although the framework is in place for supporting query commands (where the controller returns a parameter value), the methods for enacting the commands are yet to be implemented.


```
:SYSTEM(SYST)
    :RESEt
    :AUTOmatic(AUTO)
        :STARtFREQuency(STARtFREQ)
        :STOPFREQuency(STOPFREQ)
        :SCALe
        :HOLD
        :POINTs
        :PPPOINt
        :EXPEriment(EXP)
:SOURce
    :FREQuency(FREQ)
    :VOLTage(VOLT)
    :WAVEtype(WAVE)
    :LAMP
    :PUMP
        :VOLUme(VOL)
        :TIME
        :POWEr(POW)
ADD METER LATER
```
