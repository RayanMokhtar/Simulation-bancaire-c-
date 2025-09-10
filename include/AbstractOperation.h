#ifndef ABSTRACT_OPERATION_H
#define ABSTRACT_OPERATION_H

#include <string>

class AbstractOperation {
private:
    int serviceTime;

public:
    AbstractOperation(int serviceTime);
    virtual ~AbstractOperation() = default;
    
    int getServiceTime() const { return serviceTime; }
    virtual bool isUrgent() const = 0;
    virtual std::string toString() const = 0;
};

#endif
