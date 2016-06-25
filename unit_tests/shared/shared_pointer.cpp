

#	include "oolua_tests_pch.h"
#	include "oolua.h"
#	include "common_cppunit_headers.h"
#	include "expose_stub_classes.h"
#	include "expose_hierarchy.h"

#	if OOLUA_USE_SHARED_PTR == 1
#		include OOLUA_SHARED_HEADER
#		include <csetjmp>

#		ifdef _MSC_VER
#			pragma warning(push)
#			pragma warning(disable : 4702)//unreachable code
#		endif
namespace
{
	jmp_buf mark;
	int OOLua_panic(lua_State* /*vm*/) //NOLINT(readability/casting)
	{
		longjmp(mark, 1);
		return 0;
	}
} //namespace

#		ifdef _MSC_VER
#			pragma warning(pop)
#		endif

int called = 0;
struct SharedFoo
{
	SharedFoo()
		: count(0)
	{}
	void no_param_function_const() const
	{
		++called;
	}
	void no_param_function()
	{
		++count;
	}
	void shared_param(OOLUA_SHARED_TYPE<SharedFoo> input)
	{
		m_shared = input;
	}
	void shared_param_ref(OOLUA_SHARED_TYPE<SharedFoo>& input)
	{
		m_shared = input;
	}
	OOLUA_SHARED_TYPE<SharedFoo> returnsSharedPtr()
	{
		return m_shared;
	}
	OOLUA_SHARED_TYPE<SharedFoo>& returnsRefToSharedPtr()
	{
		return m_shared;
	}
	void shared_param_type_is_const(OOLUA_SHARED_TYPE<SharedFoo const> input)
	{
		m_const_shared = input;
	}
	OOLUA_SHARED_TYPE<SharedFoo const> returnsSharedConstPtr()
	{
		return m_const_shared;
	}
	int count;
	OOLUA_SHARED_TYPE<SharedFoo> m_shared;
	OOLUA_SHARED_TYPE<SharedFoo const> m_const_shared;
};

OOLUA_PROXY(SharedFoo)
	OOLUA_MFUNC(no_param_function)
	OOLUA_MFUNC_CONST(no_param_function_const)
	OOLUA_MFUNC(shared_param)
	OOLUA_MFUNC(shared_param_ref)
	OOLUA_MFUNC(returnsSharedPtr)
	OOLUA_MFUNC(returnsRefToSharedPtr)
	OOLUA_MFUNC(shared_param_type_is_const)
	OOLUA_MFUNC(returnsSharedConstPtr)
	OOLUA_MEM_FUNC_RENAME(returns_maybe_null_shared
						, maybe_null<OOLUA_SHARED_TYPE<SharedFoo> >
						, returnsSharedPtr)
	OOLUA_MGET(m_shared)
OOLUA_PROXY_END

OOLUA_EXPORT_FUNCTIONS(SharedFoo
						, no_param_function
						, shared_param
						, shared_param_ref
						, returnsSharedPtr
						, returnsRefToSharedPtr
						, shared_param_type_is_const
						, returnsSharedConstPtr
						, returns_maybe_null_shared)
OOLUA_EXPORT_FUNCTIONS_CONST(SharedFoo, no_param_function_const, get_m_shared)

struct SharedConstructor{};

OOLUA_PROXY(SharedConstructor)
	OOLUA_TAGS(Shared)
OOLUA_PROXY_END

OOLUA_EXPORT_NO_FUNCTIONS(SharedConstructor)


struct SharedValueReturn
{
	SharedValueReturn stack_return()
	{
		return SharedValueReturn();
	}
};

OOLUA_PROXY(SharedValueReturn)
	OOLUA_TAGS(Shared)
	OOLUA_MFUNC(stack_return)
OOLUA_PROXY_END
OOLUA_EXPORT_FUNCTIONS(SharedValueReturn, stack_return)
OOLUA_EXPORT_FUNCTIONS_CONST(SharedValueReturn)

struct DefaultNoSharedValueReturn
{
	DefaultNoSharedValueReturn stack_return()
	{
		return DefaultNoSharedValueReturn();
	}
};

OOLUA_PROXY(DefaultNoSharedValueReturn)
	OOLUA_MFUNC(stack_return)
OOLUA_PROXY_END
OOLUA_EXPORT_FUNCTIONS(DefaultNoSharedValueReturn, stack_return)
OOLUA_EXPORT_FUNCTIONS_CONST(DefaultNoSharedValueReturn)

struct PointerChangesToShared
{
	PointerChangesToShared* ptr(){ return new PointerChangesToShared; }
	PointerChangesToShared const* ptr_const(){ return new PointerChangesToShared; }
	PointerChangesToShared* null(){ return NULL; }
};

OOLUA_PROXY(PointerChangesToShared)
	OOLUA_MEM_FUNC(shared_return<PointerChangesToShared*>, ptr)
	OOLUA_MEM_FUNC(shared_return<PointerChangesToShared const*>, ptr_const)
	OOLUA_MEM_FUNC(maybe_null<shared_return<PointerChangesToShared*> >, null)
	OOLUA_MEM_FUNC_RENAME(none_null, maybe_null<shared_return<PointerChangesToShared*> >, ptr)
OOLUA_PROXY_END
OOLUA_EXPORT_FUNCTIONS(PointerChangesToShared, ptr, ptr_const, null, none_null)
OOLUA_EXPORT_FUNCTIONS_CONST(PointerChangesToShared)

struct Pod{};
struct PodWithFirstMemberPod
{
	Pod first_member;
};
OOLUA_PROXY(Pod)
OOLUA_PROXY_END
OOLUA_EXPORT_NO_FUNCTIONS(Pod)

OOLUA_PROXY(PodWithFirstMemberPod)
OOLUA_PROXY_END
OOLUA_EXPORT_NO_FUNCTIONS(PodWithFirstMemberPod)

struct NonePod : PodWithFirstMemberPod
{
	virtual ~NonePod(){}
};
OOLUA_PROXY(NonePod, PodWithFirstMemberPod)
OOLUA_PROXY_END
OOLUA_EXPORT_NO_FUNCTIONS(NonePod)

	class SharedPointer : public CppUnit::TestFixture
	{
		CPPUNIT_TEST_SUITE(SharedPointer);
			CPPUNIT_TEST(push_topOfStackIsUserData);
			CPPUNIT_TEST(push_udHasSharedFlagSet);
			CPPUNIT_TEST(push_sharedUseCountEqualsTwo);
			CPPUNIT_TEST(push_pushTheSharedPtrTwice_sharedUseCountEqualsTwo);
			CPPUNIT_TEST(push_pushTheSharedPtrTwice_userdatasCompareEqual);
			CPPUNIT_TEST(gc_pushTheSharedPtrPopItAndThenCallGc_userCountEqualsOne);

			CPPUNIT_TEST(pull_pushThenPull_pullReturnsTrue);
			CPPUNIT_TEST(pull_pushDerivedThenPullBase_pullReturnsTrue);
			CPPUNIT_TEST(pull_pushDerivedAndPullConstantBase_pullReturnsTrue);
			CPPUNIT_TEST(pull_pushDerivedPullConstantBaseThenCallGc_useCountEqualsTwo);

			CPPUNIT_TEST(gc_pushSharedThenPullAndGc_useCountEqualsTwo);
			CPPUNIT_TEST(pull_pushSharedThenPull_resultEqualsInput);

			CPPUNIT_TEST(push_pushBaseAndThenPushDerived_topTwoIndiciesCompareEqual);
			CPPUNIT_TEST(pull_pushDerivedPullBaseThenCallGc_useCountEqualsTwo);
			CPPUNIT_TEST(push_pushBaseAndThenPushDerived_useCountEqualsTwo);

			CPPUNIT_TEST(call_pushSharedObjectAndCallFunction_callReturnsTrue);
			CPPUNIT_TEST(call_pushSharedObjectAndCallFunction_countMemberEqualsOne);
			CPPUNIT_TEST(call_pushSharedObjectAndCallConstantFunction_globalCalledCountEqualsOne);

			CPPUNIT_TEST(pull_pushSharedPointerPullNormalPointer_pointerEqualsInputsGet);
			CPPUNIT_TEST(pull_pushSharedPtrToDerivedPullRawBasePointer_rawPointerComparesEqualToDerivedCastToBase);

			CPPUNIT_TEST(test_inTraits_pullTypeIsSharedPointerStub1);
			CPPUNIT_TEST(ref_is_not_shared_type);

			CPPUNIT_TEST(call_memberFunctionPassingSharedPointer_useCountEqualsTwo);
			CPPUNIT_TEST(call_memberFunctionPassingSharedPointer_memberPointerEqualsInput);
			CPPUNIT_TEST(inTrait_refToSharedObject_pullTypeIsSharedPointerStub1);
			CPPUNIT_TEST(call_memberFunctionPassingRefToSharedPointer_useCountEqualsTwo);

			CPPUNIT_TEST(functionReturnTrait_sharedObject_pullTypeIsSharedPointerStub1);
			CPPUNIT_TEST(functionReturnTrait_refToSharedObject_pullTypeIsSharedPointerStub1);
			CPPUNIT_TEST(functionReturn_returnsSharedObject_useCountEqualsTwo);
			CPPUNIT_TEST(functionReturn_returnsSharedObjectPullReturn_resultEqualsInput);
			CPPUNIT_TEST(functionReturn_returnsRefToSharedObjectPullReturn_resultEqualsInput);

			CPPUNIT_TEST(defaultConstructorWithOutSharedTag_createAndReturn_topOfStackIsNotSharedPtr);
			CPPUNIT_TEST(defaultConstructorSharedTag_createAndReturn_topOfStackIsSharedPtr);

			CPPUNIT_TEST(inTrait_sharedPointerToConstType_isConst);

#		if OOLUA_STORE_LAST_ERROR == 1
			CPPUNIT_TEST(pull_sharedPointerWhenIntIsOnTheStack_pullReturnsFalse);
			CPPUNIT_TEST(call_memberFunctionWhichWantsSharedPointerYetIntIsOnTheStack_callReturnsFalse);
#		endif

#		if OOLUA_USE_EXCEPTIONS == 1
			CPPUNIT_TEST(pull_sharedPointerWhenIntIsOnTheStack_throwsOoluaTypeError);
			CPPUNIT_TEST(call_memberFunctionWhichWantsSharedPointerYetIntIsOnTheStack_throwsOoluaRuntimeError);
#		endif
			CPPUNIT_TEST(push_sharedPointerToConstType_topOfStackIsConst);
			CPPUNIT_TEST(push_sharedPointerToConstTypeThenSameInstanceAsNoneConst_firstIndexIsNoneConst);
			CPPUNIT_TEST(push_sharedPointerToConstTypeThenSameInstanceAsNoneConst_secondIndexIsNoneConst);
			CPPUNIT_TEST(push_sharedPointerToConstTypeThenSameInstanceAsNoneConst_useCountEqualsThree);
			CPPUNIT_TEST(pull_pushConstThenPullConst_pullReturnsTrue);
			CPPUNIT_TEST(pull_pushConstThenPullConst_useCountEqualsTwo);
			CPPUNIT_TEST(pull_pushNoneConstThenPullConst_useCountEqualsTwo);
			CPPUNIT_TEST(pull_pushConstThenPullNoneConst_callsPanic);

#if OOLUA_USE_EXCEPTIONS == 1
			CPPUNIT_TEST(pull_pushSharedThenPullNoneRelatedSharedConst_throwsOoluaTypeError);
#elif OOLUA_STORE_LAST_ERROR == 1
			CPPUNIT_TEST(pull_pushSharedThenPullNoneRelatedSharedConst_pullReturnsFalse);
			CPPUNIT_TEST(pull_pushSharedThenPullNoneRelatedSharedConst_lastErrorIsNotEmpty);
#endif

			CPPUNIT_TEST(call_memberFunctionTakingConstSharedParam_paramUseCountEqualsTwo);
			CPPUNIT_TEST(call_memberFunctionTakingConstSharedParam_paramAndMemberAreEqual);
			CPPUNIT_TEST(call_memberFunctionReturnsSharedConst_resultEqualsMember);
			CPPUNIT_TEST(call_memberFunctionReturnsSharedConst_memberUseCountEqualsTwo);

			CPPUNIT_TEST(maybeNullTrait_isIntegral_equalsTrue);
			CPPUNIT_TEST(maybeNullTrait_constType_isConstantEqualsTrue);
			CPPUNIT_TEST(maybeNullTrait_noneConstType_isConstantEqualsFalse);
			CPPUNIT_TEST(maybeNull_instanceReturnsValidSharedPtr_topOfStackIsUserData);
			CPPUNIT_TEST(maybeNull_instanceReturnsValidSharedPtr_useCountEqualsTwo);
			CPPUNIT_TEST(maybeNull_instanceReturnsNullSharedPtr_topOfStackIsNil);
			CPPUNIT_TEST(publicMember_instanceIsNullSharedPointer_topOfStackIsNil);

			CPPUNIT_TEST(classHasSharedReturn_functionReturnsOnTheStack_topOfStackSharedFlagIsSet);
			CPPUNIT_TEST(defaultNoSharedReturn_functionReturnsOnTheStack_topOfStackSharedFlagIsNotSet);

			CPPUNIT_TEST(sharedReturn_functionReturnsPtr_topOfStackSharedFlagIsSet);
			CPPUNIT_TEST(sharedReturn_functionReturnsPtr_topOfStackConstFlagIsNotSet);
			CPPUNIT_TEST(sharedReturn_functionReturnsPtrConst_topOfStackSharedFlagIsSet);
			CPPUNIT_TEST(sharedReturn_functionReturnsPtrConst_topOfStackConstFlagIsSet);

			CPPUNIT_TEST(sharedReturnMaybeNull_functionReturnsNullPointer_topOfStackIsNil);
			CPPUNIT_TEST(sharedReturnMaybeNull_functionReturnsNoneNullPointer_topOfStackSharedFlagIsSet);

			CPPUNIT_TEST(collisionIncludingSharedPtr_popInstancesThenCallGc_ThereIsNotEntryInTheWeakTableForThePointer);
			CPPUNIT_TEST(collisionIncludingNonePodSharedPtr_popInstancesThenCallGc_ThereIsNotEntryInTheWeakTableForThePointer);
		CPPUNIT_TEST_SUITE_END();
		OOLUA::Script* m_lua;
	public:
		SharedPointer()
			: m_lua(0)
		{}
		void setUp()
		{
			m_lua = new OOLUA::Script;
		}
		void tearDown()
		{
			delete m_lua;
		}
		void set_gc_to_false(int index)
		{
			OOLUA::INTERNAL::Lua_ud* ud = static_cast<OOLUA::INTERNAL::Lua_ud*>(lua_touserdata(m_lua->state(), index));
			if (ud) OOLUA::INTERNAL::userdata_gc_value(ud, false);
		}

		bool stack_index_ud_shared_flag(int index)
		{
			OOLUA::INTERNAL::Lua_ud* ud = static_cast<OOLUA::INTERNAL::Lua_ud*>(lua_touserdata(m_lua->state(), index));
			return OOLUA::INTERNAL::userdata_is_shared_ptr(ud);
		}

		void assert_stack_index_const_value_equals(int index, bool value)
		{
			OOLUA::INTERNAL::Lua_ud* ud = static_cast<OOLUA::INTERNAL::Lua_ud*>(lua_touserdata(m_lua->state(), index));
			CPPUNIT_ASSERT_EQUAL(value, OOLUA::INTERNAL::userdata_is_constant(ud));
		}

		void push_topOfStackIsUserData()
		{
			m_lua->register_class<Stub1>();
			OOLUA_SHARED_TYPE<Stub1> sp(new Stub1);
			m_lua->push(sp);
			set_gc_to_false(-1);
			CPPUNIT_ASSERT_EQUAL(LUA_TUSERDATA, lua_type(m_lua->state(), -1));
		}
		void push_udHasSharedFlagSet()
		{
			m_lua->register_class<Stub1>();
			OOLUA_SHARED_TYPE<Stub1> sp(new Stub1);
			m_lua->push(sp);
			set_gc_to_false(-1);
			OOLUA::INTERNAL::Lua_ud* ud = static_cast<OOLUA::INTERNAL::Lua_ud*>(lua_touserdata(m_lua->state(), -1));
			CPPUNIT_ASSERT_EQUAL(true, OOLUA::INTERNAL::userdata_is_shared_ptr(ud));
		}
		void push_sharedUseCountEqualsTwo()
		{
			m_lua->register_class<Stub1>();
			OOLUA_SHARED_TYPE<Stub1> sp(new Stub1);
			m_lua->push(sp);
			set_gc_to_false(-1);
			CPPUNIT_ASSERT_EQUAL(long(2), sp.use_count());
		}
		void push_pushTheSharedPtrTwice_sharedUseCountEqualsTwo()
		{
			m_lua->register_class<Stub1>();
			OOLUA_SHARED_TYPE<Stub1> sp(new Stub1);
			m_lua->push(sp);
			m_lua->push(sp);
			set_gc_to_false(-1);
			set_gc_to_false(-2);//we will get a horrible error until the same userdata is used
			CPPUNIT_ASSERT_EQUAL(long(2), sp.use_count());
		}
		void push_pushTheSharedPtrTwice_userdatasCompareEqual()
		{
			m_lua->register_class<Stub1>();
			OOLUA_SHARED_TYPE<Stub1> sp(new Stub1);
			m_lua->push(sp);
			m_lua->push(sp);
			set_gc_to_false(-1);
			set_gc_to_false(-2);//we will get a horrible error until the same userdata is used
			CPPUNIT_ASSERT_EQUAL(true, OOLUA::idxs_equal(m_lua->state(), -1, -2));
		}
		void gc_pushTheSharedPtrPopItAndThenCallGc_userCountEqualsOne()
		{
			m_lua->register_class<Stub1>();
			OOLUA_SHARED_TYPE<Stub1> sp(new Stub1);
			m_lua->push(sp);
			lua_pop(m_lua->state(), 1);
			m_lua->gc();
			CPPUNIT_ASSERT_EQUAL(long(1), sp.use_count());
		}

		void pull_pushThenPull_pullReturnsTrue()
		{
			m_lua->register_class<Stub1>();
			OOLUA_SHARED_TYPE<Stub1> sp(new Stub1);
			m_lua->push(sp);
			OOLUA_SHARED_TYPE<Stub1> result;
			CPPUNIT_ASSERT_EQUAL(true, m_lua->pull(result));
		}

		void gc_pushSharedThenPullAndGc_useCountEqualsTwo()
		{
			m_lua->register_class<Stub1>();
			OOLUA_SHARED_TYPE<Stub1> sp(new Stub1);
			m_lua->push(sp);
			OOLUA_SHARED_TYPE<Stub1> result;
			m_lua->pull(result);
			m_lua->gc();
			CPPUNIT_ASSERT_EQUAL(long(2), result.use_count());
		}
		void pull_pushSharedThenPull_resultEqualsInput()
		{
			m_lua->register_class<Stub1>();
			OOLUA_SHARED_TYPE<Stub1> input(new Stub1);
			m_lua->push(input);
			OOLUA_SHARED_TYPE<Stub1> result;
			m_lua->pull(result);
			CPPUNIT_ASSERT(input == result);
		}

		void push_pushBaseAndThenPushDerived_topTwoIndiciesCompareEqual()
		{
			m_lua->register_class<Derived1Abstract1>();
			OOLUA_SHARED_TYPE<Derived1Abstract1> derived(new Derived1Abstract1);
			OOLUA_SHARED_TYPE<Abstract1> base(derived);

			m_lua->push(base);
			m_lua->push(derived);
			CPPUNIT_ASSERT_EQUAL(true, OOLUA::idxs_equal(m_lua->state(), -1, -2));
		}

		void pull_pushDerivedThenPullBase_pullReturnsTrue()
		{
			m_lua->register_class<Derived1Abstract1>();
			OOLUA_SHARED_TYPE<Derived1Abstract1> derived(new Derived1Abstract1);
			OOLUA_SHARED_TYPE<Abstract1> base;

			m_lua->push(derived);
			CPPUNIT_ASSERT_EQUAL(true, m_lua->pull(base));
		}
		void pull_pushDerivedAndPullConstantBase_pullReturnsTrue()
		{
			m_lua->register_class<Derived1Abstract1>();
			OOLUA_SHARED_TYPE<Derived1Abstract1> derived(new Derived1Abstract1);
			OOLUA_SHARED_TYPE<Abstract1 const> base;

			m_lua->push(derived);
			CPPUNIT_ASSERT_EQUAL(true, m_lua->pull(base));
		}
		void pull_pushDerivedPullConstantBaseThenCallGc_useCountEqualsTwo()
		{
			m_lua->register_class<Derived1Abstract1>();
			OOLUA_SHARED_TYPE<Derived1Abstract1> derived(new Derived1Abstract1);
			OOLUA_SHARED_TYPE<Abstract1 const> base;

			m_lua->push(derived);
			m_lua->pull(base);
			m_lua->gc();

			CPPUNIT_ASSERT_EQUAL(long(2), derived.use_count());
		}

		void pull_pushDerivedPullBaseThenCallGc_useCountEqualsTwo()
		{
			m_lua->register_class<Derived1Abstract1>();
			OOLUA_SHARED_TYPE<Derived1Abstract1> derived(new Derived1Abstract1);
			OOLUA_SHARED_TYPE<Abstract1> base;

			m_lua->push(derived);
			m_lua->pull(base);
			m_lua->gc();
			CPPUNIT_ASSERT_EQUAL(long(2), derived.use_count());
		}

		void push_pushBaseAndThenPushDerived_useCountEqualsTwo()
		{
			m_lua->register_class<Derived1Abstract1>();
			OOLUA_SHARED_TYPE<Derived1Abstract1> derived(new Derived1Abstract1);

			{
				//force a release for base making the counter two
				//one C++ side and one for the two userdatas Lua side
				OOLUA_SHARED_TYPE<Abstract1> base(derived);
				m_lua->push(base);
				m_lua->push(derived);
			}
			CPPUNIT_ASSERT_EQUAL(long(2), derived.use_count());
		}

		void call_pushSharedObjectAndCallFunction_callReturnsTrue()
		{
			m_lua->register_class<SharedFoo>();
			OOLUA_SHARED_TYPE<SharedFoo> foo(new SharedFoo);
			m_lua->run_chunk("return function(obj) obj:no_param_function() end");
			bool result = m_lua->call(1, foo);
			std::cout <<OOLUA::get_last_error(m_lua->state());
			CPPUNIT_ASSERT_EQUAL(true, result);
		}

		void call_pushSharedObjectAndCallFunction_countMemberEqualsOne()
		{
			m_lua->register_class<SharedFoo>();
			OOLUA_SHARED_TYPE<SharedFoo> foo(new SharedFoo);
			m_lua->run_chunk("return function(obj) obj:no_param_function() end");
			m_lua->call(1, foo);
			CPPUNIT_ASSERT_EQUAL(int(1), foo->count); //NOLINT(readability/casting)
		}
		void call_pushSharedObjectAndCallConstantFunction_globalCalledCountEqualsOne()
		{
			m_lua->register_class<SharedFoo>();
			OOLUA_SHARED_TYPE<SharedFoo> foo(new SharedFoo);
			m_lua->run_chunk("return function(obj) obj:no_param_function_const() end");
			called = 0;
			m_lua->call(1, foo);
			CPPUNIT_ASSERT_EQUAL(int(1), called); //NOLINT(readability/casting)
		}

		//TODO Hmm should this be disallowed?
		void pull_pushSharedPointerPullNormalPointer_pointerEqualsInputsGet()
		{
			m_lua->register_class<SharedFoo>();
			OOLUA_SHARED_TYPE<SharedFoo> input(new SharedFoo);
			SharedFoo* result;
			m_lua->push(input);
			m_lua->pull(result);
			CPPUNIT_ASSERT_EQUAL(input.get(), result);
		}

		void pull_pushSharedPtrToDerivedPullRawBasePointer_rawPointerComparesEqualToDerivedCastToBase()
		{
			m_lua->register_class<TwoAbstractBases>();
			OOLUA_SHARED_TYPE<TwoAbstractBases> input(new TwoAbstractBases);
			m_lua->push(input);
			Abstract2* result;
			m_lua->pull(result);
			CPPUNIT_ASSERT_EQUAL(result, static_cast<Abstract2*>(input.get()));
		}

		void test_inTraits_compiles()
		{
			OOLUA::in_p<OOLUA_SHARED_TYPE<Stub1> > test1; (void)test1;
		}
		void test_inTraits_pullTypeIsSharedPointerStub1()
		{
			typedef OOLUA_SHARED_TYPE<Stub1> shared;
			int pull_type_is_same = LVD::is_same<shared, OOLUA::in_p<shared>::pull_type>::value;
			CPPUNIT_ASSERT_EQUAL(1, pull_type_is_same);
		}

		void ref_is_not_shared_type()
		{
			int result = OOLUA::INTERNAL::is_shared_type<OOLUA::Lua_func_ref>::value;
			CPPUNIT_ASSERT_EQUAL(0, result);
		}

		void call_memberFunctionPassingSharedPointer_useCountEqualsTwo()
		{
			m_lua->register_class<SharedFoo>();
			SharedFoo object;
			OOLUA_SHARED_TYPE<SharedFoo> param(new SharedFoo);
			m_lua->run_chunk("return function(obj, param) obj:shared_param(param) end");
			m_lua->call(1, &object, param);
			m_lua->gc();
			CPPUNIT_ASSERT_EQUAL(long(2), param.use_count());
		}

		void call_memberFunctionPassingSharedPointer_memberPointerEqualsInput()
		{
			m_lua->register_class<SharedFoo>();
			SharedFoo object;
			OOLUA_SHARED_TYPE<SharedFoo> param(new SharedFoo);
			m_lua->run_chunk("return function(obj, param) obj:shared_param(param) end");
			m_lua->call(1, &object, param);
			m_lua->gc();
			CPPUNIT_ASSERT_EQUAL(param.get(), object.m_shared.get());
		}

		void inTrait_refToSharedObject_pullTypeIsSharedPointerStub1()
		{
			typedef OOLUA_SHARED_TYPE<Stub1> shared;
			int pull_type_is_same = LVD::is_same<shared, OOLUA::in_p<shared&>::pull_type>::value;
			CPPUNIT_ASSERT_EQUAL(1, pull_type_is_same);
		}

		void call_memberFunctionPassingRefToSharedPointer_useCountEqualsTwo()
		{
			m_lua->register_class<SharedFoo>();
			SharedFoo object;
			OOLUA_SHARED_TYPE<SharedFoo> param(new SharedFoo);
			m_lua->run_chunk("return function(obj, param) obj:shared_param_ref(param) end");
			m_lua->call(1, &object, param);
			m_lua->gc();
			CPPUNIT_ASSERT_EQUAL(long(2), param.use_count());
		}

		void functionReturnTrait_sharedObject_pullTypeIsSharedPointerStub1()
		{
			typedef OOLUA_SHARED_TYPE<Stub1> shared;
			int pull_type_is_same = LVD::is_same<shared, OOLUA::INTERNAL::function_return<shared>::pull_type>::value;
			CPPUNIT_ASSERT_EQUAL(1, pull_type_is_same);
		}

		void functionReturnTrait_refToSharedObject_pullTypeIsSharedPointerStub1()
		{
			typedef OOLUA_SHARED_TYPE<Stub1> shared;
			int pull_type_is_same = LVD::is_same<shared, OOLUA::INTERNAL::function_return<shared&>::pull_type>::value;
			CPPUNIT_ASSERT_EQUAL(1, pull_type_is_same);
		}

		void functionReturn_returnsSharedObject_useCountEqualsTwo()
		{
			m_lua->register_class<SharedFoo>();
			SharedFoo object;
			object.m_shared = OOLUA_SHARED_TYPE<SharedFoo>(new SharedFoo);

			m_lua->run_chunk("return function(obj) return obj:returnsSharedPtr() end");
			m_lua->call(1, &object);
			CPPUNIT_ASSERT_EQUAL(long(2), object.m_shared.use_count());
		}

		void functionReturn_returnsSharedObjectPullReturn_resultEqualsInput()
		{
			m_lua->register_class<SharedFoo>();
			SharedFoo object;
			object.m_shared = OOLUA_SHARED_TYPE<SharedFoo>(new SharedFoo);

			m_lua->run_chunk("return function(obj) return obj:returnsSharedPtr() end");
			m_lua->call(1, &object);
			OOLUA_SHARED_TYPE<SharedFoo> result;
			m_lua->pull(result);
			CPPUNIT_ASSERT_EQUAL(object.m_shared.get(), result.get());
		}

		void functionReturn_returnsRefToSharedObjectPullReturn_resultEqualsInput()
		{
			m_lua->register_class<SharedFoo>();
			SharedFoo object;
			object.m_shared = OOLUA_SHARED_TYPE<SharedFoo>(new SharedFoo);

			m_lua->run_chunk("return function(obj) return obj:returnsSharedPtr() end");
			m_lua->call(1, &object);
			OOLUA_SHARED_TYPE<SharedFoo> result;
			m_lua->pull(result);
			CPPUNIT_ASSERT_EQUAL(object.m_shared.get(), result.get());
		}

		void defaultConstructorWithOutSharedTag_createAndReturn_topOfStackIsNotSharedPtr()
		{
			m_lua->register_class<Stub1>();
			m_lua->run_chunk("return Stub1.new()");
			OOLUA::INTERNAL::Lua_ud* ud = static_cast<OOLUA::INTERNAL::Lua_ud*>(lua_touserdata(m_lua->state(), 1));

			CPPUNIT_ASSERT_EQUAL(false, OOLUA::INTERNAL::userdata_is_shared_ptr(ud));
		}

		void defaultConstructorSharedTag_createAndReturn_topOfStackIsSharedPtr()
		{
			m_lua->register_class<SharedConstructor>();
			m_lua->run_chunk("return SharedConstructor.new()");
			OOLUA::INTERNAL::Lua_ud* ud = static_cast<OOLUA::INTERNAL::Lua_ud*>(lua_touserdata(m_lua->state(), 1));

			CPPUNIT_ASSERT_EQUAL(true, OOLUA::INTERNAL::userdata_is_shared_ptr(ud));
		}

#	if OOLUA_STORE_LAST_ERROR == 1
		void pull_sharedPointerWhenIntIsOnTheStack_pullReturnsFalse()
		{
			m_lua->register_class<Stub1>();
			m_lua->run_chunk("return 1");
			OOLUA_SHARED_TYPE<Stub1> result;
			CPPUNIT_ASSERT_EQUAL(false, m_lua->pull(result));
		}
		void call_memberFunctionWhichWantsSharedPointerYetIntIsOnTheStack_callReturnsFalse()
		{
			m_lua->register_class<SharedFoo>();
			SharedFoo object;
			m_lua->run_chunk("return function(obj) obj:shared_param(1) end");
			bool result = m_lua->call(1, &object);
			CPPUNIT_ASSERT_EQUAL(false, result);
		}
#	endif

#	if OOLUA_USE_EXCEPTIONS == 1
		void pull_sharedPointerWhenIntIsOnTheStack_throwsOoluaTypeError()
		{
			m_lua->register_class<Stub1>();
			m_lua->run_chunk("return 1");
			OOLUA_SHARED_TYPE<Stub1> result;
			CPPUNIT_ASSERT_THROW(m_lua->pull(result), OOLUA::Type_error);
		}
		void call_memberFunctionWhichWantsSharedPointerYetIntIsOnTheStack_throwsOoluaRuntimeError()
		{
			m_lua->register_class<SharedFoo>();
			SharedFoo object;
			m_lua->run_chunk("return function(obj) obj:shared_param(1) end");
			CPPUNIT_ASSERT_THROW((m_lua->call(1, &object)), OOLUA::Runtime_error);
		}
#	endif

		void inTrait_sharedPointerToConstType_isConst()
		{
			typedef OOLUA_SHARED_TYPE<SharedFoo const> const_shared;
			int value = OOLUA::in_p<const_shared>::is_constant;
			CPPUNIT_ASSERT_EQUAL(1, value);
		}

		void push_sharedPointerToConstType_topOfStackIsConst()
		{
			OOLUA_SHARED_TYPE<SharedFoo const> const_shared(new SharedFoo);
			m_lua->register_class<SharedFoo>();
			m_lua->push(const_shared);
			assert_stack_index_const_value_equals(-1, true);
		}

		void push_sharedPointerToConstTypeThenSameInstanceAsNoneConst_firstIndexIsNoneConst()
		{
			OOLUA_SHARED_TYPE<SharedFoo> none_const_shared(new SharedFoo);
			OOLUA_SHARED_TYPE<SharedFoo const> const_shared(none_const_shared);
			m_lua->register_class<SharedFoo>();

			m_lua->push(const_shared);
			m_lua->push(none_const_shared);

			assert_stack_index_const_value_equals(1, false);
		}

		void push_sharedPointerToConstTypeThenSameInstanceAsNoneConst_secondIndexIsNoneConst()
		{
			OOLUA_SHARED_TYPE<SharedFoo> none_const_shared(new SharedFoo);
			OOLUA_SHARED_TYPE<SharedFoo const> const_shared(none_const_shared);
			m_lua->register_class<SharedFoo>();

			m_lua->push(const_shared);
			m_lua->push(none_const_shared);

			assert_stack_index_const_value_equals(2, false);
		}

		void push_sharedPointerToConstTypeThenSameInstanceAsNoneConst_useCountEqualsThree()
		{
			OOLUA_SHARED_TYPE<SharedFoo> none_const_shared(new SharedFoo);
			OOLUA_SHARED_TYPE<SharedFoo const> const_shared(none_const_shared);
			m_lua->register_class<SharedFoo>();

			m_lua->push(const_shared);
			m_lua->push(none_const_shared);

			CPPUNIT_ASSERT_EQUAL(long(3), none_const_shared.use_count());
		}

		void pull_pushConstThenPullConst_pullReturnsTrue()
		{
			OOLUA_SHARED_TYPE<SharedFoo const> shared(new SharedFoo);
			m_lua->register_class<SharedFoo>();
			m_lua->push(shared);
			OOLUA_SHARED_TYPE<SharedFoo const> result;

			CPPUNIT_ASSERT_EQUAL(true, m_lua->pull(result));
		}

		void pull_pushConstThenPullConst_useCountEqualsTwo()
		{
			OOLUA_SHARED_TYPE<SharedFoo const> shared(new SharedFoo);
			m_lua->register_class<SharedFoo>();
			m_lua->push(shared);
			OOLUA_SHARED_TYPE<SharedFoo const> result;
			m_lua->pull(result);
			m_lua->gc();
			CPPUNIT_ASSERT_EQUAL(long(2), shared.use_count());
		}

		void pull_pushNoneConstThenPullConst_useCountEqualsTwo()
		{
			OOLUA_SHARED_TYPE<SharedFoo> shared(new SharedFoo);
			m_lua->register_class<SharedFoo>();
			m_lua->push(shared);
			OOLUA_SHARED_TYPE<SharedFoo const> result;
			m_lua->pull(result);
			m_lua->gc();
			CPPUNIT_ASSERT_EQUAL(long(2), shared.use_count());
		}

		bool isLuaJIT2()
		{
			/*
			jit.version_num
			Contains the version number of the LuaJIT core.
			Version xx.yy.zz is represented by the decimal number xxyyzz.
			*/
			m_lua->run_chunk("local res, ret = pcall( "
								"function() return require('jit').version_num >= 20000 end) "
							"return res == true and ret == true");
			bool result; m_lua->pull(result);
			return result;
		}
		/* ====================== LuaJIT2 protected test ===========================*/
		void pull_pushConstThenPullNoneConst_callsPanic()
		{
			if(isLuaJIT2()) return;

			OOLUA_SHARED_TYPE<SharedFoo const> shared(new SharedFoo);
			m_lua->register_class<SharedFoo>();
			m_lua->push(shared);
			OOLUA_SHARED_TYPE<SharedFoo> result;

			lua_atpanic(*m_lua, &OOLua_panic);

			if (setjmp(mark) == 0)
			{
				m_lua->pull(result);
				CPPUNIT_ASSERT_EQUAL(false, true);//never jumped back
			}
			else
				CPPUNIT_ASSERT_EQUAL(true, true);//we hit the at panic
		}
		/* ====================== LuaJIT2 protected test ===========================*/

#	if OOLUA_USE_EXCEPTIONS == 1
		void pull_pushSharedThenPullNoneRelatedSharedConst_throwsOoluaTypeError()
		{
			m_lua->register_class<SharedFoo>();
			m_lua->register_class<SharedConstructor>();
			OOLUA_SHARED_TYPE<SharedFoo> shared(new SharedFoo);
			m_lua->push(shared);
			OOLUA_SHARED_TYPE<SharedConstructor const> unrelated_shared;;
			CPPUNIT_ASSERT_THROW(m_lua->pull(unrelated_shared), OOLUA::Type_error);
		}
#endif

#if OOLUA_STORE_LAST_ERROR == 1
		void pull_pushSharedThenPullNoneRelatedSharedConst_pullReturnsFalse()
		{
			m_lua->register_class<SharedFoo>();
			m_lua->register_class<SharedConstructor>();
			OOLUA_SHARED_TYPE<SharedFoo> shared(new SharedFoo);
			m_lua->push(shared);
			OOLUA_SHARED_TYPE<SharedConstructor const> unrelated_shared;;
			CPPUNIT_ASSERT_EQUAL(false, m_lua->pull(unrelated_shared));
		}
		void pull_pushSharedThenPullNoneRelatedSharedConst_lastErrorIsNotEmpty()
		{
			m_lua->register_class<SharedFoo>();
			m_lua->register_class<SharedConstructor>();
			OOLUA_SHARED_TYPE<SharedFoo> shared(new SharedFoo);
			m_lua->push(shared);
			OOLUA_SHARED_TYPE<SharedConstructor const> unrelated_shared;;
			m_lua->pull(unrelated_shared);
			CPPUNIT_ASSERT_EQUAL(false, OOLUA::get_last_error(*m_lua).empty());
		}
#endif


		void call_memberFunctionTakingConstSharedParam_paramUseCountEqualsTwo()
		{
			m_lua->register_class<SharedFoo>();
			SharedFoo object;
			OOLUA_SHARED_TYPE<SharedFoo const> param(new SharedFoo);
			m_lua->run_chunk("return function(obj, param) obj:shared_param_type_is_const(param) end");
			m_lua->call(1, &object, param);
			m_lua->gc();
			CPPUNIT_ASSERT_EQUAL(long(2), param.use_count());
		}

		void call_memberFunctionTakingConstSharedParam_paramAndMemberAreEqual()
		{
			m_lua->register_class<SharedFoo>();
			SharedFoo object;
			OOLUA_SHARED_TYPE<SharedFoo const> param(new SharedFoo);
			m_lua->run_chunk("return function(obj, param) obj:shared_param_type_is_const(param) end");
			m_lua->call(1, &object, param);
			CPPUNIT_ASSERT_EQUAL(object.m_const_shared.get(), param.get());
		}

		void call_memberFunctionReturnsSharedConst_resultEqualsMember()
		{
			m_lua->register_class<SharedFoo>();
			SharedFoo object;
			object.m_const_shared = OOLUA_SHARED_TYPE<SharedFoo const>(new SharedFoo);
			m_lua->run_chunk("return function(obj) return obj:returnsSharedConstPtr() end");
			m_lua->call(1, &object);
			OOLUA_SHARED_TYPE<SharedFoo const> result;
			m_lua->pull(result);
			CPPUNIT_ASSERT_EQUAL(object.m_const_shared.get(), result.get());
		}

		void call_memberFunctionReturnsSharedConst_memberUseCountEqualsTwo()
		{
			m_lua->register_class<SharedFoo>();
			SharedFoo object;
			object.m_const_shared = OOLUA_SHARED_TYPE<SharedFoo const>(new SharedFoo);
			m_lua->run_chunk("return function(obj) return obj:returnsSharedConstPtr() end");
			m_lua->call(1, &object);
			OOLUA_SHARED_TYPE<SharedFoo const> result;
			m_lua->pull(result);
			m_lua->gc();
			CPPUNIT_ASSERT_EQUAL(long(2), object.m_const_shared.use_count());
		}

		void maybeNullTrait_compiles()
		{
			OOLUA::maybe_null<OOLUA_SHARED_TYPE<Stub1> > foo; (void)foo;
		}

		void maybeNullTrait_isIntegral_equalsTrue()
		{
			int is_integral_result = OOLUA::maybe_null<OOLUA_SHARED_TYPE<Stub1> >::is_integral;
			CPPUNIT_ASSERT_EQUAL(1, is_integral_result);
		}

		void maybeNullTrait_constType_isConstantEqualsTrue()
		{
			int is_constant_result = OOLUA::maybe_null<OOLUA_SHARED_TYPE<Stub1 const> >::is_constant;
			CPPUNIT_ASSERT_EQUAL(1, is_constant_result);
		}

		void maybeNullTrait_noneConstType_isConstantEqualsFalse()
		{
			int is_constant_result = OOLUA::maybe_null<OOLUA_SHARED_TYPE<Stub1> >::is_constant;
			CPPUNIT_ASSERT_EQUAL(0, is_constant_result);
		}

		void maybeNull_instanceReturnsValidSharedPtr_topOfStackIsUserData()
		{
			m_lua->register_class<SharedFoo>();
			SharedFoo instance;
			instance.m_shared = OOLUA_SHARED_TYPE<SharedFoo>(new SharedFoo);

			m_lua->run_chunk("return function(instance) return instance:returns_maybe_null_shared() end");
			m_lua->call(1, &instance);
			CPPUNIT_ASSERT_EQUAL(LUA_TUSERDATA, lua_type(m_lua->state(), -1));
		}
		void maybeNull_instanceReturnsValidSharedPtr_useCountEqualsTwo()
		{
			m_lua->register_class<SharedFoo>();
			SharedFoo instance;
			instance.m_shared = OOLUA_SHARED_TYPE<SharedFoo>(new SharedFoo);

			m_lua->run_chunk("return function(instance) return instance:returns_maybe_null_shared() end");
			m_lua->call(1, &instance);
			m_lua->gc();
			CPPUNIT_ASSERT_EQUAL(long(2), instance.m_shared.use_count());
		}

		void maybeNull_instanceReturnsNullSharedPtr_topOfStackIsNil()
		{
			m_lua->register_class<SharedFoo>();
			SharedFoo instance;

			m_lua->run_chunk("return function(instance) return instance:returns_maybe_null_shared() end");
			m_lua->call(1, &instance);
			CPPUNIT_ASSERT_EQUAL(LUA_TNIL, lua_type(m_lua->state(), -1));
		}
		void publicMember_instanceIsNullSharedPointer_topOfStackIsNil()
		{
			m_lua->register_class<SharedFoo>();
			SharedFoo instance;

			m_lua->run_chunk("return function(instance) return instance:get_m_shared() end");
			m_lua->call(1, &instance);
			CPPUNIT_ASSERT_EQUAL(LUA_TNIL, lua_type(m_lua->state(), -1));
		}

		void defaultNoSharedReturn_functionReturnsOnTheStack_topOfStackSharedFlagIsNotSet()
		{
			m_lua->register_class<DefaultNoSharedValueReturn>();
			m_lua->run_chunk("return DefaultNoSharedValueReturn.new():stack_return()");
			CPPUNIT_ASSERT_EQUAL(false, stack_index_ud_shared_flag(-1));
		}

		void classHasSharedReturn_functionReturnsOnTheStack_topOfStackSharedFlagIsSet()
		{
			m_lua->register_class<SharedValueReturn>();
			m_lua->run_chunk("return SharedValueReturn.new():stack_return()");
			CPPUNIT_ASSERT_EQUAL(true, stack_index_ud_shared_flag(-1));
		}
		void sharedReturn_functionReturnsPtr_topOfStackSharedFlagIsSet()
		{
			m_lua->register_class<PointerChangesToShared>();
			m_lua->run_chunk("return PointerChangesToShared.new():ptr()");
			CPPUNIT_ASSERT_EQUAL(true, stack_index_ud_shared_flag(-1));
		}
		void sharedReturn_functionReturnsPtr_topOfStackConstFlagIsNotSet()
		{
			m_lua->register_class<PointerChangesToShared>();
			m_lua->run_chunk("return PointerChangesToShared.new():ptr()");
			assert_stack_index_const_value_equals(-1, false);
		}

		void sharedReturn_functionReturnsPtrConst_topOfStackSharedFlagIsSet()
		{
			m_lua->register_class<PointerChangesToShared>();
			m_lua->run_chunk("return PointerChangesToShared.new():ptr_const()");
			CPPUNIT_ASSERT_EQUAL(true, stack_index_ud_shared_flag(-1));
		}

		void sharedReturn_functionReturnsPtrConst_topOfStackConstFlagIsSet()
		{
			m_lua->register_class<PointerChangesToShared>();
			m_lua->run_chunk("return PointerChangesToShared.new():ptr_const()");
			assert_stack_index_const_value_equals(-1, true);
		}

		void sharedReturnMaybeNull_functionReturnsNullPointer_topOfStackIsNil()
		{
			m_lua->register_class<PointerChangesToShared>();
			m_lua->run_chunk("return PointerChangesToShared.new():null()");
			CPPUNIT_ASSERT_EQUAL(LUA_TNIL, lua_type(m_lua->state(), -1));
		}

		void sharedReturnMaybeNull_functionReturnsNoneNullPointer_topOfStackSharedFlagIsSet()
		{
			m_lua->register_class<PointerChangesToShared>();
			m_lua->run_chunk("return PointerChangesToShared.new():none_null()");
			CPPUNIT_ASSERT_EQUAL(true, stack_index_ud_shared_flag(-1));
		}

		void collisionIncludingSharedPtr_popInstancesThenCallGc_ThereIsNotEntryInTheWeakTableForThePointer()
		{
			//userdata gc methods are called in reverse order of construction
			//therefore makesure the shared pointer gc is called second
			m_lua->register_class<Pod>();
			m_lua->register_class<PodWithFirstMemberPod>();
			OOLUA_SHARED_TYPE<PodWithFirstMemberPod> instance(new PodWithFirstMemberPod);
			m_lua->push(instance);
			m_lua->push(&instance->first_member);
			lua_pop(*m_lua, 2);
			m_lua->gc();

			int weakIndex = OOLUA::INTERNAL::push_weak_table(*m_lua);
			lua_pushlightuserdata(*m_lua, instance.get());
			lua_rawget(*m_lua, weakIndex);
			CPPUNIT_ASSERT_EQUAL(LUA_TNIL, lua_type(*m_lua, -1));
		}

		void collisionIncludingNonePodSharedPtr_popInstancesThenCallGc_ThereIsNotEntryInTheWeakTableForThePointer()
		{
			//userdata gc methods are called in reverse order of construction
			//therefore make sure the shared pointer gc is called second
			m_lua->register_class<Pod>();
			m_lua->register_class<NonePod>();
			OOLUA_SHARED_TYPE<NonePod> instance(new NonePod);
			m_lua->push(instance);
			m_lua->push(&instance->first_member);
			lua_pop(*m_lua, 2);
			m_lua->gc();

			int weakIndex = OOLUA::INTERNAL::push_weak_table(*m_lua);
			lua_pushlightuserdata(*m_lua, static_cast<PodWithFirstMemberPod*>(instance.get()));
			lua_rawget(*m_lua, weakIndex);
			CPPUNIT_ASSERT_EQUAL(LUA_TNIL, lua_type(*m_lua, -1));
		}
	};

	CPPUNIT_TEST_SUITE_REGISTRATION(SharedPointer);

#	endif //OOLUA_USE_SHARED_PTR
