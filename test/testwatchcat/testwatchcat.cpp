
#include <string>
#include <memory>
#include <chrono>

#include "testwatchcat.hpp"

using std::string;

TEST_F(TimerTest, RegisterEvent)
{
	unsigned int id = timer->RegisterEvent([this](unsigned int) {
								std::cout << "First event called" << std::endl;
							});

  EXPECT_EQ(id,0) << "First event shall have id equal to 0";
}

TEST_F(TimerTest, NoDefaultActivate)
{
	auto timeout = std::chrono::seconds(2);
	unsigned int id = timer->RegisterEvent([this](unsigned int)
	    {
				std::cout << "Event called" << std::endl;
	    }, timeout);

  EXPECT_EQ(timer->IsActivated(id),false) <<
  		"By-default event should be deactivated";
}

TEST_F(TimerTest, IsExecutedNonActiveEventBeforeTimeout)
{
	auto timeout = std::chrono::seconds(2);
	unsigned int id = timer->RegisterEvent([this](unsigned int)
	    {
				std::cout << "Event called" << std::endl;
	    }, timeout);

  EXPECT_EQ(timer->IsExecuted(id),false) <<
  		"Non active event shall not be executed before timeout";
}

TEST_F(TimerTest, IsExecutedNonActiveEventAfterTimeout)
{
	auto timeout = std::chrono::seconds(2);
	unsigned int id = timer->RegisterEvent([this](unsigned int)
	    {
				std::cout << "First event called" << std::endl;
	    }, timeout);

	std::this_thread::sleep_for(std::chrono::seconds(3));

  EXPECT_EQ(timer->IsExecuted(id),false) <<
  		"Non active event shall not be executed after timeout";
}

TEST_F(TimerTest, IsExecutedActiveEventBeforeTimeout)
{
	int timeout {2};
	unsigned int id = timer->RegisterEvent([this](unsigned int)
	    {
				std::cout << "First event called" << std::endl;
	    });

	timer->ActivateEvent(id, timeout);
	EXPECT_EQ(timer->IsExecuted(id),false) <<
			"Active event shall not be executed before timeout";
}

TEST_F(TimerTest, IsExecutedActiveEventAfterTimeout)
{
	int timeout {2};
	unsigned int id = timer->RegisterEvent([this](unsigned int)
	    {
				std::cout << "First event called" << std::endl;
	    });

	timer->ActivateEvent(id, timeout);
	std::this_thread::sleep_for(std::chrono::seconds(3));
  EXPECT_EQ(timer->IsExecuted(id),true) <<
  		"Active event shall be executed after timeout";
}

TEST_F(TimerTest, IsExecutedMultipleSameTimeActiveEventAfterTimeout)
{
	int timeout {2};
	unsigned int id_1 = timer->RegisterEvent([this](unsigned int)
	    {
				std::cout << "First event called" << std::endl;
	    });

	unsigned int id_2 = timer->RegisterEvent([this](unsigned int)
	    {
				std::cout << "Second event called" << std::endl;
	    });

	timer->ActivateEvent(id_1, timeout);
	timer->ActivateEvent(id_2, timeout);
	std::this_thread::sleep_for(std::chrono::seconds(3));
  EXPECT_EQ(timer->IsExecuted(id_1) && timer->IsExecuted(id_2),true) <<
  		"Active event shall not be executed after timeout";
}

TEST_F(TimerTest, IsExecutedMultipleSameTimeActiveEventBeforeTimeout)
{
	int timeout {2};
	unsigned int id_1 = timer->RegisterEvent([this](unsigned int)
	    {
				std::cout << "First event called" << std::endl;
	    });

	unsigned int id_2 = timer->RegisterEvent([this](unsigned int)
	    {
				std::cout << "Second event called" << std::endl;
	    });

	timer->ActivateEvent(id_1, timeout);
	timer->ActivateEvent(id_2, timeout);
  EXPECT_EQ(timer->IsExecuted(id_1) || timer->IsExecuted(id_2),false) <<
  		"Any active event shall not be executed before timeout";
}


TEST_F(TimerTest, DeactivateMultipleSameTimeActiveEventBeforeTimeout)
{
	int timeout {2};
	unsigned int id_1 = timer->RegisterEvent([this](unsigned int)
	    {
				std::cout << "First event called" << std::endl;
	    });

	unsigned int id_2 = timer->RegisterEvent([this](unsigned int)
	    {
				std::cout << "Second event called" << std::endl;
	    });

	timer->ActivateEvent(id_1, timeout);
	timer->ActivateEvent(id_2, timeout);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	timer->DeactivateAllEvent();
  EXPECT_EQ(timer->IsExecuted(id_1) || timer->IsExecuted(id_2),false) <<
  		"Any active event shall not be executed if deactivated before timeout";
}

TEST_F(TimerTest, DeactivateMultipleSameTimeActiveEventAfterTimeout)
{
	int timeout {2};
	unsigned int id_1 = timer->RegisterEvent([this](unsigned int)
	    {
				std::cout << "First event called" << std::endl;
	    });

	unsigned int id_2 = timer->RegisterEvent([this](unsigned int)
	    {
				std::cout << "Second event called" << std::endl;
	    });

	timer->ActivateEvent(id_1, timeout);
	timer->ActivateEvent(id_2, timeout);
	std::this_thread::sleep_for(std::chrono::seconds(3));
	timer->DeactivateAllEvent();
  EXPECT_EQ(timer->IsExecuted(id_1) || timer->IsExecuted(id_2),false) <<
  		"Any active event shall be executed if deactivated after timeout";
}

TEST_F(TimerTest, DeactivateOneSameTimeActiveEventBeforeTimeout)
{
	int timeout {2};
	unsigned int id_1 = timer->RegisterEvent([this](unsigned int)
	    {
				std::cout << "First event called" << std::endl;
	    });

	unsigned int id_2 = timer->RegisterEvent([this](unsigned int)
	    {
				std::cout << "Second event called" << std::endl;
	    });

	timer->ActivateEvent(id_1, timeout);
	timer->ActivateEvent(id_2, timeout);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	timer->DeactiveEvent(id_2);
  EXPECT_EQ(timer->IsActivated(id_1) ^ timer->IsActivated(id_2),true) <<
  		"A particular active event shall be deactivated if deactivated before timeout";

  EXPECT_EQ(timer->IsActivated(id_1),true) <<
  		"A event_1 shall be activated if not deactivated before timeout";

  EXPECT_EQ(timer->IsActivated(id_2),false) <<
  		"A event_2 shall be deactivated if deactivated before timeout";

}

TEST_F(TimerTest, MultipleActivationOfSameEvent)
{
  int timeout {2};
  int eventCallCounter = 0;

  unsigned int event_id = timer->RegisterEvent([this, &eventCallCounter](unsigned int)
  {
    eventCallCounter++;
    std::cout << "The event was called " << eventCallCounter << " times" << std::endl;
  });

  timer->ActivateEvent(event_id, timeout);
  timer->ActivateEvent(event_id, timeout);
  timer->ActivateEvent(event_id, timeout);

  std::this_thread::sleep_for(std::chrono::seconds(3));

  EXPECT_EQ(eventCallCounter, 1) <<
                "The event should be called once after reactivation";
}