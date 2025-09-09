#ifndef TRANSFER_H
#define TRANSFER_H

#include "AbstractOperation.h"

namespace bank2::extension::client {

class Transfer : public AbstractOperation {
public:
    Transfer(int serviceTime);
    bool isUrgent() const override;
    std::string toString() const override;
};

} // namespace bank2::extension::client

#endif // TRANSFER_H
