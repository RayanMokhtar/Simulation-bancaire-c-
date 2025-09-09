#ifndef WITHDRAW_H
#define WITHDRAW_H

#include "AbstractOperation.h"

namespace bank2::extension::client {

class Withdraw : public AbstractOperation {
public:
    Withdraw(int serviceTime);
    bool isUrgent() const override;
    std::string toString() const override;
};

} // namespace bank2::extension::client

#endif // WITHDRAW_H
