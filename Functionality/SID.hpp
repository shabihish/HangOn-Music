#ifndef AP_CA7_PHASE2_SID_HPP
#define AP_CA7_PHASE2_SID_HPP


#include "Member.hpp"

class SID {
private:
    Member *associatedMember= nullptr;
    string sid;
public:
    SID(Member* associatedMember, string sid);
    Member* getMember();
    string toString();
};
#endif
