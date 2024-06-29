#ifndef EVENT_H
#define EVENT_H

enum EventTypeIndex
{
    None,
    KeyPress, KeyRelease,
    MouseMove, MouseClick,
    LevelEdit
};

#define DEFINE_EVENT_TYPE(type)                                                             \
static EventTypeIndex getStaticEventType() {return EventTypeIndex::type;}                   \
EventTypeIndex const getEventType() const override {return getStaticEventType();}           \


class Event
{
public:
    virtual const EventTypeIndex getEventType() const {return EventTypeIndex::None;}
    bool handled = false;
};


class KeyPressEvent : public Event
{
public:

    DEFINE_EVENT_TYPE(KeyPress)

    int key_;
    explicit KeyPressEvent(const int key) : key_(key) {};
};



class MouseMoveEvent : public Event
{
public:

    DEFINE_EVENT_TYPE(MouseMove)

    int x_, y_;
    MouseMoveEvent(const int x, const int y) : x_(x), y_(y) {};
};


#endif //EVENT_H
