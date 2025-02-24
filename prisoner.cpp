// Created by Augustinas Bickaitis on 2025-02-24.

#include <iostream>
#include <string>


class prisoner{

  private:
    std::string name;
    std::string surname;
    int daysLeft;

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
  /// SETTERS
      void new_Prisoner(std::string name, std::string surname, int days_left) {
        this->name = name;
        this->surname = surname;
        this->daysLeft = days_left;
      }
      void change_Days_Left( int amount) {
        this->daysLeft -= amount;
        if (this->daysLeft < 0) {
          this->daysLeft = 0;
        }
      }
  /// OTHERS
      void display() {
        std::cout << "Name: " << this->get_Name() << std::endl;
        std::cout << "Surname: " << this->get_Surname() << std::endl;
        std::cout << "Days until free: " << this->get_Days_Left() << std::endl;
      }
};

int main(){

  std::cout<<"Hello World!"<<std::endl;
  prisoner myBoy;
  myBoy.new_Prisoner("Johnny","sins",100);
  myBoy.display();


return 0;
}
