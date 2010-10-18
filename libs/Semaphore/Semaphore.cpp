#include "Semaphore.h"

void Semaphore::error_check(int retv) {
    if (retv == -1) {
		char buf[256];
#ifdef __GNU
		char *msg_ptr;
		msg_ptr = ::strerror_r(errno, buf, sizeof(buf));
        throw SemaphoreException(msg_ptr);
#else
		::strerror_r(errno, buf, sizeof(buf));
		throw SemaphoreException(buf);
#endif
    }
}

Semaphore::Semaphore(const Semaphore & other) {
}

Semaphore& Semaphore::operator=(const Semaphore & other) {
}

Semaphore::Semaphore(int value, bool pshared) {
    error_check(sem_init(&_sid, pshared == false ? 0 : 1, value));
}

Semaphore::~Semaphore() {
    sem_destroy(&_sid);
}

int Semaphore::getvalue(){
    int val;
    error_check(sem_getvalue(&_sid, &val));
    return val;
}

void Semaphore::post() {
    error_check(sem_post(&_sid));
}

void Semaphore::wait() {
    error_check(sem_wait(&_sid));
}

Semaphore& Semaphore::operator++() {
    this->post();
}

Semaphore& Semaphore::operator++(int) {
    this->post();
}

Semaphore& Semaphore::operator--() {
    this->wait();
}

Semaphore& Semaphore::operator--(int) {
    this->wait();
}
