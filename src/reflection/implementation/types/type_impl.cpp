/*========================================================
* type_impl.cpp
* @author Sergey Mikhtonyuk
* @date 28 Mar 2010
*
* Copyrights (c) Sergey Mikhtonyuk 2007-2010.
* Terms of use, copying, distribution, and modification
* are covered in accompanying LICENSE file
=========================================================*/
#include "type_impl.h"

namespace reflection
{

	//////////////////////////////////////////////////////////////////////////

	type::type_impl::type_impl(ETypeTag tag, EArchType arch, size_t size)
		: m_tag(tag)
		, m_archt(arch)
		, m_size(size)
	{
	}

	//////////////////////////////////////////////////////////////////////////

	type::type_impl::~type_impl()
	{
	}

	//////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////
	// Type
	//////////////////////////////////////////////////////////////////////////

	type::type(ETypeTag tag, EArchType arch, size_t size)
		: m_impl(new type_impl(tag, arch, size))
	{
	}

	//////////////////////////////////////////////////////////////////////////

	type::type(type_impl* impl)
		: m_impl(impl)
	{
	}

	//////////////////////////////////////////////////////////////////////////

	type::~type()
	{
		delete m_impl;
	}

	//////////////////////////////////////////////////////////////////////////

	ETypeTag type::tag() const
	{
		return m_impl->tag();
	}

	//////////////////////////////////////////////////////////////////////////

	EArchType type::arch_type() const
	{
		return m_impl->arch_type();
	}

	//////////////////////////////////////////////////////////////////////////

	size_t type::size() const
	{
		return m_impl->size();
	}

	//////////////////////////////////////////////////////////////////////////

	const char* type::name() const
	{
		return m_impl->get_name();
	}

	//////////////////////////////////////////////////////////////////////////

	bool type::equal(const type* other) const
	{
		return other
			&& size() == other->size()
			&& tag() == other->tag()
			&& arch_type() == other->arch_type();
	}

	//////////////////////////////////////////////////////////////////////////

	void type::set_name(const char* name)
	{
		m_impl->set_name(name);
	}

	//////////////////////////////////////////////////////////////////////////

	size_t type::_attribute_count() const
	{
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////

	const attribute* type::_get_attribute(size_t i) const
	{
		return 0;
	}

	//////////////////////////////////////////////////////////////////////////
	
} // namespace