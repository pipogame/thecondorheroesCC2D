#include "State.h"
#include "BaseHero.h"


State::State()
{
}

State::~State()
{
}

void State::run(BaseHero *hero)
{
	// do nothing
}

void State::jump(BaseHero *hero)
{
	// do nothing
}

void State::land(BaseHero *hero)
{
	// do nothing
}

void State::attack(BaseHero * hero)
{
	// do nothing
}



// RUNNING
Running::Running()
{
}

Running::~Running()
{
}

void Running::jump(BaseHero * hero)
{
	hero->normalJump();
	hero->changeState(new Jumping());
}

void Running::land(BaseHero * hero)
{
	hero->landing();
	hero->changeState(new Landing());
}

void Running::attack(BaseHero * hero)
{
	hero->attackNormal();
	hero->changeState(new Attack());
}




// IDLE
Idling::Idling()
{
}

Idling::~Idling()
{
}

void Idling::run(BaseHero * hero)
{
	hero->run();
	hero->changeState(new Running());
}




// JUMP
Jumping::Jumping()
{
}

Jumping::~Jumping()
{
}

void Jumping::jump(BaseHero * hero)
{
	hero->doubleJump();
	hero->changeState(new DoupleJumping());
}

void Jumping::land(BaseHero * hero)
{
	hero->landing();
	hero->changeState(new Landing());
}

void Jumping::attack(BaseHero * hero)
{
	hero->attackNormal();
	hero->changeState(new Attack());
}



// DOUBLE JUMPP
DoupleJumping::DoupleJumping()
{
}

DoupleJumping::~DoupleJumping()
{
}

void DoupleJumping::land(BaseHero * hero)
{
	hero->landing();
	hero->changeState(new Landing2());
}



// LANDING
Landing::Landing()
{
}

Landing::~Landing()
{
}

void Landing::jump(BaseHero * hero)
{
	hero->doubleJump();
	hero->changeState(new DoupleJumping());
}

void Landing::run(BaseHero * hero)
{
	hero->run();
	hero->changeState(new Running());
}

void Landing::attack(BaseHero * hero)
{
	hero->attackLanding();
	//hero->changeState(new Attack());
}



// LANDING 2
Landing2::Landing2()
{
}

Landing2::~Landing2()
{
}

void Landing2::run(BaseHero * hero)
{
	hero->run();
	hero->changeState(new Running());
}

void Landing2::attack(BaseHero * hero)
{
	hero->attackLanding();
	//hero->changeState(new Attack());
}


// ATTACK
Attack::Attack()
{
}

Attack::~Attack()
{
}

void Attack::run(BaseHero * hero)
{
	hero->run();
	hero->changeState(new Running());
}

void Attack::jump(BaseHero * hero)
{
	hero->normalJump();
	hero->changeState(new Jumping());
}

void Attack::land(BaseHero * hero)
{
	hero->landing();
	hero->changeState(new Landing());
}