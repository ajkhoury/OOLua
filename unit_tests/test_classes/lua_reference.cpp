#	include "oolua_tests_pch.h"
#	include "oolua.h"
#	include "common_cppunit_headers.h"



class Lua_reference : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(Lua_reference);
		CPPUNIT_TEST(defaultConstructor_storedStateEqualsNull);
		CPPUNIT_TEST(defaultConstructor_storedReferenceEqualsNoRef);
		CPPUNIT_TEST(luaStateConversionConstructor_passedValidState_stateReturnsInput);
		CPPUNIT_TEST(luaStateConversionConstructor_passedValidState_refReturnsNoRef);
		CPPUNIT_TEST(twoParamConstructor_passedValidState_stateReturnsInput);
		CPPUNIT_TEST(twoParamConstructor_passedValidRef_refReturnsInput);

		CPPUNIT_TEST(valid_defaultConstructor_validReturnsFalse);
		CPPUNIT_TEST(valid_validStatePassedToLuaStateConversionConstructor_validReturnsFalse);
		CPPUNIT_TEST(valid_copyConstructorPassedInvalidReference_validReturnsFalse);

		CPPUNIT_TEST(valid_nullStateAndNoneInvalidRefPassedToTwoParamConstructor_validReturnsFalse);
		CPPUNIT_TEST(valid_validStateAndRefNilPassedToTwoParamConstructor_validReturnsFalse);
		CPPUNIT_TEST(valid_validStateAndValidRefPassedToTwoParamConstructor_validReturnsTrue);

		CPPUNIT_TEST(copyConstructor_validRefPassedToConstructor_luaStateEqualsInput);
		CPPUNIT_TEST(copyConstructor_validRefPassedToConstructor_refIsRegistryCopyOfInput);

		CPPUNIT_TEST(release_whenValidRefGoesOutOfScopeDestructorCallsRelease_registryNoLongerContainsTheRef);
		CPPUNIT_TEST(release_callingSetRefOnValidRefWithNewValuesCallsRelease_registryNoLongerContainsTheRef);

		CPPUNIT_TEST(swap_afterSwap_rhsStateBeforeSwapIsAssignedToLhsState);
		CPPUNIT_TEST(swap_afterSwap_rhsRefBeforeSwapIsAssignedToLhsRef);
		CPPUNIT_TEST(swap_afterSwap_lhsStateBeforeSwapIsAssignedToRhsState);
		CPPUNIT_TEST(swap_afterSwap_lhsRefBeforeSwapIsAssignedToRhsRef);

		CPPUNIT_TEST(assignment_selfAssignmentOnValidRef_refIsRegistryCopyOfOriginalValue);
		CPPUNIT_TEST(assignment_assignmentToValidRef_registryNoLongerContainsTheValidRef);
		CPPUNIT_TEST(assignment_assignmentOfValidRefToInvalid_refIsRegistryCopyOfInput);


		CPPUNIT_TEST(equality_twoInvalidReferences_returnsTrue);
		CPPUNIT_TEST(equality_invalidRefAndValidRef_returnsfalse);
		CPPUNIT_TEST(equality_validRefAndInvalidRef_returnsfalse);
		CPPUNIT_TEST(equality_twoCopiesOfSameRef_returnsTrue);
		CPPUNIT_TEST(equality_twoDifferentRefs_returnsFalse);
		CPPUNIT_TEST(equality_twoDifferentRefsFromDifferentStatesInTheSameUniverse_returnsFalse);
		CPPUNIT_TEST(equality_sameRefFromDifferentStatesInTheSameUniverse_returnsTrue);
		CPPUNIT_TEST(equality_twoDifferentRefsFromDifferentUniverses_returnsFalse);
	CPPUNIT_TEST_SUITE_END();
	OOLUA::Script* m_lua;
public:
	Lua_reference()
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

	void change_an_invalid_reference_to_valid(OOLUA::Lua_func_ref& ref)
	{
		m_lua->load_chunk("return ");
		ref.set_ref(m_lua->state(), luaL_ref(*m_lua, LUA_REGISTRYINDEX));
	}
	void defaultConstructor_storedStateEqualsNull()
	{
		OOLUA::Lua_func_ref r;
		CPPUNIT_ASSERT_EQUAL(static_cast<lua_State*>(NULL), r.state());
	}

	void defaultConstructor_storedReferenceEqualsNoRef()
	{
		OOLUA::Lua_func_ref r;
		CPPUNIT_ASSERT_EQUAL(static_cast<int>(LUA_NOREF), r.ref());
	}

	void luaStateConversionConstructor_passedValidState_stateReturnsInput()
	{
		OOLUA::Lua_func_ref r(*m_lua);
		CPPUNIT_ASSERT_EQUAL(m_lua->state(), r.state());
	}

	void luaStateConversionConstructor_passedValidState_refReturnsNoRef()
	{
		OOLUA::Lua_func_ref r(*m_lua);
		CPPUNIT_ASSERT_EQUAL(static_cast<int>(LUA_NOREF), r.ref());
	}

	void twoParamConstructor_passedValidState_stateReturnsInput()
	{
		OOLUA::Lua_func_ref r(m_lua->state(), LUA_NOREF);
		CPPUNIT_ASSERT_EQUAL(m_lua->state(), r.state());
	}

	void twoParamConstructor_passedValidRef_refReturnsInput()
	{
		m_lua->load_chunk("return ");
		int input = luaL_ref(*m_lua, LUA_REGISTRYINDEX);
		OOLUA::Lua_func_ref r(NULL, input);
		CPPUNIT_ASSERT_EQUAL(input, r.ref());
	}

	void valid_defaultConstructor_validReturnsFalse()
	{
		OOLUA::Lua_func_ref r;
		CPPUNIT_ASSERT_EQUAL(false, r.valid());
	}

	void valid_validStatePassedToLuaStateConversionConstructor_validReturnsFalse()
	{
		OOLUA::Lua_func_ref r(*m_lua);
		CPPUNIT_ASSERT_EQUAL(false, r.valid());
	}

	void valid_copyConstructorPassedInvalidReference_validReturnsFalse()
	{
		OOLUA::Lua_func_ref param;
		OOLUA::Lua_func_ref r(param);
		CPPUNIT_ASSERT_EQUAL(false, r.valid());
	}

	void valid_nullStateAndNoneInvalidRefPassedToTwoParamConstructor_validReturnsFalse()
	{
		int none_invalid_ref_id = 1;
		OOLUA::Lua_func_ref r(NULL, none_invalid_ref_id);
		CPPUNIT_ASSERT_EQUAL(false, r.valid());
	}

	void valid_validStateAndRefNilPassedToTwoParamConstructor_validReturnsFalse()
	{
		OOLUA::Lua_func_ref r(m_lua->state(), LUA_REFNIL);
		CPPUNIT_ASSERT_EQUAL(false, r.valid());
	}

	void valid_validStateAndValidRefPassedToTwoParamConstructor_validReturnsTrue()
	{
		m_lua->load_chunk("return ");
		int input = luaL_ref(*m_lua, LUA_REGISTRYINDEX);
		OOLUA::Lua_func_ref r(m_lua->state(), input);
		CPPUNIT_ASSERT_EQUAL(true, r.valid());
	}


	void copyConstructor_validRefPassedToConstructor_luaStateEqualsInput()
	{
		m_lua->load_chunk("return ");
		OOLUA::Lua_func_ref input;
		m_lua->pull(input);

		OOLUA::Lua_func_ref ref(input);
		CPPUNIT_ASSERT_EQUAL(input.state(), ref.state());
	}

	void copyConstructor_validRefPassedToConstructor_refIsRegistryCopyOfInput()
	{
		m_lua->load_chunk("return ");
		OOLUA::Lua_func_ref input;
		m_lua->pull(input);

		OOLUA::Lua_func_ref result(input);
		lua_rawgeti(*m_lua, LUA_REGISTRYINDEX, input.ref());
		lua_rawgeti(*m_lua, LUA_REGISTRYINDEX, result.ref());

		CPPUNIT_ASSERT_EQUAL(1, lua_rawequal(*m_lua, -1, -2));
	}

	void release_whenValidRefGoesOutOfScopeDestructorCallsRelease_registryNoLongerContainsTheRef()
	{
		m_lua->load_chunk("return ");
		int ref;
		{
			OOLUA::Lua_func_ref r;
			m_lua->pull(r);
			ref = r.ref();
		}
		lua_rawgeti(*m_lua, LUA_REGISTRYINDEX, ref);
		CPPUNIT_ASSERT_EQUAL(LUA_TNIL, lua_type(*m_lua, -1));
	}

	void release_callingSetRefOnValidRefWithNewValuesCallsRelease_registryNoLongerContainsTheRef()
	{
		m_lua->load_chunk("return ");
		OOLUA::Lua_func_ref r;
		m_lua->pull(r);
		int	ref = r.ref();
		r.set_ref(NULL, LUA_NOREF);
		lua_rawgeti(*m_lua, LUA_REGISTRYINDEX, ref);
		CPPUNIT_ASSERT_EQUAL(LUA_TNIL, lua_type(*m_lua, -1));
	}

	void swap_afterSwap_rhsStateBeforeSwapIsAssignedToLhsState()
	{
		m_lua->load_chunk("return ");
		OOLUA::Lua_func_ref rhs;
		m_lua->pull(rhs);
		lua_State* rhs_state_before_swap = rhs.state();
		OOLUA::Lua_func_ref lhs;

		lhs.swap(rhs);
		CPPUNIT_ASSERT_EQUAL(rhs_state_before_swap, lhs.state());
	}

	void swap_afterSwap_rhsRefBeforeSwapIsAssignedToLhsRef()
	{
		m_lua->load_chunk("return ");
		OOLUA::Lua_func_ref rhs;
		m_lua->pull(rhs);
		int rhs_ref_before_swap = rhs.ref();
		OOLUA::Lua_func_ref lhs;

		lhs.swap(rhs);
		CPPUNIT_ASSERT_EQUAL(rhs_ref_before_swap, lhs.ref());
	}

	void swap_afterSwap_lhsStateBeforeSwapIsAssignedToRhsState()
	{
		m_lua->load_chunk("return ");
		OOLUA::Lua_func_ref rhs;
		m_lua->pull(rhs);

		OOLUA::Lua_func_ref lhs;
		lua_State* lhs_state_before_swap = lhs.state();

		lhs.swap(rhs);
		CPPUNIT_ASSERT_EQUAL(lhs_state_before_swap, rhs.state());
	}

	void swap_afterSwap_lhsRefBeforeSwapIsAssignedToRhsRef()
	{
		m_lua->load_chunk("return ");
		OOLUA::Lua_func_ref rhs;
		m_lua->pull(rhs);

		OOLUA::Lua_func_ref lhs;
		int lhs_ref_before_swap = lhs.ref();

		lhs.swap(rhs);
		CPPUNIT_ASSERT_EQUAL(lhs_ref_before_swap, rhs.ref());
	}

	void assignment_selfAssignmentOnValidRef_refIsRegistryCopyOfOriginalValue()
	{
		m_lua->load_chunk("return ");
		OOLUA::Lua_func_ref lhs;
		m_lua->pull(lhs);

		//put the instance on the stack to compare after the assignment
		int const ref_before = lhs.ref();
		lua_rawgeti(*m_lua, LUA_REGISTRYINDEX, ref_before);

		lhs = lhs;
		lua_rawgeti(*m_lua, LUA_REGISTRYINDEX, lhs.ref());
		CPPUNIT_ASSERT_EQUAL(1, lua_rawequal(*m_lua, -1, -2));
	}

	void assignment_assignmentToValidRef_registryNoLongerContainsTheValidRef()
	{
		OOLUA::Lua_func_ref lhs;
		change_an_invalid_reference_to_valid(lhs);

		int const ref_before = lhs.ref();
		OOLUA::Lua_func_ref rhs;
		lhs = rhs;
		lua_rawgeti(*m_lua, LUA_REGISTRYINDEX, ref_before);
		CPPUNIT_ASSERT_EQUAL(LUA_TNIL, lua_type(*m_lua, -1));
	}

	void assignment_assignmentOfValidRefToInvalid_refIsRegistryCopyOfInput()
	{
		OOLUA::Lua_func_ref lhs;
		OOLUA::Lua_func_ref rhs;
		change_an_invalid_reference_to_valid(rhs);
		lhs = rhs;
		lua_rawgeti(*m_lua, LUA_REGISTRYINDEX, lhs.ref());
		lua_rawgeti(*m_lua, LUA_REGISTRYINDEX, rhs.ref());

		CPPUNIT_ASSERT_EQUAL(1, lua_rawequal(*m_lua, -1, -2));
	}
	void equality_twoInvalidReferences_returnsTrue()
	{
		OOLUA::Lua_func_ref lhs, rhs;
		CPPUNIT_ASSERT_EQUAL(true, lhs == rhs);
	}

	void equality_invalidRefAndValidRef_returnsfalse()
	{
		OOLUA::Lua_func_ref lhs, rhs;
		change_an_invalid_reference_to_valid(rhs);
		CPPUNIT_ASSERT_EQUAL(false, lhs == rhs);
	}
	void equality_validRefAndInvalidRef_returnsfalse()
	{
		OOLUA::Lua_func_ref lhs, rhs;
		change_an_invalid_reference_to_valid(lhs);
		CPPUNIT_ASSERT_EQUAL(false, lhs == rhs);
	}

	void equality_twoCopiesOfSameRef_returnsTrue()
	{
		OOLUA::Lua_func_ref lhs, rhs;
		change_an_invalid_reference_to_valid(rhs);
		lhs = rhs;
		CPPUNIT_ASSERT_EQUAL(true, lhs == rhs);
	}

	void equality_twoDifferentRefs_returnsFalse()
	{
		OOLUA::Lua_func_ref lhs, rhs;
		change_an_invalid_reference_to_valid(lhs);
		change_an_invalid_reference_to_valid(rhs);

		CPPUNIT_ASSERT_EQUAL(false, lhs == rhs);
	}

	void equality_twoDifferentRefsFromDifferentStatesInTheSameUniverse_returnsFalse()
	{
		OOLUA::Lua_func_ref lhs, rhs;
		change_an_invalid_reference_to_valid(lhs);

		lua_State* s = lua_newthread(m_lua->state());
		OOLUA::load_chunk(s, "return ");
		rhs.set_ref(s, luaL_ref(s, LUA_REGISTRYINDEX));

		CPPUNIT_ASSERT_EQUAL(false, lhs == rhs);
	}

	void equality_sameRefFromDifferentStatesInTheSameUniverse_returnsTrue()
	{
		OOLUA::Lua_func_ref lhs, rhs;
		change_an_invalid_reference_to_valid(lhs);

		lua_State* s = lua_newthread(m_lua->state());//also pushed it onto the stack

		lua_rawgeti(lhs.state(), LUA_REGISTRYINDEX, lhs.ref());
		lua_xmove(m_lua->state(), s, 1);
		rhs.set_ref(s, luaL_ref(s, LUA_REGISTRYINDEX));

		CPPUNIT_ASSERT_EQUAL(true, lhs == rhs);
	}

	void equality_twoDifferentRefsFromDifferentUniverses_returnsFalse()
	{
		OOLUA::Script other_universe; //must outlive the reference

		OOLUA::Lua_func_ref lhs, rhs;
		change_an_invalid_reference_to_valid(lhs);

		other_universe.load_chunk("return ");
		rhs.set_ref(other_universe, luaL_ref(other_universe, LUA_REGISTRYINDEX));
		CPPUNIT_ASSERT_EQUAL(false, lhs == rhs);
	}
};
CPPUNIT_TEST_SUITE_REGISTRATION(Lua_reference);
