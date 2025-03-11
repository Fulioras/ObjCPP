// Created by Augustinas Bickaitis on 2025-02-24.

#include <cassert>
#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <vector>
#include <stdexcept>

using namespace std;


string name_formatter(string name){
  for(int i = 0; i < name.length(); i++) {
      if(name[i] >= 'a' && name[i] <= 'z') {
        name[i] = toupper(name[i]);
      }
      else if(name[i] < 'A' || name[i] > 'Z') {
        throw invalid_argument("Invalid simbols in prisoners name/surname");
      }
    }
  return name;
}


class Prisoner{

private:
    string name;
    string surname;
    int daysLeft = -1;

    int id;
    static int lastId ;
    static int amountOfPrisoners;

public:
  /// CONSTRUCTOR
  Prisoner (string name, string surname, int days_left) : id(++lastId) {
    set_name(name);
    set_surname(surname);
    set_days_left(daysLeft);
    amountOfPrisoners++;
  }
  Prisoner(string name, string surname) : id(++lastId) {
    set_name(name);
    set_surname(surname);
    this->daysLeft = -1; /// an exeption
    amountOfPrisoners++;
  }
  /// DESTRUCTOR
  ~Prisoner() {
    amountOfPrisoners--;
  }
  /// GETTERS
      string get_name(){
        return this->name;
      }
      string get_surname(){
        return this->surname;
      }
      int get_days_left(){
        return this->daysLeft;
      }

      int get_id() {
        return this->id;
      }

      static int get_prisoner_amount() {
        return amountOfPrisoners;
      }


  /// SETTERS
      void set_name(string name){
        this->name = name_formatter(name);
      }

      void set_surname(string surname){
        this->surname = name_formatter(surname);
      }



      void change_days_left( int amount){
        this->daysLeft += amount;
        if (this->daysLeft < 0) {
          this->daysLeft = 0;
        }
      }

      void set_days_left(int amount) {
        if(amount >= 0)
          this->daysLeft = amount;
        else 
          throw out_of_range("Can't set days left to a negative amount");
      }
  /// OTHERS
      string to_string() {

        stringstream ss;
        ss << this->get_name() << ";" << this->get_surname() << ";" << this->get_days_left()<< ";" << this->get_id() << endl;
        string info = ss.str();
        return info;
      }
};

int Prisoner::lastId = 0;
int Prisoner::amountOfPrisoners = 0;

int main() {
  
  try{
    Prisoner myBoy("A", "B", 5);
    Prisoner myPrisoner("AA", "BB");

    cout << myBoy.to_string() << myBoy.get_days_left() << endl;
    cout << myPrisoner.get_id() << endl;

    myBoy.change_days_left(-61); ///checking setters
    assert(myBoy.get_days_left() == 0);
    myBoy.change_days_left(61);
    assert(myBoy.get_days_left() == 61);

    myBoy.set_days_left(-5);/// shows error message

    assert(Prisoner::get_prisoner_amount() == 2); //checking static
    

    vector<Prisoner*> brothers;
    brothers.push_back(new Prisoner( "name", "surname", 7));
    brothers.push_back(new Prisoner( "vardas", "pavarde"));

    assert(Prisoner::get_prisoner_amount() == brothers[1]->get_id()); /// Created 4 objects and id is also 4

    for(auto prisoner : brothers) {
      delete prisoner;
    }
    assert(Prisoner::get_prisoner_amount() == 2);

    brothers.push_back(new Prisoner( "1nom", "prenom"));
    assert(brothers[2]->get_id() == 5);
    delete brothers[2];
    brothers.clear();
  }
  catch(invalid_argument& e){
    cerr << e.what() << endl;
  }
  catch(out_of_range& e){
    cerr << e.what() << endl;
  }
  assert(Prisoner::get_prisoner_amount() == 0);

return 0;
}
