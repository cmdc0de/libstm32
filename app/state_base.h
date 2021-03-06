#ifndef STATE_BASE_H
#define STATE_BASE_H

#include "../error_type.h"

namespace cmdc0de {

/*
 *	@author cmdc0de
 *	Usage:  Holds run time context information from the application, Current time thorugh the main loop, maybe some performance information
 *	Or anything else the library already knows about.
 */
class RunTimeContext {
public:
private:
	uint32_t CurrentLoop;

};

/*
 * @author: cmdc0de
 *
 * Usage:  Base class for the program state, any runtime information will need to be passed to the construction of the state
 * 	I did not want to template this class so I decided against passing some template parameter into the Run function.  Normally this will
 * 	hold references to the attached devices (LCD, Sensors, etc).
 */
class StateBase {
public:
	struct ReturnStateContext {
		ReturnStateContext(StateBase *next, const ErrorType &er) :
				NextMenuToRun(next), Err(er) {
		}
		ReturnStateContext(StateBase *n) :
				NextMenuToRun(n), Err() {
		}
		StateBase *NextMenuToRun;
		ErrorType Err;
	};
public:
	StateBase();
	ReturnStateContext run();
	uint32_t timeInState();
	ErrorType shutdown();
	virtual ~StateBase();
protected:
	static const uint32_t INIT_BIT = 0x01;
	static const uint32_t DONT_RESET = 0x02;
	static const uint32_t SHIFT_FROM_BASE = 8;
	virtual ErrorType onInit()=0;
	virtual ReturnStateContext onRun()=0;
	virtual ErrorType onShutdown()=0;
	void setState(uint32_t n) {
		StateData |= n;
	}
	void clearState(uint32_t n) {
		StateData = (StateData & ~n);
	}
	bool checkState(uint32_t n) {
		return (StateData & n) != 0;
	}
	bool hasBeenInitialized() {
		return (StateData & INIT_BIT) != 0;
	}
	bool shouldReset() {
		return (StateData & DONT_RESET) == 0;
	}
	uint32_t getTimesRunCalledAllTime() {
		return TimesRunCalledAllTime;
	}
	uint32_t getTimesRunCalledSinceLastReset() {
		return TimesRunCalledSinceLastReset;
	}
private:
	ErrorType init();
private:
	uint32_t StateData :8;
	uint32_t TimesRunCalledAllTime :24;
	uint32_t TimesRunCalledSinceLastReset;
	uint32_t StateStartTime;
};

}
#endif
