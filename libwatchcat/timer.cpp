
#include "timer.hpp"

using namespace watchcat;

void Timer::Run()
{
  std::unique_lock<std::mutex> lock(m_mutex);

  while (m_isTimerActive)
  {
    if (m_activeTimeEventSet.empty())
    {
      m_condition.wait(lock);
    }
    else
    {
      TimeEvent te = *m_activeTimeEventSet.begin();
      if (std::chrono::steady_clock::now() >= te.nextTimepoint)
      {
        m_activeTimeEventSet.erase(m_activeTimeEventSet.begin());
        lock.unlock();        
        m_eventsList[te.eventID].function(te.eventID);
        lock.lock();
        m_eventsList[te.eventID].isExecuted = true;
        if (m_eventsList[te.eventID].isActive && m_eventsList[te.eventID].isRepeated)
        {
          te.nextTimepoint += m_eventsList[te.eventID].timeout;
          m_activeTimeEventSet.insert(te);
        }
        else
        {
          m_eventsList[te.eventID].isActive = false;
        }
      }
      else
      {
        m_condition.wait_until(lock, te.nextTimepoint);
      }
    }
  }
}



Timer::Timer()
{
  std::unique_lock<std::mutex> lock(m_mutex);
  m_workerThread = std::thread([this]
  {
    Run();
  });
}

Timer::~Timer()
{
  std::unique_lock<std::mutex> lock(m_mutex);
  lock.unlock();
  m_isTimerActive = false;
  m_condition.notify_all();
  m_workerThread.join();
  m_eventsList.clear();
  m_activeTimeEventSet.clear();
  while (!m_freeEventIds.empty())
  {
    m_freeEventIds.pop();
  }
}

unsigned int Timer::RegisterEvent(std::function<void(unsigned int p_id)>&& p_function,
                                  std::chrono::seconds timeout,
                                  bool isRepeated)
{
  unsigned int id;
  std::unique_lock<std::mutex> lock(m_mutex);
  if (m_freeEventIds.empty())
  {
    id = m_eventsList.size();
    Event e(id,
            timeout,
            std::move(p_function),
            isRepeated);

    m_eventsList.push_back(std::move(e));
  }
  else
  {
    id = m_freeEventIds.top();
    Event e(id,
            timeout,
            std::move(p_function),
            isRepeated);
    m_freeEventIds.pop();
    m_eventsList[id] = std::move(e);
  }

  lock.unlock();
  m_condition.notify_all();
  return id;
}


bool Timer::ActivateEvent(unsigned int id, int timeout)
{
  std::unique_lock<std::mutex> lock(m_mutex);

  if (m_eventsList.size() == 0 || m_eventsList.size() < id)
  {
    return false;
  }
  if(timeout)
  {
    m_eventsList[id].timeout = std::chrono::seconds(timeout);
  }

  if (m_eventsList[id].timeout > std::chrono::seconds::zero())
  {
    m_eventsList[id].isActive = true;
    m_eventsList[id].isExecuted = false;
    m_eventsList[id].startTimepoint = std::chrono::steady_clock::now();

    auto it = std::find_if(m_activeTimeEventSet.begin(), m_activeTimeEventSet.end(),
                           [&](const TimeEvent & te)
    {
      return te.eventID == id;
    });

    if (it != m_activeTimeEventSet.end())
    {
      m_activeTimeEventSet.erase(it);
    }

    m_activeTimeEventSet.insert(TimeEvent {m_eventsList[id].startTimepoint + std::chrono::duration_cast<std::chrono::seconds>(m_eventsList[id].timeout),
                                           id
                                          });
  }

  lock.unlock();
  m_condition.notify_all();
  return true;
}


bool Timer::IsActivated(unsigned int id) const
{
  return m_eventsList[id].isActive;
}



bool Timer::DeactiveEvent(unsigned int id)
{
  std::unique_lock<std::mutex> lock(m_mutex);

  if (m_eventsList.size() == 0 || m_eventsList.size() < id)
  {
    return false;
  }
  m_eventsList[id].isActive = false;
  auto it = std::find_if(m_activeTimeEventSet.begin(), m_activeTimeEventSet.end(),
                         [&](const TimeEvent & te)
  {
    return te.eventID == id;
  });

  if (it != m_activeTimeEventSet.end())
  {
    m_activeTimeEventSet.erase(it);
  }

  lock.unlock();
  m_condition.notify_all();
  return true;
}


bool Timer::RemoveEvent(unsigned int id)
{
  std::unique_lock<std::mutex> lock(m_mutex);
  if (m_eventsList.size() == 0 || m_eventsList.size() < id)
  {
    return false;
  }
  m_eventsList[id].isActive = false;
  auto it = std::find_if(m_activeTimeEventSet.begin(), m_activeTimeEventSet.end(),
                         [&](const TimeEvent & te)
  {
    return te.eventID == id;
  });

  if (it != m_activeTimeEventSet.end())
  {
    m_freeEventIds.push(it->eventID);
    m_activeTimeEventSet.erase(it);
//  Note: Do not erase from eventsList, else the other ids becomes invalid
  }
  lock.unlock();
  m_condition.notify_all();
  return true;
}

bool Timer::IsExecuted(unsigned int id) const
{
  return m_eventsList[id].isExecuted;
}


bool Timer::DeactivateAllEvent()
{
  std::unique_lock<std::mutex> lock(m_mutex);
  if (m_eventsList.size() == 0)
  {
    return true;
  }

  for (unsigned int i = 0; i < m_eventsList.size(); ++i)
  {
    m_eventsList[i].isActive = false;
    m_eventsList[i].isExecuted = false;
    m_freeEventIds.push(m_eventsList[i].id);
  }

  m_activeTimeEventSet.erase(m_activeTimeEventSet.begin(), m_activeTimeEventSet.end());

  lock.unlock();
  m_condition.notify_all();
  return true;
}
