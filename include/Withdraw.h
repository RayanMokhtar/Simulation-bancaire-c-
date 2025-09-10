#ifndef WITHDRAW_H
#define WITHDRAW_H

#include "AbstractOperation.h"

class Withdraw : public AbstractOperation {
public:
    Withdraw(int serviceTime);
    
    bool isUrgent() const override { return true; }
    std::string toString() const override { return "Operation : Withdraw"; }
};

#endif
