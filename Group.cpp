//
// Created by Omer on 18/01/2018.
//

#include "Group.h"
#include "exceptions.h"
#include <cmath>

using namespace mtm;

Group::Group(const std::string &name, const std::string &clan, int children,
             int adults, int tools, int food, int morale) {
    if (name.empty()) throw GroupInvalidArgs();
    if (adults == 0 && children == 0) throw GroupInvalidArgs();
    if (adults < 0 || children < 0 || tools < 0 || food < 0) {
        throw GroupInvalidArgs();
    }
    if (morale < 0 || morale > 100) throw GroupInvalidArgs();

    this->name = name;
    this->clan = clan;
    this->children = children;
    this->adults = adults;
    this->tools = tools;
    this->food = food;
    this->morale = morale;
}

Group::Group(const std::string &name, int children, int adults) {
    if (name.empty()) throw GroupInvalidArgs();
    if (adults == 0 && children == 0) throw GroupInvalidArgs();
    if (adults < 0 || children < 0) throw GroupInvalidArgs();

    this->name = name;
    this->clan = "";
    this->children = children;
    this->adults = adults;
    this->tools = 4 * adults;
    this->food = 3 * adults + 2 * children;
    this->morale = 70;
}

Group::Group(const Group &other) = default;

Group::~Group() = default;

const std::string &Group::getName() const {
    return name;
}

int Group::getSize() const {
    return adults + children;
}

const std::string &Group::getClan() const {
    return clan;
}

void Group::changeClan(const std::string &new_clan) {
    if (clan == new_clan) return;

    double temp;
    if (clan.empty()) {
        if (new_clan.empty()) return;
        clan = new_clan;
        temp = morale + morale * 0.1;

    } else {
        clan = new_clan;
        temp = morale - morale * 0.1;
    }

    if (temp > 100) temp = 100.0;
    morale = (int) temp;
}

/*
int Group::calcPower(Group& rhs){
    return int(floor(((10 * rhs.adults + 3 * rhs.children) *
                       (10 * rhs.tools + rhs.food) * (rhs.morale)) / 100));
}
*/


bool Group::operator<(const Group& rhs) const{
    int rhs_power, this_power;
    rhs_power = int(floor(((10 * rhs.adults + 3 * rhs.children) *
                       (10 * rhs.tools + rhs.food) * (rhs.morale)) / 100));
    this_power = int(floor(((10 * adults + 3 * children) *
                        (10 * tools + food) * (morale)) / 100));
    if (rhs_power > this_power) {
        return true;
    } else if (this_power > rhs_power) {
        return false;
    } else {
        if(name < rhs.name) {
            return true;
        }else{
            return false;
        }
    }
}
/*bool Group::operator==(const Group& rhs) const{

}*/
/*
bool Group::operator>(const Group& rhs) const{
    if(!(this < rhs)){

    }
}*/