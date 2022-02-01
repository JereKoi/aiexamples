#include <stdio.h>
#include <typeinfo>
#include <Windows.h>

// Kääntööporttiobjekti
class Turnstile {
public:
	void thankYou() {
		printf("%s\n", __FUNCTION__);
	}
	void unlock() {
		printf("%s\n", __FUNCTION__);
	}
};

// Forward declaroi TurnstileFSM -luokka
class TurnstileFSM;

class TurnstileState {
protected:
	TurnstileFSM* m_fsm;
public:
	TurnstileState(TurnstileFSM* fsm) 
		: m_fsm(fsm) {
	}

	// Eventtifunktioiden määrittelyt (ei toteutusta, pelkkä deklaraatio)
	virtual void coin() = 0;
};

class LockedState : public TurnstileState {
public:
	LockedState(TurnstileFSM* fsm) 
		: TurnstileState(fsm) {
	}

	// Eventtifunktioiden mtoteutukset aliluokissa
	virtual void coin();
};

class UnlockedState : public TurnstileState {
public:
	UnlockedState(TurnstileFSM* fsm)
		: TurnstileState(fsm) {
	}

	// Eventtifunktioiden mtoteutukset aliluokissa
	virtual void coin();
};


// The FSM class
class TurnstileFSM {
public:
	TurnstileFSM(Turnstile& obj) 
		: m_obj(obj), m_currentState(0) {
	}

	void setState(TurnstileState* newState) {
		m_currentState = newState;
		printf("NEW STATE: %s\n", typeid(*m_currentState).name());
	}

	Turnstile& getTurnstile() {
		return m_obj;
	}

	// Eventtifunktkiot
	void coin() {
		m_currentState->coin();
	}


private:
	Turnstile& m_obj;
	TurnstileState* m_currentState;

};


// Nämä sitten oikeassa tilanteessa kirjoittaa .cpp tiedostoon.

void LockedState::coin() {
	// getFSM and getTurnstile is method of TurnstileState class
	m_fsm->getTurnstile().unlock(); // Calls Tunstile::unlock
	m_fsm->setState(new UnlockedState(m_fsm));
}

void UnlockedState::coin() {
	// getFSM and getTurnstile is method of TurnstileState class
	m_fsm->getTurnstile().thankYou(); // Calls Tunstile::thankYou
}





int main() {
	Turnstile obj;
	TurnstileFSM fsm(obj);
	fsm.setState(new LockedState(&fsm));

	while (1) {
		if (GetAsyncKeyState('1')) {
			fsm.coin();
		}
	}

	return 0;
}