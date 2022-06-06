#include "SID.hpp"

SID::SID(Member *associatedMember, string sid) {
    this->sid = sid;
    this->associatedMember = associatedMember;
}

Member *SID::getMember() {
    return associatedMember;
}

string SID::toString() {
    return sid;
}

