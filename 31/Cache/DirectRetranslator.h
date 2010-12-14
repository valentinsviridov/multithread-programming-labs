#pragma once

#include "Retranslator.h"
#include "RetranslatorState.h"
class DirectRetranslator : public RetranslatorState {
public:
    RetranslatorState *instance();
    void client_finished();
    void download_add_data(const Buffer *b);
    void download_finished();
    void download_connect_failed();
    void download_send_failed();
    void download_recv_failed();
};