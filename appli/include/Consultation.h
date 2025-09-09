#ifndef CONSULTATION_H
#define CONSULTATION_H

#include "AbstractOperation.h"

namespace bank2::extension::client {

class Consultation : public AbstractOperation {
public:
    Consultation(int serviceTime);
    bool isUrgent() const override;
    std::string toString() const override;
};

} // namespace bank2::extension::client

#endif // CONSULTATION_H
