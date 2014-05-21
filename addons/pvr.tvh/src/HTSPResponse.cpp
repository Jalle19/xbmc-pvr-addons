/*
 *      Copyright (C) 2005-2014 Team XBMC
 *      http://www.xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include "HTSPResponse.h"

using namespace PLATFORM;

/*
 * CHTSPResponse
 */
CHTSPResponse::CHTSPResponse()
: m_flag(false), m_msg(NULL)
{
}

CHTSPResponse::~CHTSPResponse()
{
  if (m_msg) htsmsg_destroy(m_msg);
  Set(NULL); // ensure signal is sent
}

void CHTSPResponse::Set(htsmsg_t *msg)
{
  m_msg = msg;
  m_flag = true;
  m_cond.Broadcast();
}

htsmsg_t *CHTSPResponse::Get(PLATFORM::CMutex &mutex, uint32_t timeout)
{
  m_cond.Wait(mutex, m_flag, timeout);
  
  htsmsg_t *r = m_msg;
  m_msg = NULL;
  m_flag = false;
  return r;
}

/*
 * CHTSPResponseList
 */
void CHTSPResponseList::Clear()
{
  CLockObject lock(m_mutex);
  m_sequence = 0;
  m_responses.clear();
}

void CHTSPResponseList::Erase(uint32_t seq)
{
  CLockObject lock(m_mutex);
  m_responses.erase(seq);
}

CHTSPResponse* CHTSPResponseList::Find(uint32_t seq)
{
  CLockObject lock(m_mutex);
  std::map<uint32_t, CHTSPResponse*>::iterator it = m_responses.find(seq);
  if (it != m_responses.end())
    return it->second;

  return NULL;
}

uint32_t CHTSPResponseList::Add(CHTSPResponse* response)
{
  CLockObject lock(m_mutex);
  m_responses[++m_sequence] = response;
  return m_sequence;
}
