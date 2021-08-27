#pragma once

#include <functional>
#include <chrono>
#include <vector>
#include <utility>
#include <set>
#include <stack>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <algorithm>

namespace watchcat
{

  struct Event
  {
    unsigned int id{0};
    std::chrono::time_point<std::chrono::steady_clock> startTimepoint;
    std::chrono::seconds timeout{std::chrono::seconds::zero()};
    std::function<void(unsigned int id)> function{nullptr}; // handler
    bool isRepeated{false};
    bool isActive{false};
    bool isExecuted{false};

    Event(unsigned int p_id,
          std::chrono::seconds p_timeout,
          std::function<void(unsigned int p_id)> &&p_function, bool p_isRepeated)
        : id(p_id),
          timeout(p_timeout),
          function(p_function),
          isRepeated(p_isRepeated)
    {
    }
  };

  struct TimeEvent
  {
    std::chrono::time_point<std::chrono::steady_clock> nextTimepoint;
    unsigned int eventID;
  };

  struct Comparator
  {
    bool operator()(const TimeEvent &l, const TimeEvent &r)
    {
      return l.nextTimepoint < r.nextTimepoint;
    }
  };

  class Timer
  {

  public:
    Timer();
    virtual ~Timer();

    unsigned int RegisterEvent(std::function<void(unsigned int p_id)> &&p_function,
                               std::chrono::seconds timeout = std::chrono::seconds::zero(),
                               bool isRepeated = false);
    bool RemoveEvent(unsigned int id);
    bool ActivateEvent(unsigned int id, int timeout = 0);
    bool IsActivated(unsigned int id) const;
    bool DeactiveEvent(unsigned int id);
    bool DeactivateAllEvent();
    bool IsExecuted(unsigned int id) const;

  private:
    void Run();

    std::mutex m_mutex{};
    std::condition_variable m_condition{};
    std::thread m_workerThread{};
    bool m_isTimerActive{true};

    std::vector<Event> m_eventsList{};
    std::set<TimeEvent, Comparator> m_activeTimeEventSet{}; //Auto ordered due to set
    std::stack<unsigned int> m_freeEventIds{};
  };

}