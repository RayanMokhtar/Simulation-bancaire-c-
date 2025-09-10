#ifndef CONSULTATION_H
#define CONSULTATION_H

#include "AbstractOperation.h"

class Consultation : public AbstractOperation {
public:
    Consultation(int serviceTime);
    
    bool isUrgent() const override { return false; }
    std::string toString() const override { return "Operation : Consultation"; }
};

#endif
