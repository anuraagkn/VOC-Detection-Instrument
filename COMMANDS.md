## Instrument Commands
The instrument accepts SCPI-style control commandss through the serial port. Although the framework is in place for supporting query commands (where the controller returns a parameter value), the methods for enacting the commands are yet to be implemented.

When processing commands, the instrument seprates nodes by looking at the placement of specific markers (```:```, ```<BLANKSPACE>``` and ```;```), so a valid argument should not include any of these markers, else the instrument will render the command erroneous.

### Command Tree
Below is the structure to follow when oprating the instrument using commands. Where available, any shorthand node names are enclosed in parenthesis next to their full form names e.g. ```AUTOmatic(AUTO)``` means that ```AUTO``` is the shorthand for ```AUTOmatic```. The valid arguments are specified in the comments, which begin after ```//```

```
:SYSTEM(SYST)
    :RESEt                              // reset the isntrument parameters (accepts any valid argument*)
    :AUTOmatic(AUTO)
        :STARtFREQuency(STARtFREQ)      // set the start frequency for an automated experiment
        :STOPFREQuency(STOPFREQ)        // set the stop frequency for an automated experiment
        :SCALe                          // set the scale of the automated experiment's frequency distribution
        :HOLD                           // set the hold time to spend at each frequency point
        :POINTs                         // set the number of frequency points to take measurements at
        :PPPOINt                        // specify whether to resample the analyte at each frequency point
        :EXPEriment(EXP)                // run an automated experiment
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
