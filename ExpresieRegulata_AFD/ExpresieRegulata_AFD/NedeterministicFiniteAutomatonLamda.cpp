#include "NedeterministicFiniteAutomatonLamda.h"

NedeterministicFiniteAutomatonLamda::NedeterministicFiniteAutomatonLamda()
{
	m_Sigma.push_back('#');
}

NedeterministicFiniteAutomatonLamda::NedeterministicFiniteAutomatonLamda(const NedeterministicFiniteAutomatonLamda& afn) :
	m_Q{ afn.m_Q },
	m_Sigma{ afn.m_Sigma },
	m_Delta{ afn.m_Delta },
	m_Initial{ m_Initial },
	m_Final{ afn.m_Final }
{}

NedeterministicFiniteAutomatonLamda& NedeterministicFiniteAutomatonLamda::operator=(const NedeterministicFiniteAutomatonLamda& afn)
{
	m_Q = afn.m_Q;
	m_Sigma = afn.m_Sigma;
	m_Delta = afn.m_Delta;
	m_Initial = m_Initial;
	m_Final = afn.m_Final;
	return *this;
}

void NedeterministicFiniteAutomatonLamda::SetInitial(const std::string& initial)
{
	m_Initial = initial;
}

void NedeterministicFiniteAutomatonLamda::SetFinal(const std::string & final)
{
	m_Final = final;
}

std::string NedeterministicFiniteAutomatonLamda::GetInitial() const
{
	return m_Initial;
}

std::string NedeterministicFiniteAutomatonLamda::GetFinal() const
{
	return m_Final;
}
