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

    Ad::Ad():
      id_(0),
      car_(nullptr),
      price_(0),
      mileage_(0),
      ownerCount_(0),
      postedDate_(""),
      isActive_(false)
    {}

    Ad::Ad(Car* car, const std::string& line)
    {
      size_t pos = 0;
      
      id_ = std::stoull(nextWord(line, pos));
      car_ = car;
      price_ = std::stoull(nextWord(line, pos));
      mileage_ = std::stoull(nextWord(line, pos));
      ownerCount_ = std::stoull(nextWord(line, pos));
      postedDate_ = nextWord(line, pos);
      isActive_ = true;
    }

    std::string Ad::getId() const
    {
      return formatId(id_);
    }

    const Car& Ad::getCar() const
    {
      return *car_;
    }

    size_t Ad::getPrice() const
    {
      return price_;
    }

    size_t Ad::getMileage() const
    {
      return mileage_;
    }

    size_t Ad::getOwnerCount() const
    {
      return ownerCount_;
    }

    std::string Ad::getPostedDate() const
    {
      return postedDate_;
    }

    bool Ad::isActive() const
    {
      return isActive_;
    }

    void Ad::setId(size_t id)
    {
      id_ = id;
    }

    void Ad::setCar(Car* car)
    {
      car_ = car;
    }

    void Ad::setPrice(size_t price)
    {
      price_ = price;
    }

    void Ad::setMileage(size_t mileage)
    {
      mileage_ = mileage;
    }

    void Ad::setOwnerCount(size_t count)
    {
      ownerCount_ = count;
    }

    void Ad::setPostedDate(const std::string& date)
    {
      postedDate_ = date;
    }

    void Ad::setActive(bool active)
    {
      isActive_ = active;
    }

    std::string Ad::formatId(size_t id)
    {
      std::ostringstream oss;
      oss << std::setw(4) << std::setfill('0') << id;
      return oss.str();
    }

  }

  #endif
