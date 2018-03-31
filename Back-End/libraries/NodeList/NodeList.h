/*NodeList.h by Anuraag Narang: defines the controller's commmand nodes and illustrates the tree hierarchy using indentation.*/

// for commands controlling the system object
#define SYSTEM		"SYSTem"
#define SYST		"SYST"
	#define RESET		"RESEt"
	#define AUTOMATIC	"AUTOmatic"
	#define AUTO		"AUTO"
		#define STARTFREQUENCY		"STARtFREQuency"
		#define STARTFREQ			"STARtFREQ"
		#define STOPFREQUENCY		"STOPFREQuency"
		#define STOPFREQ			"STOPFREQ"
		#define SCALE				"SCALe"
		#define HOLD				"HOLD"
		#define POINTS				"POINTs"
		#define PPP					"PPPOINt"
		#define EXPERIMENT			"EXPEriment"
		#define EXP					"EXP"
// for commands controlling the source (input object)
#define SOURCE		"SOURce"
	#define FREQUENCY	"FREQuency"
	#define FREQ		"FREQ"
	#define VOLTAGE		"VOLTage"
	#define VOLT        "VOLT"
	#define WAVETYPE	"WAVEtype"
	#define WAVE        "WAVE"
	#define LAMP        "LAMP"
	#define PUMP		"PUMP"
		#define VOLUME				"VOLUme"
		#define VOL					"VOL"
		#define TIME				"TIME" // also used for meter
		#define POWER				"POWEr"
		#define POW					"POW"
// for commands controlling the meter object
#define METER