#pragma once

#include "variable_traits.hpp"
#include "function_traits.hpp"

namespace refl 
{
	template <typename T>
	struct TypeInfo;

	template <typename Ret, typename... Params>
	auto function_pointer_type(Ret(*)(Params...)) -> Ret(*)(Params...);


	template <typename Ret, typename Class, typename... Params>
	auto function_pointer_type(Ret(Class::*)(Params...)) -> Ret(Class::*)(Params...);

	template <typename Ret, typename Class, typename... Params>
	auto function_pointer_type(Ret(Class::*)(Params...)const) -> Ret(Class::*)(Params...)const;

	template <auto T>
	using function_pointer_type_t = decltype(function_pointer_type(T));

	template <auto T>
	using function_traits_t = function_traits<function_pointer_type_t<T>>;

	template <typename T>
	auto variable_pointer_type(T) -> T;

	template <typename T, typename Class>
	auto variable_pointer_type(T Class::*) -> T Class::*;

	template <auto T>
	using variable_pointer_type_t = decltype(variable_pointer_type(T));


	/*
	* 1.除了拿到类型信息外
	* 2.拿到函数指针
	*
	*/

	template <typename T>
	struct is_function {

		static constexpr bool value = std::is_function_v<std::remove_pointer_t<T>>
			|| std::is_member_function_pointer_v<T>;
	};

	template <typename T>
	constexpr bool is_function_v = is_function<T>::value;

	template <typename T, bool isFunc>
	struct basic_field_traits;

	template <typename T>
	struct basic_field_traits<T, true> : public function_traits<T>
	{
		using traits = function_traits<T>;

		constexpr bool is_member() const {
			return is_member;
		};

		constexpr bool is_const() const {
			return is_const;
		};

		constexpr bool is_function() const {
			return true;
		};

		constexpr bool is_variable() const {
			return false;
		};
	};

	template <typename T>
	struct basic_field_traits<T, false> : public variable_traits<T>
	{
		using traits = variable_traits<T>;

		constexpr bool is_member() const {
			return is_member;
		};

		constexpr bool is_function() const {
			return false;
		};

		constexpr bool is_variable() const {
			return true;
		};

		constexpr bool is_const() const {
			return is_const;
		};
	};

	template <typename T>
	struct field_traits : public basic_field_traits<T, is_function_v<T>>
	{
		constexpr field_traits(T&& _pointer, std::string_view name) :
			pointer(_pointer),
			name(name.substr(name.find_last_of(":") + 1)) {};

		T pointer;
		std::string_view name;
	};


	//template <>
	//struct TypeInfo<Person>
	//{
	//	//auto field = field_traits(&Person::IntroduceMyself);
	//
	//	using type = Person;
	//
	//	//static constexpr field_traits field = field_traits{&Person::IntroduceMyself };
	//
	//	static constexpr auto functions = std::make_tuple(field_traits{ &Person::IntroduceMyself,""},
	//		field_traits{ &Person::isHuman,"" });
	//
	//	static constexpr auto variable = std::make_tuple(field_traits{ &Person::height,"" });
	//};

#define BEGIN_CLASS(x) template <> struct TypeInfo<x> {
#define functions(...) \
	static constexpr auto functions = std::make_tuple(__VA_ARGS__);
#define func(f) \
	field_traits{ f, #f }

#define variables(...) \
	static constexpr auto variables = std::make_tuple(__VA_ARGS__);
#define vari(f) \
	field_traits{ f, #f }

#define END_CLASS() };

}

