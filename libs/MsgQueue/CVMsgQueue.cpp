#include <list>

#include "SemMsgQueue.h"

SemMsgQueue::~SemMsgQueue() {
}

int SemMsgQueue::get(char *buf, size_t bufsize) {

    _taken--;
    _mutex--;

    if (dropped) {
        _taken++;
        _mutex++;
        return 0;
    }

    std::string str = _msgs.back();
    _msgs.pop_back();

    const char *s = str.c_str();
    int str_length = str.length();

    int i;
    for (i = 0; (i < bufsize) && (i < str_length); ++i) {
        buf[i] = s[i];
    }

    _mutex++;
    _free++;
    return i;
}

int SemMsgQueue::put(const char *msg) {
    _free--;
    _mutex--;

    if (dropped) {
        _free++;
        _mutex++;
        return 0;
    }

    std::string str(msg);
    _msgs.push_front(str);

    _mutex++;
    _taken++;

    return str.length();
}

void SemMsgQueue::drop() {
    dropped = true;

    _mutex--;
	// allowing all threads waiting for semaphores to unlock
    _free++;
    _taken++;

    _msgs.erase(_msgs.begin(), _msgs.end());

    _mutex++;
}

