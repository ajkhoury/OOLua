#	include "oolua_tests_pch.h"
#	include "oolua.h"
#	include "common_cppunit_headers.h"

template<typename T>
struct NoneSharedTemplatedClass
{
	NoneSharedTemplatedClass value(){ return NoneSharedTemplatedClass(); }
	NoneSharedTemplatedClass* ptr(){ return this; }
	NoneSharedTemplatedClass& ref(){ return *this; }
};

OOLUA_PROXY(NoneSharedTemplatedClass<int>)
	OOLUA_MFUNC(value)
	OOLUA_MFUNC(ptr)
	OOLUA_MFUNC(ref)
OOLUA_PROXY_END

typedef NoneSharedTemplatedClass<int> TemplatedIntClass;

OOLUA_EXPORT_FUNCTIONS(TemplatedIntClass
						, value
						, ptr
						, ref
						)
OOLUA_EXPORT_FUNCTIONS_CONST(TemplatedIntClass)

class TemplatedClass : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(TemplatedClass);
		CPPUNIT_TEST(noneSharedTemplateClass_returnsInstanceByValue_stackTopSharedFlagIsNotSet);
		CPPUNIT_TEST(noneSharedTemplateClass_returnsInstanceByPtr_stackTopSharedFlagIsNotSet);
		CPPUNIT_TEST(noneSharedTemplateClass_returnsInstanceByRef_stackTopSharedFlagIsNotSet);
	CPPUNIT_TEST_SUITE_END();
	OOLUA::Script* m_lua;
public:
	TemplatedClass()
	{}
	void setUp()
	{
		m_lua = new OOLUA::Script;
	}
	void tearDown()
	{
		delete m_lua;
	}

	bool stack_index_ud_shared_flag(int index)
	{
		OOLUA::INTERNAL::Lua_ud* ud = static_cast<OOLUA::INTERNAL::Lua_ud*>(lua_touserdata(m_lua->state(), index));
		return OOLUA::INTERNAL::userdata_is_shared_ptr(ud);
	}

	void noneSharedTemplateClass_returnsInstanceByValue_stackTopSharedFlagIsNotSet()
	{
		m_lua->register_class<NoneSharedTemplatedClass<int> >();
		m_lua->run_chunk("return TemplatedIntClass.new():value()");
		CPPUNIT_ASSERT_EQUAL(false, stack_index_ud_shared_flag(-1));
	}
	void noneSharedTemplateClass_returnsInstanceByPtr_stackTopSharedFlagIsNotSet()
	{
		m_lua->register_class<NoneSharedTemplatedClass<int> >();
		m_lua->run_chunk("return TemplatedIntClass.new():ptr()");
		CPPUNIT_ASSERT_EQUAL(false, stack_index_ud_shared_flag(-1));
	}
	void noneSharedTemplateClass_returnsInstanceByRef_stackTopSharedFlagIsNotSet()
	{
		m_lua->register_class<NoneSharedTemplatedClass<int> >();
		m_lua->run_chunk("return TemplatedIntClass.new():ref()");
		CPPUNIT_ASSERT_EQUAL(false, stack_index_ud_shared_flag(-1));
	}
};
CPPUNIT_TEST_SUITE_REGISTRATION(TemplatedClass);
