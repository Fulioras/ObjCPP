// Created by Augustinas Bickaitis on 2025-02-24.

#include <assert.h>
#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <vector>

class Prisoner{

  private:
    std::string name;
    std::string surname;
    int daysLeft = -1;

    int id;
    static int lastId ;
    static int amountOfPrisoners;
public:
  /// CONSTRUCTOR
  Prisoner (std::string name, std::string surname, int days_left) : id(++lastId) {
    this->name = name_Format(name);
    this->surname = name_Format(surname);
    this->daysLeft = days_left;
    amountOfPrisoners++;
  }
  Prisoner(std::string name, std::string surname) : id(++lastId) {
    this->name = name_Format(name);
    this->surname = name_Format(surname);
    this->daysLeft = -1;
    amountOfPrisoners++;
  }
  /// DESTRUCTOR
  ~Prisoner() {
    std::cout << this->to_String();
    amountOfPrisoners--;
  }
private:
 /// METHODS
  std::string name_Format(std:: string name) {
    try {
      for(int i = 0; i < name.length(); i++) {
        if(name[i] >= 'a' && name[i] <= 'z') {
          name[i] = std::toupper(name[i]);
        }
        else if(name[i] < 'A' || name[i] > 'Z') {
          throw "Error";
        }
      }
      return name;
    }
    catch (...) {
      std::cerr<<"Name/Surname can only consist of latin letters" <<std::endl;

    }
    return name;
  }

  public:
  /// GETTERS
      std::string get_Name(){
        return name;
      }
      std::string get_Surname(){
        return surname;
      }
      int get_Days_Left(){
        return daysLeft;
      }

      int get_id() {
        return this->id;
      }

      static int get_Prisoner_amount() {
        return amountOfPrisoners;
      }


  /// SETTERS
      void change_Days_Left( int amount){
        this->daysLeft += amount;
        if (this->daysLeft < 0) {
          this->daysLeft = 0;
        }
      }

      void set_Days_Left(int amount) {
        try {
          if(amount < 0)
            throw "Error";

          this->daysLeft = amount;

        } catch (...) {
          std::cerr<<"Days left must be positive or equal to zero"<<std::endl;
        }
      }
  /// OTHERS
      std::string to_String() {

        std::stringstream ss;
        ss << this->get_Name() << ";" << this->get_Surname() << ";" << this->get_Days_Left()<< ";" << this->get_id() << std::endl;
        std::string ass = ss.str();
        return ass;
      }
};

int Prisoner::lastId = 0;
int Prisoner::amountOfPrisoners = 0;

int main() {
  Prisoner myBoy("A", "B", 5);\
  Prisoner myPrisoner("AA", "BB");

  std::cout << myBoy.to_String() << myBoy.get_Days_Left() << std::endl;
  std::cout << myPrisoner.get_id() << std::endl;

  myBoy.change_Days_Left(-61); ///checking setters
  assert(myBoy.get_Days_Left() == 0);
  myBoy.change_Days_Left(61);
  assert(myBoy.get_Days_Left() == 61);

  myBoy.set_Days_Left(-5);/// shows error message

  assert(Prisoner::get_Prisoner_amount() == 2); //checking static

  std::vector<Prisoner*> brothers;
  brothers.push_back(new Prisoner( "name", "surname", 7));
  brothers.push_back(new Prisoner( "vardas", "pavarde"));

  assert(Prisoner::get_Prisoner_amount() == brothers[1]->get_id()); /// Created 4 objects and id is also 4

  for(auto prisoner : brothers) {
    delete prisoner;
  }
  assert(Prisoner::get_Prisoner_amount() == 2);

  brothers.push_back(new Prisoner( "nom", "prenom"));
  assert(brothers[0]->get_id() == 5);
  delete brothers[0];
  brothers.clear();


return 0;
}
