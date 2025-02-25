// Created by Augustinas Bickaitis on 2025-02-24.

#include <iostream>
#include <string>
#include <sstream>

class Prisoner{

  private:
    std::string name;
    std::string surname;
    int daysLeft = -1;

    static int id;
    static int amountOfPrisoners;

  std::string name_Format(std:: string name) {
    try {
      for(int i = 0; i < name.length(); i++) {
        if(name[i] >= 'a' && name[i] <= 'z') {
          name[i] = name[i] - 32;
        }
        else if(name[i] <= 'A' || name[i] >= 'Z') {
          throw "Error";
        }
      }
      return name;
    }
    catch (...) {
      std::cout<<"Name/Surname can only consist of latin letters"<<std::endl;
    }
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

      static int get_id() {
        return id;
      }

  /// SETTERS
      void change_Days_Left( int amount){
        this->daysLeft -= amount;
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
          std::cout<<"Days left must be positive or equal to zero"<<std::endl;
          return;
        }
      }
  /// OTHERS
      void to_String() {
        std::cout << this->get_Name() << ";" << this->get_Surname() << ";" << this->get_Days_Left()<< std::endl;
      }

      void new_Prisoner(std::string name, std::string surname, int days_left){

        this->name = name_Format(name);
        this->surname = name_Format(surname);
        this->daysLeft = days_left;
      }

      ~Prisoner() {
        this->to_String();
      }

    private:
      void main() {

      }

};
int Prisoner::id = 1;
int Prisoner::amountOfPrisoners = 0;

int main(){

  std::cout<<"Hello World!"<<std::endl;
  Prisoner myBoy;
  myBoy.new_Prisoner("Johnny","Sins",100);
  myBoy.to_String();

  Prisoner theBoys[8];
  delete theBoys;




return 0;
}
