//
// Created by Ilya on 1/19/2018.
//

#include "Clan.h"

using namespace std;

namespace mtm {

    Clan::Clan(const string &name) {
        if (name.empty()) {
            throw ClanEmptyName();
        }
        this->name = name;
        groups = MtmSet<GroupPointer>();
        friend_clans = MtmSet<std::string>();
    }

    Clan::Clan(const Clan &other) = default;

    Clan::~Clan() = default;

    void Clan::addGroup(const Group &group) {
        if (group.getSize() == 0) {
            throw ClanGroupIsEmpty();
        }
        if (doesContain(group.getName())) {
            throw ClanGroupNameAlreadyTaken();

        }
        Group new_group = group;
        new_group.changeClan(name);
        const GroupPointer new_pointer = GroupPointer(new Group(new_group));
        groups.insert(new_pointer);
    }

    const GroupPointer &Clan::getGroup(const std::string &group_name) const {
        //TODO: shouldn't we check if group_name==""?
        MtmSet<GroupPointer>::const_iterator it;
        GroupPointer temp;
        for (it = groups.begin(); it != groups.end(); it++) {
            temp = *it;
            if ((*temp).getName() == group_name) {
                return *it;
            }
        }
        throw ClanGroupNotFound();
    }

    bool Clan::doesContain(const std::string &group_name) const {
        MtmSet<GroupPointer>::const_iterator it;
        GroupPointer temp;
        for (it = groups.begin(); it != groups.end(); it++) {
            temp = *it;
            if ((*temp).getName() == group_name) {
                return true;
            }
        }
        return false;
    }

    int Clan::getSize() const {
        int people_amount = 0;
        MtmSet<GroupPointer>::const_iterator it;
        for (it = groups.begin(); it != groups.end(); it++) {
            people_amount += (*(*it)).getSize();
        }
        return people_amount;
    }

    void Clan::ChangeClan(Clan &clan, const std::string &new_name) {
        clan.name = new_name;
        for (auto it = clan.groups.begin(); it != clan.groups.end(); it++) {
            (*(*it)).changeClan(new_name);
        }
    }

    Clan &Clan::unite(Clan &other, const std::string &new_name) {
        if (new_name.empty()) throw ClanEmptyName();
        if (other.name == name) throw ClanCantUnite();
        for (auto it = other.groups.begin(); it != other.groups.end(); it++) {
            if (this->doesContain((*(*it)).getName())) {
                throw ClanCantUnite();
            }
        }
        friend_clans.unite(other.friend_clans);
        if (new_name == name) {
            ChangeClan(other, new_name);
        } else{
            ChangeClan(*this, new_name);
            ChangeClan(other, new_name);
        }
        GroupPointer temp= nullptr;
        groups.unite(other.groups);
        other.groups.clear();
        return *this;
    }

    GroupPointer Clan::getMax(MtmSet<mtm::GroupPointer> filtered_set){
        if (filtered_set.empty()) return nullptr;
        MtmSet<GroupPointer>::iterator it;
        GroupPointer maximum = *filtered_set.begin();
        for(it=filtered_set.begin();it!=filtered_set.end();it++){
            if((*(*it)) > *maximum) {
                maximum = *it;
            }
        }
        return maximum;
    }

    void Clan::makeFriend(Clan& other){
        if(this->isFriend(other)){
            return;
        }else{
            this->friend_clans.insert(other.name);
            other.friend_clans.insert(this->name);
        }
    }

    bool Clan::isFriend(const Clan& other) const{
        if(other.name == name) return true;
        return friend_clans.contains(other.name);
    }

    std::ostream &operator<<(std::ostream &os, const Clan &clan) {
        MtmSet<mtm::GroupPointer> filtered_set = clan.groups;
        MtmSet<GroupPointer>::iterator it2;
        os << "Clan's name: " << clan.name << "\nClan's groups:\n";
        GroupPointer temp2 = nullptr;
        do{
            temp2 = Clan::getMax(filtered_set);
            if(temp2 == nullptr) break;
            if(!(temp2->getName().empty())) {
                os << (*temp2).getName() << "\n";
            }
            filtered_set.erase(temp2);
        }while(temp2 != nullptr);
        return os;
    }
}