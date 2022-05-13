#ifndef Iterator_hpp
#define Iterator_hpp

template<typename T>
class Iterator
{

public:
  virtual ~Iterator() { }

  virtual bool IsDone() const = 0; // sprawdzanie czy dotraliśmy do końca

  virtual T& operator*() const = 0; // pobranie bieżącej wartości
  virtual void operator++() = 0; // przesunięcie się o jedno miejsce do przodu

};

#endif /* Iterator_hpp */
