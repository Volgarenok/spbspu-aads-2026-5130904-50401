  #ifndef AD_HPP
  #define AD_HPP

  #include <string>
  #include <iostream>
  #include <sstream>
  #include <iomanip>

  #include "Car.hpp"
  #include "helpedFunc.hpp"

  namespace donkeev
  {
    class Ad
    {
    public:
      Ad();
      explicit Ad(Car* car, const std::string& line);
      
      std::string getId() const;
      const Car& getCar() const;
      size_t getPrice() const;
      size_t getMileage() const;
      size_t getOwnerCount() const;
      std::string getPostedDate() const;
      bool isActive() const;
      
      void setId(size_t);
      void setCar(Car*);
      void setPrice(size_t);
      void setMileage(size_t);
      void setOwnerCount(size_t);
      void setPostedDate(const std::string&);
      void setActive(bool);
        
    private:
      size_t id_;
      Car* car_;
      size_t price_;
      size_t mileage_;
      size_t ownerCount_;
      std::string postedDate_;
      bool isActive_;

      static std::string formatId(size_t id);
    };
  }

  #endif
