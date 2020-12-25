#include <fstream>
#include <iostream>
#include <iterator>
#include <vector>
#include <tuple>
#include <map>
#include <string>
#include <iomanip>

// sequence for
template <typename T, T... S, typename F>
constexpr void for_sequence(std::integer_sequence<T, S...>, F&& f)
{
	using unpack_t = int[];
	(void)unpack_t
	{
		(static_cast<void>(f(std::integral_constant<T, S>{})), 0)..., 0
	};
}

// Sample implementation of a json-like data structure. It is only there for the example to compile and actually produce a testable output
namespace Json
{
	struct Value;

	struct ValueData
	{
		std::map<std::string, Value> subObject;
		std::string string;
		int number = 0;
	};

	struct Value
	{
		ValueData data;

		Value& operator[](std::string name)
		{
			return data.subObject[std::move(name)];
		}

		const Value& operator[](std::string name) const
		{
			auto it = data.subObject.find(std::move(name));

			if (it != data.subObject.end())
			{
				return it->second;
			}

			throw;
		}

		Value& operator=(std::string value)
		{
			data.string = value;
			return *this;
		}

		Value& operator=(double value)
		{
			data.number = value;
			return *this;
		}
	};

	template<typename T> T& asAny(Value&);
	template<typename T> const T& asAny(const Value&);

	template<>
	int& asAny<int>(Value& value)
	{
		return value.data.number;
	}

	template<>
	const int& asAny<int>(const Value& value)
	{
		return value.data.number;
	}

	template<>
	const std::string& asAny<std::string>(const Value& value)
	{
		return value.data.string;
	}

	template<>
	std::string& asAny<std::string>(Value& value)
	{
		return value.data.string;
	}
}

template<typename Class, typename T>
struct PropertyImpl
{
	constexpr PropertyImpl(T Class::* aMember, const char* aName) : member{aMember}, name{aName} {}

	using Type = T;

	T Class::* member;
	const char* name;
};

// One could overload this function to accept both a getter and a setter instead of a member.
template<typename Class, typename T>
constexpr auto property(T Class::* member, const char* name)
{
	return PropertyImpl<Class, T>{member, name};
}


// unserialize function
template<typename T>
T fromJson(const Json::Value& data)
{
	T object;

	// We first get the number of properties
	constexpr auto nbProperties = std::tuple_size<decltype(T::properties())>::value;

	// We iterate on the index sequence of size `nbProperties`
	for_sequence(std::make_index_sequence<nbProperties>{}, [&](auto i)
	{
		// get the property
		constexpr auto property = std::get<i>(T::properties());

		// get the type of the property
		using Type = typename decltype(property)::Type;

		// set the value to the member
		object.*(property.member) = Json::asAny<Type>(data[property.name]);
	});

	return object;
}

template<typename T>
Json::Value toJson(const T& object)
{
	Json::Value data;

	// We first get the number of properties
	constexpr auto nbProperties = std::tuple_size<decltype(T::properties())>::value;

	// We iterate on the index sequence of size `nbProperties`
	for_sequence(std::make_index_sequence<nbProperties>{}, [&](auto i)
	{
		// get the property
		constexpr auto property = std::get<i>(T::properties());

		// set the value to the member
		data[property.name] = object.*(property.member);
	});

	return data;
}

namespace animal
{
	struct Dog
	{
		std::string barkType;
		std::string color;
		int weight = 0;

		bool operator==(const Dog& rhs) const
		{
			return barkType == rhs.barkType && color == rhs.color && weight == rhs.weight;
		}

		constexpr static auto properties()
		{
			return std::make_tuple
			(
				property(&Dog::barkType, "barkType"),
				property(&Dog::color, "color"),
				property(&Dog::weight, "weight")
			);
		}
	};

}

struct testPair
{
	long double a;
	long double b;
};

struct testSingle
{
	long double a;
};

std::istream& operator>>(std::istream& stream, double* d)
{
	return stream >> d;
}

int main()
{
	const char* fileName = "C:\\Users\\dhr\\Desktop\\items.csv";

	std::vector<double> v;
	std::ifstream infile(fileName);

	//std::copy(std::istream_iterator<double>(infile), std::istream_iterator<double>(), std::back_inserter(v));

	//for (double d : v)
	//{
		//std::cout << "A: " << d << std::endl;
	//}


	uint64_t timeStamp;
	char c;
	std::string objectId;
	double p0, u0, a0;
	double p1, u1, a1;
	double p2, u2, a2;

	// never ever fucking do this..  
	while ((infile >> std::setprecision(std::numeric_limits<long double>::digits10 + 3) 
			>> timeStamp >> c 
			>> p0 >> c 
			>> p1 >> c 
			>> p2 >> c 
			>> u0 >> c 
			>> u1 >> c 
			>> u2 >> c 
			>> a0 >> c 
			>> a1 >> c 
			>> a2 >> c 
			>> objectId) && (c == ','))
	{
		std::cout << std::setprecision(std::numeric_limits<long double>::digits10 + 3)
			<< "Timestamp: " << timeStamp << std::endl
			<< "Object Id: " << objectId << std::endl
			<< "Position: " << p0 << ", " << p1 << ", " << p2 << std::endl
			<< "Up: " << u0 << ", " << u1 << ", " << u2 << std::endl
			<< "At: " << a0 << ", " << a1 << ", " << a2 << std::endl
			<< std::endl;
	}

	infile.close();

	std::cout << std::endl << "Program executed successfully. " << std::endl;
	return 0;
}