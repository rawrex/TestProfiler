#ifndef FACTORY_H
#define FACTORY_H

#include <memory>
#include "Classes.h"
#include "Functions.h"
	
// Global scoped enumeration with all possible object types
enum class Type { A, B, C, D, end };

class Factory
{
	Type randomType() const
	{
		auto random_number = makeRandomNumber(0, 4);
		return static_cast<Type>(random_number % static_cast<long long>(Type::end));
	}

public:
	std::unique_ptr<Base> Create(const Type& type) const
	{
		if(type == Type::A)
			return std::unique_ptr<Base>(new A());
		if(type == Type::B)
			return std::unique_ptr<Base>(new B());
		if(type == Type::C)
			return std::unique_ptr<Base>(new C());
		if(type == Type::D)
			return std::unique_ptr<Base>(new D());
		throw std::exception();
	}
	std::unique_ptr<Base> CreateRandom() const
	{
		return Create(randomType());
	}
};

#endif
