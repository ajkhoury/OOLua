#	include "oolua_tests_pch.h"
#	include "oolua.h"
#	include "common_cppunit_headers.h"

/** \cond INTERNAL*/
/*
	This fixture tests implementation details.
*/
/**\endcond */

struct Pod{};
struct PodWithPodMember
{
	Pod first_member;
};

OOLUA_PROXY(Pod)
OOLUA_PROXY_END
OOLUA_EXPORT_NO_FUNCTIONS(Pod)

OOLUA_PROXY(PodWithPodMember)
OOLUA_PROXY_END
OOLUA_EXPORT_NO_FUNCTIONS(PodWithPodMember)

struct PodFoo{ char padding;};
struct PodBar : PodFoo, PodWithPodMember
{
	Pod bar_first_member;
};

OOLUA_PROXY(PodFoo)
OOLUA_PROXY_END
OOLUA_EXPORT_NO_FUNCTIONS(PodFoo)

OOLUA_PROXY(PodBar, PodFoo, PodWithPodMember)
OOLUA_PROXY_END
OOLUA_EXPORT_NO_FUNCTIONS(PodBar)

struct Container
{
	PodBar first_member;
};

OOLUA_PROXY(Container)
OOLUA_PROXY_END
OOLUA_EXPORT_NO_FUNCTIONS(Container)


struct PodNoDestructor{};
struct PodNoDestructorWithPodFirstMemberAlsoWithNoDestructor
{
	PodNoDestructor first_member;
};

OOLUA_PROXY(PodNoDestructor)
	OOLUA_TAGS(No_public_destructor)
OOLUA_PROXY_END
OOLUA_EXPORT_NO_FUNCTIONS(PodNoDestructor)

OOLUA_PROXY(PodNoDestructorWithPodFirstMemberAlsoWithNoDestructor)
	OOLUA_TAGS(No_public_destructor)
OOLUA_PROXY_END
OOLUA_EXPORT_NO_FUNCTIONS(PodNoDestructorWithPodFirstMemberAlsoWithNoDestructor)

struct PodWithMemberWhichItselfHasAMember
{
	PodWithPodMember first_member;
};
OOLUA_PROXY(PodWithMemberWhichItselfHasAMember)
OOLUA_PROXY_END
OOLUA_EXPORT_NO_FUNCTIONS(PodWithMemberWhichItselfHasAMember)

class Pointer_cache : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(Pointer_cache);
	CPPUNIT_TEST(noOffsetBaseCase_comparePointerToPodWithPointerToItsFirstMember_voidsPointerCompareEqual);
	CPPUNIT_TEST(noOffset_pushPodPtrAndThenFirstMemberPointer_userdataAreNotEqual);
	CPPUNIT_TEST(noOffset_pushPodPtrAndThenFirstMemberPointer_firstUdHasCollisionFlagSet);
	CPPUNIT_TEST(noOffset_pushPodPtrAndThenFirstMemberPointer_secondUdHasCollisionFlagSet);
	CPPUNIT_TEST(findUd_pushPodPtrThenLookForPtrToFirstMember_findUdReturnsNull);
	CPPUNIT_TEST(addUd_pushPodPtrThenPushFirstMemberPointer_cacheEntryIsATable);
	CPPUNIT_TEST(addUd_pushPodPtrThenFirstMemberPtrThenAgainPushThePodPtr_bothPodsUseTheSameUserdata);
	CPPUNIT_TEST(gc_pushPodPtrThenFirstMemberPtrThenRunGc_weakTableHasNoEntryForThePtr);
	CPPUNIT_TEST(offset_tests_expectedBehaviour);
	CPPUNIT_TEST(findUd_pushOffsetBaseThenPushContainerClassThenPushFirstMemberWhichIsDerivedFromOffsetBase_baseAndDerivedUseSameUd);
	CPPUNIT_TEST(findUd_pushOffsetBaseOffsetBaseFirstThenPushContainerClassThenPushFirstMemberWhichIsDerivedFromOffsetBase_baseAndDerivedUseSameUd);
	CPPUNIT_TEST(gc_collisionPointersOnTypesWhichHaveNoDestructor_afterGcThereIsNoEntryForThePtr);
	CPPUNIT_TEST(addPtr_classItsFirstMemberAndItFirstMember_allTheSamePointer);
	CPPUNIT_TEST(addPtr_samePointerAsThreeDifferentTypes_findUdFindsFirstEntry);
	CPPUNIT_TEST(addPtr_samePointerAsThreeDifferentTypes_findUdFindsSecondEntry);
	CPPUNIT_TEST(addPtr_samePointerAsThreeDifferentTypes_findUdFindsThirdEntry);
	CPPUNIT_TEST(addPtr_samePointerAsThreeDifferentTypes_thirdUdHasCollisionFlagSet);

	CPPUNIT_TEST(findUd_pushBaseFirstMemberWhichHasTheSameAddressAsBaseThenPushDerived_stackContainsTwoUdsWhichAreNotEqual);
	CPPUNIT_TEST(findUd_causeCacheCollisionThenRemoveDerivedCallGcAndThenReaddIt_stackContainsTwoUdsWhichAreDifferent);
	CPPUNIT_TEST_SUITE_END();
	OOLUA::Script* m_lua;
public:
	Pointer_cache()
		:m_lua(0)
	{}
	void setUp()
	{
		m_lua = new OOLUA::Script;
	}
	void tearDown()
	{
		delete m_lua;
	}

	void noOffsetBaseCase_comparePointerToPodWithPointerToItsFirstMember_voidsPointerCompareEqual()
	{
		PodWithPodMember p;
		CPPUNIT_ASSERT_EQUAL(static_cast<void*>(&p), static_cast<void*>(&p.first_member));
	}

	void noOffset_pushPodPtrAndThenFirstMemberPointer_userdataAreNotEqual()
	{
		m_lua->register_class<PodWithPodMember>();
		m_lua->register_class<Pod>();
		PodWithPodMember p;
		m_lua->push(&p);
		m_lua->push(&p.first_member);
		CPPUNIT_ASSERT(lua_touserdata(*m_lua, 1) != lua_touserdata(*m_lua, 2));
	}

	void noOffset_pushPodPtrAndThenFirstMemberPointer_firstUdHasCollisionFlagSet()
	{
		m_lua->register_class<PodWithPodMember>();
		m_lua->register_class<Pod>();
		PodWithPodMember p;
		m_lua->push(&p);
		m_lua->push(&p.first_member);
		CPPUNIT_ASSERT_EQUAL(true, OOLUA::INTERNAL::userdata_is_ptr_collision(static_cast<OOLUA::INTERNAL::Lua_ud*>(lua_touserdata(*m_lua, 1))));
	}

	void noOffset_pushPodPtrAndThenFirstMemberPointer_secondUdHasCollisionFlagSet()
	{
		m_lua->register_class<PodWithPodMember>();
		m_lua->register_class<Pod>();
		PodWithPodMember p;
		m_lua->push(&p);
		m_lua->push(&p.first_member);
		CPPUNIT_ASSERT_EQUAL(true, OOLUA::INTERNAL::userdata_is_ptr_collision(static_cast<OOLUA::INTERNAL::Lua_ud*>(lua_touserdata(*m_lua, 2))));
	}

	void findUd_pushPodPtrThenLookForPtrToFirstMember_findUdReturnsNull()
	{
		m_lua->register_class<PodWithPodMember>();
		m_lua->register_class<Pod>();
		PodWithPodMember p;
		m_lua->push(&p);
		OOLUA::INTERNAL::Lua_ud* ud = OOLUA::INTERNAL::find_ud(*m_lua, &p.first_member, &p.first_member, false);
		CPPUNIT_ASSERT(NULL == ud);
	}

	void addUd_pushPodPtrThenPushFirstMemberPointer_cacheEntryIsATable()
	{
		m_lua->register_class<PodWithPodMember>();
		m_lua->register_class<Pod>();
		PodWithPodMember p;
		m_lua->push(&p);
		m_lua->push(&p.first_member);
		int weakIndex = OOLUA::INTERNAL::push_weak_table(*m_lua);
		lua_pushlightuserdata(*m_lua, &p);
		lua_rawget(*m_lua, weakIndex);
		CPPUNIT_ASSERT_EQUAL(LUA_TTABLE, lua_type(*m_lua, -1));
	}

	void addUd_pushPodPtrThenFirstMemberPtrThenAgainPushThePodPtr_bothPodsUseTheSameUserdata()
	{
		m_lua->register_class<PodWithPodMember>();
		m_lua->register_class<Pod>();
		PodWithPodMember p;
		m_lua->push(&p);
		m_lua->push(&p.first_member);
		m_lua->push(&p);
		CPPUNIT_ASSERT_EQUAL(lua_touserdata(*m_lua, 1), lua_touserdata(*m_lua, 3));
	}

	void gc_pushPodPtrThenFirstMemberPtrThenRunGc_weakTableHasNoEntryForThePtr()
	{
		m_lua->register_class<PodWithPodMember>();
		m_lua->register_class<Pod>();
		PodWithPodMember p;
		m_lua->push(&p);
		m_lua->push(&p.first_member);
		lua_pop(*m_lua, 2);
		lua_gc(*m_lua, LUA_GCCOLLECT, 0);
		int weakIndex = OOLUA::INTERNAL::push_weak_table(*m_lua);
		lua_pushlightuserdata(*m_lua, &p);
		lua_rawget(*m_lua, weakIndex);
		CPPUNIT_ASSERT_EQUAL(LUA_TNIL, lua_type(*m_lua, -1));
	}

	void findUd_pushBaseFirstMemberWhichHasTheSameAddressAsBaseThenPushDerived_stackContainsTwoUdsWhichAreNotEqual()
	{
		m_lua->register_class<PodBar>();
		m_lua->register_class<Pod>();

		PodBar podbar;
		PodBar* pod_ptr = &podbar;
		Pod* pod = &podbar.first_member;
		m_lua->push(pod);
		m_lua->push(pod_ptr);
		CPPUNIT_ASSERT(lua_touserdata(*m_lua, 1) != lua_touserdata(*m_lua, 2));
	}

	void findUd_causeCacheCollisionThenRemoveDerivedCallGcAndThenReaddIt_stackContainsTwoUdsWhichAreDifferent()
	{
		m_lua->register_class<PodBar>();
		m_lua->register_class<Pod>();

		PodBar podbar;
		PodBar* pod_ptr = &podbar;
		Pod* pod = &podbar.first_member;
		m_lua->push(pod);
		m_lua->push(pod_ptr);
		lua_pop(*m_lua, 1);
		m_lua->gc();
		m_lua->push(pod_ptr);

		CPPUNIT_ASSERT(lua_touserdata(*m_lua, 1) != lua_touserdata(*m_lua, 2));

	}
	void offset_tests_expectedBehaviour()
	{
		m_lua->register_class<Container>();
		m_lua->register_class<PodBar>();
		m_lua->register_class<Pod>();
		Container cont;
		PodBar* derived = &cont.first_member;
		PodWithPodMember* offset_base = derived;
		CPPUNIT_ASSERT(static_cast<void*>(offset_base) != static_cast<void*>(derived));//offset between base and derived
		CPPUNIT_ASSERT(static_cast<void*>(&cont.first_member) == static_cast<void*>(&cont));//first member same as container
	}

	void findUd_pushOffsetBaseThenPushContainerClassThenPushFirstMemberWhichIsDerivedFromOffsetBase_baseAndDerivedUseSameUd()
	{
		m_lua->register_class<Container>();
		m_lua->register_class<PodBar>();
		m_lua->register_class<Pod>();
		Container cont;
		PodBar* derived = &cont.first_member;
		PodWithPodMember* offset_base = derived;
		m_lua->push(offset_base);
		m_lua->push(&cont);
		m_lua->push(derived);
		CPPUNIT_ASSERT_EQUAL(lua_touserdata(*m_lua, 1), lua_touserdata(*m_lua, 3));
	}

	void findUd_pushOffsetBaseOffsetBaseFirstThenPushContainerClassThenPushFirstMemberWhichIsDerivedFromOffsetBase_baseAndDerivedUseSameUd()
	{
		m_lua->register_class<Container>();
		m_lua->register_class<PodBar>();
		m_lua->register_class<Pod>();
		Container cont;
		PodBar* derived = &cont.first_member;
		PodWithPodMember* offset_base = derived;
		m_lua->push(offset_base);
		m_lua->push(&offset_base->first_member);
		m_lua->push(&cont);
		m_lua->push(derived);
		CPPUNIT_ASSERT_EQUAL(lua_touserdata(*m_lua, 1), lua_touserdata(*m_lua, 4));
	}

	void gc_collisionPointersOnTypesWhichHaveNoDestructor_afterGcThereIsNoEntryForThePtr()
	{
		m_lua->register_class<PodNoDestructor>();
		m_lua->register_class<PodNoDestructorWithPodFirstMemberAlsoWithNoDestructor>();
		PodNoDestructorWithPodFirstMemberAlsoWithNoDestructor pod;
		m_lua->push(&pod);
		m_lua->push(&pod.first_member);
		lua_pop(*m_lua, 2);
		m_lua->gc();
		int weakIndex = OOLUA::INTERNAL::push_weak_table(*m_lua);
		lua_pushlightuserdata(*m_lua, &pod);
		lua_rawget(*m_lua, weakIndex);
		CPPUNIT_ASSERT_EQUAL(LUA_TNIL, lua_type(*m_lua, -1));
	}

	void addPtr_classItsFirstMemberAndItFirstMember_allTheSamePointer()
	{
		m_lua->register_class<PodWithMemberWhichItselfHasAMember>();
		m_lua->register_class<PodWithPodMember>();
		m_lua->register_class<Pod>();
		PodWithMemberWhichItselfHasAMember instance;
		CPPUNIT_ASSERT_EQUAL(static_cast<void*>(&instance), static_cast<void*>(&instance.first_member));
		CPPUNIT_ASSERT_EQUAL(static_cast<void*>(&instance), static_cast<void*>(&instance.first_member.first_member));
	}

	void addPtr_samePointerAsThreeDifferentTypes_findUdFindsFirstEntry()
	{
		m_lua->register_class<PodWithMemberWhichItselfHasAMember>();
		m_lua->register_class<PodWithPodMember>();
		m_lua->register_class<Pod>();
		PodWithMemberWhichItselfHasAMember instance;
		m_lua->push(&instance);
		m_lua->push(&instance.first_member);
		m_lua->push(&instance.first_member.first_member);
		CPPUNIT_ASSERT(OOLUA::INTERNAL::find_ud(*m_lua, &instance, &instance, false) != NULL);
	}

	void addPtr_samePointerAsThreeDifferentTypes_findUdFindsSecondEntry()
	{
		m_lua->register_class<PodWithMemberWhichItselfHasAMember>();
		m_lua->register_class<PodWithPodMember>();
		m_lua->register_class<Pod>();
		PodWithMemberWhichItselfHasAMember instance;
		m_lua->push(&instance);
		m_lua->push(&instance.first_member);
		m_lua->push(&instance.first_member.first_member);
		CPPUNIT_ASSERT(OOLUA::INTERNAL::find_ud(*m_lua, &instance.first_member, &instance.first_member, false) != NULL);
	}

	void addPtr_samePointerAsThreeDifferentTypes_findUdFindsThirdEntry()
	{
		m_lua->register_class<PodWithMemberWhichItselfHasAMember>();
		m_lua->register_class<PodWithPodMember>();
		m_lua->register_class<Pod>();
		PodWithMemberWhichItselfHasAMember instance;
		m_lua->push(&instance);
		m_lua->push(&instance.first_member);
		m_lua->push(&instance.first_member.first_member);
		CPPUNIT_ASSERT(OOLUA::INTERNAL::find_ud(*m_lua, &instance.first_member.first_member, &instance.first_member.first_member, false) != NULL);
	}

	void addPtr_samePointerAsThreeDifferentTypes_thirdUdHasCollisionFlagSet()
	{
		m_lua->register_class<PodWithMemberWhichItselfHasAMember>();
		m_lua->register_class<PodWithPodMember>();
		m_lua->register_class<Pod>();
		PodWithMemberWhichItselfHasAMember instance;
		m_lua->push(&instance);
		m_lua->push(&instance.first_member);
		m_lua->push(&instance.first_member.first_member);
		CPPUNIT_ASSERT_EQUAL(true, OOLUA::INTERNAL::userdata_is_ptr_collision(static_cast<OOLUA::INTERNAL::Lua_ud*>(lua_touserdata(*m_lua, -1))));
	}
};
CPPUNIT_TEST_SUITE_REGISTRATION(Pointer_cache);

