#ifndef TRANSFER_H
#define TRANSFER_H

#include "client/AbstractOperation.h"

class Transfer : public AbstractOperation {
public:
    Transfer(int serviceTime);
    
    bool isUrgent() const override { return true; }
    std::string toString() const override { return "Operation : Transfer"; }
};

#endif
