/*========================================================
* member_impl.cpp
* @author Sergey Mikhtonyuk
* @date 28 Mar 2010
*
* Copyrights (c) Sergey Mikhtonyuk 2007-2010.
* Terms of use, copying, distribution, and modification
* are covered in accompanying LICENSE file
=========================================================*/
#include "member_impl.h"

namespace reflection
{

	//////////////////////////////////////////////////////////////////////////

	member::member_impl::member_impl(EMemberType type, const char* name)
		: m_type(type)
		, m_name(name)
	{ }

	//////////////////////////////////////////////////////////////////////////

	member::member_impl::~member_impl()
	{ } 

	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// member
	//////////////////////////////////////////////////////////////////////////

	member::member(member_impl* impl)
		: m_impl(impl)
	{ }

	//////////////////////////////////////////////////////////////////////////

	member::~member()
	{
		delete m_impl;
	}

	//////////////////////////////////////////////////////////////////////////

	member* member::clone() const
	{
		return new member(m_impl->clone());
	}

	//////////////////////////////////////////////////////////////////////////

	void member::release()
	{
		delete this;
	}

	//////////////////////////////////////////////////////////////////////////
	
} // namespace