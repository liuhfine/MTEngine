#include <Windows.h>

#include "meta/reflection/reflection.h"

#include <type_traits>
#include <tuple>
#include <iostream>
#include <cassert>

#include "refl.hpp"

#include "editor.h"




using namespace std;
using namespace refl;


struct Person final
{
	Person(const Person& p) {
		familyName = p.familyName;
		height = p.height;
		isFamily = p.isFamily;
		cout << "Person copy " << endl;

	}

	Person(Person&& p) {
		familyName = std::move(p.familyName);
		height = std::move(p.height);
		isFamily = std::move(p.isFamily);

		cout << "Person steal " << endl;

	}

	Person() = default;
	~Person() {
		cout << "Person release " << endl; 
	};
	//Person& opeartor

	std::string familyName;
	float height;
	bool isFamily;

	void IntroduceMyself() const {
		std::cout << "Function Call IntroduceMyself" << std::endl;
	};
	bool isHuman(int age, float size) const {
		std::cout << "Function Call isHuman age: "  << age << std::endl;
		return true; };

	static void printfcg();

};

void Person::printfcg()
{
};


int funcPrint(bool, char*)
{
	return 1;
}

template <typename T>
constexpr auto reflected_type()
{
	return TypeInfo<T>{};
}

/* tuple����
* ����ģ�����չ��
*/
template <std::size_t... idx, typename Tuple, typename Func>
void visitTuple(Tuple tuple, Func&& f, std::index_sequence<idx...>)
{
	(f(std::get<idx>(tuple)), ...);
	/*int num = 0;
	((num += std::get<idx>(tuple)), ...);
	return num;*/
}

/* part4
* �������б��Ĳ���
*/
template <typename... Remains>
struct type_list {
	static constexpr size_t size = sizeof...(Remains);
};

template <typename>
struct head;

template <typename T, typename... Remains>
struct head<type_list<T, Remains...>>
{
	using type = T;
};

template <typename>
struct tail;

template <typename T, typename... Remains>
struct tail<type_list<T, Remains...>>
{
	using type = type_list<Remains...>;
};

template <typename, size_t>
struct nth;

template <typename T, typename... Remains>
struct nth<type_list<T, Remains...>, 0>
{
	using type = T;
};

template <typename T, typename... Remains, size_t N>
struct nth<type_list<T, Remains...>, N>
{
	using type = typename nth<type_list<Remains...>, N - 1>::type;
};

void printRange(int low, int high)
{
	if (low > high)
	{
		return;
	}
	std::cout << low << std::endl;
	printRange(low+1, high);
}

template <typename, size_t N>
struct get_integer_type_count;

template <typename T, typename... Remains>
struct get_integer_type_count<type_list<T, Remains...>, 1> {
	static constexpr size_t count = (std::is_integral_v<T> ? 1 : 0);
};

template <typename T, typename... Remains, size_t N>
struct get_integer_type_count<type_list<T, Remains...>, N> {
	static constexpr size_t count = (std::is_integral_v<T> ? 1 : 0) + get_integer_type_count<type_list<Remains...>, N - 1>::count;

	//typename get_integer_type_count<type_list<Remains...>, N - 1>;
};

// map����
template <typename, template <typename> typename>
struct Map1;

template <typename... Args, template <typename> typename Func>
struct Map1<type_list<Args...>, Func>{
	using type = type_list<typename Func<Args>::type ...>;
};

// ׷������Ԫ��
template <typename, typename>
struct Cons;

template <typename T, typename... Args2>
struct Cons<T, type_list<Args2...>> {
	using type = type_list<T, Args2...>;
};

// �б�ƴ��
template <typename, typename>
struct Concat;

template <typename... Args1, typename... Args2>
struct Concat<type_list<Args1...>, type_list<Args2...>> {
	using type = type_list<Args1..., Args2...>;
};

// ȥ��β��Ԫ��
template <typename>
struct Init;

template <typename T>
struct Init<type_list<T>> {
	using type = type_list<>;
};

template <typename T, typename... Remains>
struct Init<type_list<T, Remains...>> {
	using type = typename Cons<T, typename Init<type_list<Remains...>>::type>::type;
};

//���� 
template <typename, template <typename> typename>
struct Filter;

template <template <typename> typename Func>
struct Filter<type_list<>, Func> {
	using type = type_list<>;
};

template <typename T,  typename... Remains, template <typename> typename Func>
struct Filter<type_list<T, Remains...>, Func> {
	using type = std::conditional_t<Func<T>::value, 
		typename Cons<T, typename Filter<type_list<Remains...>, Func>::type>::type,
		typename Filter<type_list<Remains...>, Func>::type>;

};


template <typename, template <typename> typename, size_t>
struct Count;

template <typename T, typename... Remains, template <typename> typename Func>
struct Count<type_list<T, Remains...>, Func, 0> {
	static constexpr size_t count = (Func<T>::val ? 1 : 0);
};

template <typename T, typename... Remains, template <typename> typename Func, size_t N>
struct Count<type_list<T, Remains...>, Func, N> {
	static constexpr size_t count = (Func<T>::val ? 1 : 0) + Count<type_list<Remains...>, Func, N - 1>::count;
};

// wei c 
template <typename T>
struct func_is_integer
{
	static constexpr bool val = std::is_integral_v<T>;
};

// Map
template <typename T>
struct change_to_float
{
	using type = std::conditional_t<std::is_integral_v<T>, float, float>;
};

template <typename T>
struct remove_to_char
{
	static constexpr bool value = !std::is_integral_v<T>;
};

// ��̬����
enum MyEnum
{
	Value1 = 1,
	Value2 = 2
};
namespace drefl
{
	class Type;
	class Numeric;
	class Enum;
	std::unordered_map<std::string, const Type*> RegistrarType_Maps;


	class Type
	{
	public:
		template <typename U>
		friend class EnumFactory;

		enum class Kind
		{
			Numeric,
			Enum,
			Class
		};

		virtual ~Type() = default;
		Type(std::string name, Kind kind) :_name(name), _type_kind(kind) {};

		std::string GetName() const { return _name; };
		void setName(std::string name) { _name = name; };
		//const auto* TryConversionInstanceByName(std::string name) const
		//{
		//	auto it = RegistrarType_Maps.find(name);
		//	if (it != RegistrarType_Maps.end());
		//	{
		//		return (Enum*)(it->second);
		//	}

		//	//return this;
		//}
		Kind getKind() const {
			return _type_kind;
		};

	protected:
		std::string _name;
		Kind _type_kind;
	};

	/*
	* ���Դ洢�������͵�����
	*/
	class any final
	{
	public:
		enum class StorageType
		{
			Empty,
			Copy,
			Steal,
			Ref,
			ConstRef
		};

		struct operations
		{
			any(*copy)(const any&) = {};
			any(*steal)(any&) = {};
			void(*release)(any&) = {};
		};

		any() = default;
		~any() {
			if (ops.release && (storage_type == StorageType::Copy ||
				storage_type == StorageType::Steal))
			{
				ops.release(*this);
			}
		};

		any(const any& o) {
			this->typeinfo = o.typeinfo;
			this->_payload = o._payload;
			this->storage_type = o.storage_type;
			this->ops = o.ops;

			if (ops.copy)
			{
				auto new_any = ops.copy(o);
				_payload = new_any._payload;
				new_any._payload = nullptr;
				new_any.ops.release = nullptr;
			}
			else
			{
				storage_type = StorageType::Empty;
				typeinfo = nullptr;
			}
		}

		any(any&& o) {
			this->typeinfo = std::move(o.typeinfo);
			this->_payload = std::move(o._payload);
			o._payload = nullptr;
			this->storage_type = std::move(o.storage_type);
			this->ops = std::move(o.ops);
		}

		any& operator=(const any& o) {
			if (&o != this)
			{
				typeinfo = o.typeinfo;
				storage_type = o.storage_type;
				ops = o.ops;
			}

			if (ops.copy)
			{
				auto new_any = ops.copy(o);
				_payload = new_any._payload;
				new_any._payload = nullptr;
				new_any.ops.release = nullptr;
			}
			else
			{
				storage_type = StorageType::Empty;
				typeinfo = nullptr;
			}
			return *this;
		}

		any& operator=(any&& o) {
			if (&o != this)
			{
				this->typeinfo = std::move(o.typeinfo);
				this->_payload = std::move(o._payload);
				this->storage_type = std::move(o.storage_type);
				this->ops = std::move(o.ops);
			}

			return *this;
		}

		const drefl::Type* typeinfo{};
		void* _payload{};
		StorageType storage_type = StorageType::Empty;
		operations ops;
	private:

	};

	template <typename T>
	any make_copy(const T& elem);

	template <typename T>
	any make_steal(T&& elem);

	template <typename T>
	any make_ref(const T& elem);

	template <typename T>
	any make_constref(const T& elem);

	
	class Numeric : public Type
	{
	public:
		enum class Kind
		{
			Unknown,
			Int8,
			Int16,
			Int32,
			Int64,
			Int128,
			Float,
			Double
		};

	public:
		virtual ~Numeric() = default;
		Numeric(Kind kind, bool isSigned) :Type(Kind2Name(kind), Type::Kind::Numeric), _kind(kind), _isSigned(isSigned) {
		};

		static map<Kind, std::string> g_kind_map;
		static std::string Kind2Name(Kind kind) { return g_kind_map[kind]; };

		Kind GetNKind() const { return _kind; };
		bool IsSigned() const { return _isSigned; };

		void setValue(double val, any* elem) {
			if (elem->typeinfo->getKind() == Type::Kind::Numeric)
			{
				auto vv = dynamic_cast<const Numeric*>(elem->typeinfo);
				auto kind = vv->GetNKind();
				switch (kind)
				{
				case drefl::Numeric::Kind::Int8:
				{
					if (vv->IsSigned())// ����Ҫ�����ж�
					{
						*static_cast<char*>(elem->_payload) = val;
					}
					else
					{
						*static_cast<unsigned char*>(elem->_payload) = val;
					}
				}
					break;
				case drefl::Numeric::Kind::Int16:
					break;
				case drefl::Numeric::Kind::Int32:
					break;
				case drefl::Numeric::Kind::Int64:
					break;
				case drefl::Numeric::Kind::Int128:
					break;
				case drefl::Numeric::Kind::Float:
					break;
				case drefl::Numeric::Kind::Double:
					break;
				case drefl::Numeric::Kind::Unknown:
					break;
				}
			}
		}

		template <typename T>
		static Kind DetectKind() {
			if constexpr (std::is_same_v<T, bool>)
				return Kind::Int8;
			else if constexpr (std::is_same_v<T, char>)
				return Kind::Int8;
			else if constexpr (std::is_same_v<T, short>)
				return Kind::Int16;
			else if constexpr (std::is_same_v<T, int>)
				return Kind::Int32;
			else if constexpr (std::is_same_v<T, float>)
				return Kind::Float;
			else
				return Kind::Unknown;
		}

		template <typename T>
		static Numeric Create() {
			return Numeric(DetectKind<T>(), std::is_signed_v<T>);
		}

	private:
		Kind _kind;
		bool _isSigned;
	};

	map<Numeric::Kind, std::string> Numeric::g_kind_map = {
		{Numeric::Kind::Unknown, "Unknown"},
		{Numeric::Kind::Int8, "Int8"},
		{Numeric::Kind::Int16, "Int16"},
		{Numeric::Kind::Int32, "Int32"},
		{Numeric::Kind::Int64, "Int64"},
		{Numeric::Kind::Int128, "Int128"},
		{Numeric::Kind::Float, "Float"},
		{Numeric::Kind::Double, "Double"} };

	class Enum : public Type
	{
	public:
		struct Item
		{
			using value_type = long;
			std::string naem;
			value_type value;
		};
	public:
		virtual ~Enum() = default;
		Enum(std::string name) :Type(name, Type::Kind::Enum) {};

		template <typename T>
		void Add(const std::string& name, T val) {
			{
				m_items.emplace_back(Item{ name, static_cast<typename Item::value_type>(val) });
			}
		};

		Item getItem() const { return m_items[0]; };


	private:
		std::vector<Item> m_items;
	};

	//class Field {
	//public:

	//	template <typename T>
	//	static Field createField(const std::string& name);
	//	//private:
	//	std::string _name;
	//	const Type* _type;
	//};

	//struct Method
	//{
	//	//template <typename T>
	//	//Method(const std::string& name) {};
	////private:
	//	std::string _name;
	//	const Type* _return_type;
	//	std::vector<const Type*> _params;

	//	template <typename T>
	//	static Method createMethod(const std::string& name);

	//private:
	//	template <typename Params, size_t... Idx>
	//	static std::vector<const Type*> cvtType2Vector(std::index_sequence<Idx...>);
	//};

	class Member
	{
	public:
		virtual drefl::any call(std::vector<const drefl::any*>& anies) = 0;
	};

	template <typename clazz, typename Type>
	class MemberVariable : public Member
	{
	public:
		std::string name;
		const Type* type;
		Type clazz::* ptr;

		static MemberVariable Create(const std::string& _name);

		virtual drefl::any call(std::vector<const drefl::any*>& anies) override {

			assert(anies.size() == 1 && anies[0]->typeinfo == drefl::GetType<Type>());
			
			clazz* instance = static_cast<clazz*>(anies[0]->_payload);
			auto value = instance->*ptr;

			return drefl::make_copy(value);
		};
	private:

	};

	template <typename T>
	T& unwrap(drefl::any& value) {
		assert(value.typeinfo == drefl::GetType<T>());
		return *static_cast<T*>(value._payload);
	}

	template <typename clazz, typename RetType, size_t... Idx, typename... Args>
	void inner_call(RetType(clazz::* ptr)(Args...), const std::vector<const drefl::any>& params, std::index_sequence<Idx...>) {
		clazz* instance = static_cast<clazz*>(params [0]->_payload);
		auto return_value = (instance->*ptr)(unwrap<Args>(params[Idx + 1])...);
		//auto return_value = ((clazz::*)params[0]._payload->*ptr)(unwrap<Args>(params[Idx + 1])...);
		return make_copy(return_value);
	};

	template <typename clazz, typename Type, typename... Args>
	class MemberFunction : public Member
	{
	public:
		std::string name;
		const Type* retType;
		std::vector<const Type*> paramTypes;
		Type(clazz::* ptr)(Args...);

		static MemberFunction Create(const std::string& _name);

		virtual drefl::any call(std::vector<const drefl::any*>& anies) override {

			assert(anies.size() == paramTypes.size() + 1)
			for (int i=0; i < paramTypes.size(); ++i)
			{
				assert(paramTypes[i] == anies[i+1].typeinfo)
			}

			/*clazz* instance = static_cast<clazz*>(anies[0]->_payload);
			auto value = (instance->*ptr)(...);*/

			return inner_call(ptr, anies, std::make_index_sequence<sizeof...(Args)>());
		};

	private:
		//template <typename Params, size_t... Idx>
		//static std::vector<const Type*> cvtType2Vector(std::index_sequence<Idx...>);

	};

	class Class : public Type
	{
	public:


	public:
		virtual ~Class() = default;
		Class(std::string name) :Type(name, Type::Kind::Class) {
			
		};

		template <typename clazz>
		void create()
		{
			classType = GetType<clazz>();
		}

		template <typename clazz, typename Type>
		void Add(MemberVariable<clazz, Type>&& field) {
			//_fields.emplace_back(std::move(field));

			//_fields.emplace_back(std::make_unique<MemberVariable<Person, float>>(std::move(field)));
		};

		template <typename clazz, typename Type, typename... Args>
		void Add(MemberFunction<clazz, Type, Args...>&& method) {
			_methods.emplace_back(std::move(method));
		};

		auto& getField() const {
			return _fields[0];
		};

		auto& getMethod() const {
			return _methods;
		};

	private:
		const Type* classType;

		// fields
		std::vector<std::unique_ptr<Member>> _fields;
		// methods
		std::vector<std::unique_ptr<Member>> _methods;
	};


	class TypeInfo
	{
	public:

		//TypeInfo(){};

	private:
		Type type;
	};


	template <typename T>
	class NumericFactory final
	{
	public:
		static NumericFactory& Instance() {
			static NumericFactory inst{ Numeric::Create<T>() };

			//RegistrarType_Maps[_numeric._name] = &_numeric;

			return inst;
		};

		const Numeric& Info() { return _numeric; };

	private:
		Numeric _numeric;
		NumericFactory(Numeric&& numeric) : _numeric(std::move(numeric)) {};
	};


	template <typename T>
	class EnumFactory final
	{
	public:
		static EnumFactory& Instance() {
			static EnumFactory inst;
			return inst;
		};

		const Enum& Info() { return _info; };

		EnumFactory& Regist(const std::string& name) {
			_info._name = name;

			RegistrarType_Maps[name] = &_info;

			return *this;
		};

		template <typename U>
		EnumFactory& Add(const std::string& valName, T value) {
			_info.Add(valName, value);
			return *this;
		};

	private:
		Enum _info;
		EnumFactory() : _info("") {};
	};

	template <typename T>
	class ClassFactory
	{
	public:
		~ClassFactory() {};

		static ClassFactory& Instance() {
			static ClassFactory ins;
			return ins;
		};

		const Class& Info() { return *_info; };

		ClassFactory& Regist(const std::string& name) {

			if (!_info)
			{
				_info = new Class(name);
				_info->create<T>();
			}

			RegistrarType_Maps[name] = _info;

			return *this;
		};

		template <typename T, typename U>
		ClassFactory& Add(const std::string& valName) {

			//_info->Add(MemberVariable::Create<T, U>(valName));
			return *this;
		};

		template <typename T, typename U>
		ClassFactory& AddMethod(const std::string& valName) {

			//_info->Add(MemberFunction::Create<T, U>(valName));
			return *this;
		};

	private:
		ClassFactory() {};

		Class* _info{ nullptr };
	};



	template <typename T>
	class Factory final
	{
	public:
		static auto& GetFactory() {

			if constexpr (std::is_fundamental_v<T>)
				return NumericFactory<T>::Instance();
			else if constexpr (std::is_enum_v<T>)
				return EnumFactory<T>::Instance();
			else if constexpr (std::is_class_v<T>)
				return ClassFactory<T>::Instance();
		}
	};


	template <typename T>
	auto& Registrar()
	{
		return Factory<T>::GetFactory();
	}

	template <typename T>
	const auto* GetTypeInfo()
	{
		return &Factory<T>::GetFactory().Info();
	}

	template <typename T>
	const Type* GetType()
	{
		return &Factory<T>::GetFactory().Info();
	}

	//template <typename T>
	//Field Field::createField(const std::string& name) {
	//	//_type = GetType<T>();
	//	//_type->setName(name);
	//	Field field;
	//	field._name = name;
	//	field._type = GetType<T>();
	//	//field1._type->setName(name);
	//	return field;
	//};

	template <typename clazz, typename Type>
	MemberVariable<clazz, Type>  MemberVariable<clazz, Type>::Create(const std::string& name) {
		MemberVariable member;
		member.name = name;
		member.type = drefl::GetType<Type>();
		member.ptr = nullptr; // 需要在实际使用时正确设置成员指针
		return member;
	};

	template <typename clazz, typename Type, typename... Args>
	MemberFunction<clazz, Type, Args...> MemberFunction<clazz, Type, Args...>::Create(const std::string& name) {
		using traits = function_traits<Type>;
		using args = typename traits::args;

		MemberFunction method;
		method.name = name;
		method.retType = GetType<traits::return_type>();

		auto ff = std::make_index_sequence<std::tuple_size_v<args>>();
		//method.paramTypes = cvtType2Vector<args>(ff);

		//return Method{ name, GetType<traits::return_type>(),
		//	cvtType2Vector<args>(std::make_index_sequence<std::tuple_size_v<args>>())};
		return method;
	};

	/*template <typename Params, size_t... Idx>
	std::vector<const Type*> MemberFunction::cvtType2Vector(std::index_sequence<Idx...>)
	{
		return { GetType<std::tuple_element_t<Idx, Params>>() ... };
	};*/

	

	template <typename T>
	struct operations_traits {
		static any copy(const any& ele) {
			if (ele.typeinfo != GetType<T>())
			{
				return any{};
			}

			any return_value;
			return_value._payload = new T{ *static_cast<T*>(ele._payload) };
			return_value.typeinfo = ele.typeinfo;
			return_value.storage_type = any::StorageType::Copy;
			return_value.ops = ele.ops;
			return return_value;
		};

		static any steal(any& ele) {
			if (ele.typeinfo != GetType<T>())
			{
				return any{};
			}

			any return_value;
			return_value._payload = new T{ std::move(*static_cast<T*>(ele._payload)) };
			return_value.typeinfo = ele.typeinfo;
			return_value.storage_type = any::StorageType::Copy;
			ele.storage_type = any::StorageType::Steal;
			return_value.ops = ele.ops;
			return return_value;
		};

		static void release(any& ele) {
			if (ele.typeinfo != GetType<T>())
			{
				return;
			}

			delete (T*)ele._payload;
			ele._payload = nullptr;
			ele.typeinfo = nullptr;
			ele.storage_type = any::StorageType::Empty;
		};
	};

	template <typename T>
	any make_copy(const T& elem) {
		any return_value;
		return_value._payload = new T{ elem };
		return_value.typeinfo = GetType<T>();
		return_value.storage_type = any::StorageType::Copy;

		if constexpr (std::is_copy_constructible_v<T>){
			return_value.ops.copy = &operations_traits<T>::copy;
		}

		if constexpr (std::is_move_constructible_v<T>) {
			return_value.ops.steal = &operations_traits<T>::steal;
		}

		if constexpr (std::is_destructible_v<T>) {
			return_value.ops.release = &operations_traits<T>::release;
		}
		
		return return_value;
	}

	template <typename T>
	any make_steal(T&& elem) {
		any return_value;

		T dd = std::move(elem);
		return_value._payload = new T{ dd };
		return_value.typeinfo = GetType<T>();
		return_value.storage_type = any::StorageType::Steal;

		if constexpr (std::is_copy_constructible_v<T>) {
			return_value.ops.copy = &operations_traits<T>::copy;
		}

		if constexpr (std::is_move_constructible_v<T>) {
			return_value.ops.steal = &operations_traits<T>::steal;
		}

		if constexpr (std::is_destructible_v<T>) {
			return_value.ops.release = &operations_traits<T>::release;
		}

		return return_value;
	}

	template <typename T>
	any make_ref(T& elem) {
		any return_value;
		return_value._payload = &elem;
		return_value.typeinfo = GetType<T>();
		return_value.storage_type = any::StorageType::Ref;

		if constexpr (std::is_copy_constructible_v<T>) {
			return_value.ops.copy = &operations_traits<T>::copy;
		}

		if constexpr (std::is_move_constructible_v<T>) {
			return_value.ops.steal = &operations_traits<T>::steal;
		}

		if constexpr (std::is_destructible_v<T>) {
			return_value.ops.release = &operations_traits<T>::release;
		}
		return return_value;
	}

	template <typename T>
	any make_constref(const T& elem) {
		any return_value;
		return_value._payload = (void*)&elem;
		return_value.typeinfo = GetType<T>();
		return_value.storage_type = any::StorageType::ConstRef;

		if constexpr (std::is_copy_constructible_v<T>) {
			return_value.ops.copy = &operations_traits<T>::copy;
		}

		if constexpr (std::is_move_constructible_v<T>) {
			return_value.ops.steal = &operations_traits<T>::steal;
		}

		if constexpr (std::is_destructible_v<T>) {
			return_value.ops.release = &operations_traits<T>::release;
		}
		return return_value;
	}

	
}

//#include <any>






#include "meta/meta_example.h"
#include "meta/serializer/serializer.h"
#include "meta/reflection/reflection.h"

#include "meta/reflection/reflection_register.h"
#include "_generated/serializer/all_serializer.h"


#include <iostream>
#include <fstream>

using namespace Piccolo;

template<typename T>
static Json write1(const T& instance)
{

	if constexpr (std::is_pointer_v<T>)
	{
		return Json();
	}
	else
	{
		static_assert(always_false<T>, "Serializer::write<T> has not been implemented yet!");
		return Json();
	}
}

void startEngine(const std::string& config_file_path)
{
	Reflection::TypeMetaRegister::metaRegister();

	std::cout << ("engine start") << std::endl;
}

void shutdownEngine()
{
	std::cout << ("engine shutdown") << std::endl;

	//g_runtime_global_context.shutdownSystems();

	Reflection::TypeMetaRegister::metaUnregister();
}

template <typename T>
T* try_cast1(drefl::any* ay) {
	if (ay->typeinfo == drefl::GetType<T>())
	{
		return static_cast<T*>(ay->_payload);
	}
	else
	{
		return nullptr;
	}
}

//class Member
//{
//public:
//	virtual drefl::any call(std::vector<const drefl::any*>& anies) = 0;
//};
//
//template <typename clazz, typename Type>
//class MemberVariable : public Member
//{
//public:
//	std::string name;
//	const Type* type;
//	Type clazz::* ptr;
//
//	virtual drefl::any call(std::vector<const drefl::any*>& anies) override {
//
//		if (anies.empty() || anies[0]->typeinfo == drefl::GetType<Type>())
//		{
//			assert(false);
//		}
//
//		clazz* instance = static_cast<clazz*>(anies[0]->_payload);
//		auto value = instance->*ptr;
//
//		return drefl::make_copy(value);
//	};
//
//
//	template <typename T>
//	static MemberVariable Create(const std::string& _name) {
//		return MemberVariable{ _name };
//	};
//private:
//
//};

//template <typename T>
//T& unwrap(drefl::any& value) {
//	assert(value->typeinfo == drefl::GetType<Type>());
//	return *(T*)value._payload;
//}
//
//template <typename clazz, typename RetType, size_t... Idx, typename... Args>
//void inner_call(RetType(drefl::Class::* ptr)(Args...), const std::vector<const drefl::any>& params) {
//	auto return_value = ((drefl::Class::*)params[0]._payload->*ptr)(unwrap<Args>(params[Idx + 1])...);
//
//	return make_copy(return_value);
//};
//
//
//template <typename clazz, typename Type, typename... Args>
//class MemberFunction : public Member
//{
//public:
//	std::string name;
//	const Type* return_type;
//	std::vector<const Type*> paramTypes;
//	Type (drefl::Class::* ptr) (Args...);
//
//
//	virtual drefl::any call(std::vector<const drefl::any*>& anies) override {
//
//		if (anies.empty() || anies[0]->typeinfo == drefl::GetType<Type>())
//		{
//			assert(false);
//		}
//
//		clazz* instance = static_cast<clazz*>(anies[0]->_payload);
//		auto value = instance->*ptr(...);
//
//		return drefl::make_copy(value);
//	};
//
//	template <typename T>
//	static MemberFunction Create(const std::string& name) {};
//private:
//
//};

BEGIN_CLASS(Color)
functions(
	func(&Color::toVector3),
	func(&Color::isValid)
)
variables(
	vari(&Color::r),
	vari(&Color::g),
	vari(&Color::b)
)
END_CLASS()


//BEGIN_CLASS(Person)
//
//functions(
//	func(&Person::IntroduceMyself),
//	func(&Person::isHuman)
//)
//
//variables(
//	vari(&Person::familyName),
//	vari(&Person::height),
//	vari(&Person::isFamily)
//)
//
//END_CLASS()

template <>
struct TypeInfo<Person>
{
	using type = Person;

	static constexpr auto functions = std::make_tuple(
		field_traits{ &Person::IntroduceMyself, "Person::IntroduceMyself"},
		field_traits{ &Person::isHuman,			"Person::isHuman" });

	static constexpr auto variables = std::make_tuple(
		field_traits{ &Person::familyName,	"Person::familyName" },
		field_traits{ &Person::height,		"Person::height" },
		field_traits{ &Person::isFamily,	"Person::isFamily" });
};



namespace erefl
{
	struct TypeInfo
	{
		std::string_view getName() { return _name; };

		std::string_view _name;
	};

	struct Numeric : public TypeInfo
	{
		enum class Kind
		{
			Unknown,
			Int8,
			Int16,
			Int32,
			Int64,
			Int128,
			Float,
			Double
		};

		Numeric(const Kind kind, const bool is_signed) :_signed(is_signed){
			kind2Name(kind);
		};

		Kind _kind;
		bool _signed;

		template <typename T>
		static Numeric Create() {
			return Numeric(DetectKind<T>(), std::is_signed_v<T>);
		}

	private:

		template <typename T>
		static Kind DetectKind() {
			if constexpr (std::is_same_v<T, bool>)
				return Kind::Int8;
			if constexpr (std::is_same_v<T, char>)
				return Kind::Int8;
			else if constexpr (std::is_same_v<T, short>)
				return  Kind::Int16;
			else if constexpr (std::is_same_v<T, int>)
				return  Kind::Int32;
			else if constexpr (std::is_same_v<T, float>)
				return  Kind::Float;
			else if constexpr (std::is_same_v<T, double>)
				return  Kind::Double;
		}

		void kind2Name(const Kind kind) {
			_kind = kind;

			switch (_kind)
			{
			case erefl::Numeric::Kind::Unknown:
				_name = "Unknown";
				break;
			case erefl::Numeric::Kind::Int8:
				_name = "Char";
				break;
			case erefl::Numeric::Kind::Int16:
				_name = "Short";
				break;
			case erefl::Numeric::Kind::Int32:
				_name = "Int";
				break;
			case erefl::Numeric::Kind::Int64:
				break;
			case erefl::Numeric::Kind::Int128:
				break;
			case erefl::Numeric::Kind::Float:
				_name = "Float";
				break;
			case erefl::Numeric::Kind::Double:
				_name = "Double";
				break;
			}
		};
	};

	template <typename T>
	class Factory final
	{
	public:
		static auto& GetFactory() {
			if constexpr (std::is_fundamental_v<T>)
			{
				return Numeric::Create<T>();
			}
			else if constexpr (std::is_class_v<T>)
			{
				return Numeric::Create<T>();
			}
			else if constexpr (std::is_enum_v<T>)
			{
				return Numeric::Create<T>();
			}
		}
	};



	void Registrar(const std::string& name)
	{
		return;
	}
}



int main(int argc, char* argv[])
{

#if 0
	startEngine("");
	{
		Test1 test1_in;
		test1_in.m_int = 12;
		test1_in.m_char = 'g';
		int i = 1;
		test1_in.m_int_vector.emplace_back(&i);

		Test1 test1_out;
		// test on array
		Test2 test2_in;
		test2_in.m_test_base_array.emplace_back("Test1", &test1_in);
		Test1 Test2_temp;
		test2_in.m_test_base_array.emplace_back("Test1", &Test2_temp);

		
		// serializer & deserializer
		Serializer::write(test1_in);
		auto test1_json_in = Serializer::write(test1_in);

		std::string test1_context = test1_json_in.dump();

		//// read Test1_context (json) to Test1_out (object)
		//std::string err;

		//Json&& Test1_json = Json::parse(test1_context, err);
		//Serializer::read(Test1_json, test1_out);
		//LOG_INFO(test1_context);

		/*auto        Test2_json_in = Serializer::write(test2_in);
		std::string test2_context = Test2_json_in.dump();

		std::fstream out_put("out.txt", std::ios::out);
		out_put << test2_context;
		out_put.flush();
		out_put.close();

		Test2  test2_out;
		auto&& test2_json = Json::parse(test2_context, err);
		Serializer::read(test2_json, test2_out);*/

		// reflection
		auto                       meta = TypeMetaDef(Test1, &test1_in);
		Reflection::FieldAccessor* fields;
		int                        fields_count = meta.m_meta.getFieldsList(fields);
		for (int i = 0; i < fields_count; ++i)
		{
			auto filed_accesser = fields[i];
			std::cout << filed_accesser.getFieldTypeName() << " " << filed_accesser.getFieldName() << " "
				<< (char*)filed_accesser.get(meta.m_instance) << std::endl;
			if (filed_accesser.isArrayType())
			{
				Reflection::ArrayAccessor array_accesser;
				if (Reflection::TypeMeta::newArrayAccessorFromName(filed_accesser.getFieldTypeName(), array_accesser))
				{
					void* field_instance = filed_accesser.get(meta.m_instance);
					int   count = array_accesser.getSize(field_instance);
					auto  typeMetaItem = Reflection::TypeMeta::newMetaFromName(array_accesser.getElementTypeName());
					for (int index = 0; index < count; ++index)
					{
						std::cout << ":L:" << index << ":R:" << (int*)array_accesser.get(index, field_instance)
							<< std::endl;
						;
					}
				}
			}
		}
	}
	shutdownEngine();
#endif

#if 1
	
	char storage[128];

	int i = 99;
	float i3 = 998.0;

	int* i1 = new(storage) int[5]{ (int)i3, 2, 0,1 };

	erefl::Numeric _numd = erefl::Factory<float>::GetFactory();
	_numd._name;

	auto numIns = drefl::NumericFactory<char>::Instance();
	auto numIns2 = drefl::NumericFactory<char>::Instance();

	auto numIns1 = drefl::NumericFactory<float>::Instance();
	auto numInfo = numIns.Info();
	numInfo.GetName();
	

	

 	auto& calsss = drefl::Registrar<Person>();
	calsss.Regist("Person").Add<Person, float>("height").Add<Person, bool>("isFamily");
	calsss.AddMethod<Person, decltype(&Person::isHuman)>("isHuman");
	calsss.AddMethod<Person, decltype(&Person::IntroduceMyself)>("IntroduceMyself");

	auto typeInfo3 = (drefl::Class*)(drefl::GetType<Person>());
	typeInfo3->GetName();


	{
		Person person;
		person.familyName = "wang";
		person.height = 180;
		person.isFamily = false;

		drefl::any aqa = drefl::make_copy(person);

		auto dsfd = try_cast1<Person>(&aqa);
		dsfd->height = 170;

		drefl::MemberVariable<Person, float> memberVar;
		memberVar.name = aqa.typeinfo->GetName();
		memberVar.ptr = &Person::height;

		std::vector<const drefl::any*> anies{ &aqa };
		auto return_any = memberVar.call(anies);

		int i = 0;
	}

	{
		Person person;
		person.familyName = "wang";
		person.height = 180;
		person.isFamily = false;

		drefl::any aqa = drefl::make_steal(std::move(person));
		int i = 0;
	}

	{
		Person person;
		person.familyName = "wang";
		person.height = 180;
		person.isFamily = false;

		drefl::any aqa = drefl::make_ref(person);
		
		auto typeInfo33 = (drefl::Class*)(drefl::GetType<Person>());
		auto method = typeInfo33->getMethod();
		//method
		
	}

	{
		Person person;
		person.familyName = "wang";
		person.height = 180;
		person.isFamily = false;

		drefl::any aqa = drefl::make_constref(person);
		int i = 0;
	}
	// any ����ʵ������info����ע�����͵�����ʵ������invoke
	//Any any = person;
	//typeInfo3->getMethod().call(any);

	Person person;
	//float height;
	auto& enumIns = drefl::Registrar<MyEnum>();

	//auto enumIns = drefl::EnumFactory<int>::Instance();
	enumIns.Regist("MyEnum").Add<int>("Value1", MyEnum::Value1).Add<int>("Value2", MyEnum::Value2);

	auto enumInfo = drefl::RegistrarType_Maps["MyEnum"];

	//auto enumInfo = drefl::GetTypeInfo<MyEnum>();
	//auto item1 =enumInfo->getItem();
	auto typeInfo1 = drefl::GetType<MyEnum>();
	auto typeInfo = drefl::GetTypeInfo<MyEnum>();
	typeInfo->getItem();

	//auto typeInfo1 = typeInfo1->TryConversionInstanceByName("MyEnum")
	//enumInfo->GetName();
	//enumInfo->getItem();

	

	constexpr auto info = reflected_type<Person>();
	constexpr auto info1 = reflected_type<Color>();

	auto ggg32 = &info;
	Person ert1;
	//Piccolo::Reflection::TypeMetaRegisterinterface::registerToReflMap<Person>(ggg32);

	static constexpr auto refl_types = std::make_tuple(info, info1);
	auto person_info = std::get<0>(refl_types);

	std::cout << std::get<0>(person_info.functions).name << std::endl;
	//info.functions;


	auto tuples = std::make_tuple(1, 2, 3, 4, 5);
	visitTuple(tuples, [](auto&& elem) {
		std::cout << elem << std::endl;
		},
		std::make_index_sequence<std::tuple_size_v<decltype(tuples)>>());

	using types = type_list<double, int, std::string,int>;
	using headType = head<types>::type;
	using headType1 = tail<types>::type;
	//using headType = head<tail<tail<types>::type>::type>::type;
	using headType2 = nth<types, 1>::type;

	constexpr auto yui = get_integer_type_count<types, types::size>::count;
	constexpr auto yui1 = Count<types, func_is_integer, types::size-1>::count;
	using yui2 =  Map1<types, change_to_float>::type;
	using yui3 = Cons<std::string, types>::type;
	using yui4 = Concat<type_list<std::string, std::string>, types>::type;

	using yui5 = Init<types>::type;

	using yui6 = Filter<types, remove_to_char>::type;

	//auto  funType11 = std::get<0>(info.functions);
	//funType11.pointer;

	std::cout << std::get<0>(info.variables).name << std::endl;
	auto varPro = std::get<0>(info.variables).pointer;
	person.*varPro = "hello refl";

	std::cout << std::get<0>(info.functions).name << std::endl;

	auto funcPro = std::get<0>(info.functions).pointer;
	(person.*funcPro)();
	std::invoke(funcPro, person);

	std::cout << std::get<1>(info.functions).name << std::endl;

	auto funcPro1 = std::get<1>(info.functions).pointer;
	(person.*funcPro1)(10, 0.1);
	std::invoke(funcPro1, person, 99, 0.1);


	using functions = std::tuple<function_traits<decltype(&Person::IntroduceMyself)>>;

	using return_pointType = function_traits_t<&Person::IntroduceMyself>::args_with_class;
	using pointType = function_pointer_type_t<&Person::IntroduceMyself>;
	using pointType1  = decltype(function_pointer_type(&Person::IntroduceMyself));
	
	auto field = field_traits(&Person::IntroduceMyself, "IntroduceMyself");
	auto field1 = field_traits(&Person::isFamily, "isFamily");
	auto field2 = field_traits(&Person::height, "height");

	
	auto isfunc = refl::is_function_v<decltype(&Person::isFamily)>;
	

	person.*(field1.pointer) = false;
	auto memberVal = std::invoke(field1.pointer, person);

	person.*(field2.pointer) = 180;
	std::invoke(field2.pointer, person) = 999;
	auto memberVal1 = std::invoke(field2.pointer, person);

	std::cout << (field.pointer == &Person::IntroduceMyself) << std::endl;

	static_assert(std::is_same_v<pointType, void (Person::*)() const>);
	//using func_pointer_type = function_pointer_type_t<&Person::IntroduceMyself>;

 	int ii = 0;

	/*using type = remove_pointer<int*>::type;
	type ff = 9;

	using function_info = function_traits<decltype(&funcPrint)>;
	using return_type = function_info::return_type;
	using param_type = function_info::param_type;*/


	/*using function_info1 = function_traits<decltype(&Person::ffff)>;
	using return_type1 =function_info1::return_type;
	using param_type1 = function_info1::param_type;
	using class_type =function_info1::class_type;*/

#endif

	Mountian::Editor editor;
	editor.initialize(argc, argv);

	editor.run();

	return 0;
}
