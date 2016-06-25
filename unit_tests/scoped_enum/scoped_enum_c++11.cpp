#	include "oolua_tests_pch.h"
#	include "oolua.h"
#	include "common_cppunit_headers.h"

enum class scoped_enum{ ENUM = 1 };

/*[CppClassScopedEnum]*/
struct Has_scoped_enum
{
	enum class scoped_enum{INVALID, VALID};
	Has_scoped_enum()
		:e(scoped_enum::VALID)
	{}
	Has_scoped_enum(scoped_enum input)
		:e(input)
	{}
	scoped_enum e;
	void param(scoped_enum /*e*/){}
	scoped_enum return_enum(){return scoped_enum::VALID;}
};
/*[CppClassScopedEnum]*/

/*[ClassScopedEnumExpose]*/
OOLUA_PROXY(Has_scoped_enum)
	OOLUA_TAGS(Register_class_enums)
	OOLUA_CTORS(
		OOLUA_CTOR(Has_scoped_enum::scoped_enum)
	)
	OOLUA_ENUMS(
		OOLUA_SCOPED_ENUM(INVALID, scoped_enum::INVALID)
		OOLUA_SCOPED_ENUM(VALID, scoped_enum::VALID)
	)
	OOLUA_MGET_MSET(e)
	OOLUA_MFUNC(param)
	OOLUA_MFUNC(return_enum)
OOLUA_PROXY_END
/*[ClassScopedEnumExpose]*/

/*[ClassScopedEnumExport]*/
OOLUA_EXPORT_FUNCTIONS(Has_scoped_enum
						, param
						, return_enum
						, set_e)
OOLUA_EXPORT_FUNCTIONS_CONST(Has_scoped_enum
							, get_e)
/*[ClassScopedEnumExport]*/



class enums_11 : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(enums_11);
		CPPUNIT_TEST(isClass_passedClass_valueIsOne);
		CPPUNIT_TEST(isClass_passedInt_valueIsZero);
		CPPUNIT_TEST(isClass_passedScopedEnum_valueIsZero);
		CPPUNIT_TEST(isClass_passedVoid_valueIsZero);
		CPPUNIT_TEST(isClass_passedLuaCfunction_valueIsZero);

		CPPUNIT_TEST(isScopedEnum_passedClass_valueIsZero);
		CPPUNIT_TEST(isScopedEmum_passedNormalEnum_valueIsZero);
		CPPUNIT_TEST(isScopedEmum_passedInt_valueIsZero);
		CPPUNIT_TEST(isScopedEmum_passedVoid_valueIsZero);
		CPPUNIT_TEST(isScopedEmum_passedLuaCfunction_valueIsZero);
		CPPUNIT_TEST(isScopedEmum_passedScopedEnum_valueIsOne);

		CPPUNIT_TEST(scopedEnum_push_returnsTrue);
		CPPUNIT_TEST(scopedEnum_push_topOfStackIsNumber);
		CPPUNIT_TEST(scopedEnum_pull_compiles);

		CPPUNIT_TEST(enumParam_LuaPassesInt_runChunkReturnsTrue);
		CPPUNIT_TEST(enumReturn_functionReturnsEnum_runChunkReturnsTrue);
		CPPUNIT_TEST(enumReturn_functionReturnsEnum_resultEqualsValidEnum);

		CPPUNIT_TEST(publicMember_returnPublicMember_runChunkReturnsTrue);
		CPPUNIT_TEST(publicMember_returnPublicMember_resultEqualsValid);
		CPPUNIT_TEST(publicMember_inCppSetMemberToInvalidInLuaSetToValid_resultEqualsValid);

		CPPUNIT_TEST(constructor_passedInvalid_enumEqualsInvalid);
		CPPUNIT_TEST(constructor_passedValid_enumEqualsValid);
	CPPUNIT_TEST_SUITE_END();
	OOLUA::Script* m_lua;
public:
	enums_11()
	{}
	void setUp()
	{
		m_lua = new OOLUA::Script;
	}
	void tearDown()
	{
		delete m_lua;
	}

	void isClass_passedClass_valueIsOne()
	{
		CPPUNIT_ASSERT_EQUAL(static_cast<int>(1), static_cast<int>(LVD::is_class_type<enums_11>::value));
	}

	void isClass_passedInt_valueIsZero()
	{
		CPPUNIT_ASSERT_EQUAL(static_cast<int>(0), static_cast<int>(LVD::is_class_type<int>::value));
	}

	void isClass_passedScopedEnum_valueIsZero()
	{
		CPPUNIT_ASSERT_EQUAL(static_cast<int>(0), static_cast<int>(LVD::is_class_type<scoped_enum>::value));
	}

	void isClass_passedVoid_valueIsZero()
	{
		CPPUNIT_ASSERT_EQUAL(static_cast<int>(0), static_cast<int>(LVD::is_class_type<void>::value));
	}

	void isClass_passedLuaCfunction_valueIsZero()
	{
		CPPUNIT_ASSERT_EQUAL(static_cast<int>(0), static_cast<int>(LVD::is_class_type<int(*)(lua_State*)>::value)); // NOLINT
	}

	void isScopedEnum_passedClass_valueIsZero()
	{
		CPPUNIT_ASSERT_EQUAL(static_cast<int>(0), static_cast<int>(OOLUA::INTERNAL::is_scoped_enum<enums_11>::value));
	}

	void isScopedEmum_passedNormalEnum_valueIsZero()
	{
		enum normal_enum{normal_enum_value};
		CPPUNIT_ASSERT_EQUAL(static_cast<int>(0), static_cast<int>(OOLUA::INTERNAL::is_scoped_enum<normal_enum>::value));
	}

	void isScopedEmum_passedInt_valueIsZero()
	{
		CPPUNIT_ASSERT_EQUAL(static_cast<int>(0), static_cast<int>(OOLUA::INTERNAL::is_scoped_enum<int>::value));
	}

	void isScopedEmum_passedVoid_valueIsZero()
	{
		CPPUNIT_ASSERT_EQUAL(static_cast<int>(0), static_cast<int>(OOLUA::INTERNAL::is_scoped_enum<void>::value));
	}

	void isScopedEmum_passedLuaCfunction_valueIsZero()
	{
		CPPUNIT_ASSERT_EQUAL(static_cast<int>(0), static_cast<int>(OOLUA::INTERNAL::is_scoped_enum<int(*)(lua_State*)>::value)); // NOLINT
	}

	void isScopedEmum_passedScopedEnum_valueIsOne()
	{
		CPPUNIT_ASSERT_EQUAL(static_cast<int>(1), static_cast<int>(OOLUA::INTERNAL::is_scoped_enum<scoped_enum>::value));
	}

	void scopedEnum_push_returnsTrue()
	{
		CPPUNIT_ASSERT_EQUAL(true, m_lua->push(scoped_enum::ENUM));
	}

	void scopedEnum_push_topOfStackIsNumber()
	{
		m_lua->push(scoped_enum::ENUM);
		CPPUNIT_ASSERT_EQUAL(LUA_TNUMBER, lua_type(*m_lua, -1));
	}

	void scopedEnum_pull_compiles()
	{
		scoped_enum result;
		m_lua->push(scoped_enum::ENUM);
		m_lua->pull(result);
	}

	void enumParam_LuaPassesInt_runChunkReturnsTrue()
	{
		m_lua->register_class<Has_scoped_enum>();
		bool result = m_lua->run_chunk("local obj = Has_scoped_enum.new() "
										"obj:param(0)");
		CPPUNIT_ASSERT_EQUAL(true, result);
	}

	void enumReturn_functionReturnsEnum_runChunkReturnsTrue()
	{
		m_lua->register_class<Has_scoped_enum>();
		bool result = m_lua->run_chunk("return Has_scoped_enum.new():return_enum()");
		CPPUNIT_ASSERT_EQUAL(true, result);
	}

	void enumReturn_functionReturnsEnum_resultEqualsValidEnum()
	{
		m_lua->register_class<Has_scoped_enum>();
		m_lua->run_chunk("return Has_scoped_enum.new():return_enum()");
		Has_scoped_enum::scoped_enum result = Has_scoped_enum::scoped_enum::INVALID;
		m_lua->pull(result);
		CPPUNIT_ASSERT_EQUAL(static_cast<int>(Has_scoped_enum::scoped_enum::VALID), static_cast<int>(result));
	}

	void publicMember_returnPublicMember_runChunkReturnsTrue()
	{
		m_lua->register_class<Has_scoped_enum>();
		bool result = m_lua->run_chunk("return Has_scoped_enum.new():get_e()");
		std::cout <<OOLUA::get_last_error(*m_lua);
		CPPUNIT_ASSERT_EQUAL(true, result);
	}

	void publicMember_returnPublicMember_resultEqualsValid()
	{
		Has_scoped_enum::scoped_enum result = Has_scoped_enum::scoped_enum::INVALID;
		m_lua->register_class<Has_scoped_enum>();
		m_lua->run_chunk("return Has_scoped_enum.new():get_e()");
		m_lua->pull(result);
		CPPUNIT_ASSERT_EQUAL(static_cast<int>(Has_scoped_enum::scoped_enum::VALID), static_cast<int>(result));
	}

	/*[ClassScopedEnumUsage]*/
	void publicMember_inCppSetMemberToInvalidInLuaSetToValid_resultEqualsValid()
	{
		Has_scoped_enum instance;
		instance.e = Has_scoped_enum::scoped_enum::INVALID;
		m_lua->register_class<Has_scoped_enum>();
		m_lua->run_chunk("return function(obj) obj:set_e(Has_scoped_enum.VALID) end");
		m_lua->call(-1, &instance);
		CPPUNIT_ASSERT_EQUAL(static_cast<int>(Has_scoped_enum::scoped_enum::VALID), static_cast<int>(instance.e));
	}
	/*[ClassScopedEnumUsage]*/

	void constructor_passedInvalid_enumEqualsInvalid()
	{
		m_lua->register_class<Has_scoped_enum>();
		m_lua->run_chunk("return Has_scoped_enum.new(Has_scoped_enum.INVALID):get_e()");
		Has_scoped_enum::scoped_enum result = Has_scoped_enum::scoped_enum::VALID;
		m_lua->pull(result);
		CPPUNIT_ASSERT_EQUAL(static_cast<int>(Has_scoped_enum::scoped_enum::INVALID), static_cast<int>(result));
	}
	void constructor_passedValid_enumEqualsValid()
	{
		m_lua->register_class<Has_scoped_enum>();
		m_lua->run_chunk("return Has_scoped_enum.new(Has_scoped_enum.VALID):get_e()");
		Has_scoped_enum::scoped_enum result = Has_scoped_enum::scoped_enum::INVALID;
		m_lua->pull(result);
		CPPUNIT_ASSERT_EQUAL(static_cast<int>(Has_scoped_enum::scoped_enum::VALID), static_cast<int>(result));
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(enums_11);
