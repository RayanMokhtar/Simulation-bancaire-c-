#include "Consultation.h"

namespace bank2::extension::client {

Consultation::Consultation(int serviceTime) : AbstractOperation(serviceTime) {}

bool Consultation::isUrgent() const { return false; }

std::string Consultation::toString() const { return "Operation: Consultation"; }

} // namespace bank2::extension::client
