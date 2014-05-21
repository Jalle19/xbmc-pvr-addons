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
#ifndef HTSPRESPONSE_H
#define	HTSPRESPONSE_H

#include <map>
#include "platform/threads/mutex.h"

extern "C" {
#include "libhts/htsmsg_binary.h"
}

/*
 * HTSP Response handler
 */
class CHTSPResponse
{
public:
  CHTSPResponse(void);
  ~CHTSPResponse();
  htsmsg_t *Get(PLATFORM::CMutex &mutex, uint32_t timeout);
  void Set(htsmsg_t *m);
private:
  PLATFORM::CCondition<volatile bool> m_cond;
  bool                                m_flag;
  htsmsg_t                           *m_msg;
};

/*
 * HTSP Response list
 */
class CHTSPResponseList
{
public:
  /**
   * Erases all messages and reset the sequence number
   */
  void Clear();

  /**
   * Erases a message from the list
   * @param seq the message sequence
   */
  void Erase(uint32_t seq);

  /**
   * Returns a pointer to the specified response
   * @param seq the message sequence
   * @return the response
   */
  CHTSPResponse* Find(uint32_t seq);

  /**
   * Adds a response to the list and returns its sequence
   * @param response the response
   * @return the sequence number
   */
  uint32_t Add(CHTSPResponse* response);
  
private:
  PLATFORM::CMutex                   m_mutex;
  uint32_t                           m_sequence;
  std::map<uint32_t, CHTSPResponse*> m_responses;
};

#endif	/* HTSPRESPONSE_H */

