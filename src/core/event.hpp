#pragma once

enum class EventID
{
	// window events
	WIN_CLOSE, WIN_RESIZE, KEY, CHAR, MOUSE_BUTTON, SCROLL, CURSOR
};

// message sent
struct Event
{
	virtual EventID getId() const = 0;
};

#define EVENT_ID(x) EventID getId() const override { return EventID::x; }

// receives events
struct Observer
{
	friend class Sender;
	virtual ~Observer() = default;
protected:
        virtual void onEvent(const std::unique_ptr<Event>&) = 0;
};

// sends events to a list of observers instantly
class Sender
{
        std::vector<Observer*> m_obs;

public:
        template<typename T, typename... Args>
        void send(Args&&... args) 
	{
		auto e = std::unique_ptr<Event>(new T(std::forward<Args>(args)...));
		for (auto* o : m_obs)
			o->onEvent(e);
	}
    
	void addObserver(Observer* o)    { m_obs.push_back(o); }
        void removeObserver(Observer* o) { m_obs.erase(std::remove(m_obs.begin(), m_obs.end(), o), m_obs.end()); }
};
