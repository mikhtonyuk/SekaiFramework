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
#include "custom/member.h"
#include "custom/attribute.h"

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
		for(size_t i = 0, size = m_attributes.size(); i != size; ++i)
			m_attributes[i].first->release();

		for(size_t i = 0, size = m_members.size(); i != size; ++i)
			m_members[i]->release();
	}

	//////////////////////////////////////////////////////////////////////////

	void type::type_impl::add_member(const member& mem)
	{
		m_members.push_back(mem.clone());
	}

	//////////////////////////////////////////////////////////////////////////

	void type::type_impl::add_attribute(const attribute& attr)
	{
		m_attributes.push_back(std::make_pair<attribute*, member*>(attr.clone(), 0));
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

	void type::set_name(const char* name)
	{
		m_impl->set_name(name);
	}

	//////////////////////////////////////////////////////////////////////////

	void type::add_member(const member& mem)
	{
		m_impl->add_member(mem);
	}

	//////////////////////////////////////////////////////////////////////////

	void type::add_attribute(const attribute& attr)
	{
		m_impl->add_attribute(attr);
	}

	//////////////////////////////////////////////////////////////////////////
	
} // namespace
